/*
    CFinalizeGamePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CFinalizeGamePhase.h>
#include <CGame.h>
#include <CGameSettings.h>
#include <CMainMenuPhase.h>
#include <CMenuPhase.h>
#include <WeFont.h>
#include <WeCanvas.h>
#include <CFunnyPhase.h>
#include <CTournamentPhase.h>
#include <CTournamentMapSettings.h>
#include <CGameClient.h>
#include <CProfileManager.h>
#include <CStoryTellerPhase.h>
#include <CProfileManager.h>
#include <Constants.h>
#include <CTuber.h>
#include <CGamePlayerConfig.h>
#include <CStatisticsPhase.h>
#include <CMainGamePhase.h>
#include <WeTextureManager.h>
#include <WeMaterialManager.h>
#include <WeShaderManager.h>
#include <WeMeshManager.h>
#include <CGameServer.h>
#include <WeModelManager.h>

CFinalizeGamePhase *CFinalizeGamePhase::Instance = new CFinalizeGamePhase;


CFinalizeGamePhase::CFinalizeGamePhase()
{
}

void CFinalizeGamePhase::Start()
{	
	// Ako je turnir onda podesi open close liste kod profila. kako bi mogli igrati dalje
	if (CGameSettings::ServerInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
	{
		CGameSettings::ServerInstance->MapSettings->DoOpenClose(CProfileManager::Instance()->GetCurrentProfile());
	}
	else
	{
		// da vratimo stari profil od ucitavanja i kazemo mu da nema broda kupljenog
		CProfileManager::Instance()->RestoreToLoadedProfile();
		CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = false;	
	}
		

	// Unload mape uradi. Ovo brise i tuber-e koje ne moramo ponovo brisati	
	// Obrisi sve texture i mesheve.
	CMainGamePhase::Instance->Map.Pickups.Clear();


	// Zavrsena je logika predji u sledecu fazu
	CMainGamePhase::Instance->SetPhase(CStatisticsPhase::Instance);
	//CGame::Instance->SetPhase(CMenuPhase::Instance);
	//CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	CMainGamePhase::Instance->InputPlayer = NULL;
}



void CFinalizeGamePhase::End()
{
}

void CFinalizeGamePhase::Tick()
{
	
}

void CFinalizeGamePhase::Render()
{

}

