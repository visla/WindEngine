/*
    WeProjectorRenderer.cpp

    Autor: Nebojsa Vislavski
	Date: 13.7.2006.
	Version: 1.0

	

*/

#include <WeProjectorRenderer.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeBatchItem.h>
#include <WeLight.h>
#include <WeRenderableNode.h>
#include <WeRenderTree.h>
#include <WeBatchRenderer.h>
#include <WeShaderBatch.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeProfiler.h>
#include <WeMaterial.h>
#include <WeMaterialManager.h>
#include <WeMesh.h>



WeProjectorRenderer *WeProjectorRenderer::_instance = NULL;


WeProjectorRenderer *WeProjectorRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeProjectorRenderer;
	return _instance;
}

bool WeProjectorRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	OnResetDevice();

	return true;
}


void WeProjectorRenderer::ShutDown()
{
	if (!bInitialized) return;

	bInitialized = false;
}


void WeProjectorRenderer::OnLostDevice()
{
}

void WeProjectorRenderer::OnResetDevice()
{
}



WeProjectorRenderer::WeProjectorRenderer()
{
	bInitialized = false;
}


void WeProjectorRenderer::ClearAll()
{
	for (LightBatchesSpot.First(); !LightBatchesSpot.End(); )
	{
		delete LightBatchesSpot.GetItem();
		LightBatchesSpot.Delete();
	}	

	for (LightBatchesOmni.First(); !LightBatchesOmni.End(); )
	{
		delete LightBatchesOmni.GetItem();
		LightBatchesOmni.Delete();
	}	
}



void WeProjectorRenderer::PrepareQueuedBatches(WeWorld *world, WeCamera *Camera)
{
	if (!WeRenderDevice::Instance()->GetProjectors()) return;

	// Just make rendering from Octree possible. Currently for prefabs
	ClearAll();

	// find all lights in frustum
	CSmartList <WeLight *> Lights;
	//WeActor *Iterator, *tmp;
	WeLight *LI;
	for(LI = world->Lights->NextLight; LI != world->Lights; LI = LI->NextLight)
	{		
		if (LI->Projector)
		{
			if (Camera->Frustum.Collide(LI->BSphere))
			{				
				WeLightBatch *lbatch;
				lbatch = new WeLightBatch;
				lbatch->Light = LI;
				if (LI->ProjectorIsOmni)
					LightBatchesOmni.PushBack(lbatch);				
				else
					LightBatchesSpot.PushBack(lbatch);									
			}			
		}
	}

	for (WeBatchRenderer::Instance()->ShaderBatchList.First(); !WeBatchRenderer::Instance()->ShaderBatchList.End(); WeBatchRenderer::Instance()->ShaderBatchList.Next())
	{
		for (WeBatchRenderer::Instance()->ShaderBatchList.GetItem()->BatchItems.First(); !WeBatchRenderer::Instance()->ShaderBatchList.GetItem()->BatchItems.End(); WeBatchRenderer::Instance()->ShaderBatchList.GetItem()->BatchItems.Next())
		{
			QueueBatchItem( WeBatchRenderer::Instance()->ShaderBatchList.GetItem()->BatchItems.GetItem() );            
		}
	}

}


void WeProjectorRenderer::RenderOmniBatches(WeWorld *World)
{
	if (LightBatchesOmni.Empty()) return; 

	static WeShader *OmniProjector = NULL;
	OmniProjector = WeShaderManager::Instance()->Load("Effects.Projectors.ProjectorOmni", false, true);

	if (OmniProjector)
	{
		//WeRenderDevice::Instance()->BeginScene();

		WeBatchItem* BatchItem = NULL;
		static WeMaterial* ProjectorMaterial = NULL;
		CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

			
		int k = 0;

		WeProfiler::Instance()->ShaderBegins++;				
		OmniProjector->Begin();

		for(LightBatchesOmni.First(); !LightBatchesOmni.End(); LightBatchesOmni.Next())
		{
			k++;

			OmniProjector->SetProjectorPosition(LightBatchesOmni.GetItem()->Light->Location);
			OmniProjector->SetProjectorMatrix(LightBatchesOmni.GetItem()->Light->TM);
			ProjectorMaterial = LightBatchesOmni.GetItem()->Light->ProjectorMaterial;
			if (!ProjectorMaterial) continue;
			
			for (int i = 0; i < (int)OmniProjector->GetPassesCount(); i++)
			{
				OmniProjector->BeginPass(i);
				tmpBatchItems = &LightBatchesOmni.GetItem()->BatchItems;


				for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
				{			
					
					//if (k == 2) MessageBox(0, "123123", 0, 0);
					BatchItem = tmpBatchItems->GetItem();
					// Tell this Actor not to be occluded
					if (BatchItem->Actor) BatchItem->Actor->OccludedTime = 0;
					// If this item is to be skipped then skip 
					if (BatchItem->SkipItem) continue;				
					// Set data for shader.
					WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
					WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								
							
					// set textures
					ProjectorMaterial->SetTextures(LightBatchesOmni.GetItem()->Light->ObjectAge);
					OmniProjector->SetParams(ProjectorMaterial);
					WeShaderManager::Instance()->SetMaterialColor(ProjectorMaterial->MaterialColor);
					
					
					// set env mapping if we have to sampler env sampler
					OmniProjector->CommitChanges();

					// And finaly draw geometry
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange);
					WeProfiler::Instance()->BatchItemsRendered++;
					WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
				}

				OmniProjector->EndPass();
			}		
		}
		OmniProjector->End();
		//WeRenderDevice::Instance()->EndScene();
	}
}


void WeProjectorRenderer::RenderSpotBatches(WeWorld *World)
{
	if (LightBatchesSpot.Empty()) return; 

	static WeShader *SpotProjector = NULL;
	SpotProjector = WeShaderManager::Instance()->Load("Effects.Projectors.ProjectorSpot", false, true);

	if (SpotProjector)
	{
		//WeRenderDevice::Instance()->BeginScene();

		WeBatchItem* BatchItem = NULL;
		static WeMaterial* ProjectorMaterial = NULL;
		CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

			
		int k = 0;

		WeProfiler::Instance()->ShaderBegins++;				
		SpotProjector->Begin();

		for(LightBatchesSpot.First(); !LightBatchesSpot.End(); LightBatchesSpot.Next())
		{
			k++;
			if (k > 8) break;

			SpotProjector->SetProjectorPosition(LightBatchesSpot.GetItem()->Light->Location);

			WeMatrix M, P;
			WeVector x,y,z;

			LightBatchesSpot.GetItem()->Light->GetLocalAxis(x,y,z);
			x = LightBatchesSpot.GetItem()->Light->Location+z;
			D3DXMatrixLookAtLH(&M, &LightBatchesSpot.GetItem()->Light->Location, &x, &y);		
			D3DXMatrixPerspectiveFovLH(&P, LightBatchesSpot.GetItem()->Light->SpotAngle, 1.0f, 1.0f, LightBatchesSpot.GetItem()->Light->Range);
			M = M * P;
			SpotProjector->SetProjectorMatrix(M);


			ProjectorMaterial = LightBatchesSpot.GetItem()->Light->ProjectorMaterial;
			if (!ProjectorMaterial) continue;	

			for (int i = 0; i < (int)SpotProjector->GetPassesCount(); i++)
			{
				SpotProjector->BeginPass(i);
				tmpBatchItems = &LightBatchesSpot.GetItem()->BatchItems;

				for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
				{			
						
					//if (k == 2) MessageBox(0, "123123", 0, 0);
					BatchItem = tmpBatchItems->GetItem();
					// Tell this Actor not to be occluded
					if (BatchItem->Actor) BatchItem->Actor->OccludedTime = 0;
					// If this item is to be skipped then skip 
					if (BatchItem->SkipItem) continue;				
					// Set data for shader.
					WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
					WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								
							
					// set textures
					ProjectorMaterial->SetTextures(LightBatchesSpot.GetItem()->Light->ObjectAge);
					SpotProjector->SetTextureAnimation(ProjectorMaterial->TextureAnimationM);
					SpotProjector->SetParams(ProjectorMaterial);
					WeShaderManager::Instance()->SetMaterialColor(ProjectorMaterial->MaterialColor);					
					// set env mapping if we have to sampler env sampler
					SpotProjector->CommitChanges();

					// And finaly draw geometry
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange);
					WeProfiler::Instance()->BatchItemsRendered++;
					WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
				}

				SpotProjector->EndPass();
			}		
		}
		SpotProjector->End();
		//WeRenderDevice::Instance()->EndScene();
	}
}



void WeProjectorRenderer::RenderQueuedBatches(WeWorld *World)
{
	if (!WeRenderDevice::Instance()->GetProjectors()) return;

	RenderOmniBatches(World);
	RenderSpotBatches(World);
}

void WeProjectorRenderer::QueueBatchItem(WeBatchItem *BatchItem)
{
	if (BatchItem->Actor->ReceiveProjectors)
	{
		if (!BatchItem->Alpha)
		{
			// if this batch item is in some of found lights. queue it there.
			bool added = false;
			for (LightBatchesOmni.First(); !LightBatchesOmni.End(); LightBatchesOmni.Next())
			{
				if (LightBatchesOmni.GetItem()->Light->BSphere.Intersect(&BatchItem->BSphere))
				{
					// add it sorted
					added = false;
					for (LightBatchesOmni.GetItem()->BatchItems.First(); !LightBatchesOmni.GetItem()->BatchItems.End(); LightBatchesOmni.GetItem()->BatchItems.Next())
					{
						if (LightBatchesOmni.GetItem()->BatchItems.GetItem()->Material == BatchItem->Material)
						{
							LightBatchesOmni.GetItem()->BatchItems.InsertAfter(BatchItem);							
							added = true;
							break;					                            
						}
					}
					if (!added) LightBatchesOmni.GetItem()->BatchItems.PushBack(BatchItem);                                        										
				}
			}

			
			for (LightBatchesSpot.First(); !LightBatchesSpot.End(); LightBatchesSpot.Next())
			{
				if (LightBatchesSpot.GetItem()->Light->BSphere.Intersect(&BatchItem->BSphere))
				{
					// add it sorted
					added = false;
					for (LightBatchesSpot.GetItem()->BatchItems.First(); !LightBatchesSpot.GetItem()->BatchItems.End(); LightBatchesSpot.GetItem()->BatchItems.Next())
					{
						if (LightBatchesSpot.GetItem()->BatchItems.GetItem()->Material == BatchItem->Material)
						{
							LightBatchesSpot.GetItem()->BatchItems.InsertAfter(BatchItem);							
							added = true;
							break;					                            
						}
					}
					if (!added) LightBatchesSpot.GetItem()->BatchItems.PushBack(BatchItem);                                        										
				}
			}
		}
	}
}

