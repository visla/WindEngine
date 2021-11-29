/*
    CSinglePlayerPhase.cpp

	Autor: Vuk Vranjkovic
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CSinglePlayerPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CSingleRacePhase.h>
#include <CInstantRacePhase.h>
#include <CTournamentPhase.h>
#include <CGUIHelper.h>
#include <CMainMenuPhase.h>
#include <CMultiPlayerPhase.h>
#include <CMainMenuPhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>



CSinglePlayerPhase *CSinglePlayerPhase::Instance = new CSinglePlayerPhase;



CSinglePlayerPhase::CSinglePlayerPhase()
{	
}



void CSinglePlayerPhase::Start()
{
	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *btn;

	CGUIHelper::CreateWnd("SinglePlayer_Window",CLocalizationSettings::Instance()->GetLocalText("SINGLE PLAYER"),
		0.0f, 0.55f, 0.08f, 0.4f, 0.5f,"RootWindow");


	CGUIHelper::CreateButton("SinglePlayer_SingleRace_Button",CLocalizationSettings::Instance()->GetLocalText("SINGLE RACE"),0.05f, 0.28f, 1.0f, 0.18f, "SinglePlayer_Window", &CSinglePlayerPhase::OnSingleRace);

	btn = CGUIHelper::CreateButton("SinglePlayer_Tournament_Button",CLocalizationSettings::Instance()->GetLocalText("TOURNAMENT"),0.05f, 0.46f,1.0f, 0.18f,"SinglePlayer_Window", &CSinglePlayerPhase::OnTournament);
	btn->setEnabled(false);
	btn->setAlpha(0.5f);

	btn = CGUIHelper::CreateButton("SinglePlayer_InstantRace_Button",CLocalizationSettings::Instance()->GetLocalText("INSTANT RACE"),0.05f, 0.64f,1.0f, 0.18f,"SinglePlayer_Window", &CSinglePlayerPhase::OnInstantRace);
	btn->setEnabled(false);
	btn->setAlpha(0.5f);

	winMgr.getWindow("MainMenu_Window")->setProperty("Visible", "true");
}



void CSinglePlayerPhase::End()
{
	CEGUI::WindowManager &pMng = CEGUI::WindowManager::getSingleton();
	pMng.getWindow("SinglePlayer_Window")->destroy();
	if (CMenuPhase::Instance->NextPhase != CMultiPlayerPhase::Instance)
	{
		CMainMenuPhase::Instance->End();
	}
}



void CSinglePlayerPhase::Tick()
{
	CMainMenuPhase::Instance->Tick();

	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("SinglePlayer_Window", WNDAlpha);
}



void CSinglePlayerPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



bool CSinglePlayerPhase::OnSingleRace(const CEGUI::EventArgs& e)
{	
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CSingleRacePhase::Instance);
	return true;
}



bool CSinglePlayerPhase::OnInstantRace(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	CMenuPhase::Instance->SetPhase(CInstantRacePhase::Instance);
    return true;
}



bool CSinglePlayerPhase::OnTournament(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	CMenuPhase::Instance->SetPhase(CTournamentPhase::Instance);
    return true;
}

