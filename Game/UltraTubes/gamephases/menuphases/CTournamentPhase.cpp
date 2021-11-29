/*
    CTournamentPhase.cpp

	Autor: Vuk Vranjkovic
	Date: 10.3.2006.
	Version: 1.0
    
*/


#include <CMainMenuPhase.h>
#include <CShoppingPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CGUIHelper.h>
#include <CString.h>
#include <CGameServer.h>
#include <CGameClient.h>
#include <CProfile.h>
#include <CProfileManager.h>
#include <CTournamentManager.h>
#include <CTournamentPhase.h>
#include <WeWorld.h>
#include <WeTexture.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <WeTimer.h>
#include <WeRenderDevice.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>



CTournamentPhase *CTournamentPhase::Instance = new CTournamentPhase;


CTournamentPhase::CTournamentPhase()
{
	GameStarted = false;
	MapDescImage = NULL;
}

void CTournamentPhase::Start()
{
	MapDescImage = NULL;
	//BackEnters = false;
	//BackLeaves = false;
	//TEnters = false;
	//TLeaves = false;

	WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;

	for(int i=0; i<5;  i++)
		ScrollerPictures[i] = NULL;

	SelectedMapIndex = -1;
	IndexChange = 0;
	MapImagesList.SetCircular(true);

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	CGUIHelper::CreateWnd("SinglePlayer_Tournament_Window", CLocalizationSettings::Instance()->GetLocalText("TOURNAMENT DESCRIPTION"),
		0.0f, 0.0f, 0.15f, 0.5f, 0.5f,"RootWindow");
	Mng.getWindow("SinglePlayer_Tournament_WindowTitle")->setFont("Font_16");
	CGUIHelper::CreateWnd("SinglePlayer_RaseDescription_Window", CLocalizationSettings::Instance()->GetLocalText("RACE DESCRIPTION"),
		0.0f, 0.5f, 0.15f, 0.5f, 0.5f,"RootWindow");
	Mng.getWindow("SinglePlayer_RaseDescription_WindowTitle")->setFont("Font_16");

	CGUIHelper::CreateText("TOURNAMENT_TEXT", CLocalizationSettings::Instance()->GetLocalText("TOURNAMENT"), 0.15f, 0.03f, 0.5f, 0.05f, "Font_24", "", "", "");

	CGUIHelper::CreateButton("SinglePlayer_Tournament_BackButton",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.65f,0.7f,0.35f,0.1f, "RootWindow", &CTournamentPhase::OnBack);

	CGUIHelper::CreateButton("SinglePlayer_Tournament_EnterButton",CLocalizationSettings::Instance()->GetLocalText("ENTER TOURNAMENT"),0.65f,0.8f,0.35f,0.1f, "RootWindow", &CTournamentPhase::OnTournament);
	
	CEGUI::Window* Scroller = CGUIHelper::CreateStaticImage("MapBrowserImageList","ImageScroller",0.1f,0.7f,0.45f,0.2f, "RootWindow");
	Scroller->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CTournamentPhase::OnScrollerClick,this));
	
	CEGUI::Window* LeftB;
	CEGUI::Window* RightB;
	LeftB = CGUIHelper::CreateStaticImage("MapBrowserLeft","_ButtonLeftNormal",0.03f,0.76f,0.07f,0.08f, "RootWindow");
	LeftB->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CTournamentPhase::OnMoveLeft,this));
	RightB = CGUIHelper::CreateStaticImage("MapBrowserRight","_ButtonRightNormal",0.55f,0.76f,0.07f,0.08f, "RootWindow");
	RightB->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CTournamentPhase::OnMoveRight,this));
	

	CProfile* Profil;
	Profil = CProfileManager::Instance()->GetCurrentProfile();
	Strings Params;

	Profil->Tournaments.First();


	CTournament* tmpTournament;
	for(Profil->Tournaments.First(); !Profil->Tournaments.End(); Profil->Tournaments.Next())
	{
		if(Profil->Tournaments.GetItem().Opened)
		{
			tmpTournament = &Profil->Tournaments.GetItem();
			if(CheckMaps(tmpTournament))
				Params.PushBack(Profil->Tournaments.GetItem().Name);
		}
	}


	CEGUI::Combobox *pCombobox = CGUIHelper::CreateCombobox("SinglePlayer_Tournament_Combobox", 0.2f, 0.1f, 0.2f, 0.2f, &Params, "", &OnTournamentChange);

	CGUIHelper::CreateText("SinglePlayer_Tournament_Combobox_Title",CLocalizationSettings::Instance()->GetLocalText("TOURNAMENT"), 0.05f, 0.105f, 0.2f, 0.1f, "", "", "", "");

	CGUIHelper::CreateText("MapStateTitle",CLocalizationSettings::Instance()->GetLocalText("RACE NAME"), 0.5f, 0.3f, 0.5f, 0.1f, "","","","SinglePlayer_RaseDescription_Window"); 
	CGUIHelper::CreateText("MapStateName",CLocalizationSettings::Instance()->GetLocalText("NO SELECTED MAP"), 0.5f, 0.35f, 0.5f, 0.1f, "","","","SinglePlayer_RaseDescription_Window");
	CGUIHelper::CreateText("RaceDesc","", 0.05f, 0.6f, 0.9f, 0.4f, "","","","SinglePlayer_RaseDescription_Window"); 
	
	if(Params.GetItemsCount() > 0)
	{
		CEGUI::EventArgs e;
		OnTournamentChange(e);
	}
	return;
}


bool CTournamentPhase::CheckMaps(CTournament* chkTournament)
{
	for(chkTournament->Maps.First(); !chkTournament->Maps.End(); chkTournament->Maps.Next())
	{
		if(chkTournament->Maps.GetItem().Opened) return true;
	}

	return false;
}


void CTournamentPhase::End()
{
	MapImagesList.SetCircular(false);
	for(MapImagesList.First(); !MapImagesList.End(); MapImagesList.Next())
	{
		WeTextureManager::Instance()->Free(MapImagesList.GetItem());
	}
	MapImagesList.Clear();
	MapNames.Clear();
	RaceDesc.Clear();

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("SinglePlayer_Tournament_Window")->destroy();
	Mng.getWindow("SinglePlayer_RaseDescription_Window")->destroy();
	Mng.getWindow("SinglePlayer_Tournament_BackButton")->destroy();
	Mng.getWindow("SinglePlayer_Tournament_EnterButton")->destroy();
	Mng.getWindow("TOURNAMENT_TEXT")->destroy();
	Mng.getWindow("SinglePlayer_Tournament_Combobox")->destroy();
	Mng.getWindow("SinglePlayer_Tournament_Combobox_Title")->destroy();
	Mng.getWindow("MapBrowserImageList")->destroy();
	Mng.getWindow("MapBrowserLeft")->destroy();
	Mng.getWindow("MapBrowserRight")->destroy();
}

void CTournamentPhase::Tick()
{
	if(CGameClient::Instance->GetStatus() == GC_JOINED)
	{
		CGameClient::Instance->StartGame();
		CMenuPhase::Instance->SetPhase(CShoppingPhase::Instance);
		*CGameSettings::ServerInstance = *CGameSettings::ClientInstance;
	}

	UpdateScroller();

	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}

void CTournamentPhase::Render()
{
	CMenuPhase::Instance->SaveStates();
	CMenuPhase::Instance->MenuWorld.Render((WeCamera*)CMenuPhase::Instance->MenuCamera);
	
	int i=0;
	if(MapImagesList.GetItemsCount() > 0)
	{
		MapImagesList.First();
		while(i < 5)
		{
			if(i < MapImagesList.GetItemsCount())
			{
				WeCanvas::Instance()->DrawRectangle((int)(130*WOffset+(75*WOffset*i)+10*WOffset*i),(int)(580*HOffset),
					(int)(75*WOffset),(int)(75*HOffset),D3DCOLOR_RGBA(255,255,255,255),MapImagesList.GetItem());
				MapImagesList.Next();
			}
			i++;
		}
	}
	
	CMenuPhase::Instance->SaveStates(false);
	CEGUI::System::getSingleton().renderGUI();
	if(MapDescImage)
	{
		float WOffset = WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
		float HOffset = WeRenderDevice::Instance()->GetResHeight() / 768.0f;
		WeCanvas::Instance()->DrawRectangle(500,200,200,200,D3DCOLOR_XRGB(255,255,255),MapDescImage);
		CEGUI::MouseCursor::getSingleton().draw();
	}
}

bool CTournamentPhase::OnBack(const CEGUI::EventArgs &e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
    return true;
}


bool CTournamentPhase::OnTournament(const CEGUI::EventArgs &e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CTournamentPhase::Instance->SelectedMapIndex < 0) return true;

	CTournament *tmpTournament;
	CTournamentMapSettings tmpMapSettings;


	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
	String TournamentName = Mng.getWindow("SinglePlayer_Tournament_Combobox")->getText().c_str();
	String MapName;
	
	CTournamentPhase::Instance->MapNames.SetCircular(true);
	int i=0;
	while(i < CTournamentPhase::Instance->SelectedMapIndex)
	{
		CTournamentPhase::Instance->MapNames.Next();
		i++;
	}
	CTournamentPhase::Instance->MapNames.SetCircular(false);
	
	MapName = CTournamentPhase::Instance->MapNames.GetItem();

	for(CProfileManager::Instance()->GetCurrentProfile()->Tournaments.First(); !CProfileManager::Instance()->GetCurrentProfile()->Tournaments.End(); CProfileManager::Instance()->GetCurrentProfile()->Tournaments.Next())
	{
		if(CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem().Name == TournamentName)
		{
			tmpTournament = &CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem();
		}
	}

	CGameSettings::ClientInstance->MapSettings = NULL;
	int tmpCount = 0;
	for(tmpTournament->Maps.First(); !tmpTournament->Maps.End(); tmpTournament->Maps.Next())
	{
		if(tmpTournament->Maps.GetItem().StateName == MapName)
		{
			CGameSettings::ClientInstance->MapSettings = &tmpTournament->Maps.GetItem();
			break;
		}
	}


	CGameSettings::ClientInstance->MapType = CGameSettings::ClientInstance->MAPTYPE_SP_TOURNAMENT;
	CGameSettings::ClientInstance->MapName = CGameSettings::ClientInstance->MapSettings->MapName + ".utm";
	CGameSettings::ClientInstance->SetWeapons(CGameSettings::ClientInstance->MapSettings->Weapons);
	CGameSettings::ClientInstance->LapCount = CGameSettings::ClientInstance->MapSettings->LapCount;

	
	CGameServer::Instance->Initialize();
	CGameServer::Instance->StartHosting();

	CGameClient::Instance->Initialize();	
	CGameClient::Instance->CreateGame(NULL, "", true);

	CTournamentPhase::Instance->GameStarted = true;


	return true;
}


bool CTournamentPhase::OnTournamentChange(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
	CTournament *tmpTournament;
	CTournamentMapSettings tmpMapSettings;
	CTournamentPhase::Instance->IndexChange = 0;
	CTournamentPhase::Instance->SelectedMapIndex = -1;
	Mng.getWindow("MapStateName")->setText(CLocalizationSettings::Instance()->GetLocalText("NO SELECTED MAP").GetString());
	Mng.getWindow("RaceDesc")->setText("");

	String TournamentName = Mng.getWindow("SinglePlayer_Tournament_Combobox")->getText().c_str();
	
	for(CProfileManager::Instance()->GetCurrentProfile()->Tournaments.First(); !CProfileManager::Instance()->GetCurrentProfile()->Tournaments.End(); CProfileManager::Instance()->GetCurrentProfile()->Tournaments.Next())
	{
		if(CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem().Name == TournamentName)
			tmpTournament = &CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem();
	}
	
	
	CTournamentPhase::Instance->MapImagesList.Clear();
	CTournamentPhase::Instance->MapNames.Clear();
	CTournamentPhase::Instance->RaceDesc.Clear();
	for(tmpTournament->Maps.First(); !tmpTournament->Maps.End(); tmpTournament->Maps.Next())
	{
		if(tmpTournament->Maps.GetItem().Opened)
		{
			CTournamentPhase::Instance->MapImagesList.PushBack(WeTextureManager::Instance()->Load(tmpTournament->Maps.GetItem().MapDescriptionImage,true));
			CTournamentPhase::Instance->MapNames.PushBack(tmpTournament->Maps.GetItem().StateName);	
			CTournamentPhase::Instance->RaceDesc.PushBack(tmpTournament->Maps.GetItem().MapDescription);
			CTournamentPhase::Instance->MapDescImage = WeTextureManager::Instance()->Load(tmpTournament->Maps.GetItem().MapDescriptionImage,true);
		}
	}

	return true;
}


void CTournamentPhase::UpdateScroller()
{
	if(MapImagesList.GetItemsCount() == 0) return;

	MapImagesList.SetCircular(false);
	MapImagesList.First();
	MapImagesList.SetCircular(true);

	MapNames.First();
	RaceDesc.First();
	MapNames.SetCircular(true);
	RaceDesc.SetCircular(true);
	if(IndexChange < 0)
	{
		int i=0;
		while(i > IndexChange)
		{
			MapImagesList.Prev();
			MapNames.Prev();
			RaceDesc.Prev();
			i--;
		}
	}

	if(IndexChange > 0)
	{
		int i=0;
		while(i < IndexChange)
		{
			MapImagesList.Next();
			MapNames.Next();
			RaceDesc.Next();
			i++;
		}
	}
	MapNames.SetCircular(false);
	RaceDesc.SetCircular(false);
}


bool CTournamentPhase::OnLButtonMouseMove(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	(Mng.getWindow("MapBrowserLeft"))->setProperty("Image", "set:TaharezLook image:_ButtonLeftHover");
	
	return true;
}


bool CTournamentPhase::OnLButtonMouseLeave(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	(Mng.getWindow("MapBrowserLeft"))->setProperty("Image", "set:TaharezLook image:_ButtonLeftNormal");
	
	return true;
}


bool CTournamentPhase::OnMoveLeft(const CEGUI::EventArgs &e)
{
	if(MapImagesList.GetItemsCount() < 6) return true;

	IndexChange--;
	return true;
}


bool CTournamentPhase::OnRButtonMouseMove(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	(Mng.getWindow("MapBrowserRight"))->setProperty("Image", "set:TaharezLook image:_ButtonRightHover");
	
	return true;
}


bool CTournamentPhase::OnRButtonMouseLeave(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	(Mng.getWindow("MapBrowserRight"))->setProperty("Image", "set:TaharezLook image:_ButtonRightNormal");
	
	return true;
}


bool CTournamentPhase::OnMoveRight(const CEGUI::EventArgs &e)
{
	if(MapImagesList.GetItemsCount() < 6) return true;

	IndexChange++;
	return true;
}


bool CTournamentPhase::OnScrollerClick(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	CEGUI::Point tmpP = CEGUI::MouseCursor::getSingleton().getPosition();
	SelectedMapIndex = -1;

	if(tmpP.d_x > 130*WOffset && tmpP.d_x < 205*WOffset)
		SelectedMapIndex = 0;
	if(tmpP.d_x > 215*WOffset && tmpP.d_x < 290*WOffset)
		SelectedMapIndex = 1;
	if(tmpP.d_x > 300*WOffset && tmpP.d_x < 375*WOffset)
		SelectedMapIndex = 2;
	if(tmpP.d_x > 385*WOffset && tmpP.d_x < 450*WOffset)
		SelectedMapIndex = 3;
	if(tmpP.d_x > 460*WOffset && tmpP.d_x < 535*WOffset)
		SelectedMapIndex = 4;
	
	MapNames.SetCircular(true);
	RaceDesc.SetCircular(true);
	if(SelectedMapIndex > -1 && SelectedMapIndex < MapNames.GetItemsCount())
	{
		int i=0;
		while(i < SelectedMapIndex)
		{
			MapNames.Next();
			RaceDesc.Next();
			i++;
		}
		Mng.getWindow("MapStateName")->setText(MapNames.GetItem().GetString());
		Mng.getWindow("RaceDesc")->setText(RaceDesc.GetItem().GetString());
	}else{
		SelectedMapIndex = -1;
		Mng.getWindow("MapStateName")->setText(CLocalizationSettings::Instance()->GetLocalText("NO SELECTED MAP").GetString());
		Mng.getWindow("RaceDesc")->setText("");
	}
	MapNames.SetCircular(false);
	RaceDesc.SetCircular(false);

	return false;
}

