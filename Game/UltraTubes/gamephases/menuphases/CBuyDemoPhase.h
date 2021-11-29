/*
   CBuyDemoPhase.h
   Autor: Vuk Vranjkovic
   Date: 10.3.2006.
   Version: 1.0
*/


#ifndef _CBuyDemoPhase_H
#define _CBuyDemoPhase_H

#include <CAbstractPhase.h>
class WeTexture;

class CBuyDemoPhase: public CAbstractPhase
{
public:
	static CBuyDemoPhase*	Instance;

						CBuyDemoPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

private:
	WeTexture *pTex;
};


#endif