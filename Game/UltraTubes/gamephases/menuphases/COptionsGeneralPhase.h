/*
   COptionsGeneralPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _COptionsGeneralPhase_H
#define _COptionsGeneralPhase_H

#include <CAbstractPhase.h>

class COptionsGeneralPhase: public CAbstractPhase
{
protected:
public:
	static COptionsGeneralPhase*	Instance;

						COptionsGeneralPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif