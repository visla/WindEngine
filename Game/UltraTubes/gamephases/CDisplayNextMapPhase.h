/*
   CDisplayNextMapPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CDisplayNextMapPhase_H
#define _CDisplayNextMapPhase_H

#include <CAbstractPhase.h>

class CDisplayNextMapPhase: public CAbstractPhase
{
protected:
public:
	static CDisplayNextMapPhase*	Instance;

						CDisplayNextMapPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif