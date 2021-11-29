/*
   CShoppingPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CShoppingPhase_H
#define _CShoppingPhase_H

#include <CAbstractPhase.h>
#include <WeTexture.h>

class CShoppingPhase: public CAbstractPhase
{
protected:
public:
	static CShoppingPhase*	Instance;

	WeTexture			*ShopBackground;

						CShoppingPhase();

	void				RenderStdScreen();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif