/*
   File    : CHighscorePhase.h
   Autor   : Vuk Vranjkovic
   Date    : 10.3.2006.
   Version : 1.0
*/
#ifndef _CHighscorePhase_H
#define _CHighscorePhase_H

#include <CAbstractPhase.h>

class CHighscorePhase: public CAbstractPhase
{
public:
	static CHighscorePhase*	Instance;

						CHighscorePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();




private:
};

#endif