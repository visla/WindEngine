/*
       File: CApplication.cpp
	   Author: Tomislav Kukic
	   Date: 11.01.2006.
	   Version: 1.0
*/



#include <Windows.h>
#include <IPhase.h>
#include <CUTApplication.h>
#include <CInitializePhase.h>
#include <CMainPhase.h>
#include <CTerminatePhase.h>



CUTApplication *CUTApplication::Instance = new CUTApplication;




bool CUTApplication::Run()
{
	PHASES Phase;
	IPhase* PhaseObject = NULL;

	Phase = PHASE_INITIALIZE;
	while (Phase != PHASE_APPEND)
	{
		switch (Phase)
		{
			// Faza inicijalizacije
			case PHASE_INITIALIZE:
				delete PhaseObject;
				PhaseObject = new CInitializePhase;		 
				Phase = PhaseObject->Start();
				break;

			// Glavna faza
			case PHASE_MAINLOOP: 
				delete PhaseObject;
				PhaseObject = new CMainPhase;
				Phase = PhaseObject->Start();
				break;

			// Faza unistavanja
			case PHASE_DEINITIALIZE:
				delete PhaseObject;
				PhaseObject = new CTerminatePhase;
				Phase = PhaseObject->Start();
				break;	 
		}
	}
	if (PhaseObject)
		delete PhaseObject;
	return true;
}

