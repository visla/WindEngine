/*
    CAbstractPhase.h

	autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0

	This is abstract phase
*/

#ifndef _CABSTRACTPHASE_H
#define _CABSTRACTPHASE_H



#include <windows.h>



class CAbstractPhase
{
public:
	CAbstractPhase*	CurrentPhase;
	CAbstractPhase*	PrevPhase;
	CAbstractPhase*	NextPhase;

	virtual void	Tick();
	virtual void	Render();
	virtual void	Start();
	virtual void	End();

	CAbstractPhase(): CurrentPhase(NULL), NextPhase(NULL), PrevPhase(NULL) {}


	virtual void SetPhase(CAbstractPhase *newphase)
	{
		NextPhase = newphase;
	};
};


#endif