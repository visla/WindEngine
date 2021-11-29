/*
      File: CDemoWindow.h
	  Author: Tomislav Kukic
	  Date: 12.01.2006.
	  Version: 1.0
*/



#include <WeEngine.h>
#include <WeTimer.h>
#include <CUTWindow.h>
#include <CGame.h>
#include <WeCEGUI.h>
#include <CAVPlayback.h>
#include <CLoadMenuPhase.h>


using namespace CEGUI;


CUTWindow *CUTWindow::Instance = new CUTWindow;

CUTWindow::CUTWindow()
{
}


void CUTWindow::OnChar(DWORD c)
{
	if (CLoadMenuPhase::Instance->Initialized)
		CEGUI::System::getSingleton().injectChar(c);
}


void CUTWindow::OnGraphNotify()
{
	CAVPlayback::Instance->HandleGraphEvent();
}



void CUTWindow::OnDblClick(DWORD ID)
{
	CGame::Instance->OnDblClick();
}



