/*
    CJoiningGamePhase.cpp

	Autor: Vuk Vranjkovic
	Date: 17.3.2006.
	Version: 1.0
    
*/
#include <CJoiningGamePhase.h>
#include <WeCanvas.h>
#include <WeFont.h>
#include <WeTimer.h>
#include <CGUIHelper.h>
#include <CMenuPhase.h>
#include <CLANServerBrowsePhase.h>
#include <CCreateMultiplayerGamePhase.h>
#include <CMenuPhase.h>
#include <CGame.h>
#include <CGameClient.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>
#include <CMultiRacePhase.h>



CJoiningGamePhase *CJoiningGamePhase::Instance = new CJoiningGamePhase;


CJoiningGamePhase::CJoiningGamePhase()
{

}

void CJoiningGamePhase::Start()
{
	Time = 0.0f;
	pFont = WeCanvas::Instance()->MakeFont("Arial", 100);

	CGUIHelper::CreateButton("JOINGAME_CANCEL_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CANCEL"),0.6f,0.6f,0.3f,0.1f, "RootWindow", &CJoiningGamePhase::OnCancel);

	CGameClient::Instance->JoinGame(&CGame::Instance->ServerInfo,"");
}



void CJoiningGamePhase::End()
{
	CEGUI::WindowManager::getSingleton().getWindow("JOINGAME_CANCEL_BUTTON")->destroy();
}



void CJoiningGamePhase::Tick()
{
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnCancel(e);
	}

	if (CGameClient::Instance->GetStatus() == GC_JOINED)
	{
		//CMenuPhase::Instance->SetPhase(CCreateMultiplayerGamePhase::Instance);
		CMenuPhase::Instance->SetPhase(CMultiRacePhase::Instance);
		CMultiRacePhase::Instance->MultiRaceState = MRS_JOINING;
	}
	else if (CGameClient::Instance->GetStatus() == GC_REFUSED)
	{
		CGameClient::Instance->Disconnect();
		CGameClient::Instance->ResetRefuse();
		CMenuPhase::Instance->SetPhase(CMenuPhase::Instance->PrevPhase);
	}

	Time += WeTimer::Instance()->GetDeltaTimeF();
	if (Time>15.0f)
		CMenuPhase::Instance->SetPhase(CMenuPhase::Instance->PrevPhase);
}



void CJoiningGamePhase::Render()
{
	WeCanvas::Instance()->Draw2DText(pFont, "Joining...", 100, 100, 1000, 1000, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CMenuPhase::Instance->RenderStdScreen();
}



bool CJoiningGamePhase::OnCancel(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);
	return true;
}

