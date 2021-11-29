/*
   CPlayerBrowsePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CPlayerBrowsePhase_H
#define _CPlayerBrowsePhase_H

#include <CAbstractPhase.h>

class CPlayerBrowsePhase: public CAbstractPhase
{
protected:
public:
	static CPlayerBrowsePhase*	Instance;

						CPlayerBrowsePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif