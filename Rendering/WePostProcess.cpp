/*
    WePostProcess.cpp

	Autor: Nebojsa Vislavski
	Date: 2.11.2005.
	Version: 1.0

	This is post process system. It is almost automaticly called by engine.
	You only have to add post process effects.

*/

#include <WePostProcess.h>
#include <WeRenderTargetManager.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeTimer.h>
#include <WeMaterialManager.h>
#include <WeMaterial.h>
#include <WeEngine.h>

WePostProcess *WePostProcess::_instance = NULL;

WePostProcess *WePostProcess::Instance()
{
	if (!_instance) _instance = new WePostProcess;
	return _instance;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WePostProcess::Initialize()
{
	if (Initialized) return true;

	Initialized = false;

	if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(4*sizeof(WePostProcess::SQuadVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_QUADVERTEX, D3DPOOL_DEFAULT, &lpQuadBuffer, NULL)))
	{
		return false;
	}


	ResetQuadArray(1.0f);
	RefractiveMaterial = NULL;

	
	// create normal color render target
	ColorRT = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8, WeRenderDevice::Instance()->GetMultiSampleType());
	GlowRT = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8);
	RefractiveRT = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8, WeRenderDevice::Instance()->GetMultiSampleType());

	// make velocity and old velocity texture
	if (FAILED(WeRenderDevice::Instance()->d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_G16R16F)))
	{
		VelocityRT = NULL;
		OldVelocityRT = NULL;
	}
	else
	{
		VelocityRT = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_G16R16F);
		OldVelocityRT = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_G16R16F);
	}


	OldScreenRT[0] = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8);
	OldScreenRT[1] = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8);

	// Create color flip chain
	ColorFlipChain[0][0] = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8);
	ColorFlipChain[0][1] = WeRenderTargetManager::Instance()->CreateRT(WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DFMT_A8R8G8B8);


	if (!ColorFlipChain[0][0]) return false;
	if (!ColorFlipChain[0][1]) return false;


	Initialized = true;
	Capturing = false;
	return true;
}



void WePostProcess::ResetQuadArray(float z)
{
	float W,H;
	LPDIRECT3DSURFACE9 lpTmp;
	D3DSURFACE_DESC desc;

    // TODO: Check To see if this can be rewritten somehow
	WeRenderDevice::Instance()->Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &lpTmp);
	lpTmp->GetDesc(&desc);
	W = (float)desc.Width;
	H = (float)desc.Height;
	lpTmp->Release();

	float d = 0.5f;

	QuadVertices[0].x = -d;
	QuadVertices[0].y = -d;
	QuadVertices[0].z = z;
	QuadVertices[0].rhw = 1.0f;



	QuadVertices[0].u = 0.0f;  QuadVertices[0].v = 0.0f;
	QuadVertices[0].u2 = 0.0f;  QuadVertices[0].v2 = 0.0f;


	QuadVertices[1].x = W-d;
	QuadVertices[1].y = -d;
	QuadVertices[1].z = z;
	QuadVertices[1].rhw = 1.0f;
	

	QuadVertices[1].u = 1.0f;  QuadVertices[1].v = 0.0f;
	QuadVertices[1].u2 = 1.0f;  QuadVertices[1].v2 = 0.0f;

	QuadVertices[2].x = -d;
	QuadVertices[2].y = H-d;
	QuadVertices[2].z = z;
	QuadVertices[2].rhw = 1.0f;


	QuadVertices[2].u = 0.0f;  QuadVertices[2].v = 1.0f;
	QuadVertices[2].u2 = 0.0f;  QuadVertices[2].v2 = 1.0f;

	QuadVertices[3].x = W-d;
	QuadVertices[3].y = H-d;
	QuadVertices[3].z = z;
	QuadVertices[3].rhw = 1.0f;


	QuadVertices[3].u = 1.0f;  QuadVertices[3].v = 1.0f;
	QuadVertices[3].u2 = 1.0f;  QuadVertices[3].v2 = 1.0f;

    void *data;
	lpQuadBuffer->Lock(0, 0, &data, D3DLOCK_DISCARD);
	CopyMemory(data, QuadVertices, sizeof(QuadVertices));
	lpQuadBuffer->Unlock();

	ExtentX = 1.0f;
	ExtentY = 1.0f;

}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::BeginScreenCapture()
{
	if (!Initialized) return;
	if (!Turned()) return;
	
	
	BackSurface = WeRenderDevice::Instance()->SaveBackBuffer();
	//// make info to render to ColorRT
	//if (WeRenderDevice::Instance()->SupportsMultipleRT())	
	//{

	//	WeRenderDevice::Instance()->SetRenderTarget(0, ColorRT);
	//	if(WeRenderDevice::Instance()->GetClearScreen())		
	//		WeRenderDevice::Instance()->Device->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, WeRenderDevice::Instance()->GetClearColor(), 1, 0);
	//	WeRenderDevice::Instance()->SetRenderTarget(1, VelocityRT);				
	//		WeRenderDevice::Instance()->Device->Clear(0,NULL,D3DCLEAR_TARGET, 0, 1, 0);

	//} else
	//{
	//if (WeRenderDevice::Instance()->GetAntialiasingFactor() == 0)
	WeRenderDevice::Instance()->SetRenderTarget(0, ColorRT);
	//WeRenderDevice::Instance()->Device->SetRenderTarget(0, ColorRT->GetSurface(0));

	if(WeRenderDevice::Instance()->GetClearScreen())		
		WeRenderDevice::Instance()->Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, WeRenderDevice::Instance()->GetClearColor(), 1, 0);
	//}

	WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);	
	Capturing = true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::EndScreenCapture()
{
	if (!Initialized) return;
	if (!Turned()) return;

	// copy main surface into ColorRT;	
	//if (WeRenderDevice::Instance()->GetAntialiasingFactor() == 0)
		WeRenderDevice::Instance()->RestoreBackBuffer(BackSurface);
	Capturing = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::ShutDown()
{
	if (!Initialized) return;

	SAFE_RELEASE (lpQuadBuffer);
	WeRenderTargetManager::Instance()->Free(ColorRT);
	WeRenderTargetManager::Instance()->Free(OldScreenRT[0]);
	WeRenderTargetManager::Instance()->Free(OldScreenRT[1]);

	WeRenderTargetManager::Instance()->Free(ColorFlipChain[0][0]);
	WeRenderTargetManager::Instance()->Free(ColorFlipChain[0][1]);

	WeRenderTargetManager::Instance()->Free(VelocityRT);
	WeRenderTargetManager::Instance()->Free(OldVelocityRT);

	WeRenderTargetManager::Instance()->Free(GlowRT);
	WeRenderTargetManager::Instance()->Free(RefractiveRT);


	Initialized = false;
	Capturing = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WePostProcessEffect *WePostProcess::AddEffect(WePostProcessEffect *Effect)
{
	if (!Effect) return NULL;
	Effects.PushBack(Effect);	
	return Effect;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WePostProcessEffect *WePostProcess::AddEffect(StringParam ShaderName, float ScaleXBy, float ScaleYBy, bool FromPackage)
{
	WePostProcessEffect *newEffect = new WePostProcessEffect;
	newEffect->Initialize(ShaderName, ScaleXBy, ScaleYBy, FromPackage);	
	Effects.PushBack(newEffect);
	return newEffect;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::RemoveEffect(WePostProcessEffect *Effect)
{
	if (!Effect) return;

	for (Effects.First(); !Effects.End(); Effects.Next())
	{
		if(Effects.GetItem() == Effect)
		{	
			Effects.Delete();
		}
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
void WePostProcess::ClearEffects()
{
	Effects.Clear();
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::DoRefraction()
{	
	if (!WeRenderDevice::Instance()->GetRefractive()) return;

	if (WeRenderDevice::Instance()->GetAntialiasFactor() > 0)
	{
		//MessageBox(0, "23234", 0, 0);
		D3DXLoadSurfaceFromSurface(RefractiveRT->GetTextureSurface(0), NULL, NULL, RefractiveRT->GetSurface(0), NULL, NULL, D3DX_DEFAULT, 0);	
	}


	int effcount = 1;

	RefractiveMaterial = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DistortionMaterialName);

	if (!RefractiveMaterial) return;
	
	WeRenderDevice::Instance()->SetRenderTarget(0, ColorFlipChain[0][1]);
	WeRenderDevice::Instance()->Device->SetTexture(2, RefractiveRT->GetTexture());
	WeRenderDevice::Instance()->SetTexture(3, RefractiveMaterial->Textures[0]);
	WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);	
	WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET,0x00000000, 1.0f, 0L );

	RefractiveMaterial->Shader->SetParams(RefractiveMaterial);	
	RefractiveMaterial->Shader->SetTextureAnimation(RefractiveMaterial->TextureAnimationM);
	
	WeShaderManager::Instance()->SetDeltaTime(WeTimer::Instance()->GetDeltaTimeF());
	WeShaderManager::Instance()->SetElapsedTime(WeTimer::Instance()->GetElapsedTimeF());

	RefractiveMaterial->Shader->Begin();
	RefractiveMaterial->Shader->BeginPass(0);
	DrawQuad(1.0f, 1.0f);
	RefractiveMaterial->Shader->EndPass();
	RefractiveMaterial->Shader->End();

	// now switch flip chain
	WeRenderTarget *tmp;
	tmp = ColorFlipChain[0][0];
	ColorFlipChain[0][0] = ColorFlipChain[0][1];
	ColorFlipChain[0][1] = tmp;

	// Set texture
	WeRenderDevice::Instance()->Device->SetTexture(0, ColorFlipChain[0][0]->GetTexture());
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::DoGlowing()
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
void WePostProcess::PostRender()
{
	if (!Initialized) return;
	if (!Turned()) return;

	bool OldWireFrame;
	OldWireFrame = WeRenderDevice::Instance()->GetWireFrame();
	WeRenderDevice::Instance()->SetWireFrame(false);

	int index = 0;

	// Reset Buffer z to size 1.0f
	ResetQuadArray(1.0f);
	BackSurface = WeRenderDevice::Instance()->SaveBackBuffer();	
	
	if (WeRenderDevice::Instance()->GetAntialiasFactor() > 0)
	{
		//MessageBox(0, "23234", 0, 0);
		D3DXLoadSurfaceFromSurface(ColorRT->GetTextureSurface(0), NULL, NULL, ColorRT->GetSurface(0), NULL, NULL, D3DX_DEFAULT, 0);	
	}
	


	WeRenderDevice::Instance()->Device->SetTexture(0, ColorRT->GetTexture());
	WeRenderDevice::Instance()->Device->SetTexture(1, ColorRT->GetTexture());	

	if (WeRenderDevice::Instance()->GetGlow())
	{
		WeRenderDevice::Instance()->Device->SetTexture(4, GlowRT->GetTexture());	
	}

	if (WeRenderDevice::Instance()->GetPixelMotionBlur())
	{
		if (VelocityRT)	WeRenderDevice::Instance()->Device->SetTexture(2, VelocityRT->GetTexture());
		if (OldVelocityRT) WeRenderDevice::Instance()->Device->SetTexture(3, OldVelocityRT->GetTexture());
	}


	if (WeRenderDevice::Instance()->GetAddedPostProcessEffects() && WeRenderDevice::Instance()->SupportsShader20())
	{
		int effcount = 1;
		for (Effects.First(); !Effects.End(); Effects.Next())
		{
			if (!Effects.GetItem()->Active)
			{				
				continue;
			}
			
			WeRenderDevice::Instance()->SetRenderTarget(0, ColorFlipChain[0][1]);
			WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);	
			WeRenderDevice::Instance()->Device->Clear( 0L, NULL, D3DCLEAR_TARGET,0x00000000, 1.0f, 0L );

			Effects.GetItem()->Shader->SetParams();
			Effects.GetItem()->Shader->SetParam1(Effects.GetItem()->Param1);
			Effects.GetItem()->Shader->SetParam2(Effects.GetItem()->Param2);
			Effects.GetItem()->Shader->SetParam3(Effects.GetItem()->Param3);
			Effects.GetItem()->Shader->SetParam4(Effects.GetItem()->Param4);			

			WeShaderManager::Instance()->SetDeltaTime(WeTimer::Instance()->GetDeltaTimeF());
			WeShaderManager::Instance()->SetElapsedTime(WeTimer::Instance()->GetElapsedTimeF());

			Effects.GetItem()->Shader->Begin();
			Effects.GetItem()->Shader->BeginPass(0);
			DrawQuad(Effects.GetItem()->ScaleXBy, Effects.GetItem()->ScaleYBy);
			Effects.GetItem()->Shader->EndPass();
			Effects.GetItem()->Shader->End();

			// now switch flip chain
			WeRenderTarget *tmp;
			tmp = ColorFlipChain[0][0];
			ColorFlipChain[0][0] = ColorFlipChain[0][1];
			ColorFlipChain[0][1] = tmp;

			// Set texture
			WeRenderDevice::Instance()->Device->SetTexture(0, ColorFlipChain[0][0]->GetTexture());
		
		}
	}

	DoRefraction();
	DoGlowing();

	// switch velocity textures
	WeRenderTarget *tmpRT;
	tmpRT = VelocityRT;
	VelocityRT = OldVelocityRT;
	OldVelocityRT = tmpRT;
	// TODO: switch normal textures


	if (WeRenderDevice::Instance()->GetMotionBlur())
	{
		// Now do mixing with new texture in sampler0 and oldScreen and set restult to RT
		OldScreenRT[1]->BeginScene();				
		//WeRenderDevice::Instance()->Device->SetRenderTarget(0, OldScreenRT[1]->GetTextureSurface(0));
		WeRenderDevice::Instance()->Device->SetTexture(1, OldScreenRT[0]->GetTexture());

		D3DXCOLOR Blur;
		Blur.a = WeRenderDevice::Instance()->GetBlurFactor();
		WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_TEXTUREFACTOR, Blur);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
		WeRenderDevice::Instance()->Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
		WeRenderDevice::Instance()->Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		WeRenderDevice::Instance()->Device->SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_CURRENT);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		ResetQuadArray(0.0);
		DrawQuad(1.0f, 1.0f);

		WeRenderTarget *tmp;
		tmp = OldScreenRT[0];
		OldScreenRT[0] = OldScreenRT[1];
		OldScreenRT[1] = tmp;
		
		WeRenderDevice::Instance()->Device->SetTexture(0, OldScreenRT[0]->GetTexture());
		//    
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		WeRenderDevice::Instance()->Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_TEXTURE);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		
		// Here we should draw this new texture with some alpha
		// Just draw;
	}

	WeRenderDevice::Instance()->RestoreBackBuffer(BackSurface);
	WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);	

	ResetQuadArray(0.0);
	DrawQuad(1.0f, 1.0f);

	

	WeRenderDevice::Instance()->SetWireFrame(OldWireFrame);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::DrawFullScreenQuad()
{
	if (!Initialized) return;

    void *data;

	QuadVertices[2].x = 1.0f;
	QuadVertices[3].x = 1.0f;

	QuadVertices[0].y = -1.0f;
	QuadVertices[3].y = -1.0f;

	ExtentX = 1.0f;
	ExtentY = 1.0f;

	lpQuadBuffer->Lock(0,0, &data, D3DLOCK_DISCARD);
	CopyMemory(data, QuadVertices, sizeof(QuadVertices));
	lpQuadBuffer->Unlock();



	WeMatrix View;
	WeMatrix Projection, World;

	D3DXMatrixIdentity(&World);
	D3DXMatrixIdentity(&View);
	D3DXMatrixIdentity(&Projection);

//	View = WeRenderDevice::Instance()->BuildViewMatrix(&WeVector(0,0,1), &WeVector(0,0,1), &WeVector(0,1,0));
//	Projection = WeRenderDevice::Instance()->BuildProjMatrix(D3DX_PI/2.0f, 1.0f, 10.0f);

	WeRenderDevice::Instance()->Device->SetTransform(D3DTS_PROJECTION, &Projection);
	WeRenderDevice::Instance()->Device->SetTransform(D3DTS_WORLD, &World);
	WeRenderDevice::Instance()->Device->SetTransform(D3DTS_VIEW, &View);


	//WeRenderDevice::Instance()->BeginScene();
	WeRenderDevice::Instance()->Device->SetStreamSource(0, lpQuadBuffer, 0, sizeof(WePostProcess::SQuadVertex));
	WeRenderDevice::Instance()->Device->SetFVF(D3DFVF_QUADVERTEX);

	WeRenderDevice::Instance()->Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);		

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
void WePostProcess::OnLostDevice()
{
	if (!Initialized) return;
	SAFE_RELEASE(lpQuadBuffer);

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WePostProcess::OnResetDevice()
{
	if (!Initialized) return;
	if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(4*sizeof(WePostProcess::SQuadVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_QUADVERTEX, D3DPOOL_DEFAULT, &lpQuadBuffer, NULL)))
	{
		return;
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
void WePostProcess::ClearOtherMRT()
{
	if (Capturing)
	{
		WeRenderDevice::Instance()->Device->SetRenderTarget(1, NULL);
		WeRenderDevice::Instance()->SetScissorRect(&WeRenderDevice::Instance()->ScissorRect, WeRenderDevice::Instance()->ScissorEnable);	
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
void WePostProcess::DrawQuad(float fScaleX, float fScaleY)
{
	if (!Initialized) return;
    
    void *data;
	bool UpdateQB = false;

	if (ExtentX != QuadVertices[1].u)
	{
		QuadVertices[1].u = ExtentX;
		QuadVertices[3].u = ExtentX;
		UpdateQB = true;
	}

	if (ExtentY != QuadVertices[2].v)
	{
		QuadVertices[2].v = ExtentY;
		QuadVertices[3].v = ExtentY;
		UpdateQB = true;
	}

	if (fScaleX != 1.0f)
	{
		//QuadVertices[1].x = QuadVertices[0].x + (QuadVertices[1].x-QuadVertices[0].x)*fScaleX;
		//QuadVertices[3].x = QuadVertices[0].x + (QuadVertices[3].x-QuadVertices[0].x)*fScaleX;
		QuadVertices[1].x = (QuadVertices[1].x+0.5f)*fScaleX-0.5f;
		QuadVertices[3].x = (QuadVertices[3].x+0.5f)*fScaleX-0.5f;
		UpdateQB = true;
	}

	if (fScaleY != 1.0f)
	{
		QuadVertices[2].y = (QuadVertices[2].y+0.5f)*fScaleY-0.5f;
		QuadVertices[3].y = (QuadVertices[3].y+0.5f)*fScaleY-0.5f;
		//QuadVertices[2].y = QuadVertices[0].y + (QuadVertices[2].y-QuadVertices[0].y)*fScaleY;
		//QuadVertices[3].y = QuadVertices[0].y + (QuadVertices[3].y-QuadVertices[0].y)*fScaleY;
		UpdateQB = true;
	}


	if (UpdateQB)
	{
		lpQuadBuffer->Lock(0,0, &data, D3DLOCK_DISCARD);
		CopyMemory(data, QuadVertices, sizeof(QuadVertices));
		lpQuadBuffer->Unlock();
	}



	ExtentX *= fScaleX;
	ExtentY *= fScaleY;


	//WeRenderDevice::Instance()->BeginScene();
	WeRenderDevice::Instance()->Device->SetStreamSource(0, lpQuadBuffer, 0, sizeof(WePostProcess::SQuadVertex));
	WeRenderDevice::Instance()->Device->SetFVF(D3DFVF_QUADVERTEX);
	WeRenderDevice::Instance()->Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		
	//WeRenderDevice::Instance()->EndScene();	

}



void WePostProcess::TurnOn()
{
	WeRenderDevice::Instance()->SetPostProcessEffects(true);
}



void WePostProcess::TurnOff()
{
	WeRenderDevice::Instance()->SetPostProcessEffects(false);
}



bool WePostProcess::Turned()
{
	return WeRenderDevice::Instance()->GetPostProcessEffects();
}



void WePostProcess::UpdateEffects()
{
	ClearEffects();

	if (WeRenderDevice::Instance()->GetGlow())
	{
		AddEffect("Effects.PostProcess.ColorCombineGlow", 1, 1, true);		
		AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);	
		AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);		
		//AddEffect("Effects.PostProcess.ColorBrightPass", 1, 1, true);
		//AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true); 

		AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);		
		AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true); 
		//AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);		
		//AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true);		

		AddEffect("Effects.PostProcess.ColorBright", 1, 1, true);
		AddEffect("Effects.PostProcess.ColorUpFilter4", 4, 4, true);
		AddEffect("Effects.PostProcess.ColorCombine4", 4, 4, true);	
	} 
	else
	{
		AddEffect("Effects.PostProcess.ColorBright", 1, 1, true);
	}


	if (WeRenderDevice::Instance()->GetPixelMotionBlur())
	{
		AddEffect("Effects.PostProcess.MotionBlur", 1, 1, true);
	}
}
