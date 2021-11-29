/*
   CFunnyPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CFunnyPhase_H
#define _CFunnyPhase_H

#include <CAbstractPhase.h>
#include <WeTexture.h>
#include <CMiniGame.h>


class CFunnyPhase: public CAbstractPhase
{
public:
	static CFunnyPhase*	Instance;

	CMiniGame			*MiniGame;

						CFunnyPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif