/*
   CIntroPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CINTROPHASE_H
#define _CINTROPHASE_H

#include <CAbstractPhase.h>
#include <WeTexture.h>


enum IntroStates
{
	INTRO_STATE_NONE,
	INTRO_STATE_MOVIE_LOAD,
	INTRO_STATE_MOVIE_LOGO,
	INTRO_STATE_WALLSPLASH
};


class CIntroPhase: public CAbstractPhase
{
protected:
	//static CIntroPhase*	_instance;
	IntroStates			CurrentState;

public:
	static CIntroPhase*	Instance;
	static CIntroPhase *Instance1();
	WeTexture			*Background;

						CIntroPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif