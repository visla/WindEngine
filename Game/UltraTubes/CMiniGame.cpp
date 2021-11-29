/*
    CMiniGame.cpp

	Autor: Goran Bogdanovic
	Date: 3.4.2006.
	Version: 1.0
*/

#include <CMiniGame.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <WeTimer.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>


CMiniGame::CMiniGame()
{
	Finished = false;
	Background = NULL;
}



CMiniGame::~CMiniGame()
{
	WeTextureManager::Instance()->Free(Background);
	Background = NULL;
}


void CMiniGame::Tick()
{
//
}

void CMiniGame::Render()
{
	WeCanvas::Instance()->DrawRectangle(
										0, 0, 
										WeRenderDevice::Instance()->GetResWidth(), 
										WeRenderDevice::Instance()->GetResHeight(), 
										0xffffffff,
										Background );
}
