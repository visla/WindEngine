/*
    CMainMenuPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CMainMenuPhase.h>
#include <CGame.h>
#include <CExitMenuPhase.h>
#include <CMenuPhase.h>
#include <CMultiPlayerPhase.h>
#include <CSinglePlayerPhase.h>
#include <COptionsPhase.h>
#include <COptionsVideoPhase.h>
#include <CCreditsPhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>
#include <CGUIHelper.h>
#include <CUberCreditsPhase.h>
#include <CChooseProfilePhase.h>



CMainMenuPhase *CMainMenuPhase::Instance = new CMainMenuPhase;


CMainMenuPhase::CMainMenuPhase()
{
    PrevPhase = NULL;
	CurrentPhase = NULL;
	SomethingChanged = false;
	MustRestartText = NULL;
	SingleMultiPhase = NULL;
}



CMainMenuPhase::~CMainMenuPhase()
{
	MustRestartText->destroy();
	MustRestartText = NULL;
}



void CMainMenuPhase::Start()
{
	CEGUI::Window* wnd;

	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	MainMenuWnd = (CEGUI::FrameWindow*)CGUIHelper::CreateWnd("MainMenu_Window", CLocalizationSettings::Instance()->GetLocalText("MAIN MENU"),
		0.0f, 0.1f, 0.13f, 0.4f, 0.8f,"RootWindow");

	CGUIHelper::CreateButton("MainMenu_SinglePlayer_Button", CLocalizationSettings::Instance()->GetLocalText("SINGLE PLAYER"),
		0.05f, 0.18f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnSinglePlayer);

	CGUIHelper::CreateButton("MainMenu_MultiPlayer_Button", CLocalizationSettings::Instance()->GetLocalText("MULTIPLAYER"),
		0.05f, 0.3f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnMultiPlayer);

	CGUIHelper::CreateButton("MainMenu_Options_Button", CLocalizationSettings::Instance()->GetLocalText("OPTIONS"),
		0.05f, 0.42f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnOptions);

	CGUIHelper::CreateButton("MainMenu_Profiles_Button", CLocalizationSettings::Instance()->GetLocalText("PROFILES"),
		0.05f, 0.54f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnProfiles);

	wnd = CGUIHelper::CreateButton("MainMenu_HighScore_Button", CLocalizationSettings::Instance()->GetLocalText("HIGH SCORE"),
		0.05f, 0.66f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnHighScore);
	wnd->setAlpha(0.5f);
	wnd->setEnabled(false);

	CGUIHelper::CreateButton("MainMenu_Credits_Button", CLocalizationSettings::Instance()->GetLocalText("CREDITS"),
		0.05f, 0.78f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnCredits);

	CGUIHelper::CreateButton("MainMenu_Exit_Button", CLocalizationSettings::Instance()->GetLocalText("EXIT"),
		0.05f, 0.90f, 1.0f, 0.12f, "MainMenu_Window", &CMainMenuPhase::OnExit);

	if (!MustRestartText)
	{
		MustRestartText = CGUIHelper::CreateText("YOU_MUST_RESTART_Text", CLocalizationSettings::Instance()->GetLocalText("MUST_RESTART"), 0.01f,0.95f,1.0f,0.1f,"Font_10", "", "", "");
		MustRestartText->setVisible(SomethingChanged);
	}
}


void CMainMenuPhase::End()
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
	CAbstractPhase *pPhase = CMenuPhase::Instance->NextPhase;

	if (pPhase != COptionsPhase::Instance)
	{
		MustRestartText->setVisible(false);
	}

	if ((pPhase != CSinglePlayerPhase::Instance) && (pPhase != CMultiPlayerPhase::Instance))
	{
		Mng.getWindow("MainMenu_Window")->destroy();
	}
}



void CMainMenuPhase::Tick()
{
	if(WNDFadeIn)
		WNDFadeIn = CGUIHelper::WindowFade("MainMenu_Window", WNDAlpha);

	if (SingleMultiPhase == NULL)
	{
		SingleMultiPhase = CSinglePlayerPhase::Instance;
	}

	if (CMenuPhase::Instance->CurrentPhase != SingleMultiPhase)
		CMenuPhase::Instance->SetPhase(SingleMultiPhase);

	if (SomethingChanged)
	{
		MustRestartText->setVisible(true);
	}
	
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnExit(e);
	}
}



void CMainMenuPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}

bool CMainMenuPhase::OnSinglePlayer(const CEGUI::EventArgs& e)
{  
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMenuPhase::Instance->CurrentPhase != CSinglePlayerPhase::Instance)
	{
		CMenuPhase::Instance->SetPhase(CSinglePlayerPhase::Instance);
		CMainMenuPhase::Instance->SingleMultiPhase = CSinglePlayerPhase::Instance;
	}
	return true;
}



bool CMainMenuPhase::OnMultiPlayer(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CMenuPhase::Instance->CurrentPhase != CMultiPlayerPhase::Instance)
	{
	    CMenuPhase::Instance->SetPhase(CMultiPlayerPhase::Instance);
		CMainMenuPhase::Instance->SingleMultiPhase = CMultiPlayerPhase::Instance;
	}
	return true;
}



bool CMainMenuPhase::OnOptions(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(COptionsPhase::Instance);
	COptionsPhase::Instance->FromInGame = false;
	return true;
}


bool CMainMenuPhase::OnProfiles(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CChooseProfilePhase::Instance);
	return true;
}


bool CMainMenuPhase::OnHighScore(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);

	return true;
}



bool CMainMenuPhase::OnCredits(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	//CMenuPhase::Instance->SetPhase(CCreditsPhase::Instance);
	CMenuPhase::Instance->SetPhase(CUberCreditsPhase::Instance);
	return true;
}



bool CMainMenuPhase::OnExit(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CExitMenuPhase::Instance);
	return true;
}
