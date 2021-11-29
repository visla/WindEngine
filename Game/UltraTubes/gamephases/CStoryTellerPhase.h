/*
   CStoryTellerPhase.h

   Autor: Nebojsa Vislavski
   Date: 30.3.2006.
   Version: 1.0

*/


#ifndef _CStoryTellerPhase_H
#define _CStoryTellerPhase_H

#include <CAbstractPhase.h>
#include <WeCanvas.h>
#include <WeTexture.h>

class CStoryTellerPhase: public CAbstractPhase
{
protected:
public:
	static CStoryTellerPhase*	Instance;

	WeTexture			*Background;


						CStoryTellerPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif