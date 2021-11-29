/*
    COptionsPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <COptionsPhase.h>
#include <CGame.h>
#include <COptionsAudioPhase.h>
#include <COptionsVideoPhase.h>
#include <COptionsGeneralPhase.h>
#include <COptionsInputPhase.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <WeCEGUI.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>
#include <CGUIHelper.h>


COptionsPhase *COptionsPhase::Instance = new COptionsPhase;


COptionsPhase::COptionsPhase()
{
	FromInGame = false;
}

void COptionsPhase::Start()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CGUIHelper::CreateWnd("Options_OptionsWindow", CLocalizationSettings::Instance()->GetLocalText("OPTIONS"),
		0.0f, 0.1f, 0.13f, 0.4f, 0.8f, "RootWindow");

	CGUIHelper::CreateButton("Options_AudioButton", CLocalizationSettings::Instance()->GetLocalText("AUDIO"),
		0.05f, 0.18f, 1.0f, 0.12f, "Options_OptionsWindow", &COptionsPhase::OnAudio);

	CGUIHelper::CreateButton("Options_VideoButton", CLocalizationSettings::Instance()->GetLocalText("VIDEO"),
		0.05f, 0.3f, 1.0f, 0.12f, "Options_OptionsWindow", &COptionsPhase::OnVideo);

	CGUIHelper::CreateButton("Options_ControlsButton", CLocalizationSettings::Instance()->GetLocalText("CONTROLS"),
		0.05f, 0.42f, 1.0f, 0.12f, "Options_OptionsWindow", &COptionsPhase::OnControls);

	CMenuPhase::Instance->SetPhase(COptionsVideoPhase::Instance);
}

void COptionsPhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	if((CMenuPhase::Instance->NextPhase != COptionsVideoPhase::Instance) &&
		(CMenuPhase::Instance->NextPhase != COptionsAudioPhase::Instance) &&
		(CMenuPhase::Instance->NextPhase != COptionsInputPhase::Instance) &&
		(CMenuPhase::Instance->NextPhase != COptionsGeneralPhase::Instance))
	{
		winMgr.getWindow("Options_OptionsWindow")->destroy();
		if (FromInGame)
		{
			winMgr.getWindow("IN_GAME_MENU")->setVisible(true);
			CMenuPhase::Instance->CurrentPhase = NULL;
		}
	}
}

void COptionsPhase::Tick()
{	
	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("Options_OptionsWindow", WNDAlpha);
}

void COptionsPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}


bool COptionsPhase::OnAudio(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CMenuPhase::Instance->CurrentPhase == COptionsAudioPhase::Instance) return true;
	CMenuPhase::Instance->SetPhase(COptionsAudioPhase::Instance);

	return true;
}


bool COptionsPhase::OnVideo(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CMenuPhase::Instance->CurrentPhase == COptionsVideoPhase::Instance) return true;
	CMenuPhase::Instance->SetPhase(COptionsVideoPhase::Instance);

	return true;
}

bool COptionsPhase::OnControls(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CMenuPhase::Instance->CurrentPhase == COptionsInputPhase::Instance) return true;
	CMenuPhase::Instance->SetPhase(COptionsInputPhase::Instance);

	return true;
}

