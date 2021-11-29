/*
       File: CMainPhase.h
	   Author: Tomislav Kukic
	   Date: 12.01.2006.
	   Version: 1.0
*/



#ifndef CMAINPHASE
#define CMAINPHASE



#include <IPhase.h>
#include <WeCEGUI.h>

class CMainPhase : public IPhase
{
public:
    PHASES Start();

	bool handleQuit(const CEGUI::EventArgs& e);
};




#endif
