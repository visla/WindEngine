/*
   CLoadMenuPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CLoadMenuPhase_H
#define _CLoadMenuPhase_H

#include <CAbstractPhase.h>
#include <d3d9renderer.h>

class CLoadMenuPhase: public CAbstractPhase
{
protected:

public:
	static CLoadMenuPhase*	Instance;

	bool				Initialized;
	void				CreateFonts(int ResHeight);

						CLoadMenuPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	CEGUI::DirectX9Renderer* Renderer;
};


#endif