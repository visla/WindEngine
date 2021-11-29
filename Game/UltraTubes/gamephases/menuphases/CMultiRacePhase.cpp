/*
    CMultiRacePhase.cpp

	Autor: Alexa
	Date: 20.09.2006
*/


#include <CMultiRacePhase.h>
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
#include <CLANServerBrowsePhase.h>
#include <CInternetServerBrowsePhase.h>


using CEGUI::Point;
using CEGUI::Size;
using CEGUI::LeftAligned;
using CEGUI::Window;
using CEGUI::RightAligned;
using CEGUI::Event;
using CEGUI::PushButton;
using CEGUI::MultiColumnList;


float WaitTime;

CMultiRacePhase *CMultiRacePhase::Instance = new CMultiRacePhase;

CMultiRacePhase::CMultiRacePhase()
{
	//bPressed=false;
	GameStarted = false;
	MapDescImage = NULL;
}



void CMultiRacePhase::Start()
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	// Load all maps' headers
	MapExt = ".utm";
	CMapProvider::Instance->LoadAllMapNames((String)"*" + MapExt);


	// Create UI

	CGUIHelper::CreateWnd("MultiRace_RootWindow", CLocalizationSettings::Instance()->GetLocalText("SINGLE RACE"),
		0.0f, 0.05f, 0.05f, 0.4f, 0.9f, "RootWindow");

	CreateRootComboboxes();


	CGUIHelper::CreateWnd("MultiRace_MapDescription_Window", CLocalizationSettings::Instance()->GetLocalText("MAP DESCRIPTION"),
		0.0f, 0.48f, 0.0f, 0.5f, 0.5f, "RootWindow");

	CGUIHelper::CreateText("MultiRace_MapDescription","",0.5f,0.27f,0.5f,1.0f,
		"","","","MultiRace_MapDescription_Window");

	CGUIHelper::CreateText("MultiRace_MapName","",0.1f,0.9f,0.5f,0.1f,
		"","","","MultiRace_MapDescription_Window");

	CGUIHelper::CreateStaticImage("MultiRace_MapImage", "", 0.1f, 0.1f, 0.5f, 0.5f, "MultiRace_MapDescription_Window");

	CGUIHelper::CreateButton("MultiRace_PrevMap", CLocalizationSettings::Instance()->GetLocalText("PREV"), 0.1f, 0.9f, 0.1f, 0.05f, "MultiRace_MapDescription_Window", &OnPrevMap);

	CGUIHelper::CreateButton("MultiRace_NextMap", CLocalizationSettings::Instance()->GetLocalText("NEXT"), 0.8f, 0.9f, 0.1f, 0.05f, "MultiRace_MapDescription_Window", &OnNextMap);


	CGUIHelper::CreateWnd("MultiRace_Options_Window", CLocalizationSettings::Instance()->GetLocalText("GAME OPTIONS"),
		0.0f, 0.48f, 0.5f, 0.5f, 0.5f, "RootWindow");

	CreateOptionsComboboxes();


	CGUIHelper::CreateButton("MultiRace_Back_Button",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.35f, 0.65f,0.65f,0.14f,"MultiRace_Options_Window", &CMultiRacePhase::OnBack);

	CGUIHelper::CreateButton("MultiRace_Create_Button",CLocalizationSettings::Instance()->GetLocalText("CREATE"),0.35f,0.8f,0.65f, 0.14f,"MultiRace_Options_Window", &CMultiRacePhase::OnCreate);

	CGUIHelper::CreateButton("MultiRace_Start_Button",CLocalizationSettings::Instance()->GetLocalText("START"),0.35f,0.8f,0.65f, 0.14f,"MultiRace_Options_Window", &CMultiRacePhase::OnStart);
	Mng.getWindow("MultiRace_Start_Button")->setVisible(false);


	// Setup Server/Client

	if (MultiRaceState == MRS_CHOOSEMAP)
	{
		CurrentMapIndex = 0;
		CGameSettings::ClientInstance->MapName = CMapProvider::Instance->MapNames[CurrentMapIndex] + MapExt;
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_SINGLERACE;
		CGameSettings::ClientInstance->SetWeapons(true);
		CGameSettings::ClientInstance->SetCrates(false);
		CGameSettings::ClientInstance->StartingMoney = CGameSettings::CashCategories[0];
		CGameSettings::ClientInstance->LapCount = 10;
	}
	else // MRS_JOINING
	{
		Mng.getWindow("MultiRace_Create_Button")->setVisible(false);

		Mng.getWindow("MultiRace_PrevMap")->setEnabled(false);
		Mng.getWindow("MultiRace_PrevMap")->setAlpha(0.5f);
		Mng.getWindow("MultiRace_NextMap")->setEnabled(false);
		Mng.getWindow("MultiRace_NextMap")->setAlpha(0.5f);

		Mng.getWindow("MultiRace_RaceType_Combobox")->setEnabled(false);
		Mng.getWindow("MultiRace_RaceType_Combobox")->setAlpha(0.5f);
		Mng.getWindow("MultiRace_Weapons_Combobox")->setEnabled(false);
		Mng.getWindow("MultiRace_Weapons_Combobox")->setAlpha(0.5f);
		//Mng.getWindow("MultiRace_Crates_Combobox")->setEnabled(false);
		//Mng.getWindow("MultiRace_Crates_Combobox")->setAlpha(0.5f);
		Mng.getWindow("MultiRace_LapCount_Combobox")->setEnabled(false);
		Mng.getWindow("MultiRace_LapCount_Combobox")->setAlpha(0.5f);
		Mng.getWindow("MultiRace_Money_Combobox")->setEnabled(false);
		Mng.getWindow("MultiRace_Money_Combobox")->setAlpha(0.5f);
	}


	// Update UI

	UpdateMapInfo();

	switch (CGameSettings::ClientInstance->MapType)
	{
	case CGameSettings::MAPTYPE_MP_SINGLERACE:
		Mng.getWindow("MultiRace_RaceType_Combobox")->setText(CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE").GetString());
		break;

	case CGameSettings::MAPTYPE_MP_LASTONE:
		Mng.getWindow("MultiRace_RaceType_Combobox")->setText(CLocalizationSettings::Instance()->GetLocalText("LASTONE").GetString());
		break;

	case CGameSettings::MAPTYPE_MP_DEATHMATCH:
		Mng.getWindow("MultiRace_RaceType_Combobox")->setText(CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH").GetString());
		break;
	}

	Mng.getWindow("MultiRace_Weapons_Combobox")->setText(
		(CGameSettings::ClientInstance->GetWeapons() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
	);

	Mng.getWindow("MultiRace_Crates_Combobox")->setText(
		(CGameSettings::ClientInstance->GetCrates() ?
		CLocalizationSettings::Instance()->GetLocalText("ON") : 
		CLocalizationSettings::Instance()->GetLocalText("OFF")).GetString()
	);

	char tmp[100];

	sprintf(tmp, "%d", CGameSettings::ClientInstance->StartingMoney);
	Mng.getWindow("MultiRace_Money_Combobox")->setText(tmp);

	sprintf(tmp, "%d", CGameSettings::ClientInstance->LapCount);
	Mng.getWindow("MultiRace_LapCount_Combobox")->setText(tmp);
}




void CMultiRacePhase::MakeTableFromSettings()
{
	char tmp[100];
	CEGUI::Combobox *UserCombo, *TeamCombo;
	CEGUI::Window *ColorImage, *ColorBackImage;
	String s;

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();


	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		sprintf(tmp, "MultiRace_PlayersCombo %d", i);
		UserCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "MultiRace_Teams %d", i);
		TeamCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "MultiRace_TeamColors_Back %d", i);
		ColorBackImage = (CEGUI::Window *)Mng.getWindow(tmp);
		sprintf(tmp, "MultiRace_TeamColors %d", i);
		ColorImage = (CEGUI::Window *)Mng.getWindow(tmp);

		UserCombo->setText(CLocalizationSettings::Instance()->GetLocalText("OPEN").GetString());

		if ((MultiRaceState == MRS_CHOOSEMAP) || (MultiRaceState == MRS_HOSTING))
		{
			UserCombo->setEnabled(false);
			UserCombo->setAlpha(0.5f);
			TeamCombo->setEnabled(false);
			TeamCombo->setAlpha(0.5f);
			ColorBackImage->setVisible(false);
			ColorImage->setVisible(false);
		}
		else
		{
			if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE && 
				CGameClient::Instance->IsGameCreator())
			{
				UserCombo->setEnabled(true);
				UserCombo->setAlpha(1.0f);
				TeamCombo->setEnabled(false);
				TeamCombo->setAlpha(0.5);
				ColorBackImage->setVisible(false);
				ColorImage->setVisible(false);
			}
			else 
			{
				ColorBackImage->setVisible(true);
				ColorImage->setVisible(true);
			}

			// ako nisam creator igre onda ne mogu da modifikujem nista ni tudje timove
			if (!CGameClient::Instance->IsGameCreator())
			{
				if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
					CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE)
				{
					ColorBackImage->setVisible(false);
					ColorImage->setVisible(false);
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
}



bool CMultiRacePhase::OnTeamChange(const CEGUI::EventArgs &e)
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



void CMultiRacePhase::End()
{
	//CreateProfiles();
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("MultiRace_RootWindow")->destroy();
	Mng.getWindow("MultiRace_MapDescription_Window")->destroy();
	Mng.getWindow("MultiRace_Options_Window")->destroy();

	if (Mng.isWindowPresent("HOSTGAME_CANCEL_BUTTON"))
		Mng.getWindow("HOSTGAME_CANCEL_BUTTON")->destroy();
}



void CMultiRacePhase::Tick()
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	MakeTableFromSettings();
	CheckLastOne();

	switch (MultiRaceState)
	{

	case MRS_CHOOSEMAP:
		if (CGame::Instance->InputMessage.Exit)
		{
			CEGUI::EventArgs e;
			OnBack(e);
		}
		break;

	case MRS_HOSTING:
		WaitTime -= WeTimer::Instance()->GetDeltaTimeF();
		if (CGameClient::Instance->GetStatus() == GC_JOINED && WaitTime < 0)
		{
			MultiRaceState = MRS_ACCEPTING;

			Mng.getWindow("HOSTGAME_CANCEL_BUTTON")->destroy();
			Mng.getWindow("MultiRace_Back_Button")->setVisible(true);
			Mng.getWindow("MultiRace_Start_Button")->setVisible(true);
		}

		if(CGame::Instance->InputMessage.Exit)
		{
			CEGUI::EventArgs e;
			OnCancel(e);
		}

		break;

	case MRS_ACCEPTING:
		if (CGame::Instance->InputMessage.Exit || (CGameClient::Instance->GetStatus() == GC_IDLE))
		{
			CEGUI::EventArgs e;
			OnBack(e);
		}

		else if (CGameClient::Instance->GetStatus() == GC_GAME_STARTED)
		{
			CMenuPhase::Instance->SetPhase(CShoppingPhase::Instance);
		}

		if(CGame::Instance->InputMessage.Exit)
		{
			CEGUI::EventArgs e;
			OnBack(e);
		}

		break;

	case MRS_JOINING:
		if (CGameClient::Instance->GetStatus() == GC_IDLE)
		{
			if (CGame::Instance->ServerInfo.LAN)
				CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
			else
				CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);

			CGameClient::Instance->Disconnect();	
			CGameClient::Instance->ShutDown();
			CGameServer::Instance->StopHosting();
			CGameServer::Instance->ShutDown();	
		}

		else if (CGameClient::Instance->GetStatus() == GC_GAME_STARTED)
		{
			CMenuPhase::Instance->SetPhase(CShoppingPhase::Instance);
		}

		if(CGame::Instance->InputMessage.Exit)
		{
			CEGUI::EventArgs e;
			OnBack(e);
		}

		break;

	}
}



void CMultiRacePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();

	if (MapDescImage)
	{
		CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
		CEGUI::Window *wnd = Mng.getWindow("MultiRace_MapImage");

		CEGUI::Rect rect = wnd->getPixelRect();
		WeCanvas::Instance()->DrawRectangle((int)rect.d_left, (int)rect.d_top, (int)rect.d_right - (int)rect.d_left, (int)rect.d_bottom - (int)rect.d_top, 0xFFFFFFFF, MapDescImage);

		CEGUI::MouseCursor::getSingleton().draw();
	}
}



void CMultiRacePhase::UpdateMapInfo()
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CMap TmpMap; 
	TmpMap.LoadMapHeader(CGameSettings::ClientInstance->MapName);
	(winMgr.getWindow("MultiRace_MapDescription"))->setText(TmpMap.MapHeader.MapDescription.GetString());
	(winMgr.getWindow("MultiRace_MapName"))->setText(CGameSettings::ClientInstance->MapName.Extract('.',0).GetString());

	CMultiRacePhase::Instance->MapDescImage = WeTextureManager::Instance()->Load(TmpMap.MapHeader.MapDescriptionImage,true);
}



void CMultiRacePhase::CheckLastOne()
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	if (winMgr.getWindow("MultiRace_Start_Button")->isVisible())
	{
		bool disable = false;

		if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_LASTONE)
		{
			int n = 0;

			for (int i = 0; i < MAX_PLAYERS; ++i)
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

		CEGUI::Window *btn = winMgr.getWindow("MultiRace_Start_Button");

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
}



bool CMultiRacePhase::OnRaceTypeCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::String tmp = winMgr.getWindow("MultiRace_RaceType_Combobox")->getText();
	CGameSettings::MAPTYPE LastMapType = CGameSettings::ClientInstance->MapType;
	CMultiRacePhase::Instance->MapExt = ".utm";


	if (tmp == CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_SINGLERACE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("LASTONE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_LASTONE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_DEATHMATCH;
		CMultiRacePhase::Instance->MapExt = "*.utd";
	}


	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_LASTONE)
	{
		CEGUI::Combobox *combo = (CEGUI::Combobox*)winMgr.getWindow("MultiRace_LapCount_Combobox");
		combo->setEnabled(false);
		combo->setAlpha(0.5f);

		CEGUI::Window *title = (CEGUI::Combobox*)winMgr.getWindow("MultiRace_LapCount_Combobox_title");
		title->setAlpha(0.5f);
	}
	else
	{
		CEGUI::Combobox *combo = (CEGUI::Combobox*)winMgr.getWindow("MultiRace_LapCount_Combobox");
		combo->setEnabled(true);
		combo->setAlpha(1.0f);

		CEGUI::Window *title = (CEGUI::Combobox*)winMgr.getWindow("MultiRace_LapCount_Combobox_title");
		title->setAlpha(1.0f);
	}


	if ((CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_DEATHMATCH) !=
		(LastMapType == CGameSettings::MAPTYPE_MP_DEATHMATCH))
	{
		CMapProvider::Instance->LoadAllMapNames((String)"*" + CMultiRacePhase::Instance->MapExt);

		CMultiRacePhase::Instance->CurrentMapIndex = 0;
		CGameSettings::ClientInstance->MapName = "";

		if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
		{
			CGameSettings::ClientInstance->MapName =
				CMapProvider::Instance->MapNames[CMultiRacePhase::Instance->CurrentMapIndex] + CMultiRacePhase::Instance->MapExt;

			CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

			CMultiRacePhase::Instance->UpdateMapInfo();
		}
	}


	return true;
}



bool CMultiRacePhase::OnPrevMap(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
	{
		--CMultiRacePhase::Instance->CurrentMapIndex;
		if (CMultiRacePhase::Instance->CurrentMapIndex < 0)
			CMultiRacePhase::Instance->CurrentMapIndex = CMapProvider::Instance->MapNames.GetItemsCount() - 1;

		CGameSettings::ClientInstance->MapName =
			CMapProvider::Instance->MapNames[CMultiRacePhase::Instance->CurrentMapIndex] + CMultiRacePhase::Instance->MapExt;

		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

		CMultiRacePhase::Instance->UpdateMapInfo();
	}

	return true;
}



bool CMultiRacePhase::OnNextMap(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMapProvider::Instance->MapNames.GetItemsCount() > 0)
	{
		++CMultiRacePhase::Instance->CurrentMapIndex;
		if (CMultiRacePhase::Instance->CurrentMapIndex == CMapProvider::Instance->MapNames.GetItemsCount())
			CMultiRacePhase::Instance->CurrentMapIndex = 0;

		CGameSettings::ClientInstance->MapName =
			CMapProvider::Instance->MapNames[CMultiRacePhase::Instance->CurrentMapIndex] + CMultiRacePhase::Instance->MapExt;

		CGameClient::Instance->SendGameSettings(CGameSettings::ClientInstance);

		CMultiRacePhase::Instance->UpdateMapInfo();
	}

	return true;
}



bool CMultiRacePhase::OnUserChange(const CEGUI::EventArgs& e)
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



bool CMultiRacePhase::OnCreate(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);

	CMultiRacePhase::Instance->MultiRaceState = MRS_HOSTING;
	WaitTime = 2.0f;

	CGUIHelper::CreateButton("HOSTGAME_CANCEL_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CANCEL"),0.6f,0.6f,0.3f,0.1f, "MultiRace_Options_Window", &OnCancel);
	Mng.getWindow("MultiRace_Create_Button")->setVisible(false);
	Mng.getWindow("MultiRace_Back_Button")->setVisible(false);

	Mng.getWindow("MultiRace_PrevMap")->setEnabled(false);
	Mng.getWindow("MultiRace_PrevMap")->setAlpha(0.5f);
	Mng.getWindow("MultiRace_NextMap")->setEnabled(false);
	Mng.getWindow("MultiRace_NextMap")->setAlpha(0.5f);

	Mng.getWindow("MultiRace_RaceType_Combobox")->setEnabled(false);
	Mng.getWindow("MultiRace_RaceType_Combobox")->setAlpha(0.5f);
	Mng.getWindow("MultiRace_Weapons_Combobox")->setEnabled(false);
	Mng.getWindow("MultiRace_Weapons_Combobox")->setAlpha(0.5f);
	//Mng.getWindow("MultiRace_Crates_Combobox")->setEnabled(false);
	//Mng.getWindow("MultiRace_Crates_Combobox")->setAlpha(0.5f);
	Mng.getWindow("MultiRace_LapCount_Combobox")->setEnabled(false);
	Mng.getWindow("MultiRace_LapCount_Combobox")->setAlpha(0.5f);
	Mng.getWindow("MultiRace_Money_Combobox")->setEnabled(false);
	Mng.getWindow("MultiRace_Money_Combobox")->setAlpha(0.5f);

	CGameServer::Instance->Initialize();
	CGameServer::Instance->StartHosting();
	CGameClient::Instance->Initialize();	
	CGameClient::Instance->CreateGame(&CGame::Instance->ServerInfo, "", false);

	return true;
}



bool CMultiRacePhase::OnStart(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CGameClient::Instance->StartGame();
	return true;
}



bool CMultiRacePhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);

	switch (CMultiRacePhase::Instance->MultiRaceState)
	{
	case MRS_CHOOSEMAP:
		if (CGame::Instance->ServerInfo.LAN)
			CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
		else
			CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);

		break;

	case MRS_ACCEPTING:
	case MRS_JOINING:
		if (CGame::Instance->ServerInfo.LAN)
			CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
		else
			CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);

		CGameClient::Instance->Disconnect();	
		CGameClient::Instance->ShutDown();
		CGameServer::Instance->StopHosting();
		CGameServer::Instance->ShutDown();	

		break;

	}

	return true;
}



bool CMultiRacePhase::OnCancel(const CEGUI::EventArgs &)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);

	if (CMultiRacePhase::Instance->MultiRaceState == MRS_HOSTING)
	{
		Mng.getWindow("HOSTGAME_CANCEL_BUTTON")->destroy();
		Mng.getWindow("MultiRace_Create_Button")->setVisible(true);
		Mng.getWindow("MultiRace_Back_Button")->setVisible(true);

		Mng.getWindow("MultiRace_PrevMap")->setEnabled(true);
		Mng.getWindow("MultiRace_PrevMap")->setAlpha(1.0f);
		Mng.getWindow("MultiRace_NextMap")->setEnabled(true);
		Mng.getWindow("MultiRace_NextMap")->setAlpha(1.0f);

		Mng.getWindow("MultiRace_RaceType_Combobox")->setEnabled(true);
		Mng.getWindow("MultiRace_RaceType_Combobox")->setAlpha(1.0f);
		Mng.getWindow("MultiRace_Weapons_Combobox")->setEnabled(true);
		Mng.getWindow("MultiRace_Weapons_Combobox")->setAlpha(1.0f);
		//Mng.getWindow("MultiRace_Crates_Combobox")->setEnabled(true);
		//Mng.getWindow("MultiRace_Crates_Combobox")->setAlpha(1.0f);
		Mng.getWindow("MultiRace_LapCount_Combobox")->setEnabled(true);
		Mng.getWindow("MultiRace_LapCount_Combobox")->setAlpha(1.0f);
		Mng.getWindow("MultiRace_Money_Combobox")->setEnabled(true);
		Mng.getWindow("MultiRace_Money_Combobox")->setAlpha(1.0f);

		CMultiRacePhase::Instance->MultiRaceState = MRS_CHOOSEMAP;
	}

	return true;
}



bool CMultiRacePhase::OnWeaponCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("MultiRace_Weapons_Combobox")->getText();
	CGameSettings::ClientInstance->SetWeapons( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	return true;
}



bool CMultiRacePhase::OnCratesCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("MultiRace_Crates_Combobox")->getText();
	CGameSettings::ClientInstance->SetCrates( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
    return true;
}



bool CMultiRacePhase::OnMoneyCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("MultiRace_Money_Combobox")->getText();
	CGameSettings::ClientInstance->StartingMoney = atoi(tmp.c_str());
    return true;
}



bool CMultiRacePhase::OnLapCountCombobox(const CEGUI::EventArgs &e)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("MultiRace_LapCount_Combobox")->getText();
	CGameSettings::ClientInstance->LapCount = atoi(tmp.c_str());
    return true;
}



void CMultiRacePhase::CreateRootComboboxes()
{
	// Team and user comboboxes
	Strings s, s2;
	s.Clear();
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("OPEN"));
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("EASY") + ")");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("MEDIUM") + ")");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI") + " (" + CLocalizationSettings::Instance()->GetLocalText("HARD") + ")");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("CLOSED"));

	s2.Clear();
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		char tmp[100];
		sprintf(tmp, "%s %d", CLocalizationSettings::Instance()->GetLocalText("TEAM").GetString(), i+1);
		s2.PushBack(tmp);
	}
    
	float y = 0.18f;
	CEGUI::Window *ColorBack, *Color;
	CEGUI::Combobox *combo;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{        
		char tmp[100];
		sprintf(tmp, "MultiRace_PlayersCombo %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.02f, y, 0.25f, 0.148f, &s, "MultiRace_RootWindow", &OnUserChange);

		sprintf(tmp, "MultiRace_Teams %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.28f, y, 0.12f, 0.18f, &s2, "MultiRace_RootWindow", &OnTeamChange);
		sprintf(tmp, "%s %d", CLocalizationSettings::Instance()->GetLocalText("TEAM").GetString(), i+1);
		combo->setText(tmp);

		sprintf(tmp, "MultiRace_TeamColors_Back %d", i);
		ColorBack = CGUIHelper::CreateStaticColor(tmp, WeColor(0xFF000000), 0.41f, y, 0.05f, 0.03f, "MultiRace_RootWindow");

		sprintf(tmp, "MultiRace_TeamColors %d", i);
		Color = CGUIHelper::CreateStaticColor(tmp, CGameSettings::TeamColors[i], 0.41f, y, 0.05f, 0.03f, "MultiRace_RootWindow");
		Color->setWindowXPosition(CEGUI::UDim(ColorBack->getWindowXPosition().d_scale, ColorBack->getWindowXPosition().d_offset + 1));
		Color->setWindowYPosition(CEGUI::UDim(ColorBack->getWindowYPosition().d_scale, ColorBack->getWindowYPosition().d_offset + 1));
		Color->setWindowWidth(CEGUI::UDim(ColorBack->getWindowWidth().d_scale, ColorBack->getWindowWidth().d_offset - 2));
		Color->setWindowHeight(CEGUI::UDim(ColorBack->getWindowHeight().d_scale, ColorBack->getWindowHeight().d_offset - 2));

		y += 0.035f;
	}

}



void CMultiRacePhase::CreateOptionsComboboxes()
{
	Strings Params;
	CEGUI::Combobox *pCombobox;


	CGUIHelper::CreateText("MultiRace_RaceType_Title",CLocalizationSettings::Instance()->GetLocalText("MAP TYPE"),0.06f,0.1f,0.1f,0.02f,"","", "", "MultiRace_Options_Window");

	Params.Clear(); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE")); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("LASTONE"));
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"));

	CGUIHelper::CreateCombobox("MultiRace_RaceType_Combobox",0.16f, 0.1f,0.25f,0.13f,&Params, "MultiRace_Options_Window", &OnRaceTypeCombobox);


	Params.Clear(); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("ON")); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));


	CGUIHelper::CreateText("MultiRace_Crates_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("CRATES"),0.06f, 0.4f, 0.22f, 0.1f,"", "","", "MultiRace_Options_Window")->setAlpha(0.5f);
	pCombobox = CGUIHelper::CreateCombobox("MultiRace_Crates_Combobox",0.28f,0.4f,0.2f,0.15f,&Params,"MultiRace_Options_Window", &OnCratesCombobox);
	pCombobox->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	pCombobox->setEnabled(false);
	pCombobox->setAlpha(0.5f);

	CGUIHelper::CreateText("MultiRace_Weapons_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("WEAPONS"),0.06f,0.3f,0.22f,0.1f,"","","","MultiRace_Options_Window");
	CGUIHelper::CreateCombobox("MultiRace_Weapons_Combobox",0.28f,0.3f,0.2f,0.17f,&Params, "MultiRace_Options_Window", &OnWeaponCombobox);


	Params.Clear();
	//for (int i = 0; i < MAX_CASH_CATEGORIES; i++) // samo za ve-ce-ge
	for (int i = 0; i < 1; i++)
	{
		char tmp[10];
		itoa(CGameSettings::CashCategories[i], tmp, 10);
		Params.PushBack(tmp);
	}	

	CGUIHelper::CreateText("MultiRace_Money_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("MONEY"),0.48f, 0.4f, 0.22f, 0.1f, "","","","MultiRace_Options_Window");
	CGUIHelper::CreateCombobox("MultiRace_Money_Combobox",0.72f,0.4f,0.2f,0.1f,&Params,"MultiRace_Options_Window", &OnMoneyCombobox);


	Params.Clear();
	for (int i = 0; i < MAX_LAP_PRESETS; i++)
	{
		char tmp[10];
		itoa(CGameSettings::LapCountPresets[i], tmp, 10);
		Params.PushBack(tmp);
	}

	CGUIHelper::CreateText("MultiRace_LapCount_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("LAP COUNT"),0.48f,0.3f,0.22f,0.1f,"","","","MultiRace_Options_Window");
	CGUIHelper::CreateCombobox("MultiRace_LapCount_Combobox",0.72f,0.3f,0.2f,0.6f,&Params,"MultiRace_Options_Window", &OnLapCountCombobox);
}
