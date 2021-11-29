
/*
       File: CTerminatePhase.cpp
	   Author: Tomislav Kukic
	   Date: 07.11.2005.
	   Version: 1.0
*/




#include <IPhase.h>
#include <WeEngine.h>
#include <CMapEditorWindow.h>
#include <WePostProcess.h>
#include <CTerminatePhase.h>




PHASES CTerminatePhase::Start()
{
	WePostProcess::Instance()->ShutDown();
	WeEngine::Instance()->Stop();

	EditorWindow->Terminate();

	delete EditorWindow;
	EditorWindow = NULL;

	return PHASE_APPEND;
}

