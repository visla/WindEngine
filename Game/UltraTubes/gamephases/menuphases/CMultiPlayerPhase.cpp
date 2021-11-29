/*
    CMultiPlayerPhase.cpp

	Autor: Vuk Vranjkovic
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CMultiPlayerPhase.h>
#include <CGame.h>
#include <CMainMenuPhase.h>
#include <CMenuPhase.h>
#include <CLANServerBrowsePhase.h>
#include <CGUIHelper.h>
#include <CMenuPhase.h>
#include <CSinglePlayerPhase.h>
#include <CJoiningGamePhase.h>
#include <CMainMenuPhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>


CMultiPlayerPhase *CMultiPlayerPhase::Instance = new CMultiPlayerPhase;


CMultiPlayerPhase::CMultiPlayerPhase()
{

}

void CMultiPlayerPhase::Start()
{
	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *btn;

	CGUIHelper::CreateWnd("MultiPlayer_Window",CLocalizationSettings::Instance()->GetLocalText("MULTIPLAYER"),0.0f,0.55f,0.08f,0.4f,0.5f,"RootWindow");
	Mng.getWindow("MultiPlayer_Window")->setAlpha(WNDAlpha);

	btn = CGUIHelper::CreateButton("MultiPlayer_InternetButton",CLocalizationSettings::Instance()->GetLocalText("INTERNET"),0.05f, 0.3f, 1.0f, 0.18f,"MultiPlayer_Window", &CMultiPlayerPhase::OnInternetGame);
	btn->setEnabled(false);
	btn->setAlpha(0.5f);
	
	btn = CGUIHelper::CreateButton("MultiPlayer_LANButton",CLocalizationSettings::Instance()->GetLocalText("LAN"),0.05f, 0.5f, 1.0f, 0.18f,"MultiPlayer_Window", &CMultiPlayerPhase::OnLANGame);
	btn->setEnabled(false);
	btn->setAlpha(0.5f);
}

void CMultiPlayerPhase::End()
{
	CEGUI::WindowManager &pMng = CEGUI::WindowManager::getSingleton();

	pMng.getWindow("MultiPlayer_Window")->destroy();
	if (CMenuPhase::Instance->NextPhase != CSinglePlayerPhase::Instance)
	{
		CMainMenuPhase::Instance->End();
	}
}

void CMultiPlayerPhase::Tick()
{
	CMainMenuPhase::Instance->Tick();
	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("MultiPlayer_Window", WNDAlpha);
}

void CMultiPlayerPhase::Render()
{
    CMenuPhase::Instance->RenderStdScreen();
}

bool CMultiPlayerPhase::OnInternetGame(const CEGUI::EventArgs &e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	return true;
}

bool CMultiPlayerPhase::OnLANGame(const CEGUI::EventArgs &e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
    return true;
}

