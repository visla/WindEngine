/*
    CApplication.cpp

	Version: 2.02
	Date: 7.5.2005.
	Author: Nebojsa Vislavski

	This is main application class. It is used to control hole game

	History:
	  2.0 - check .h file


*/

#include <CApplication.h>
#include <Windows.h>




CApplication::CApplication()
{
	bStarted = false;
	bTerminated = false;	
	Phase = PHASE_APPSTART;
	PhaseObject = NULL;
	Start();
}



CApplication::~CApplication()
{
   Stop();
}


void CApplication::Start()
{
  // We need to create logers.
  if (bStarted) return;

  bStarted = true;
}



void CApplication::Stop()
{
  if (!bStarted) return;

  Phase = PHASE_APPEND;
  delete PhaseObject;
}



void CApplication::Terminate()
{
	bTerminated = true;	
	Phase = PHASE_TERMINATED;
	if (PhaseObject) PhaseObject->Stop();
}



