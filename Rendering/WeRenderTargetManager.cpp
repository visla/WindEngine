/*
   WeRenderTargetManager.cpp

   Autor: Nebojsa Vislavski
   Date: 2.11.2005.
   Version: 1.0

   This is main RenderTarget manager

   
*/

#include <WeRenderTargetManager.h>
#include <stdio.h>

WeRenderTargetManager *WeRenderTargetManager::_instance = NULL;

WeRenderTargetManager *WeRenderTargetManager::Instance()
{
	if (!_instance) _instance = new WeRenderTargetManager;
	return _instance;
}


WeRenderTarget *WeRenderTargetManager::CreateRT(int w, int h, D3DFORMAT format, D3DMULTISAMPLE_TYPE MType)
{
	WeRenderTarget *tmp;
	tmp = new WeRenderTarget;
	if (!tmp->Initialize(w, h, format, MType))
	{
		delete tmp;
		return NULL;
	}
	RTs.PushBack(tmp);
	return tmp;
}

WeRenderTarget *WeRenderTargetManager::CreateCubeRT(int Size, D3DFORMAT format)
{
	WeRenderTarget *tmp;
	tmp = new WeRenderTarget;
	if (!tmp->InitializeCube(Size, format))
	{
		delete tmp;
		return NULL;
	}
	RTs.PushBack(tmp);
	return tmp;
}



void WeRenderTargetManager::Free(WeRenderTarget *rt)
{
	if (!rt) return;

	for (RTs.First(); !RTs.End(); RTs.Next())
	{
		if (RTs.GetItem() == rt)
		{
			SAFE_DELETE (RTs.GetItem());
			RTs.Delete();
			return;
		}
	}
}

void WeRenderTargetManager::FreeAll()
{
	for (RTs.First(); !RTs.End(); RTs.Next())
	{
		SAFE_DELETE (RTs.GetItem());
	}
	RTs.Clear();
}

void WeRenderTargetManager::OnLostDevice()
{
	for (RTs.First(); !RTs.End(); RTs.Next())
	{
		RTs.GetItem()->OnLostDevice();
	}

}

void WeRenderTargetManager::OnResetDevice()
{
	for (RTs.First(); !RTs.End(); RTs.Next())
	{
		RTs.GetItem()->OnResetDevice();
	}
}
