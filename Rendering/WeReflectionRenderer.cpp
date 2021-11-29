/*
    WeReflectionRenderer.cpp

	Autor: Nebojsa Vislavski
	Date: 23.11.2005.
	Version: 1.0
	Platform: Win32


	This is used to render env maps for batch items that needs them. 
	We need that batch item need this if material is ReflectionType = REALTIME, 
	and Batch Item queuing will give us this item if it detects that material.
	This is integral part of engine and cannot be used by client
	

*/


#include <WeReflectionRenderer.h>
#include <WeCamera.h>
#include <WeWorld.h>
#include <WeBatchItem.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WeRenderDevice.h>
#include <WeMaterial.h>
#include <WeRenderTargetManager.h>
#include <WeActor.h>
#include <WeProfiler.h>

WeReflectionRenderer *WeReflectionRenderer::_instance = NULL;

WeReflectionRenderer *WeReflectionRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeReflectionRenderer;
	return _instance;
}


WeReflectionRenderer::WeReflectionRenderer(): ItemsAddedCount(0), BatchIndex(0), ReflectionUpdate(1), EnvMapSize(128), DoDoubleReflections(true), UpdateInterval(1), FrameCounter(0), Status(WEREFLSTATUS_CLEARED) 
{
	WeRenderDevice::Instance()->Renderers.PushBack(this);
}

void WeReflectionRenderer::ClearAll()
{
	// Clear Only items that are going to be rendered again next frame	
	ReflectiveBatches.First();
	for (ReflectiveBatches.First(); !ReflectiveBatches.End(); )
	{
		delete ReflectiveBatches.GetItem();
		ReflectiveBatches.Delete();
	}
	
}









bool WeReflectionRenderer::QueueBatchItem(WeBatchItem *BatchItem)
{
	ItemsAddedCount = 0;
	static int CounterStart = 0;

	if (Status == WEREFLSTATUS_CLEARED) 
	{		
		// if this item is already in list then do not add it
		for (ReflectiveBatches.First(); !ReflectiveBatches.End(); ReflectiveBatches.Next())
		{			
			if (ReflectiveBatches.GetItem()->Actor == BatchItem->Actor)
			{
				ReflectiveBatches.GetItem()->LastUpdateFrame = FrameCounter;				
				delete BatchItem;
				return true;
			}
		}
		// we dont have it in list so find first empty
		for (ReflectiveBatches.First(); !ReflectiveBatches.End(); ReflectiveBatches.Next())
		{			
			if (FrameCounter > ReflectiveBatches.GetItem()->LastUpdateFrame)
			{
				ReflectiveBatches.GetItem()->Actor = BatchItem->Actor;
				ReflectiveBatches.GetItem()->LastUpdateFrame = FrameCounter;
				ReflectiveBatches.GetItem()->FrameCounter = CounterStart++;
				ItemsAddedCount++;
				delete BatchItem;
				return true;
			}
		}
		// now just add new item with new Env map
		WeReflectionRenderer::ReflectionBatch *ReflBatch = new WeReflectionRenderer::ReflectionBatch;
		ReflBatch->LastUpdateFrame = FrameCounter;
		ReflBatch->Actor = BatchItem->Actor;
		ReflBatch->EnvMap = WeRenderTargetManager::Instance()->CreateCubeRT(EnvMapSize, D3DFMT_A8R8G8B8);
		ReflBatch->FrameCounter = CounterStart++;

		if (ReflBatch->EnvMap)
		{
			
		}		
		ReflectiveBatches.PushFront(ReflBatch);
		ItemsAddedCount++;
		return true;


		
	} else if (Status == WEREFLSTATUS_ACTIVE)
	{


		delete BatchItem;
		return true; // let some other part take care of this batch item


	} else 
	{
		// it is finished state so env map is made for that batch item and 
		// assigned to 
        // find reflectivebatch which is this envmap.
		for (ReflectiveBatches.First(); !ReflectiveBatches.End(); ReflectiveBatches.Next())
		{
			if (BatchItem->Actor == ReflectiveBatches.GetItem()->Actor)
			{
				BatchItem->EnvMap = ReflectiveBatches.GetItem()->EnvMap;
                return false;
			}
		}
        return true; // engine should never reach this code
	}
}







void WeReflectionRenderer::Render(WeWorld *World, WeCamera *Camera)
{
	if (Status != WEREFLSTATUS_CLEARED) return;
	if (ReflectiveBatches.Empty()) return;
	



	// -------------------------------------------------------------------
	WeCamera *tmpCamera = new WeCamera;
	WeVector Up[6], LookAt[6], RealLook;


	LookAt[0] = WeVector(1,0,0);
	LookAt[1] = WeVector(-1,0,0);
	LookAt[2] = WeVector(0,1,0);
	LookAt[3] = WeVector(0,-1,0);
	LookAt[4] = WeVector(0,0,1);
	LookAt[5] = WeVector(0,0,-1);

	Up[0] = WeVector(0,1,0);
	Up[1] = WeVector(0,1,0);
	Up[2] = WeVector(0,0,-1);
	Up[3] = WeVector(0,0,1);
	Up[4] = WeVector(0,1,0);
	Up[5] = WeVector(0,1,0);
		
	Status = WEREFLSTATUS_ACTIVE;


	LPDIRECT3DSURFACE9 BackSurface;
	BackSurface = WeRenderDevice::Instance()->SaveBackBuffer();



	for (ReflectiveBatches.First(); !ReflectiveBatches.End(); ReflectiveBatches.Next())
	{
		// if it is alive this frame
		if (FrameCounter != ReflectiveBatches.GetItem()->LastUpdateFrame) continue;

		// if this is updating frame of this batch
		ReflectiveBatches.GetItem()->FrameCounter++;
		if (ReflectiveBatches.GetItem()->FrameCounter % ReflectionUpdate) continue;

 
		// now we need to render all six sides from this batch item
		// first make camera location
		tmpCamera->Location = ReflectiveBatches.GetItem()->Actor->Location;
		tmpCamera->SetNearClip(.01f);
		tmpCamera->SetFarClip(100.0f);
		tmpCamera->SetFov(D3DX_PI / 2.0f);
		tmpCamera->SetAspect(1.0f);

		// Bake EnvMap
		for (int i = 0; i < 6; i++)
		{
			tmpCamera->Direction = LookAt[i];
			tmpCamera->Up = Up[i];						
			tmpCamera->Update();
			

			// Set Render Target 			
			ReflectiveBatches.GetItem()->EnvMap->BeginScene(0, i);
			WeRenderDevice::Instance()->Device->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 1);

			// Draw world
			//if (i == 0)
			//WeRenderDevice::Instance()->Device->BeginScene();
			World->Render(tmpCamera);			
			//WeRenderDevice::Instance()->Device->EndScene();
			
		}
		WeProfiler::Instance()->ReflectiveObjectsDrawn++;
		//printf("Processed: %d\n", ItemsProcessed);
		//i++;
	}


	// restore current backbuffer		
	WeRenderDevice::Instance()->RestoreBackBuffer(BackSurface);

	Status = WEREFLSTATUS_FINISHED;
	// Renderuj svet normalno
	World->Render(Camera);


	// -------------------------------------------------------------------


	Status = WEREFLSTATUS_CLEARED;
	FrameCounter++;
	// this is not frame we want this method to happen
}