// zamenjeno sa CMultiRacePhase.cpp

/*
    File    : CHostGamePhase.cpp
	Autor   : Vuk Vranjkovic
	Date    : 27.3.2006.
	Version : 1.0
*/
#include <CHostGamePhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CCreateMultiplayerGamePhase.h>
#include <CGUIHelper.h>
#include <CMenuPhase.h>
#include <CGameClient.h>
#include <CGameServer.h>
#include <CChooseMapPhase.h>
#include <WeCanvas.h>
#include <WeFont.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>


CHostGamePhase *CHostGamePhase::Instance = new CHostGamePhase;

CHostGamePhase::CHostGamePhase()
{

}

void CHostGamePhase::Start()
{
	pFont=WeCanvas::Instance()->MakeFont("Arial",100);

	CGUIHelper::CreateButton("HOSTGAME_CANCEL_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CANCEL"),0.6f,0.6f,0.3f,0.1f, "RootWindow", &CHostGamePhase::OnCancel);

	CGameServer::Instance->Initialize();
	CGameServer::Instance->StartHosting();
	CGameClient::Instance->Initialize();	
	CGameClient::Instance->CreateGame(&CGame::Instance->ServerInfo, "", false);
}

void CHostGamePhase::End()
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("HOSTGAME_CANCEL_BUTTON")->destroy();
}

void CHostGamePhase::Tick()
{
	if(CGameClient::Instance->GetStatus()==GC_JOINED)
	{
		CMenuPhase::Instance->SetPhase(CCreateMultiplayerGamePhase::Instance);		
	}

	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnCancel(e);
	}
}

void CHostGamePhase::Render()
{
	WeCanvas::Instance()->Draw2DText(pFont, "Hosting...", 100, 100, 1000, 1000, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CMenuPhase::Instance->RenderStdScreen();
}

bool CHostGamePhase::OnCancel(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CChooseMapPhase::Instance);
	return true;
}

