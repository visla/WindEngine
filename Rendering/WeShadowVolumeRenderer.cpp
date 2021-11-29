/*
    WeShadowVolumeRenderer.cpp

    Autor: Nebojsa Vislavski
	Date: 13.7.2006.
	Version: 1.0

	

*/

#include <WeShadowVolumeRenderer.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeBatchItem.h>
#include <WeLight.h>
#include <WeRenderableNode.h>
#include <WeRenderTree.h>
#include <WeMesh.h>
#include <WeShader.h>
#include <WeShaderManager.h>
#include <WeProfiler.h>
#include <WeRenderDevice.h>



WeShadowVolumeRenderer *WeShadowVolumeRenderer::_instance = NULL;


WeShadowVolumeRenderer *WeShadowVolumeRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeShadowVolumeRenderer;
	return _instance;
}

bool WeShadowVolumeRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	OnResetDevice();

	return true;
}


void WeShadowVolumeRenderer::ShutDown()
{
	if (!bInitialized) return;

	bInitialized = false;
}


void WeShadowVolumeRenderer::OnLostDevice()
{
}

void WeShadowVolumeRenderer::OnResetDevice()
{
}



WeShadowVolumeRenderer::WeShadowVolumeRenderer()
{
	bInitialized = false;
	Preparing = false;
}


void WeShadowVolumeRenderer::ClearAll()
{
	for (LightBatches.First(); !LightBatches.End(); LightBatches.Next())
	{
		for(LightBatches.GetItem()->BatchItems.First(); !LightBatches.GetItem()->BatchItems.End(); LightBatches.GetItem()->BatchItems.Next())
		{	
			delete LightBatches.GetItem()->BatchItems.GetItem();
		}
		LightBatches.GetItem()->BatchItems.Clear();
		delete LightBatches.GetItem();
	}
	LightBatches.Clear();
}

void WeShadowVolumeRenderer::PrepareQueuedBatches(WeWorld *world, WeCamera *Camera)
{
	// Just make rendering from Octree possible. Currently for prefabs
	ClearAll();
	Preparing = true;

	// find all lights in frustum	
	WeLight *LI;
//	MessageBox(0, "1...", 0, 0);

	for(LI = world->Lights->NextLight; LI != world->Lights; LI = LI->NextLight)
	{		
		if (LI->CastShadowVolume && LI->IsDynamic)
		{
			if (Camera->Frustum.Collide(LI->BSphere))
			{
				WeLightBatch *LB;
				LB = new WeLightBatch;
				LB->Light = LI;								
				WeProfiler::Instance()->LightsCastingShadows++;
				LightBatches.PushBack(LB);
			}			
		}
	}

//	MessageBox(0, "2...", 0, 0);
	CSmartList <WeRenderableNode *> Nodes;
	CSmartListIterator <WeLightBatch *> I;
	WeRenderableNode *node;


	// find all objects affected by the lights and render them
	for (I = LightBatches.FirstI(); I != LightBatches.EndI(); ++I)
	{			
		world->RenderTree->GetNodesInSphere(I.GetItem()->Light->BSphere, Nodes);		
		for (Nodes.First(); !Nodes.End(); Nodes.Next())
		{
			node = Nodes.GetItem();
			for (node->Prefabs.First(); !node->Prefabs.End(); node->Prefabs.Next())
			{
				if (node->Prefabs.GetItem()->CastShadowVolume)
				{
					node->Prefabs.GetItem()->OnRender(Camera);
				}
			}
		}
		Nodes.Clear();

		WeActor *Iterator, *tmp;
		for(Iterator = world->Actors->NextActor; Iterator != world->Actors; )
		{				
			tmp = Iterator->NextActor;						
			if (!Iterator->IsPrefab && !Iterator->IsCloth && !Iterator->IsParticleSystem && !Iterator->IsLight)
			{
				if (Iterator->CastShadowVolume)
				{
					Iterator->OnRender(Camera);	    									
				}
			}
			Iterator = tmp;
		}        

	}

//	MessageBox(0, "Preparing...", 0, 0);
	Preparing = false;
}

void WeShadowVolumeRenderer::RenderQueuedBatches(WeWorld *World)
{
	if (LightBatches.Empty()) return;

	static WeShader *Extruder = NULL;
	Extruder = WeShaderManager::Instance()->Load("Effects.Default.ShadowExtruder", false, true);

	// clear stencil
	//WeRenderDevice::Instance()->Device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
	//WeRenderDevice::Instance()->BeginScene();

	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
	
	WeProfiler::Instance()->ShaderBegins++;				
	Extruder->Begin();


	for (LightBatches.First(); !LightBatches.End(); LightBatches.Next())
	{		
		Extruder->SetShadowOmniLightPosition(LightBatches.GetItem()->Light->Location);
		tmpBatchItems = &LightBatches.GetItem()->BatchItems;

		for (int i = 0; i < (int)Extruder->GetPassesCount(); i++)
		{
			Extruder->BeginPass(i);

			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{								
				//if (k == 2) MessageBox(0, "123123", 0, 0);				
				BatchItem = tmpBatchItems->GetItem();
				// Tell this Actor not to be occluded				

				// Set data for shader.
				WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);																		
				// set textures				
				// set env mapping if we have to sampler env sampler
				Extruder->CommitChanges();

				// And finaly draw geometry

				BatchItem->Mesh->DrawShadowSubset(&BatchItem->AttribRange);			
				WeProfiler::Instance()->BatchItemsRendered++;
				WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
			}

			Extruder->EndPass();
		}		
	}
	Extruder->End();
	//WeRenderDevice::Instance()->EndScene();	

}

void WeShadowVolumeRenderer::QueueBatchItem(WeBatchItem *BatchItem)
{
	if (!Preparing) return;
	if (BatchItem->Alpha) 
	{
		delete BatchItem;
		return;
	}

	bool added = false;
	CSmartListIterator <WeLightBatch *> I;
	
	WeBatchItem *tmpItem;
	for (I = LightBatches.FirstI(); I != LightBatches.EndI(); ++I)
	{		
		if (I.GetItem()->Light->BSphere.Intersect(&BatchItem->BSphere))		
		{		
			tmpItem = new WeBatchItem;
			*tmpItem = *BatchItem;
			WeProfiler::Instance()->ObjectsCastingShadows++;
			I.GetItem()->BatchItems.PushBack(tmpItem);		
		}
	}			
	delete BatchItem;
}


void WeShadowVolumeRenderer::QueueAlphaBatch(WeBatchItem *BatchItem)
{
}
