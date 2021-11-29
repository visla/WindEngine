/*
     File: CTerminatePhase.cpp
	 Author: Tomsialv Kukic
	 Date: 12.01.2006.
	 Version: 1.0
*/


#include <CUTWindow.h>
#include <CTerminatePhase.h>
#include <WeEngine.h>
#include <CLocalizationSettings.h>


PHASES CTerminatePhase::Start()
{
	ReleaseCapture();
	CLocalizationSettings::Instance()->ClearAll();
	WeEngine::Instance()->Stop();
	CUTWindow::Instance->Terminate();

	return PHASE_APPEND;
}
