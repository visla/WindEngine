/*
    CSingleRacePhase.cpp

	Autor: Vuk Vranjkovic
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CSingleRacePhase.h>
#include <CMainMenuPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CGameSettings.h>
#include <CSinglePlayerPhase.h>
#include <CShoppingPhase.h>
#include <CMapProvider.h>
#include <CGUIHelper.h>
#include <CProfileManager.h>
#include <CMap.h>
#include <CMapHeader.h>
#include <CServerInfo.h>
#include <CGameClient.h>
#include <CGameServer.h>
#include <CAIProfileManager.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <WeTextureManager.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <CLocalizationSettings.h>
#include <CEGUIPropertyHelper.h>


using CEGUI::Point;
using CEGUI::Size;
using CEGUI::LeftAligned;
using CEGUI::Window;
using CEGUI::RightAligned;
using CEGUI::Event;
using CEGUI::PushButton;
using CEGUI::MultiColumnList;



#define MAX_ACTIVE_PLAYERS 8



CSingleRacePhase *CSingleRacePhase::Instance = new CSingleRacePhase;

CSingleRacePhase::CSingleRacePhase()
{
	//bPressed=false;
	GameStarted = false;
	MapDescImage = NULL;
}



void CSingleRacePhase::Start()
{
	// reset tuber-a
	CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = false;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.WeaponName = "";
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MineName = "";
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Price = 0;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Name = "";
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Description = "";
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MaxAcceleration = 0.0f;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MaxSpeed = 0.0f;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.HP = 0;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Manouverability = 0.0f;

	MapExt = ".utm";
	CMapProvider::Instance->LoadAllMapNames((String)"*" + MapExt);

	CurrentMapIndex = 0;
	CGameSettings::ClientInstance->MapName = CMapProvider::Instance->MapNames[CurrentMapIndex] + MapExt;
	CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_SP_SINGLERACE;

	CMap TmpMap;
	TmpMap.LoadMapHeader(CGameSettings::ClientInstance->MapName);
	MapDescImage = WeTextureManager::Instance()->Load(TmpMap.MapHeader.MapDescriptionImage, true);


	CGUIHelper::CreateWnd("SingleRace_RootWindow", CLocalizationSettings::Instance()->GetLocalText("SINGLE RACE"),
		0.0f, 0.05f, 0.05f, 0.4f, 0.9f, "RootWindow");

	CreateRootComboboxes();


	CGUIHelper::CreateWnd("SingleRace_MapDescription_Window", CLocalizationSettings::Instance()->GetLocalText("MAP DESCRIPTION"),
		0.0f, 0.48f, 0.0f, 0.5f, 0.5f, "RootWindow");

	CGUIHelper::CreateText("SingleRace_MapDescription",TmpMap.MapHeader.MapDescription,0.5f,0.27f,0.5f,1.0f,
		"","","","SingleRace_MapDescription_Window");

	CGUIHelper::CreateText("SingleRace_MapName",CGameSettings::ClientInstance->MapName.Extract('.',0),0.1f,0.9f,0.5f,0.1f,
		"","","","SingleRace_MapDescription_Window");

	CGUIHelper::CreateStaticImage("SingleRace_MapImage", "", 0.1f, 0.1f, 0.5f, 0.5f, "SingleRace_MapDescription_Window");

	CGUIHelper::CreateButton("SingleRace_PrevMap", CLocalizationSettings::Instance()->GetLocalText("PREV"), 0.1f, 0.9f, 0.1f, 0.05f, "SingleRace_MapDescription_Window", &OnPrevMap);

	CGUIHelper::CreateButton("SingleRace_NextMap", CLocalizationSettings::Instance()->GetLocalText("NEXT"), 0.8f, 0.9f, 0.1f, 0.05f, "SingleRace_MapDescription_Window", &OnNextMap);

	CGUIHelper::CreateButton("SingleRace_Back_Button",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.35f, 0.65f,0.65f,0.14f,"SingleRace_MapDescription_Window", &CSingleRacePhase::OnBack);

	CGUIHelper::CreateButton("SingleRace_Start_Button",CLocalizationSettings::Instance()->GetLocalText("START"),0.35f,0.8f,0.65f, 0.14f,"SingleRace_MapDescription_Window", &CSingleRacePhase::OnStart);


	CGUIHelper::CreateWnd("SingleRace_Options_Window", CLocalizationSettings::Instance()->GetLocalText("GAME OPTIONS"),
		0.0f, 0.48f, 0.5f, 0.5f, 0.5f, "RootWindow");

	CreateOptionsComboboxes();


	// Reset tuber description to nothing and money to nothing
	CGameServer::Instance->Initialize();
	CGameServer::Instance->StartHosting();
	CGameClient::Instance->Initialize();	

	CGameSettings::ClientInstance->SetWeapons(true);
	CGameSettings::ClientInstance->SetCrates(false);
	CGameSettings::ClientInstance->SetUnlimitedSpeed(false);
	CGameSettings::ClientInstance->SetCatchUp(true);
	CGameSettings::ClientInstance->StartingMoney = CGameSettings::CashCategories[0];
	CGameSettings::ClientInstance->LapCount = 10;
	CGameSettings::ClientInstance->Funny = false;

	
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("SingleRace_Weapons_Combobox")->setText(
		(CGameSettings::ClientInstance->GetWeapons() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
		);

	Mng.getWindow("SingleRace_Crates_Combobox")->setText(
		(CGameSettings::ClientInstance->GetCrates() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
		);

	Mng.getWindow("SingleRace_UnlimitedSpeed_Combobox")->setText(
		(CGameSettings::ClientInstance->GetUnlimitedSpeed() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
		);

	Mng.getWindow("SingleRace_CatchUp_Combobox")->setText(
		(CGameSettings::ClientInstance->GetCatchUp() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
		);

	char tmp[100];

	sprintf(tmp, "%d", CGameSettings::ClientInstance->StartingMoney);
	Mng.getWindow("SingleRace_Money_Combobox")->setText(tmp);

	sprintf(tmp, "%d", CGameSettings::ClientInstance->LapCount);
	Mng.getWindow("SingleRace_LapCount_Combobox")->setText(tmp);


	CGameClient::Instance->CreateGame(&CGame::Instance->ServerInfo,"",true);
}




void CSingleRacePhase::MakeTableFromSettings()
{
	char tmp[100];
	CEGUI::Combobox *UserCombo, *TeamCombo;
	CEGUI::Window *ColorImage, *ColorBackImage;
	String s;

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();


	for (int i = 0; i < MAX_ACTIVE_PLAYERS; i++)
	{
		sprintf(tmp, "SingleRace_PlayersCombo %d", i);
		UserCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "SingleRace_Teams %d", i);
		TeamCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "SingleRace_TeamColors %d", i);
		ColorImage = (CEGUI::Window *)Mng.getWindow(tmp);
		sprintf(tmp, "SingleRace_TeamColors_Back %d", i);
		ColorBackImage = (CEGUI::Window *)Mng.getWindow(tmp);

		UserCombo->setText(CLocalizationSettings::Instance()->GetLocalText("OPEN").GetString());

	
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
			(CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE && 
			CGameClient::Instance->IsGameCreator()))
		{
			TeamCombo->setEnabled(false);
			TeamCombo->setAlpha(0.5);
			ColorImage->setVisible(false);
			ColorBackImage->setVisible(false);

			if (i < MAX_ACTIVE_PLAYERS)
			{
				UserCombo->setEnabled(true);
				UserCombo->setAlpha(1.0f);
			}
			else
			{
				UserCombo->setEnabled(false);
				UserCombo->setAlpha(0.5f);
			}
		}
		else 
		{
			ColorImage->setVisible(true);
			ColorBackImage->setVisible(true);
		}

		// ako nisam creator igre onda ne mogu da modifikujem nista ni tudje timove
		if (!CGameClient::Instance->IsGameCreator())
		{
			if ((i >= MAX_ACTIVE_PLAYERS) ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE)
			{
				ColorImage->setVisible(false);
				ColorBackImage->setVisible(false);
			}


			UserCombo->setEnabled(false);
			UserCombo->setAlpha(0.5);

			if (!CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{				
				TeamCombo->setEnabled(false);
				TeamCombo->setAlpha(0.5);
			}
			else
			{
				TeamCombo->setEnabled(true);
				TeamCombo->setAlpha(1);
			}
		}
		else 
		{
			// ako jesam game editor ne mogu da menjam timove ostalim igracima
			if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_HUMAN && !CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{
				TeamCombo->setEnabled(false);
				TeamCombo->setAlpha(0.5);

			}
			else if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_HUMAN && CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{
				UserCombo->setEnabled(false);
				UserCombo->setAlpha(0.5);
				TeamCombo->setEnabled(true);
				TeamCombo->setAlpha(1);
			}
			else if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIEASY ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIMEDIUM ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIHARD)
			{
				TeamCombo->setEnabled(true);
				TeamCombo->setAlpha(1);
			}
		}

		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_HUMAN)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name;
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIEASY)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " " + CLocalizationSettings::Instance()->GetLocalText("EASY") + ")";
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIMEDIUM)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " " + CLocalizationSettings::Instance()->GetLocalText("MEDIUM") + ")";
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIHARD)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " " + CLocalizationSettings::Instance()->GetLocalText("HARD") + ")";
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE)
		{		
			UserCombo->setText(CLocalizationSettings::Instance()->GetLocalText("CLOSED").GetString());
		}

		if (CGameSettings::ClientInstance->tmpPlayers[i].User)
		{
			int TeamNumber;
			TeamNumber = CGameSettings::ClientInstance->tmpPlayers[i].User->TeamNumber;
			sprintf(tmp, "%s %d", CLocalizationSettings::Instance()->GetLocalText("TEAM").GetString(), TeamNumber);
			TeamCombo->setText(tmp);

			CEGUI::colour col;
			col.set(
				CGameSettings::ClientInstance->TeamColors[TeamNumber-1].r,
				CGameSettings::ClientInstance->TeamColors[TeamNumber-1].g,
				CGameSettings::ClientInstance->TeamColors[TeamNumber-1].b);
			ColorImage->setProperty("Colour", CEGUI::PropertyHelper::colourToString(col).c_str());
		}		
	}
}



bool CSingleRacePhase::OnTeamChange(const CEGUI::EventArgs &e)
{
	CEGUI::WindowEventArgs *we;
	we = (CEGUI::WindowEventArgs*)&e;
	CEGUI::Combobox *Combo = (CEGUI::Combobox*) we->window;

	String s;
	int TeamSelected;
	int ComponentSelected;
	CEGUI::String cs;

	cs = Combo->getName();
	s = cs.c_str();
	s = s.Extract(' ', 1);
	ComponentSelected = s.AsInt();

	cs = Combo->getText();
	s = cs.c_str();
	s = s.Extract(' ', 1);
	TeamSelected = s.AsInt()-1;		


	CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User->TeamNumber = TeamSelected+1;
	CGameClient::Instance->SendTableInfo(ComponentSelected);
	return true;
}



void CSingleRacePhase::End()
{
	//CreateProfiles();
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("SingleRace_RootWindow")->destroy();
	Mng.getWindow("SingleRace_MapDescription_Window")->destroy();
	Mng.getWindow("SingleRace_Options_Window")->destroy();

	CProfileManager::Instance()->RestoreToLoadedProfile();
	CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = false;	
}



void CSingleRacePhase::Tick()
{
	MakeTableFromSettings();

	CheckLastOne();
	CheckCatchUp();

	CEGUI::EventArgs e;
	if(CGame::Instance->InputMessage.Exit)
		OnBack(e);

	if (CGameClient::Instance->GetStatus() == GC_GAME_STARTED)
		CMenuPhase::Instance->SetPhase(CShoppingPhase::Instance);
}



void CSingleRacePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
	if (MapDescImage)
	{
		CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
		CEGUI::Window *wnd = Mng.getWindow("SingleRace_MapImage");

		CEGUI::Rect rect = wnd->getPixelRect();
		WeCanvas::Instance()->DrawRectangle((int)rect.d_left, (int)rect.d_top, (int)rect.d_right - (int)rect.d_left, (int)rect.d_bottom - (int)rect.d_top, 0xFFFFFFFF, MapDescImage);

		CEGUI::MouseCursor::getSingleton().draw();
	}
}



void CSingleRacePhase::SetVisibleOtherPlayers(bool Visible)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	char tmp[100], sVisible[10];

	if (Visible)
		strcpy(sVisible, "true");
	else
		strcpy(sVisible, "false");

	for (int i = 1; i < MAX_ACTIVE_PLAYERS; i++)
	{
		sprintf(tmp, "SingleRace_PlayersCombo %d", i);
		Mng.getWindow(tmp)->setProperty("Visible", sVisible);
		sprintf(tmp, "SingleRace_Teams %d", i);
		Mng.getWindow(tmp)->setProperty("Visible", sVisible);
		sprintf(tmp, "SingleRace_TeamColors %d", i);
		Mng.getWindow(tmp)->setProperty("Visible", sVisible);
		sprintf(tmp, "SingleRace_TeamColors_Back %d", i);
		Mng.getWindow(tmp)->setProperty("Visible", sVisible);
	}
}



void CSingleRacePhase::UpdateMapInfo()
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

	CMap TmpMap; 
	TmpMap.LoadMapHeader(CGameSettings::ClientInstance->MapName);
	(winMgr.getWindow("SingleRace_MapDescription"))->setText(TmpMap.MapHeader.MapDescription.GetString());
	(winMgr.getWindow("SingleRace_MapName"))->setText(CGameSettings::ClientInstance->MapName.Extract('.',0).GetString());

	CSingleRacePhase::Instance->MapDescImage = WeTextureManager::Instance()->Load(TmpMap.MapHeader.MapDescriptionImage,true);
}



void CSingleRacePhase::CheckLastOne()
{
	bool disable = false;

	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE)
	{
		int n = 0;

		for (int i = 0; i < MAX_ACTIVE_PLAYERS; ++i)
		{
			if ((CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_CLOSE) &&
				(CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_OPEN))
			{
				++n;
			}
		}

		if (n < 2)
			disable = true;
	}

	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *btn = winMgr.getWindow("SingleRace_Start_Button");

	if (disable)
	{
		btn->setEnabled(false);
		btn->setAlpha(0.5f);
	}
	else
	{
		btn->setEnabled(true);
		btn->setAlpha(1.0f);
	}
}



void CSingleRacePhase::CheckCatchUp()
{
	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL) return;

	int n = 0;

	for (int i = 0; i < MAX_ACTIVE_PLAYERS; ++i)
	{
		if ((CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_CLOSE) &&
			(CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_OPEN))
		{
			++n;
		}
	}

	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	if (n < 2)
	{
		CEGUI::Combobox *combo;
		CEGUI::Window *title;

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox");
		combo->setEnabled(false);
		combo->setAlpha(0.5f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox_title");
		title->setAlpha(0.5f);

		CGameSettings::ClientInstance->SetCatchUp(false);
		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	}
	else
	{
		CEGUI::Combobox *combo;
		CEGUI::Window *title;

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox");
		combo->setEnabled(true);
		combo->setAlpha(1.0f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox_title");
		title->setAlpha(1.0f);

		CEGUI::String tmp = combo->getText();
		CGameSettings::ClientInstance->SetWeapons(tmp != CLocalizationSettings::Instance()->GetLocalText("OFF"));
		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	}
}



bool CSingleRacePhase::OnRaceTypeCombobox(const CEGUI::EventArgs &e)
{
	bool OthersVisible = true;

	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::String tmp = winMgr.getWindow("SingleRace_RaceType_Combobox")->getText();
	CGameSettings::MAPTYPE LastMapType = CGameSettings::ClientInstance->MapType;
	CSingleRacePhase::Instance->MapExt = ".utm";

	if (tmp == CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_SP_SINGLERACE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("LASTONE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_SP_LASTONE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("TIME TRIAL"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_SP_TIMETRIAL;
		OthersVisible = false;
		for (int i = 1; i < MAX_ACTIVE_PLAYERS; i++)
		{
			CGameSettings::ClientInstance->tmpPlayers[i].Status = PCFG_OPEN;
			CGameClient::Instance->SendTableInfo(i);
		}
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_SP_DEATHMATCH;
		CSingleRacePhase::Instance->MapExt = ".utd";
	}


	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL)
	{
		CEGUI::Combobox *combo;
		CEGUI::Window *title;

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_Weapons_Combobox");
		combo->setEnabled(false);
		combo->setAlpha(0.5f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_Weapons_Combobox_title");
		title->setAlpha(0.5f);

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox");
		combo->setEnabled(false);
		combo->setAlpha(0.5f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox_title");
		title->setAlpha(0.5f);

		CGameSettings::ClientInstance->SetWeapons(false);
		CGameSettings::ClientInstance->SetCatchUp(false);
		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	}
	else
	{
		CEGUI::Combobox *combo;
		CEGUI::Window *title;

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_Weapons_Combobox");
		combo->setEnabled(true);
		combo->setAlpha(1.0f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_Weapons_Combobox_title");
		title->setAlpha(1.0f);

		combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox");
		combo->setEnabled(true);
		combo->setAlpha(1.0f);

		title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_CatchUp_Combobox_title");
		title->setAlpha(1.0f);

		CGameSettings::ClientInstance->SetWeapons( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	}


	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE)
	{
		CEGUI::Combobox *combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_LapCount_Combobox");
		combo->setEnabled(false);
		combo->setAlpha(0.5f);

		CEGUI::Window *title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_LapCount_Combobox_title");
		title->setAlpha(0.5f);
	}
	else
	{
		CEGUI::Combobox *combo = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_LapCount_Combobox");
		combo->setEnabled(true);
		combo->setAlpha(1.0f);

		CEGUI::Window *title = (CEGUI::Combobox*)winMgr.getWindow("SingleRace_LapCount_Combobox_title");
		title->setAlpha(1.0f);
	}


	CSingleRacePhase::Instance->SetVisibleOtherPlayers(OthersVisible);


	if ((CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_DEATHMATCH) !=
		(LastMapType == CGameSettings::MAPTYPE_SP_DEATHMATCH))
	{
		CMapProvider::Instance->LoadAllMapNames((String)"*" + CSingleRacePhase::Instance->MapExt);

		CSingleRacePhase::Instance->CurrentMapIndex = 0;
		CGameSettings::ClientInstance->MapName = "";

		if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
		{
			CGameSettings::ClientInstance->MapName =
				CMapProvider::Instance->MapNames[CSingleRacePhase::Instance->CurrentMapIndex] + CSingleRacePhase::Instance->MapExt;
			CSingleRacePhase::Instance->UpdateMapInfo();
		}
	}


	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

	return true;
}



bool CSingleRacePhase::OnPrevMap(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
	{
		--CSingleRacePhase::Instance->CurrentMapIndex;
		if (CSingleRacePhase::Instance->CurrentMapIndex < 0)
			CSingleRacePhase::Instance->CurrentMapIndex = CMapProvider::Instance->MapNames.GetItemsCount() - 1;

		CGameSettings::ClientInstance->MapName =
			CMapProvider::Instance->MapNames[CSingleRacePhase::Instance->CurrentMapIndex] + CSingleRacePhase::Instance->MapExt;

		CSingleRacePhase::Instance->UpdateMapInfo();
	}

	return true;
}



bool CSingleRacePhase::OnNextMap(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
	{
		++CSingleRacePhase::Instance->CurrentMapIndex;
		if (CSingleRacePhase::Instance->CurrentMapIndex == CMapProvider::Instance->MapNames.GetItemsCount())
			CSingleRacePhase::Instance->CurrentMapIndex = 0;

		CGameSettings::ClientInstance->MapName =
			CMapProvider::Instance->MapNames[CSingleRacePhase::Instance->CurrentMapIndex] + CSingleRacePhase::Instance->MapExt;

		CSingleRacePhase::Instance->UpdateMapInfo();
	}

	return true;
}



bool CSingleRacePhase::OnUserChange(const CEGUI::EventArgs& e)
{
	CEGUI::WindowEventArgs *we;
	we = (CEGUI::WindowEventArgs*)&e;
	CEGUI::Combobox *Combo = (CEGUI::Combobox*) we->window;
	String s;
	int ComponentSelected;
	CEGUI::String cs;


	cs = Combo->getName();
	s = cs.c_str();
	s = s.Extract(' ', 1);
	ComponentSelected = s.AsInt();

    
	cs = Combo->getText();
	if (cs == CLocalizationSettings::Instance()->GetLocalText("OPEN")) 
	{		
		SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_OPEN;
		CGameClient::Instance->SendTableInfo(ComponentSelected);
	}
	else if (cs == CLocalizationSettings::Instance()->GetLocalText("CLOSED"))
	{
		SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_CLOSE;
		CGameClient::Instance->SendTableInfo(ComponentSelected);
	}
	else if (cs == CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("EASY") + ")") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIEASY);
	}
	else if (cs == CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("MEDIUM") + ")") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIMEDIUM);
	}
	else if (cs == CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("HARD") + ")") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIHARD);
	}


	return true;
}



bool CSingleRacePhase::OnStart(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CGameClient::Instance->StartGame();
	return true;
}



bool CSingleRacePhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);             
	CGameClient::Instance->Disconnect();
	CGameClient::Instance->ShutDown();
	CGameServer::Instance->StopHosting();
	CGameServer::Instance->ShutDown();

	return true;
}



bool CSingleRacePhase::OnWeaponCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_Weapons_Combobox")->getText();
	CGameSettings::ClientInstance->SetWeapons( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

	return true;
}



bool CSingleRacePhase::OnCratesCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_Crates_Combobox")->getText();
	CGameSettings::ClientInstance->SetCrates( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
    return true;
}



bool CSingleRacePhase::OnUnlimitedSpeedCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_UnlimitedSpeed_Combobox")->getText();
	CGameSettings::ClientInstance->SetUnlimitedSpeed( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	return true;
}



bool CSingleRacePhase::OnCatchUpCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_CatchUp_Combobox")->getText();
	CGameSettings::ClientInstance->SetCatchUp( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
	return true;
}



bool CSingleRacePhase::OnMoneyCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_Money_Combobox")->getText();
	CGameSettings::ClientInstance->StartingMoney = atoi(tmp.c_str());
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

    return true;
}



bool CSingleRacePhase::OnLapCountCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("SingleRace_LapCount_Combobox")->getText();
	CGameSettings::ClientInstance->LapCount = atoi(tmp.c_str());
	CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);
    return true;
}



void CSingleRacePhase::CreateRootComboboxes()
{
	// Team and user comboboxes
	Strings s, s2;
	s.Clear();
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("OPEN"));
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("EASY") + ")");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("MEDIUM") + ")");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("HARD") + ")");
	//s.PushBack(CLocalizationSettings::Instance()->GetLocalText("CLOSED"));

	s2.Clear();
	for (int i = 0; i < MAX_ACTIVE_PLAYERS; i++)
	{
		char tmp[100];
		sprintf(tmp, "%s %d", CLocalizationSettings::Instance()->GetLocalText("TEAM").GetString(), i+1);
		s2.PushBack(tmp);
	}
    
	float y = 0.18f;
	CEGUI::Window *ColorBack, *Color;
	CEGUI::Combobox *combo;

	for (int i = 0; i < MAX_ACTIVE_PLAYERS; i++)
	{        
		char tmp[100];
		sprintf(tmp, "SingleRace_PlayersCombo %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.02f, y, 0.25f, 0.148f, &s, "SingleRace_RootWindow", &OnUserChange);

		sprintf(tmp, "SingleRace_Teams %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.28f, y, 0.12f, 0.18f, &s2, "SingleRace_RootWindow", &OnTeamChange);
		sprintf(tmp, "%s %d", CLocalizationSettings::Instance()->GetLocalText("TEAM").GetString(), i+1);
		combo->setText(tmp);

		sprintf(tmp, "SingleRace_TeamColors_Back %d", i);
		ColorBack = CGUIHelper::CreateStaticColor(tmp, WeColor(0xFF000000), 0.41f, y, 0.05f, 0.03f, "SingleRace_RootWindow");

		sprintf(tmp, "SingleRace_TeamColors %d", i);
		Color = CGUIHelper::CreateStaticColor(tmp, CGameSettings::TeamColors[i], 0.41f, y, 0.05f, 0.03f, "SingleRace_RootWindow");
		Color->setWindowXPosition(CEGUI::UDim(ColorBack->getWindowXPosition().d_scale, ColorBack->getWindowXPosition().d_offset + 1));
		Color->setWindowYPosition(CEGUI::UDim(ColorBack->getWindowYPosition().d_scale, ColorBack->getWindowYPosition().d_offset + 1));
		Color->setWindowWidth(CEGUI::UDim(ColorBack->getWindowWidth().d_scale, ColorBack->getWindowWidth().d_offset - 2));
		Color->setWindowHeight(CEGUI::UDim(ColorBack->getWindowHeight().d_scale, ColorBack->getWindowHeight().d_offset - 2));

		y += 0.035f;
	}

}



void CSingleRacePhase::CreateOptionsComboboxes()
{
	Strings Params;
	CEGUI::Combobox *pCombobox;


	CGUIHelper::CreateText("SingleRace_RaceType_Title",CLocalizationSettings::Instance()->GetLocalText("MAP TYPE"),0.06f,0.1f,0.1f,0.02f,"","", "", "SingleRace_Options_Window");

	Params.Clear(); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE")); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("TIME TRIAL"));
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("LASTONE"));
	//Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"));

	CGUIHelper::CreateCombobox("SingleRace_RaceType_Combobox",0.16f, 0.1f,0.25f,0.13f,&Params, "SingleRace_Options_Window", &OnRaceTypeCombobox);


	Params.Clear(); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("ON")); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));


	CGUIHelper::CreateText("SingleRace_Weapons_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("WEAPONS"),0.06f,0.3f,0.22f,0.1f,"","","","SingleRace_Options_Window");
	CGUIHelper::CreateCombobox("SingleRace_Weapons_Combobox",0.28f,0.3f,0.2f,0.17f,&Params, "SingleRace_Options_Window", &OnWeaponCombobox);


	CGUIHelper::CreateText("SingleRace_Crates_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("CRATES"),0.06f, 0.4f, 0.22f, 0.1f,"", "","", "SingleRace_Options_Window")->setAlpha(0.5f);
	pCombobox = CGUIHelper::CreateCombobox("SingleRace_Crates_Combobox",0.28f,0.4f,0.2f,0.15f,&Params,"SingleRace_Options_Window", &OnCratesCombobox);
	pCombobox->setEnabled(false);
	pCombobox->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	pCombobox->setProperty("Alpha","0.5");


	CGUIHelper::CreateText("SingleRace_UnlimitedSpeed_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("UNLIMITED_SPEED"),0.06f, 0.45f, 0.22f, 0.1f,"", "","", "SingleRace_Options_Window")->setAlpha(0.5f);
	pCombobox = CGUIHelper::CreateCombobox("SingleRace_UnlimitedSpeed_Combobox",0.28f,0.45f,0.6f,0.15f,&Params,"SingleRace_Options_Window", &OnUnlimitedSpeedCombobox);
	pCombobox->setEnabled(false);
	pCombobox->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	pCombobox->setProperty("Alpha","0.5");


	CGUIHelper::CreateText("SingleRace_CatchUp_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("CATCHUP"),0.06f, 0.49f, 0.22f, 0.1f,"", "","", "SingleRace_Options_Window");
	CGUIHelper::CreateCombobox("SingleRace_CatchUp_Combobox",0.28f,0.49f,0.2f,0.15f,&Params,"SingleRace_Options_Window", &OnCatchUpCombobox);


	Params.Clear();
	//for (int i = 0; i < MAX_CASH_CATEGORIES; i++) // samo za ve-ce-ge
	for (int i = 0; i < 1; i++)
	{
		char tmp[10];
		itoa(CGameSettings::CashCategories[i], tmp, 10);
		Params.PushBack(tmp);
	}	

	CGUIHelper::CreateText("SingleRace_Money_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("MONEY"),0.48f, 0.4f, 0.22f, 0.1f, "","","","SingleRace_Options_Window");
	CGUIHelper::CreateCombobox("SingleRace_Money_Combobox",0.72f,0.4f,0.2f,0.1f,&Params,"SingleRace_Options_Window", &OnMoneyCombobox);


	Params.Clear();
	for (int i = 0; i < MAX_LAP_PRESETS; i++)
	{
		char tmp[10];
		itoa(CGameSettings::LapCountPresets[i], tmp, 10);
		Params.PushBack(tmp);
	}

	CGUIHelper::CreateText("SingleRace_LapCount_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("LAP COUNT"),0.48f,0.3f,0.22f,0.1f,"","","","SingleRace_Options_Window");
	CGUIHelper::CreateCombobox("SingleRace_LapCount_Combobox",0.72f,0.3f,0.2f,0.6f,&Params,"SingleRace_Options_Window", &OnLapCountCombobox);
}
