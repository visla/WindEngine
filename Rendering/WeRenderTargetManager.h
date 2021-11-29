/*
   WeRenderTargetManager.cpp

   Autor: Nebojsa Vislavski
   Date: 2.11.2005.
   Version: 1.0

   This is main RenderTarget manager

   
*/


#ifndef _WERENDERTARGETMANAGER_H
#define _WERENDERTARGETMANAGER_H

#include <WeRenderTarget.h>
#include <SmartList.h>
#include <WindEngine.h>



class WINDENGINE_API WeRenderTargetManager
{
private:
	static WeRenderTargetManager *_instance;
protected:
	CSmartList <WeRenderTarget *> RTs; // list of need to now render targets

public:
	static WeRenderTargetManager *Instance();

	~WeRenderTargetManager() { FreeAll(); }


	WeRenderTarget *CreateRT(int w, int h, D3DFORMAT format, D3DMULTISAMPLE_TYPE MType = D3DMULTISAMPLE_NONE);
	WeRenderTarget *CreateCubeRT(int Size, D3DFORMAT format);
	void FreeAll();
	void Free(WeRenderTarget *rt);



	void	OnLostDevice(); // to release all memory 
	void	OnResetDevice();  // to recreate all rts
	
};

#endif