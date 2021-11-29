/*
      File: CMapEditorApplication.h
	  Author: Tomislav Kukic
	  Date: 04.11.2005.
	  Version: 1.0
*/



#include <IPhase.h>
#include <CApplication.h>
#include <CInitializePhase.h>
#include <CMainPhase.h>
#include <CTerminatePhase.h>
#include <CCreateDialogsPhase.h>
#include <CMapEditorApplication.h>




bool CMapEditorApplication::Run()
{
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

     // Faza inicijalizacije dijaloga
	 case PHASE_CREATEDIALOGS:
		 delete PhaseObject;
		 PhaseObject = new CCreateDialogsPhase;		 
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
	return true;
}

