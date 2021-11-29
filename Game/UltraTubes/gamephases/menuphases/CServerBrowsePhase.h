/*
   CServerBrowsePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CServerBrowsePhase_H
#define _CServerBrowsePhase_H

#include <CAbstractPhase.h>

class CServerBrowsePhase: public CAbstractPhase
{
protected:
public:
	static CServerBrowsePhase*	Instance;

						CServerBrowsePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif