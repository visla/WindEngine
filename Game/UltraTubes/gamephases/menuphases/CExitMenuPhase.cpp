/*
    File     : CExitMenuPhase.cpp
	Autor    : Vuk Vranjkovic
	Date     : 10.3.2006.
	Version  : 1.0
*/

#include <CExitMenuPhase.h>
#include <CGame.h>
#include <CBuyDemoPhase.h>
#include <CUTWindow.h>
#include <CMenuPhase.h>
#include <WeEngine.h>
#include <CMainGamePhase.h>


CExitMenuPhase *CExitMenuPhase::Instance = new CExitMenuPhase;


CExitMenuPhase::CExitMenuPhase()
{
	bWasBuyPhase=false;
}

void CExitMenuPhase::Start()
{
	
}

void CExitMenuPhase::End()
{
	
}

void CExitMenuPhase::Tick()
{
	if((CGame::Instance->bDemo)&&(!bWasBuyPhase))
	{
		CMainGamePhase::Instance->PlaySong(SongNone);
		CMenuPhase::Instance->SetPhase(CBuyDemoPhase::Instance);
		bWasBuyPhase=true;
	}
	else
	{
		CMainGamePhase::Instance->PlaySong(SongNone);
		WeEngine::Instance()->Stop();
		CUTWindow::Instance->Terminate(); 
	}
}

void CExitMenuPhase::Render()
{

}