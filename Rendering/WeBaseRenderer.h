/*

   WeBaseRenderer.h

   Autor: Nebojsa Vislavski
   Date: 2.3.2006.
   Version: 1.0
   Platform: Win32

   This is base class for all renderers and subrenderers


*/

#ifndef _WEBASERENDERER_H
#define _WEBASERENDERER_H

#include <WindEngine.h>


class WINDENGINE_API WeBaseRenderer
{
public:
	WeBaseRenderer() {};
	virtual ~WeBaseRenderer() {};



	virtual void OnLostDevice() {};
	virtual void OnResetDevice() {};


	virtual void ShutDown() {};
	virtual bool Initialize() { return true; }
};



#endif