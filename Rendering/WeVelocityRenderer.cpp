/*
    WeVelocityRenderer.cpp

    Autor: Nebojsa Vislavski
	Date: 13.7.2006.
	Version: 1.0

	

*/

#include <WeVelocityRenderer.h>
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
#include <WeShaderManager.h>
#include <WePostProcess.h>
#include <WeMesh.h>
#include <WeSkyBox.h>
#include <WeTimer.h>
#include <WeLoger.h>



WeVelocityRenderer *WeVelocityRenderer::_instance = NULL;


WeVelocityRenderer *WeVelocityRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeVelocityRenderer;
	return _instance;
}

bool WeVelocityRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	OnResetDevice();

	return true;
}


void WeVelocityRenderer::ShutDown()
{
	if (!bInitialized) return;

	bInitialized = false;
}


void WeVelocityRenderer::OnLostDevice()
{
}

void WeVelocityRenderer::OnResetDevice()
{
}



WeVelocityRenderer::WeVelocityRenderer()
{
	bInitialized = false;
}



void WeVelocityRenderer::RenderQueuedBatches(WeWorld *World, WeCamera *Cam)
{
	// if pixel motion blur is turned off or we are not capturing using post process there is no need to render
	// to velocity texture
	if (!WeRenderDevice::Instance()->GetPostProcessEffects()) return;
	if (!WeRenderDevice::Instance()->GetPixelMotionBlur()) return;	
	if (!WePostProcess::Instance()->IsCapturing()) return;
	if (!WePostProcess::Instance()->VelocityRT) return;
	
	static float ShaderParam1 = 0;
	float DeltaTime = WeTimer::Instance()->GetDeltaTimeF();
	if (DeltaTime != 0)
		ShaderParam1 = 1 / DeltaTime;


	static WeShader *MotionShader = NULL;
	MotionShader = WeShaderManager::Instance()->Load("Effects.Default.PixelMotionBlur", false, true);	

	if (MotionShader)
	{
		LPDIRECT3DSURFACE9 lpSurf;
		LPDIRECT3DSURFACE9 lpOldDepthSurf;

		lpSurf = WeRenderDevice::Instance()->SaveBackBuffer();

		if (!WeRenderDevice::Instance()->lpNotMultisampledDepthSurf)
			WeRenderDevice::Instance()->Device->CreateDepthStencilSurface(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &WeRenderDevice::Instance()->lpNotMultisampledDepthSurf, NULL);



		//WeRenderDevice::Instance()->BeginScene();
		WeRenderDevice::Instance()->SetRenderTarget(0, WePostProcess::Instance()->VelocityRT);
		WeRenderDevice::Instance()->Device->GetDepthStencilSurface(&lpOldDepthSurf);
		WeRenderDevice::Instance()->Device->SetDepthStencilSurface(WeRenderDevice::Instance()->lpNotMultisampledDepthSurf);
		
		WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
		WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);
		

		WeBatchItem* BatchItem = NULL;
		CSmartList<WeBatchItem*> *tmpBatchItems = NULL;
		CSmartList<WeShaderBatch *> *ShaderBatches= NULL;
	
		WeProfiler::Instance()->ShaderBegins++;				
		MotionShader->Begin();

		World->SkyBox->OnRender(Cam, MotionShader);

		ShaderBatches = &WeBatchRenderer::Instance()->ShaderBatchList;

		for(ShaderBatches->First(); !ShaderBatches->End(); ShaderBatches->Next())
		{
			for (int i = 0; i < (int)MotionShader->GetPassesCount(); i++)
			{
				MotionShader->BeginPass(i);

				tmpBatchItems = &ShaderBatches->GetItem()->BatchItems;

				for(tmpBatchItems->First(); !tmpBatchItems->End(); tmpBatchItems->Next())
				{							
					BatchItem = tmpBatchItems->GetItem();
					// Tell this Actor not to be occluded
					if (BatchItem->Actor) BatchItem->Actor->OccludedTime = 0;
					// If this item is to be skipped then skip 
					if (BatchItem->SkipItem) continue;				
					// Set data for shader.
					WeShaderManager::Instance()->SetWorldMatrix(BatchItem->WorldMatrix);								
					WeShaderManager::Instance()->SetPrevWorldMatrix(BatchItem->PrevWorldMatrix);								

					//MotionShader->SetParam1(WeRenderDevice::Instance()->GetPixelMotionBlurFactor());
					MotionShader->SetParam1(ShaderParam1);
					//WeLoger::DebugLog("Param1 = %20.3f / %20.20f\n", DeltaTime, ShaderParam1);
							
					// set textures
					// set env mapping if we have to sampler env sampler
					MotionShader->CommitChanges();

					// And finaly draw geometry
					BatchItem->Mesh->DrawSubset(&BatchItem->AttribRange);
					WeProfiler::Instance()->BatchItemsRendered++;
					WeProfiler::Instance()->PolygonsRendered += BatchItem->AttribRange.FaceCount;
				}
				MotionShader->EndPass();
			}		
		}
		MotionShader->End();
		//WeRenderDevice::Instance()->EndScene();

		WeRenderDevice::Instance()->RestoreBackBuffer(lpSurf);
		WeRenderDevice::Instance()->Device->SetDepthStencilSurface(lpOldDepthSurf);
	
		// clear z buffer so we can draw again
		WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);
		//WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
	}
}

