/*
    CApplication.h

	Version: 2.02
	Date: 7.5.2005.
	Author: Nebojsa Vislavski

	This is main application class. It is used to control hole game

	Hisotry: 
	  2.0 - we jumped to version 2.0 because main intention of this class is changed. 
	        This class is now used only to control phases, and to initialize some very global things
			like logers, etc.
			Using this class is like this. First you inherit this class and 
			override Run method with your own code. If you need you can also override 
			Start and Stop methods and add extra initialization if you need (but this 
			would be really rare situations)
	   2.02 - fixed some loger constructors


*/

#pragma once

#include <IPhase.h>



class CApplication
{
protected:
   bool         bTerminated;
   bool         bStarted;
   IPhase       *PhaseObject;
   PHASES       Phase;

public:
      	    CApplication();
	virtual ~CApplication();

    virtual bool Run() = 0;

	void Terminate();
	bool GetTerminated() { return bTerminated; }

	void Start();
	void Stop();
};


