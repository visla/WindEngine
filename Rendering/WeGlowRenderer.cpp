/*
WeGlowRenderer.cpp

Autor: Nebojsa Vislavski
Date: 22.9.2006.
Version: 1.0



*/

#include <WeGlowRenderer.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeBatchItem.h>
#include <WeLight.h>
#include <WeRenderableNode.h>
#include <WeParticleRenderer.h>
#include <WeRenderTree.h>
#include <WeBatchRenderer.h>
#include <WeShaderBatch.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeProfiler.h>
#include <WeMaterial.h>
#include <WeMaterialManager.h>
#include <WeShaderManager.h>
#include <WePostProcess.h>
#include <WeMesh.h>
#include <WeSkyBox.h>
#include <WeTimer.h>
#include <WeLoger.h>



WeGlowRenderer *WeGlowRenderer::_instance = NULL;


WeGlowRenderer *WeGlowRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeGlowRenderer;
	return _instance;
}

bool WeGlowRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	OnResetDevice();

	return true;
}


void WeGlowRenderer::ShutDown()
{
	if (!bInitialized) return;

	bInitialized = false;
}


void WeGlowRenderer::OnLostDevice()
{
}

void WeGlowRenderer::OnResetDevice()
{
}



WeGlowRenderer::WeGlowRenderer()
{
	bInitialized = false;
}



void WeGlowRenderer::RenderQueuedBatches(WeWorld *World, WeCamera *Cam)
{
	static float ShaderParam1 = 0;
	// if pixel motion blur is turned off or we are not capturing using post process there is no need to render
	// to velocity texture
	if (!WeRenderDevice::Instance()->GetPostProcessEffects()) return;
	if (!WeRenderDevice::Instance()->GetGlow()) return;	


	static WeShader *GlowShader = NULL;
	GlowShader = WeShaderManager::Instance()->Load("Effects.Default.Glow", false, true);	

	if (GlowShader)
	{
		LPDIRECT3DSURFACE9 lpSurf;
		LPDIRECT3DSURFACE9 lpOldDepthSurf;

		lpSurf = WeRenderDevice::Instance()->SaveBackBuffer();

		if (!WeRenderDevice::Instance()->lpNotMultisampledDepthSurf)
			WeRenderDevice::Instance()->Device->CreateDepthStencilSurface(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &WeRenderDevice::Instance()->lpNotMultisampledDepthSurf, NULL);

		//WeRenderDevice::Instance()->BeginScene();
		WeRenderDevice::Instance()->SetRenderTarget(0, WePostProcess::Instance()->GlowRT);

		WeRenderDevice::Instance()->Device->GetDepthStencilSurface(&lpOldDepthSurf);
		WeRenderDevice::Instance()->Device->SetDepthStencilSurface(WeRenderDevice::Instance()->lpNotMultisampledDepthSurf);

		// WeVelocityRenderer already clears ZBUFFER
		if (WeRenderDevice::Instance()->GetPixelMotionBlur())
			WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 0.0f, 0L );
		else
			WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
		WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);


		WeBatchItem* BatchItem = NULL;
		CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
		CSmartList<WeShaderBatch *> *ShaderBatches= NULL;
				
		GlowShader->Begin();
		GlowShader->SetAmbientTechnique();
		World->SkyBox->OnRender(Cam, GlowShader);		


		ShaderBatches = &WeBatchRenderer::Instance()->ShaderBatchList;

		for (int i = 0; i < (int)GlowShader->GetPassesCount(); i++)
		{
			GlowShader->BeginPass(i);
			for(ShaderBatches->First(); !ShaderBatches->End(); ShaderBatches->Next())
			{				
				tmpBatchItems = &ShaderBatches->GetItem()->BatchItems;
				for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
				{										
					BatchItem = tmpBatchItems->GetItem();
					if (!BatchItem->Material->Glowing) continue;
					if (BatchItem->Actor) 
					{
						if (BatchItem->Actor->UseMaterialParams)
						{
							GlowShader->SetParam1(BatchItem->Actor->MaterialParam1);
							GlowShader->SetParam2(BatchItem->Actor->MaterialParam2);
							GlowShader->SetParam3(BatchItem->Actor->MaterialParam3);
							GlowShader->SetParam4(BatchItem->Actor->MaterialParam4);					                    
							WeShaderManager::Instance()->SetMaterialColor(BatchItem->Actor->MaterialColor);

						} else
						{
							GlowShader->SetParams(BatchItem->Material);
							WeShaderManager::Instance()->SetMaterialColor(BatchItem->Material->MaterialColor);
						}
						BatchItem->Actor->OccludedTime = 0;			
					}	
					// If this item is to be skipped then skip 
					if (BatchItem->SkipItem) continue;				
					// Set data for shader.
					WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
					WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								
				
					BatchItem->Material->SetTextures(BatchItem->Actor->ObjectAge);
					GlowShader->SetTextureAnimation(BatchItem->Material->TextureAnimationM);
					GlowShader->SetParams(BatchItem->Material);
					GlowShader->CommitChanges();

					// And finaly draw geometry
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
					WeProfiler::Instance()->BatchItemsRendered++;
					WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
				}				
			}

			// ========================================================
			// do it for alpha batch items
			tmpBatchItems = &WeBatchRenderer::Instance()->AlphaBatchList;
			for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
			{										
				BatchItem = tmpBatchItems->GetItem();
				if (!BatchItem->Material->Glowing) continue;
				if (BatchItem->SkipItem) continue;				
				if (BatchItem->Actor) 
				{
					if (BatchItem->Actor->UseMaterialParams)
					{
						GlowShader->SetParam1(BatchItem->Actor->MaterialParam1);
						GlowShader->SetParam2(BatchItem->Actor->MaterialParam2);
						GlowShader->SetParam3(BatchItem->Actor->MaterialParam3);
						GlowShader->SetParam4(BatchItem->Actor->MaterialParam4);					                    
						WeShaderManager::Instance()->SetMaterialColor(BatchItem->Actor->MaterialColor);

					} else
					{
						GlowShader->SetParams(BatchItem->Material);
						WeShaderManager::Instance()->SetMaterialColor(BatchItem->Material->MaterialColor);				
					}
					BatchItem->Actor->OccludedTime = 0;			
				}	
				if (BatchItem->Actor->IsParticleSystem) continue;
				if (BatchItem->Actor->Type == WETYPE_TRAIL) continue;
				// If this item is to be skipped then skip 
				// Set data for shader.
				WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
				WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								

				BatchItem->Material->SetTextures(BatchItem->Actor->ObjectAge);
				GlowShader->SetTextureAnimation(BatchItem->Material->TextureAnimationM);
				GlowShader->SetParams(BatchItem->Material);
				
				GlowShader->CommitChanges();

				// And finaly draw geometry
				BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange, BatchItem->ZDist);
				WeProfiler::Instance()->BatchItemsRendered++;
				WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
			}		

			GlowShader->EndPass();
		}


		GlowShader->End();
		//WeRenderDevice::Instance()->EndScene();

		WeRenderDevice::Instance()->RestoreBackBuffer(lpSurf);
		WeRenderDevice::Instance()->Device->SetDepthStencilSurface(lpOldDepthSurf);
		// clear z buffer so we can draw again
		WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);
		//WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
	}
}

