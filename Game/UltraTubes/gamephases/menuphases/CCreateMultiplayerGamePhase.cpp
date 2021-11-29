// zamenjeno sa CMultiRacePhase.cpp

/*
    CCreateMultiplayerGamePhase.cpp

	Autor: Vuk Vranjkovic
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CCreateMultiplayerGamePhase.h>
#include <CGame.h>
#include <CMenuPhase.h> 
#include <CGUIHelper.h>
#include <CGameSettings.h>
#include <CProfileManager.h>
#include <CGameServer.h>
#include <CGamePlayerConfig.h>
#include <CLANServerBrowsePhase.h>
#include <CInternetServerBrowsePhase.h>
#include <CGameClient.h>
#include <CShoppingPhase.h>
#include <CAIProfileManager.h>
#include <WeTimer.h>
#include <CLocalizationSettings.h>



#define _ILLEGAL		255
#define _CLOSE			0
#define _AI				1
#define _OPEN			2
#define _PLAYER			3
#define ILLEGAL_UID		-60000

CCreateMultiplayerGamePhase *CCreateMultiplayerGamePhase::Instance = new CCreateMultiplayerGamePhase;

//*************************** Helpers functions *****************************************
inline String _bool_to_str(bool b) {return (b) ? String("Yes") : String("No");}
inline String _num_to_str(int num) {char buf[10]; return String(itoa(num,buf,10));}
inline String _type_to_str(CGameSettings::MAPTYPE t) {return (t==CGameSettings::MAPTYPE_MP_SINGLERACE) ? String("Single Race") : String("Tournament");}
inline String _utm_out(String map) {return map.Extract('.',0);}
//***************************************************************************************



CCreateMultiplayerGamePhase::CCreateMultiplayerGamePhase()
{
}



void CCreateMultiplayerGamePhase::Start()
{
	 bAdmin= CGameClient::Instance->IsAdmin();

	if (bAdmin)
	{
		CGUIHelper::CreateButton("MULTIPLAYER_START_BUTTON",CLocalizationSettings::Instance()->GetLocalText("START"),0.7f,0.85f,0.3f,0.1f, "RootWindow", &CCreateMultiplayerGamePhase::OnStart);
	}

	CGUIHelper::CreateButton("MULTIPLAYER_BACK_BUTTON",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.02f,0.85f,0.3f,0.1f, "RootWindow", &CCreateMultiplayerGamePhase::OnCancel);

	CGUIHelper::CreateWnd("Multiplayer_GameInfo_Window", CLocalizationSettings::Instance()->GetLocalText("GAME INFO"),
		0.0f,0.5f,0.1f,0.5f,0.5f,"RootWindow");
	CGUIHelper::CreateText("Multiplayer_GameInfo_HeaderText","Weapons \n\nCrates \n\nLaps    \n\nMoney    \n\nMap     \n\nType    ",
						   0.1f,0.3f,0.5f,1.0f,"", "", "","Multiplayer_GameInfo_Window");

	String Info = 
				_bool_to_str(CGameSettings::ClientInstance->GetWeapons()) + "\n\n"	
				+ _bool_to_str(CGameSettings::ClientInstance->GetCrates())+ "\n\n"
				+ _num_to_str(CGameSettings::ClientInstance->LapCount)+"\n\n" + 
				  _num_to_str(CGameSettings::ClientInstance->StartingMoney)+"\n\n"+
				  _utm_out(CGameSettings::ClientInstance->MapName)+"\n\n"+
				  _type_to_str(CGameSettings::ClientInstance->MapType)+"\n\n";

	CGUIHelper::CreateText("Multiplayer_GameInfo_InfoText",Info,0.5f,0.3f,0.5f,1.0f,"", "", "","Multiplayer_GameInfo_Window");
		
	CreateRootComboboxes();
}



void CCreateMultiplayerGamePhase::MakeTableFromSettings()
{
	char tmp[100];
	CEGUI::Combobox *UserCombo, *TeamCombo;
	CEGUI::Window *ColorImage;
	String s;

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();


	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		sprintf(tmp, "Multiplayer_PlayersCombo %d", i);
		UserCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "Multiplayer_Teams %d", i);
		TeamCombo = (CEGUI::Combobox *)Mng.getWindow(tmp);
		sprintf(tmp, "Multiplayer_TeamColors %d", i);
		ColorImage = (CEGUI::Window *)Mng.getWindow(tmp);

		UserCombo->setText(CLocalizationSettings::Instance()->GetLocalText("OPEN").GetString());

	
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
			CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE && 
			CGameClient::Instance->IsGameCreator())
		{
			TeamCombo->setEnabled(false);
			TeamCombo->setAlpha(0.5);
			ColorImage->setVisible(false);
			UserCombo->setEnabled(true);
			UserCombo->setAlpha(1.0f);			
		} else 
		{
			ColorImage->setVisible(true);
		}

		// ako nisam creator igre onda ne mogu da modifikujem nista ni tudje timove
		if (!CGameClient::Instance->IsGameCreator())
		{
			if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
				CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE)
			{
				ColorImage->setVisible(false);                
			}


			UserCombo->setEnabled(false);
			UserCombo->setAlpha(0.5);
			if (!CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{				
				TeamCombo->setEnabled(false);
				TeamCombo->setAlpha(0.5);
			} else
			{
				TeamCombo->setEnabled(true);
				TeamCombo->setAlpha(1);
			}

		} else 
		{
			// ako jesam game editor ne mogu da menjam timove ostalim igracima
			if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_HUMAN && !CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{
				TeamCombo->setEnabled(false);
				TeamCombo->setAlpha(0.5);

			} else if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_HUMAN && CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
			{
				UserCombo->setEnabled(false);
				UserCombo->setAlpha(0.5);
			} else if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIEASY ||
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
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " EASY)";
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIMEDIUM)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " MEDIUM)";
			UserCombo->setText(s.GetString());
		}
		if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_AIHARD)
		{
			s = CGameSettings::ClientInstance->tmpPlayers[i].User->Name + " (" + CLocalizationSettings::Instance()->GetLocalText("AI") + " HARD)";
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
			sprintf(tmp, "Team %d", TeamNumber);
			TeamCombo->setText(tmp);			

			CEGUI::colour col;
			col.set(CGameSettings::ClientInstance->TeamColors[TeamNumber-1].r, CGameSettings::ClientInstance->TeamColors[TeamNumber-1].g, CGameSettings::ClientInstance->TeamColors[TeamNumber-1].b);
			//ColorImage->setImageColours(col);
		}		
	}
}




void CCreateMultiplayerGamePhase::End()
{
    
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	if (bAdmin)
	{
		Mng.getWindow("MULTIPLAYER_START_BUTTON")->destroy();
	}

	Mng.getWindow("MULTIPLAYER_BACK_BUTTON")->destroy();
	Mng.getWindow("Multiplayer_GameInfo_Window")->destroy();

	char tmp[100];

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		sprintf(tmp, "Multiplayer_PlayersCombo %d", i);
		Mng.getWindow(tmp)->destroy();
		sprintf(tmp, "Multiplayer_Teams %d", i);
		Mng.getWindow(tmp)->destroy();
		sprintf(tmp, "Multiplayer_TeamColors %d", i);
		Mng.getWindow(tmp)->destroy();
	}

}




void CCreateMultiplayerGamePhase::CreateRootComboboxes()
{		
	// Team and user comboboxes
	Strings s, s2;
	s.Clear();
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("OPEN"));
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (EASY)");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (MEDIUM)");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (HARD)");
	s.PushBack(CLocalizationSettings::Instance()->GetLocalText("CLOSED"));

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		char tmp[100];
		sprintf(tmp, "Team %d", i+1);
		s2.PushBack(tmp);
	}
    
	CEGUI::Combobox *combo;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{        
		float y = 0.18f + i * 0.035f;

		char tmp[100];
		sprintf(tmp, "Multiplayer_PlayersCombo %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.02f, y, 0.25f, 0.14f, &s, "", &OnUserChange);

		s.Clear();
		s.PushBack(CLocalizationSettings::Instance()->GetLocalText("OPEN"));
		s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (EASY)");
		s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (MEDIUM)");
		s.PushBack(CLocalizationSettings::Instance()->GetLocalText("AI")+" (HARD)");
		s.PushBack(CLocalizationSettings::Instance()->GetLocalText("CLOSED"));

		sprintf(tmp, "Multiplayer_Teams %d", i);
		combo = CGUIHelper::CreateCombobox(tmp, 0.28f, y, 0.12f, 0.18f, &s2, "", &OnTeamChange);
		sprintf(tmp, "Team %d", i+1);
		combo->setText(tmp);

		sprintf(tmp, "Multiplayer_TeamColors %d", i);
		CGUIHelper::CreateStaticColor(tmp, CGameSettings::Instance->TeamColors[i], 0.41f, y, 0.05f, 0.032f, "RootWindow");
	}

}




bool CCreateMultiplayerGamePhase::OnTeamChange(const CEGUI::EventArgs &e)
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



bool  CCreateMultiplayerGamePhase::OnUserChange(const CEGUI::EventArgs &e)
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
	if (cs == CLocalizationSettings::Instance()->GetLocalText("CLOSED"))
	{
		SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_CLOSE;
		CGameClient::Instance->SendTableInfo(ComponentSelected);
	}
	if (cs == CLocalizationSettings::Instance()->GetLocalText("AI")+" (EASY)") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIEASY);
		//SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		//CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User	= CAIProfileManager::Instance->GetProfile(CGameSettings::Instance->StartingMoney);
		//if (CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User)
		//	CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_AIEASY;
	}
	if (cs == CLocalizationSettings::Instance()->GetLocalText("AI")+" (MEDIUM)") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIMEDIUM);
		//SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		//CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User	= CAIProfileManager::Instance->GetProfile(CGameSettings::Instance->StartingMoney);
		//if (CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User)
		//	CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_AIMEDIUM;
	}
	if (cs == CLocalizationSettings::Instance()->GetLocalText("AI")+" (HARD)") 
	{
		CGameClient::Instance->AddAI(ComponentSelected, PCFG_AIHARD);
		//SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User);
		//CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User	= CAIProfileManager::Instance->GetProfile(CGameSettings::Instance->StartingMoney);
		//if (CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].User)
		//	CGameSettings::ClientInstance->tmpPlayers[ComponentSelected].Status = PCFG_AIHARD;
	}

	//MakeTableFromSettings();
	return true;
}




void CCreateMultiplayerGamePhase::Tick()
{
	MakeTableFromSettings();

	if (CGameClient::Instance->GetStatus() == GC_IDLE)
	{
		// idi u prethodnu fazu 
		if(CGame::Instance->ServerInfo.LAN) CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
		else CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);
		CGameClient::Instance->Disconnect();	
		CGameClient::Instance->ShutDown();
		CGameServer::Instance->StopHosting();
		CGameServer::Instance->ShutDown();			
	}


	if (CGameClient::Instance->GetStatus() == GC_GAME_STARTED)
	{
		CMenuPhase::Instance->SetPhase(CShoppingPhase::Instance);
	}
	
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnCancel(e);
	}
}



void CCreateMultiplayerGamePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



/**********************************************************************************************
										GUI Callbacks
***********************************************************************************************/

bool CCreateMultiplayerGamePhase::OnCancel(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	if(CGame::Instance->ServerInfo.LAN) CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
	else CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);
	CGameServer::Instance->StopHosting();
	CGameServer::Instance->ShutDown();	
	CGameClient::Instance->Disconnect();	
	CGameClient::Instance->ShutDown();
	return true;
}



bool CCreateMultiplayerGamePhase::OnStart(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CGameClient::Instance->StartGame();
	return true;
}




