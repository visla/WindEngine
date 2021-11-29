/*
   CInstantRacePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CInstantRacePhase_H
#define _CInstantRacePhase_H

#include <CAbstractPhase.h>

class CInstantRacePhase: public CAbstractPhase
{
public:
	static CInstantRacePhase*	Instance;

						CInstantRacePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	bool				GameStarted;
};


#endif