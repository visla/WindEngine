/*
       File: WeBatchRenderer.cpp
	   Author: Tomislav Kukic
	   Date: 12.09.2005.
	   Version: 1.0
*/



#include <WeBatchRenderer.h>
#include <WeMaterial.h>
#include <WeMesh.h>
#include <WeActor.h>
#include <WeBatchItem.h>
#include <WeShaderBatch.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeRenderableNode.h>
#include <WeRenderTree.h>
#include <WeProfiler.h>
#include <WeParticleSystem.h>
#include <WeParticleRenderer.h>
#include <WeReflectionRenderer.h>
#include <WeRenderTarget.h>
#include <WeEngine.h>
#include <WeTrailRenderer.h>
#include <WePostProcess.h>
#include <WeZone.h>
#include <WePortal.h>
#include <WeCamera.h>



WeBatchRenderer* WeBatchRenderer::_instance = NULL;


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeBatchRenderer::WeBatchRenderer(void)
{
	WeRenderDevice::Instance()->Renderers.PushBack(this);
	Preparing = false;
}


WeBatchRenderer::~WeBatchRenderer(void)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeBatchRenderer* WeBatchRenderer::Instance()
{
	if(_instance != NULL) return _instance;
	else{
		_instance = new WeBatchRenderer();
		return _instance;
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::QueryQueuedBatches(WeWorld *World)
{
	WeRenderDevice::Instance()->SetRenderState(D3DRS_COLORWRITEENABLE, 0);

	

	//WeRenderDevice::Instance()->BeginScene();

	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

	WeRenderDevice::Instance()->Device->SetTexture(0,NULL);
	
	// We are using default shader for this quering
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{
		tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;
		for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
		{				
			BatchItem = tmpBatchItems->GetItem();

			if (!BatchItem->QueryMesh) continue;
			if (!BatchItem->Query) continue;

			
			BatchItem->Query->Begin();
			WeRenderDevice::Instance()->Device->SetTransform(D3DTS_WORLD, &BatchItem->WorldMatrix);
			BatchItem->QueryMesh->DrawSubset(&BatchItem->AttribRange);
			BatchItem->Query->End();

			WeProfiler::Instance()->BatchItemsQueried++;
		}
	}


	//WeRenderDevice::Instance()->EndScene();

	// Now just wait queries to finish
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{
		tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;
		for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
		{	
			// if we have query then check pixels drawn
			if (tmpBatchItems->GetItem()->Query)
			{
				if (tmpBatchItems->GetItem()->Query->GetPixelsDrawn() == 0)
				{
					WeProfiler::Instance()->BatchItemsSkipped++;
					tmpBatchItems->GetItem()->SkipItem = true; 					
				}
			}
		}
	}

	WeRenderDevice::Instance()->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	WeRenderDevice::Instance()->Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::DrawQueuedBatches(WeWorld *World)
{
	//WeRenderDevice::Instance()->BeginScene();

	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
	

	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);


	// Go through all shader batches and do diffuse or bump passes
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{
		// set appropriate technique
		if (!ShaderBatchList.GetItem()->Shader->SetTechnique()) continue;				

		ShaderBatchList.GetItem()->Shader->Begin(); 

		
		for (int i = 0; i < (int)ShaderBatchList.GetItem()->Shader->GetPassesCount(); i++)
		{
			ShaderBatchList.GetItem()->Shader->BeginPass(i);
			tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;

			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{				
				BatchItem = tmpBatchItems->GetItem();
				if (!BatchItem->Material->AcceptLights) continue;

				// Tell this Actor not to be occluded
				if (BatchItem->SkipItem) continue;				
			
				SetStandardParams(BatchItem->Material->Shader, BatchItem);


				// Render everything for each light batch item has.
				for (BatchItem->Lights.First(); !BatchItem->Lights.End(); BatchItem->Lights.Next())
				{
					BatchItem->Material->Shader->SetLight(BatchItem->Lights.GetItem());
					ShaderBatchList.GetItem()->Shader->CommitChanges();
					SetAlphaRef(BatchItem->Material, false);

					// And finaly draw geometry
					if (BatchItem->Material->DoubleSided)
					{
						WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
						BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
						WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
						
					}
					else
					{
						BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
					}
					
					WeProfiler::Instance()->PolygonsRendered+= BatchItem->AttribRange.FaceCount;
				}
				WeProfiler::Instance()->BatchItemsRendered++;
			}

			ShaderBatchList.GetItem()->Shader->EndPass();
		}		
		ShaderBatchList.GetItem()->Shader->End();
	}

	SetAlphaRef(NULL, true);

	
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//WeRenderDevice::Instance()->EndScene();

}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::DrawRefractiveBatches(WeWorld *World)
{
	if (!WeRenderDevice::Instance()->GetRefractive()) return;

	LPDIRECT3DSURFACE9 lpSurf;
	lpSurf = WeRenderDevice::Instance()->SaveBackBuffer();

	if (WeRenderDevice::Instance()->GetAntialiasFactor() > 0)
		WeRenderDevice::Instance()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	WeRenderDevice::Instance()->SetRenderTarget(0, WePostProcess::Instance()->RefractiveRT);	
	WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 0.0f, 0L );
	WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	

	
	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;	
	
	// Go through all shader batches and do diffuse or bump passes
	for(RefractiveShaderBatchList.First(); !RefractiveShaderBatchList.End(); RefractiveShaderBatchList.Next())
	{
		// set appropriate technique
		if (!RefractiveShaderBatchList.GetItem()->Shader->SetRefractiveTechnique()) continue;				

		RefractiveShaderBatchList.GetItem()->Shader->Begin(); 


		for (int i = 0; i < (int)RefractiveShaderBatchList.GetItem()->Shader->GetPassesCount(); i++)
		{
			RefractiveShaderBatchList.GetItem()->Shader->BeginPass(i);
			tmpBatchItems = &RefractiveShaderBatchList.GetItem()->BatchItems;

			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{				
				BatchItem = tmpBatchItems->GetItem();
				if (!BatchItem->Material->AcceptLights) continue;

				// Tell this Actor not to be occluded
				if (BatchItem->SkipItem) continue;				


				SetStandardParams(BatchItem->Material->Shader, BatchItem);

				if (BatchItem->Actor->IsParticleSystem)
				{	
					WeParticleRenderer::Instance()->Render(World, (WeParticleSystem *)BatchItem->Actor);								
				} 
				// RENDERING TRAILS
				else if (BatchItem->Actor->Type == WETYPE_TRAIL)
				{

					WeTrailRenderer::Instance()->Render(World, (WeTrail *)BatchItem->Actor);			
				} 
				else
				{

					// And finally draw geometry
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);				
					WeProfiler::Instance()->BatchItemsRendered++;
					WeProfiler::Instance()->PolygonsRendered+= BatchItem->AttribRange.FaceCount;
				}
			}

			RefractiveShaderBatchList.GetItem()->Shader->EndPass();
		}		
		RefractiveShaderBatchList.GetItem()->Shader->End();
	}

    if (WeRenderDevice::Instance()->GetAntialiasFactor() > 0)
		WeRenderDevice::Instance()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	WeRenderDevice::Instance()->RestoreBackBuffer(lpSurf);	
	WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);


}











/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::RenderQueuedBatches(WeWorld* World)
{

    // First we want to query batch items and set skipitem to those occluded	
	if (WeEngine::Instance()->Desc.ItemQuering)
		QueryQueuedBatches(World);

	// Second we want to render actual batch items
	DrawQueuedBatches(World);	

	// show alpha objects cause maybe we are lookging through windows some ps
	DrawAlphaBatches(World);
	
}










/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::PrepareQueuedBatches(WeWorld* World, WeCamera* Camera)
{
	ClearAll();
	Preparing = true;

	if (World->GetZonedRendering()) 
	{
		MessageBox(0, "Currenly not supported on this platform :`)", 0, 0);
		PrepareZonedQueuedBatches(World, Camera);
	}
	else
	{
		// Just make rendering from Octree possible. Currently for prefabs
		CSmartList<WeRenderableNode *> Nodes;
		WeRenderableNode *node;
		WeActor *prefab;

		World->RenderTree->GetNodesInView(Camera, Nodes);

		for (Nodes.First(); !Nodes.End(); Nodes.Next())
		{		
			node = Nodes.GetItem();
			for (node->Prefabs.First(); !node->Prefabs.End(); node->Prefabs.Next())
			{			
				prefab = node->Prefabs.GetItem();
				if (prefab->IntersectFrustum(&Camera->Frustum))
				{
					prefab->OnRender(Camera);
					WeProfiler::Instance()->ObjectsRendered++;
				}
			}
		}
		
		WeActor *Iterator, *tmp;
		for(Iterator = World->Actors->NextActor; Iterator != World->Actors; )
		{
			tmp = Iterator->NextActor;
			if (!Iterator->IsPrefab) 
			{
				WeSphere s;
				if (Iterator->IntersectFrustum(&Camera->Frustum))
				{
					Iterator->OnRender(Camera);	    		
					WeProfiler::Instance()->ObjectsRendered++;
				}
			}
			Iterator = tmp;
		}
	}
	Preparing = false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::PrepareZonedQueuedBatches(WeWorld* World, WeCamera* Camera)
{
	CSmartList <WeZone *> Zones;


	World->GatherCameraZones(Camera, Zones);
	WeProfiler::Instance()->Zones.Clear();

	WeProfiler::Instance()->ZonesVisible = Zones.GetItemsCount();
	for (Zones.First(); !Zones.End(); Zones.Next())
	{		
		WeProfiler::Instance()->Zones.PushBack(Zones.GetItem()->Name);
		for (Zones.GetItem()->Frustums.First(); !Zones.GetItem()->Frustums.End(); Zones.GetItem()->Frustums.Next())
		{
			//if (Zones.GetItem()->Frustums.GetItemsCount() > 1) 
			//{
			//	char tmp[100];
			//	sprintf(tmp, "Frustums count: %d", Zones.GetItem()->Frustums.GetItemsCount());
			//	MessageBox(0, tmp, 0, 0);
			//}
			//if (Zones.GetItemsCount() == 1) MessageBox(0, "Jedna zona", 0, 0);

			for (Zones.GetItem()->Actors.First(); !Zones.GetItem()->Actors.End(); Zones.GetItem()->Actors.Next())
			{
				Zones.GetItem()->Actors.GetItem()->OnRender(Camera);
				WeProfiler::Instance()->ObjectsRendered++;
			}
		}
	}

	WeActor *Iterator, *tmp;
	for(Iterator = World->Actors->NextActor; Iterator != World->Actors; )
	{				
		tmp = Iterator->NextActor;
		if (!Iterator->IsPrefab)  
		{
			Iterator->OnRender(Camera);	    		
			WeProfiler::Instance()->ObjectsRendered++;
		}
		Iterator = tmp;
	}

}






/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::ClearAll()
{
   // clear particle renderer 
	WeParticleRenderer::Instance()->ClearAll();

	// clear other objects
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{
		delete ShaderBatchList.GetItem();	
	}
	ShaderBatchList.Clear();


    // clear refractive lists
	for(RefractiveShaderBatchList.First(); !RefractiveShaderBatchList.End(); RefractiveShaderBatchList.Next())
	{
		delete RefractiveShaderBatchList.GetItem();	
	}
	RefractiveShaderBatchList.Clear();


	// clear alpha lists
	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
		delete AlphaBatchList.GetItem();

	AlphaBatchList.Clear();



	// clear foreground lists
	for(ForegroundBatchList.First(); !ForegroundBatchList.End(); ForegroundBatchList.Next())
		delete ForegroundBatchList.GetItem();

	ForegroundBatchList.Clear();
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::QueueAlphaBatch(WeBatchItem *BatchItem)
{
	WeProfiler::Instance()->AlphaBatchItemsQueued++;

	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
	{
		if(AlphaBatchList.GetItem()->ZDist <= BatchItem->ZDist)
		{				
			AlphaBatchList.InsertBefore(BatchItem);
			return;
		}
	}
	AlphaBatchList.PushBack(BatchItem);				
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::QueueForegroundBatch(WeBatchItem *BatchItem)
{
	WeProfiler::Instance()->ForegroundBatchItemsQueued++;

	for(ForegroundBatchList.First(); !ForegroundBatchList.End(); ForegroundBatchList.Next())
	{
		if(ForegroundBatchList.GetItem()->ZDist <= BatchItem->ZDist)
		{				
			ForegroundBatchList.InsertBefore(BatchItem);
			return;
		}
	}
	ForegroundBatchList.PushBack(BatchItem);				
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::QueueBatchItem(WeBatchItem* BatchItem)
{
	if (!Preparing) return;

	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
	WeShaderBatch* tmpBatch = NULL;

	WeProfiler::Instance()->BatchItemsQueued++;

	// For queuing reflective materials
	if (BatchItem->Material->ReflectionType == WEREFLECTIONTYPE_REALTIME)
	{
		// if this reflection renderer took care of this then return;
		if (WeReflectionRenderer::Instance()->QueueBatchItem(BatchItem))
		{
            return;
		}
	}
	
	// for queing alpha blended materials
	if (BatchItem->Material->Type == WEMATERIALTYPE_ALPHABLENDED)
	{
		// different batching if alpha objects are used
		QueueAlphaBatch(BatchItem);
		return;
	}

	if (BatchItem->Material->Type == WEMATERIALTYPE_REFRACTIVE)
	{
		QueueRefractiveBatch(BatchItem);
		return;
	}

	if (BatchItem->Material->Type == WEMATERIALTYPE_FOREGROUND)
	{
		// different batching if alpha objects are used
		QueueForegroundBatch(BatchItem);
		return;
	}

	// This is normal Batch so do rendering normal way
	// --------------------------------------------------------------------------
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{
		tmpBatch = ShaderBatchList.GetItem();
		if(tmpBatch->Shader == BatchItem->Material->Shader)
		{

			// For normal material 
			tmpBatchItems = &tmpBatch->BatchItems;
			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{
				if(tmpBatchItems->GetItem()->Material == BatchItem->Material)
				{				
					tmpBatchItems->InsertAfter(BatchItem);
					return;
				}
			}
			tmpBatchItems->PushBack(BatchItem);
			return;
		}
	}

	WeShaderBatch* tmpShaderBatch = new WeShaderBatch;
	tmpShaderBatch->Shader = BatchItem->Material->Shader;
	tmpShaderBatch->BatchItems.PushBack(BatchItem);

	ShaderBatchList.PushBack(tmpShaderBatch);
}









/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::QueueRefractiveBatch(WeBatchItem *BatchItem)
{
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
	WeShaderBatch* tmpBatch = NULL;

	for(RefractiveShaderBatchList.First(); !RefractiveShaderBatchList.End(); RefractiveShaderBatchList.Next())
	{
		tmpBatch = RefractiveShaderBatchList.GetItem();
		if(tmpBatch->Shader == BatchItem->Material->Shader)
		{			
			tmpBatchItems = &tmpBatch->BatchItems;
			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{
				if(tmpBatchItems->GetItem()->Material == BatchItem->Material)
				{				
					tmpBatchItems->InsertAfter(BatchItem);
					return;
				}
			}
			tmpBatchItems->PushBack(BatchItem);
			return;
		}
	}

	WeShaderBatch* tmpShaderBatch = new WeShaderBatch;
	tmpShaderBatch->Shader = BatchItem->Material->Shader;
	tmpShaderBatch->BatchItems.PushBack(BatchItem);

	RefractiveShaderBatchList.PushBack(tmpShaderBatch);
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::RenderAlphaBatchesHigh(WeWorld *World)
{
	if (AlphaBatchList.Empty()) return;

	//WeRenderDevice::Instance()->BeginScene();


	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	WeShader *CurrentShader;

	CurrentShader = NULL;
	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
	{
		// If this item is to be skipped then skip 
		if (AlphaBatchList.GetItem()->SkipItem) continue;		

		if (!AlphaBatchList.GetItem()->Material->AcceptLights &&
			!AlphaBatchList.GetItem()->Actor->IsParticleSystem &&
			AlphaBatchList.GetItem()->Actor->Type != WETYPE_TRAIL) continue;

		WeProfiler::Instance()->AlphaBatchItemsRendered++;

		// Turn on shader if we need to
		if (CurrentShader != AlphaBatchList.GetItem()->Material->Shader)
		{
			// end last shader
			if (CurrentShader) CurrentShader->End();

			// begin with new shader
			CurrentShader = AlphaBatchList.GetItem()->Material->Shader;			
			if (!CurrentShader->SetTechnique()) continue;
			CurrentShader->Begin();			
		}


		// go for all passes
		for (int i = 0; i < (int)CurrentShader->GetPassesCount(); i++)
		{
			// Set data for shader.
			CurrentShader->BeginPass(i);

			SetStandardParams(AlphaBatchList.GetItem()->Material->Shader, AlphaBatchList.GetItem());


			// If this is particle system then render it
			// PARTICLE SYSTEM
			if (AlphaBatchList.GetItem()->Actor->IsParticleSystem)
			{
				WeParticleRenderer::Instance()->Render(World, (WeParticleSystem *)AlphaBatchList.GetItem()->Actor);
			}
			// RENDERING TRAILS
			else if (AlphaBatchList.GetItem()->Actor->Type == WETYPE_TRAIL)
			{
				WeTrailRenderer::Instance()->Render(World, (WeTrail *)AlphaBatchList.GetItem()->Actor);
			} 
			// NORMAL MODELS
			else
			{
				SetAlphaRef(AlphaBatchList.GetItem()->Material, false);

				for (AlphaBatchList.GetItem()->Lights.First(); !AlphaBatchList.GetItem()->Lights.End(); AlphaBatchList.GetItem()->Lights.Next())
				{
					AlphaBatchList.GetItem()->Material->Shader->SetLight(AlphaBatchList.GetItem()->Lights.GetItem());
					AlphaBatchList.GetItem()->Material->Shader->CommitChanges();

					// And finaly draw geometry
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
					AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);
					WeProfiler::Instance()->PolygonsRendered += AlphaBatchList.GetItem()->AttribRange.FaceCount*2;
				}
				WeProfiler::Instance()->BatchItemsRendered++;					
			}

			CurrentShader->EndPass();
		}
	}
	if (CurrentShader)	CurrentShader->End();

	SetAlphaRef(NULL, true);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//WeRenderDevice::Instance()->EndScene();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::RenderBatchesHigh(WeWorld *World)
{
	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{		
		if (!ShaderBatchList.GetItem()->Shader->SetTechnique()) continue;

		ShaderBatchList.GetItem()->Shader->Begin();

		for (int i = 0; i < (int)ShaderBatchList.GetItem()->Shader->GetPassesCount(); i++)
		{
			ShaderBatchList.GetItem()->Shader->BeginPass(i);
			tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;

			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{				
				BatchItem = tmpBatchItems->GetItem();

				// set alpha ref.
				SetStandardParams(ShaderBatchList.GetItem()->Shader, BatchItem);
				SetAlphaRef(BatchItem->Material, false);
				ShaderBatchList.GetItem()->Shader->SetLights(BatchItem->Lights);
				

				// And finaly draw geometry
				if (BatchItem->Material->DoubleSided)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

				} else
				{
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
				}

				WeProfiler::Instance()->BatchItemsRendered++;
				WeProfiler::Instance()->PolygonsRendered+= BatchItem->AttribRange.FaceCount;
			}
			ShaderBatchList.GetItem()->Shader->EndPass();			
		}		
		ShaderBatchList.GetItem()->Shader->End();
	}
	SetAlphaRef(NULL, true);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::RenderZBuffer(WeWorld *World)
{
	//WeRenderDevice::Instance()->BeginScene();

	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

	WeShader *ZBufferEffect;
	ZBufferEffect = WeShaderManager::Instance()->Load("Effects.Default.ZBuffer", false, true);
	if (!ZBufferEffect) return;

	
	ZBufferEffect->Begin();
	ZBufferEffect->BeginPass(0);

	// Render normal batches only to ZBuffer
	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{		
		tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;

		for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
		{				
			BatchItem = tmpBatchItems->GetItem();

			// set alpha ref.
			SetStandardParams(ZBufferEffect, BatchItem);
			SetAlphaRef(BatchItem->Material, false);

			// And finaly draw geometry
			if (BatchItem->Material->DoubleSided)
			{
				WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			} else
			{
				BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
			}

			WeProfiler::Instance()->BatchItemsRendered++;
			WeProfiler::Instance()->PolygonsRendered+= BatchItem->AttribRange.FaceCount;
		}
	}


	// Render alpha batches only to ZBuffer
	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
	{
		BatchItem = AlphaBatchList.GetItem();

		if (BatchItem->Actor->Type == WETYPE_PARTICLESYSTEM ||
			BatchItem->Actor->Type == WETYPE_TRAIL) continue;

		// If this item is to be skipped then skip 
		
		if (BatchItem->SkipItem) continue;
		WeProfiler::Instance()->AlphaBatchItemsRendered++;

		SetStandardParams(BatchItem->Material->Shader, BatchItem);

		WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
		WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);

		WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount*2;
	}


	ZBufferEffect->EndPass();
	ZBufferEffect->End();

	SetAlphaRef(NULL, true);
	//WeRenderDevice::Instance()->EndScene();	
}






/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::DrawAlphaBatches(WeWorld *World)
{
	if (AlphaBatchList.Empty()) return;

	//WeRenderDevice::Instance()->BeginScene();
	

	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	WeShader *CurrentShader;

	CurrentShader = NULL;
	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
	{
		// If this item is to be skipped then skip 
		if (AlphaBatchList.GetItem()->SkipItem) continue;		

		if (!AlphaBatchList.GetItem()->Material->AcceptLights &&
			!AlphaBatchList.GetItem()->Actor->IsParticleSystem &&
			AlphaBatchList.GetItem()->Actor->Type != WETYPE_TRAIL) continue;

		WeProfiler::Instance()->AlphaBatchItemsRendered++;

		// Turn on shader if we need to
		if (CurrentShader != AlphaBatchList.GetItem()->Material->Shader)
		{
			// end last shader
			if (CurrentShader) CurrentShader->End();

			// begin with new shader
			CurrentShader = AlphaBatchList.GetItem()->Material->Shader;			
			if (!CurrentShader->SetTechnique()) continue;
			CurrentShader->Begin();			
		}


		// go for all passes
		for (int i = 0; i < (int)CurrentShader->GetPassesCount(); i++)
		{
			// Set data for shader.
			CurrentShader->BeginPass(i);

			SetStandardParams(AlphaBatchList.GetItem()->Material->Shader, AlphaBatchList.GetItem());


			// If this is particle system then render it
			// PARTICLE SYSTEM
			if (AlphaBatchList.GetItem()->Actor->IsParticleSystem)
			{
				WeParticleRenderer::Instance()->Render(World, (WeParticleSystem *)AlphaBatchList.GetItem()->Actor);
			}
			// RENDERING TRAILS
			else if (AlphaBatchList.GetItem()->Actor->Type == WETYPE_TRAIL)
			{
				WeTrailRenderer::Instance()->Render(World, (WeTrail *)AlphaBatchList.GetItem()->Actor);
			} 
			// NORMAL MODELS
			else
			{
				SetAlphaRef(AlphaBatchList.GetItem()->Material, false);

				for (AlphaBatchList.GetItem()->Lights.First(); !AlphaBatchList.GetItem()->Lights.End(); AlphaBatchList.GetItem()->Lights.Next())
				{
					AlphaBatchList.GetItem()->Material->Shader->SetLight(AlphaBatchList.GetItem()->Lights.GetItem());
					AlphaBatchList.GetItem()->Material->Shader->CommitChanges();

					// And finaly draw geometry
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
					AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);
					WeProfiler::Instance()->PolygonsRendered += AlphaBatchList.GetItem()->AttribRange.FaceCount*2;
				}
				WeProfiler::Instance()->BatchItemsRendered++;					
			}

			CurrentShader->EndPass();
		}
	}
	if (CurrentShader)	CurrentShader->End();

	SetAlphaRef(NULL, true);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//WeRenderDevice::Instance()->EndScene();
}






/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeBatchRenderer::DrawForegroundBatches(WeWorld *World)
{
	if (ForegroundBatchList.Empty()) return;

	//WeRenderDevice::Instance()->BeginScene();	

	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZENABLE, FALSE);

	WeShader *CurrentShader;
	WEMATERIAL_BLEND_TYPE LastBlendType;

	LastBlendType = WEMATERIAL_BLEND_STANDARD;

	CurrentShader = NULL;
	for(ForegroundBatchList.First(); !ForegroundBatchList.End(); ForegroundBatchList.Next())
	{
		if (ForegroundBatchList.GetItem()->Actor->Type == WETYPE_PARTICLESYSTEM ||
			ForegroundBatchList.GetItem()->Actor->Type == WETYPE_TRAIL) continue;

		// If this item is to be skipped then skip 
		if (ForegroundBatchList.GetItem()->SkipItem) continue;
		WeProfiler::Instance()->AlphaBatchItemsRendered++;
		// Turn on shader if we need to
		if (CurrentShader != ForegroundBatchList.GetItem()->Material->Shader)
		{
			// end last shader
			if (CurrentShader) CurrentShader->End();

			// begin with new shader
			CurrentShader = ForegroundBatchList.GetItem()->Material->Shader;			
			if (!CurrentShader->SetAmbientTechnique()) continue;
			CurrentShader->Begin();			
		}

		// go for all passes
		for (int i = 0; i < (int)CurrentShader->GetPassesCount(); i++)
		{						
			// Set data for shader.
			CurrentShader->BeginPass(i);
			//if (ForegroundBatchList.GetItem()->Actor) 
			//{
			//	if (ForegroundBatchList.GetItem()->Actor->UseMaterialParams)
			//	{
			//		ForegroundBatchList.GetItem()->Material->Shader->SetParam1(ForegroundBatchList.GetItem()->Actor->MaterialParam1);
			//		ForegroundBatchList.GetItem()->Material->Shader->SetParam2(ForegroundBatchList.GetItem()->Actor->MaterialParam2);
			//		ForegroundBatchList.GetItem()->Material->Shader->SetParam3(ForegroundBatchList.GetItem()->Actor->MaterialParam3);
			//		ForegroundBatchList.GetItem()->Material->Shader->SetParam4(ForegroundBatchList.GetItem()->Actor->MaterialParam4);					                    
			//		WeShaderManager::Instance()->SetMaterialColor(ForegroundBatchList.GetItem()->Actor->MaterialColor);
			//	} else
			//	{
			//		ForegroundBatchList.GetItem()->Material->Shader->SetParams(ForegroundBatchList.GetItem()->Material);
			//		WeShaderManager::Instance()->SetMaterialColor(ForegroundBatchList.GetItem()->Material->MaterialColor);
			//	}

			//	ForegroundBatchList.GetItem()->Actor->OccludedTime = 0;
			//}

			//ForegroundBatchList.GetItem()->Material->Shader->SetScaling(ForegroundBatchList.GetItem()->Actor->Scale);
			//// If this is particle system then render it
			//WeShaderManager::Instance()->SetWorldMatrix(ForegroundBatchList.GetItem()->WorldMatrix);
			//WeShaderManager::Instance()->SetPrevWorldMatrix(ForegroundBatchList.GetItem()->PrevWorldMatrix);								

			////WeShaderManager::Instance()->SetLights(WeRenderDevice::Instance()->GetLight());
			//ForegroundBatchList.GetItem()->Actor->LightMap.Use();								
			//ForegroundBatchList.GetItem()->Material->Shader->SetLights(ForegroundBatchList.GetItem()->Lights); 

			//// Here just set material textures								
			//ForegroundBatchList.GetItem()->Material->SetTextures(ForegroundBatchList.GetItem()->Actor->ObjectAge);
			//ForegroundBatchList.GetItem()->Material->Shader->SetParams(ForegroundBatchList.GetItem()->Material);

			//// env map texture
			////if (AlphaBatchList.GetItem()->EnvMap) WeRenderDevice::Instance()->Device->SetTexture(AlphaBatchList.GetItem()->Material->EnvMapSampler, AlphaBatchList.GetItem()->EnvMap->GetTexture());


			//// And finaly draw geometry. 
			//// First draw backfaces and then front faces
			//ForegroundBatchList.GetItem()->Material->Shader->CommitChanges();
			SetStandardParams(ForegroundBatchList.GetItem()->Material->Shader, ForegroundBatchList.GetItem());
			SetAlphaRef(ForegroundBatchList.GetItem()->Material, false);

			// Turn on blend type if it is not standard
			if (LastBlendType != ForegroundBatchList.GetItem()->Material->BlendType)
			{
				if (ForegroundBatchList.GetItem()->Material->BlendType == WEMATERIAL_BLEND_ADDITIVE)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				}
				else if (ForegroundBatchList.GetItem()->Material->BlendType == WEMATERIAL_BLEND_STANDARD)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				}				
			}
			LastBlendType = ForegroundBatchList.GetItem()->Material->BlendType;

			WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			ForegroundBatchList.GetItem()->Mesh->DrawSubset(&ForegroundBatchList.GetItem()->AttribRange, ForegroundBatchList.GetItem()->ZDist);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			ForegroundBatchList.GetItem()->Mesh->DrawSubset(&ForegroundBatchList.GetItem()->AttribRange, ForegroundBatchList.GetItem()->ZDist);

			WeProfiler::Instance()->PolygonsRendered += ForegroundBatchList.GetItem()->AttribRange.FaceCount*2;
			CurrentShader->EndPass();
		}
	}
	if (CurrentShader)	CurrentShader->End();

	SetAlphaRef(NULL, true);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZENABLE, TRUE);

	//WeRenderDevice::Instance()->EndScene();
}







void WeBatchRenderer::SetAlphaRef(WeMaterial *Material, bool TurnOff)
{
	
	if (!Material) 
	{
		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHAREF, 0);
		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	} 
	//else if (TurnOff || Material->AlphaRef == 0) 
	//{
	//	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//	Turned = false;
	//}
	else
	{
		//if (Material->AlphaRef > 0)
		//{
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHAREF, Material->AlphaRef);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		//}
	}
}


void WeBatchRenderer::SetStandardParams(WeShader *Shader, WeBatchItem *BatchItem)
{
	if (BatchItem->Actor) BatchItem->Actor->OccludedTime = 0;

	WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
	WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								

	BatchItem->Material->SetTextures(BatchItem->Actor->ObjectAge);				
	if (BatchItem->Actor->UseMaterialParams)
	{		
		BatchItem->Material->Shader->SetParam1(BatchItem->Actor->MaterialParam1);
		BatchItem->Material->Shader->SetParam2(BatchItem->Actor->MaterialParam2);
		BatchItem->Material->Shader->SetParam3(BatchItem->Actor->MaterialParam3);
		BatchItem->Material->Shader->SetParam4(BatchItem->Actor->MaterialParam4);					                    
		WeShaderManager::Instance()->SetMaterialColor(BatchItem->Actor->MaterialColor);
	} else
	{
		WeShaderManager::Instance()->SetMaterialColor(BatchItem->Material->MaterialColor);
	}

	BatchItem->Actor->LightMap.Use();	
	BatchItem->Material->Shader->SetScaling(BatchItem->Actor->Scale);
	BatchItem->Material->Shader->SetParams(BatchItem->Material);
	BatchItem->Material->Shader->CommitChanges();


}

void WeBatchRenderer::RenderAmbientBatches(WeWorld *World)
{

	WeBatchItem* BatchItem = NULL;
	CSmartList<WeBatchItem*> *tmpBatchItems = NULL;

	for(ShaderBatchList.First(); !ShaderBatchList.End(); ShaderBatchList.Next())
	{		
		if (!ShaderBatchList.GetItem()->Shader->SetAmbientTechnique()) continue;

		ShaderBatchList.GetItem()->Shader->Begin();

		for (int i = 0; i < (int)ShaderBatchList.GetItem()->Shader->GetPassesCount(); i++)
		{
			ShaderBatchList.GetItem()->Shader->BeginPass(i);
			tmpBatchItems = &ShaderBatchList.GetItem()->BatchItems;

			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{				
				BatchItem = tmpBatchItems->GetItem();

				// set alpha ref.
				SetStandardParams(ShaderBatchList.GetItem()->Shader, BatchItem);
				SetAlphaRef(BatchItem->Material, false);

				// And finaly draw geometry
				if (BatchItem->Material->DoubleSided)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

				}
				else
				{
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
				}

				WeProfiler::Instance()->BatchItemsRendered++;
				WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
			}
			ShaderBatchList.GetItem()->Shader->EndPass();			
		}		
		ShaderBatchList.GetItem()->Shader->End();
	}
	SetAlphaRef(NULL, true);
}

void WeBatchRenderer::RenderAmbientAlphaBatches(WeWorld *World)
{
	if (AlphaBatchList.Empty()) return;

	//WeRenderDevice::Instance()->BeginScene();	

	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	WeShader *CurrentShader;
	WEMATERIAL_BLEND_TYPE LastBlendType;

	LastBlendType = WEMATERIAL_BLEND_STANDARD;

	CurrentShader = NULL;
	for(AlphaBatchList.First(); !AlphaBatchList.End(); AlphaBatchList.Next())
	{
		if (AlphaBatchList.GetItem()->Actor->Type == WETYPE_PARTICLESYSTEM ||
			AlphaBatchList.GetItem()->Actor->Type == WETYPE_TRAIL) continue;

		// If this item is to be skipped then skip 
		if (AlphaBatchList.GetItem()->SkipItem) continue;
		WeProfiler::Instance()->AlphaBatchItemsRendered++;
		// Turn on shader if we need to
		if (CurrentShader != AlphaBatchList.GetItem()->Material->Shader)
		{
			// end last shader
			if (CurrentShader) CurrentShader->End();

			// begin with new shader
			CurrentShader = AlphaBatchList.GetItem()->Material->Shader;			
			if (!CurrentShader->SetAmbientTechnique()) continue;
			CurrentShader->Begin();			
		}

		// go for all passes
		for (int i = 0; i < (int)CurrentShader->GetPassesCount(); i++)
		{						
			// Set data for shader.
			CurrentShader->BeginPass(i);
			SetStandardParams(AlphaBatchList.GetItem()->Material->Shader, AlphaBatchList.GetItem());

			// Turn on blend type if it is not standard
			if (LastBlendType != AlphaBatchList.GetItem()->Material->BlendType)
			{
				if (AlphaBatchList.GetItem()->Material->BlendType == WEMATERIAL_BLEND_ADDITIVE)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				}
				else if (AlphaBatchList.GetItem()->Material->BlendType == WEMATERIAL_BLEND_STANDARD)
				{
					WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				}				
			}
			LastBlendType = AlphaBatchList.GetItem()->Material->BlendType;

			WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			AlphaBatchList.GetItem()->Mesh->DrawSubset(&AlphaBatchList.GetItem()->AttribRange, AlphaBatchList.GetItem()->ZDist);

			WeProfiler::Instance()->PolygonsRendered += AlphaBatchList.GetItem()->AttribRange.FaceCount*2;
			CurrentShader->EndPass();
		}
	}
	if (CurrentShader)	CurrentShader->End();

	SetAlphaRef(NULL, true);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//WeRenderDevice::Instance()->EndScene();
}
