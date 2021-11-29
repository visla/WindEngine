/*
    CInstantRacePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CInstantRacePhase.h>
#include <CGame.h>
#include <CGameSettings.h>
#include <CMainGamePhase.h>
#include <CGame.h>


CInstantRacePhase *CInstantRacePhase::Instance = new CInstantRacePhase;


CInstantRacePhase::CInstantRacePhase()
{
	GameStarted = false;
}

void CInstantRacePhase::Start()
{
	CGameSettings::Instance->SetCrates( (rand()%2 != 0) );
	CGameSettings::Instance->SetWeapons( (rand()%2 != 0) );
	CGameSettings::Instance->LapCount = rand()%9+1;
	CGameSettings::Instance->MapType = CGameSettings::MAPTYPE_SP_SINGLERACE;
}

void CInstantRacePhase::End()
{

}

void CInstantRacePhase::Tick()
{
	GameStarted = true;
    CGame::Instance->SetPhase(CMainGamePhase::Instance);
}

void CInstantRacePhase::Render()
{

}