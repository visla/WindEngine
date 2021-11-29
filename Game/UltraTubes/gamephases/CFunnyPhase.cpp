/*
    CFunnyPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CFunnyPhase.h>
#include <CGame.h>
#include <CLoadMenuPhase.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <WeTimer.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <CMiniGameJim.h>
#include <CGameSettings.h>
#include <CTournamentPhase.h>


CFunnyPhase* CFunnyPhase::Instance = new CFunnyPhase;



CFunnyPhase::CFunnyPhase()
{
//
}


void CFunnyPhase::Start()
{
    //randomize here to get other mini games
	MiniGame = new CMiniGameJim();
}


void CFunnyPhase::End()
{
	SAFE_DELETE(MiniGame);
}

void CFunnyPhase::Tick()
{
	MiniGame->Tick();

	if(MiniGame->Finished)
	{
		switch(CGameSettings::Instance->MapType)
		{
		case CGameSettings::MAPTYPE_SP_TOURNAMENT:
			CGame::Instance->SetPhase(CMenuPhase::Instance);
			CMenuPhase::Instance->SetPhase(CTournamentPhase::Instance);
			break;

		default:
			CGame::Instance->SetPhase(CMenuPhase::Instance);
			CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
			break;
		}

	}
}

void CFunnyPhase::Render()
{
	MiniGame->Render();
}
