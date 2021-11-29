/*
    WeSkyBoxRenderer.cpp

	Autor: Nebojsa Vislavski
	Date: 2.3.2006.
	Version: 1.0
	Platform: Win32
*/


#include <WeSkyBoxRenderer.h>
#include <WeRenderDevice.h>
#include <WeSkyBox.h>
#include <WeCamera.h>
#include <WeMesh.h>
#include <WeMaterialManager.h>
#include <WeEngine.h>
#include <WeTimer.h>

WeSkyBoxRenderer *WeSkyBoxRenderer::_instance = NULL;

WeSkyBoxRenderer *WeSkyBoxRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeSkyBoxRenderer;
	return _instance;
}


WeSkyBoxRenderer::WeSkyBoxRenderer()
{
	bInitialized = false;
	WeRenderDevice::Instance()->Renderers.PushBack(this);
}


bool WeSkyBoxRenderer::Initialize()
{
	if (bInitialized) return true;


	bInitialized = false;
	return true;
}


void WeSkyBoxRenderer::ShutDown()
{
	if (!bInitialized) return;
}

void WeSkyBoxRenderer::OnLostDevice()
{

}

void WeSkyBoxRenderer::OnResetDevice()
{

}


void WeSkyBoxRenderer::Render(WeSkyBox *skyBox, WeCamera *Cam, WeShader *UseEffect)
{
	if (!skyBox) return;
	if (!skyBox->SkyBoxMesh) return;

	WeMatrix ViewProjMat, ViewMat, IdentityMat;
	WeVector EyePosVec;
	D3DXMatrixIdentity(&IdentityMat);

	WeShader *CurrentShader;
	CurrentShader = NULL;

	//WeRenderDevice::Instance()->BeginScene();
	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ZENABLE, false);
	

	ViewMat = Cam->View;
	ViewProjMat = Cam->ViewProj;
	EyePosVec = Cam->Location;

	//---material loading---// // prebaceno u WeSkyBox.Initialize
    CSmartList<WeMaterial*>* Materials;
	Materials = &skyBox->SkyBoxMesh->Materials[0];
	//if (Materials->Empty())
	//{
	//	int Size;
	//	WeMaterial *tmpMat;

	//	if (!Materials->Empty()) Materials->Clear();
	//	Size = skyBox->SkyBoxMesh->MeshAttributes.GetItemsCount();

	//	skyBox->SkyBoxMesh->MaterialNames[0].First();
	//	for (int i = 0; i < Size; i++)
	//	{
	//		tmpMat = WeMaterialManager::Instance()->Load(skyBox->SkyBoxMesh->MaterialNames[0].GetItem());
	//		if (!tmpMat)
	//			tmpMat = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);
	//		Materials->PushBack(tmpMat);
	//		skyBox->SkyBoxMesh->MaterialNames[0].Next();
	//	}
	//}
	//-----------------------//


	for(Materials->First(), skyBox->SkyBoxMesh->MeshAttributes.First(); !skyBox->SkyBoxMesh->MeshAttributes.End(); Materials->Next(), skyBox->SkyBoxMesh->MeshAttributes.Next())
	{
		if (UseEffect) 
		{
			CurrentShader = UseEffect;
		}
		else
		{
			if (CurrentShader != Materials->GetItem()->Shader)
			{
				if (CurrentShader) CurrentShader->End();
				CurrentShader = Materials->GetItem()->Shader;
				// set technique;
				if (!CurrentShader->SetAmbientTechnique()) 
				{
					continue;		
				}
				CurrentShader->Begin();
			}
		}


		for (int i = 0; i < (int)CurrentShader->GetPassesCount(); i++)
		{
			CurrentShader->BeginPass(i);

			WeMatrix ViewMatrix = ViewMat;
			ViewMatrix._41 = 0.0f; ViewMatrix._42 = 0.0f; ViewMatrix._43 = 0.0f;
			WeShaderManager::Instance()->SetViewMatrix(ViewMatrix);
			WeShaderManager::Instance()->SetWorldMatrix(IdentityMat);
			WeShaderManager::Instance()->SetEyePosition(WeVector(0.0f, 0.0f, 0.0f));

			Materials->GetItem()->SetTextures(WeTimer::Instance()->GetElapsedTimeF());
			CurrentShader->SetParams(Materials->GetItem());			
			CurrentShader->SetTextureAnimation(Materials->GetItem()->TextureAnimationM);
			WeShaderManager::Instance()->SetMaterialColor(Materials->GetItem()->MaterialColor);

			CurrentShader->CommitChanges();
			skyBox->SkyBoxMesh->DrawSubset(&skyBox->SkyBoxMesh->MeshAttributes.GetItem());
			CurrentShader->EndPass();
		}
	}

	if (CurrentShader && !UseEffect) CurrentShader->End();

	WeShaderManager::Instance()->SetViewMatrix(ViewMat);
	WeShaderManager::Instance()->SetViewProjection(ViewProjMat);
	WeShaderManager::Instance()->SetEyePosition(EyePosVec);

	WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ZENABLE, true);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//WeRenderDevice::Instance()->EndScene();
}
