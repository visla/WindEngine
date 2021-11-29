/*
       File: CMainPhase.h
	   Author: Tomislav Kukic
	   Date: 12.01.2006.
	   Version: 1.0
*/



#include <CUTWindow.h>
#include <CMainPhase.h>
#include <CGame.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <WeEngine.h>
#include <WeFont.h>
#include <WeCamera.h>
#include <WeTimer.h>
#include <WeInput.h>
#include <WeCEGUI.h>
#include <d3d9renderer.h>
#include <CMainGamePhase.h>
#include <CPlayGamePhase.h>
#include <WeProfiler.h>
using namespace CEGUI;

/*
void MyRender()
{	
	if(CGame::Instance()->GetState() == STATE_MAINMENU)
	{
		WeRenderDevice::Instance()->Device->BeginScene();
		System::getSingleton().renderGUI();
		WeRenderDevice::Instance()->Device->EndScene();
	}else{
		CGame::Instance()->Map->Render(CGame::Instance()->Camera);
		CGame::Instance()->Interfejs->DoRender();
	}
}
*/


PHASES CMainPhase::Start()
{
	while(CUTWindow::Instance->Running())
	{
		static float ProfilingTimer = 0.0f;
		ProfilingTimer -= WeTimer::Instance()->GetDeltaTimeF();
		if (ProfilingTimer <= 0.0f)
		{
			WeProfiler::Instance()->SubResetBlocks();
			ProfilingTimer = 1.0f;
		}

		WeProfiler::Instance()->EnterBlock(0);

		WeProfiler::Instance()->EnterBlock(1);
		CUTWindow::Instance->Update();
		WeProfiler::Instance()->LeaveBlock(1);

		//if(CGame::Instance()->GetState() == STATE_MAINMENU)
		//	System::getSingleton().injectTimePulse(WeTimer::Instance()->GetDeltaTimeF());

		WeProfiler::Instance()->EnterBlock(2);
		CGame::Instance->Tick();
		WeProfiler::Instance()->LeaveBlock(2);
		
		WeProfiler::Instance()->EnterBlock(3);
		if (CGame::Instance->Camera)
			WeEngine::Instance()->Update(CGame::Instance->Camera);
		else
			WeEngine::Instance()->Update(&CMainGamePhase::Instance->Camera);
		WeProfiler::Instance()->LeaveBlock(3);

		WeProfiler::Instance()->EnterBlock(4);
		WeRenderDevice::Instance()->DoRender();
		WeProfiler::Instance()->LeaveBlock(4);

		WeProfiler::Instance()->LeaveBlock(0);
	}

	return PHASE_DEINITIALIZE;
}

