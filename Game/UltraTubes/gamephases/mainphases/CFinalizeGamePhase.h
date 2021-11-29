/*
   CFinalizeGamePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CFinalizeGamePhase_H
#define _CFinalizeGamePhase_H

#include <CAbstractPhase.h>

class CFinalizeGamePhase: public CAbstractPhase
{
protected:
public:
	static CFinalizeGamePhase*	Instance;

						CFinalizeGamePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif