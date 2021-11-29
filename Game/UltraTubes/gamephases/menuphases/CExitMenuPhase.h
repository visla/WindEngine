/*
   CExitMenuPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CExitMenuPhase_H
#define _CExitMenuPhase_H

#include <CAbstractPhase.h>

class CExitMenuPhase: public CAbstractPhase
{
public:
	static CExitMenuPhase*	Instance;

						CExitMenuPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();


private:
	bool				bWasBuyPhase;
};


#endif