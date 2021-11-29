/*
   File    : CInternetServerBrowsePhase.h
   Autor   : Vuk Vranjkovic
   Date    : 31.3.2006.
   Version : 1.0
*/
#ifndef _CInternetServerBrowsePhase_H
#define _CInternetServerBrowsePhase_H

#include <CAbstractPhase.h>

class CInternetServerBrowsePhase: public CAbstractPhase
{
public:
	static CInternetServerBrowsePhase*	Instance;

						CInternetServerBrowsePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();




private:
};

#endif