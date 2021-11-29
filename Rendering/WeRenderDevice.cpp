/*
       File: WeRenderDevice.cpp
	   Author: Nebojsa Vislavski
	   Date: 13.09.2005.
	   Version: 1.0
*/


#include <d3dx9.h>
#include <WeCanvas.h>
#include <WeInput.h>
#include <WeShaderManager.h>
#include <WeTextureManager.h>
#include <WeCamera.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WeMaterial.h>
#include <WeMesh.h>
#include <WeTimer.h>
#include <WeRenderDevice.h>
#include <WeQueryManager.h>
#include <WeRenderTarget.h>
#include <WeCanvas.h>
#include <WePostProcess.h>
#include <WeRenderTargetManager.h>
#include <WeTexture.h>
#include <WeBatchRenderer.h>
#include <WeParticleRenderer.h>
#include <WeMeshManager.h>
#include <WeTrailRenderer.h>
#include <WeFlareRenderer.h>
#include <WeEngine.h>
#include <WeFont.h>
#include <WeLoadCounter.h>
#include <WeProfiler.h>



WeRenderDevice* WeRenderDevice::_instance = NULL;


WeRenderDevice* WeRenderDevice::Instance()
{
	if(_instance != NULL) return _instance;
		else{
		_instance = new WeRenderDevice();
		return _instance;
	}
}


WeRenderDevice::WeRenderDevice()
{
	bFullScreen  =	true;
	bDeviceLost  =	false;
	bPaused      =	false;
	bWireFrame   =	false;
	bClearScreen =	true;
	bInitialized =	false;
	bLight		 =  true;
	bDoRendering =	true;
	UseShadowVolumes = true;
	BlurFactor	 =	0.0f; 
	BloomScale	 =	1.5f;
	Luminance	 =  0.08f;
	Brightness	 =  0.0f;
	BrightnessColor.r = 1.0f;
	BrightnessColor.g = 1.0f;
	BrightnessColor.b = 1.0f;
	BrightnessColor.a = 1.0f;
	//AmbientColor = WeColor(0.1f, 0.1f, 0.1f, 1.0f);
	AmbientColor = WeColor(0.0f, 0.0f, 0.0f, 1.0f);
	SkyColor = WeColor(0.1f, 0.1f, 0.1f, 1.0f);
	GroundColor = WeColor(0.1f, 0.1f, 0.1f, 1.0f);		
	FogEnabled = false;
	FogStart = 0.0f;
	FogEnd = 0.0f;
	FogDensity = 0.0f;
	ScissorEnable = false;
	bPixelMotionBlur = true;
	PixelMotionBlurFactor = 1.0f;
	bProjectors = true;
	bFlares = true;
	bShadows = true;
	bBumpMapping = false;
	bSpecular = false;
	bAnisotropic = false;
	bGlow = true;
	bRefractive = true;
	bMotionBlur = true;
	AnisotropicFactor = 1;
	AntialiasingFactor = 0;
	MultiSampleType = D3DMULTISAMPLE_NONE;
	SurfaceFormat = D3DFMT_A8R8G8B8;

	lpNotMultisampledDepthSurf = NULL;

	bPostProcessEffects = true;
	bAddedPostProcessEffects = true;
	bUsingShaders	= true;
	bSupportShader20 = false;
	bSupportShader2a = false;
	bSupportShader30 = false;
	bUseMaxRenderDistance = true;

	TextureManager =	NULL;
	ShaderManager =		NULL;
	MaterialManager =	NULL;

	RenderCallback =	NULL;
	BackBufferScale = true;

	if (bFullScreen)
	{
		ResWidth  = 1024;
		ResHeight = 768;
	}
	else
	{
		ResWidth = 1024;
		ResHeight = 768;
	}

	Aspect = (float)ResWidth / (float)ResHeight;
	LostDeviceCallback = NULL;
	ResetDeviceCallback = NULL;
}



WeRenderDevice::~WeRenderDevice(void)
{
	ShutDown();
}



void WeRenderDevice::CheckMultiSampleType()
{
	bool ok = false;
	MultiSampleType = (D3DMULTISAMPLE_TYPE)AntialiasingFactor;
	while (!ok)
	{		
		if (FAILED(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, SurfaceFormat, !bFullScreen, MultiSampleType, NULL)))
		{
			MultiSampleType = (D3DMULTISAMPLE_TYPE)(MultiSampleType - 1);
		} else ok = true;
	}
	AntialiasingFactor = MultiSampleType;
}




void WeRenderDevice::MakePresentParams()
{	
	AntialiasingFactor = WeEngine::Instance()->Desc.AntialiasFactor;
	ZeroMemory(&PresentParams, sizeof(PresentParams)); 

	PresentParams.hDeviceWindow = hWnd;

	if (WeEngine::Instance()->Desc.ImmediatePresent)
		PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	else
		PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


	PresentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	PresentParams.EnableAutoDepthStencil = true;

	PresentParams.BackBufferCount = 1;
	PresentParams.BackBufferWidth = ResWidth;
	PresentParams.BackBufferHeight = ResHeight;
	PresentParams.BackBufferFormat = SurfaceFormat;

	CheckMultiSampleType();
	PresentParams.MultiSampleType = MultiSampleType;
	PresentParams.MultiSampleQuality = 0;

	if(!BackBufferScale)
	{
		SourceRect.right = ResWidth;
		SourceRect.bottom = ResHeight;
		DestRect.right = ResWidth;
		DestRect.bottom = ResHeight;
	}

	if(bFullScreen)
	{
		PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		PresentParams.Windowed = false;
	}else{
		PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		PresentParams.Windowed = true;
	}
	Aspect = (float)ResWidth / (float)ResHeight;
}



bool WeRenderDevice::Initialize(HINSTANCE hInst, HWND _hWnd, String CmdLine, int reswidth, int resheight, bool FullScreen, StringParam DefaultShaderName)
{ 
	bFullScreen = FullScreen;

	if (bInitialized) return true;

	hWnd = _hWnd;
	ResWidth = reswidth;
	ResHeight = resheight;


	// Create d3d object
	// ------------------------------------------------------
	if(NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))  
	{
		MessageBox(0, "Error Direct3DCreate9", 0, 0);
		return false;
	}

	MakePresentParams();
	// Create device
	// ------------------------------------------------------
   // ONLY FOR NVIDIA PERFHUD INIT 
	// TODO: Remove this when publishing 
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
	UINT AdapterToUse = D3DADAPTER_DEFAULT;

	for (UINT Adapter=0;Adapter<d3d->GetAdapterCount();Adapter++) 
	{ 
		D3DADAPTER_IDENTIFIER9 Identifier; 
		HRESULT Res;
		Res = d3d->GetAdapterIdentifier(Adapter,0,&Identifier); 
		if (strcmp(Identifier.Description,"NVIDIA NVPerfHUD") == 0) 
		{ 
			AdapterToUse=Adapter; DeviceType=D3DDEVTYPE_REF; break; 
		} 
	}

	HRESULT Res;
	Res = d3d->CreateDevice(AdapterToUse, DeviceType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParams, &Device);
	if (Res != D3D_OK)
	{
		// just try software vertex processing.
		Res = d3d->CreateDevice(AdapterToUse, DeviceType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PresentParams, &Device);
		if (Res != D3D_OK)
		{
			// our last chance is to make REF
			DeviceType = D3DDEVTYPE_REF;
			Res = d3d->CreateDevice(AdapterToUse, DeviceType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PresentParams, &Device);
			if (Res != D3D_OK) return false;
		}
	}


	D3DCAPS9 Caps;

	d3d->GetDeviceCaps(AdapterToUse, DeviceType, &Caps);


	bSupportShader30 = false;
	bSupportShader2a = false;
	bSupportShader20 = false;

	if (LOWORD(Caps.PixelShaderVersion) >= 0x0300)
	{
		bSupportShader30 = true;
	} 
	if (LOWORD(Caps.PixelShaderVersion) > 0x0200) 
	{
		bSupportShader2a = true;
	}
	if (LOWORD(Caps.PixelShaderVersion) >= 0x0200)
	{
		bSupportShader20 = true;
	}


	if (Caps.NumSimultaneousRTs > 1)
		bSupportMultipleRT = true;
	else 
		bSupportMultipleRT = false;	

	// if it is not possible to have mrt pixelshader blending then 
	if (Caps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING)
		bSupportMRTBlending = true;
	else 
		bSupportMRTBlending = false;


	SetClearColor(D3DXCOLOR(0,0,0,255));
	
	// Set Rendering State to all turned on
	RenderingState = 0xffffffff;


	WeParticleRenderer::Instance()->Initialize();
	WeTrailRenderer::Instance()->Initialize();
	WeFlareRenderer::Instance()->Initialize();


	
	// Crating Texture manager & Material manager
	TextureManager = WeTextureManager::Instance();	
	ShaderManager = WeShaderManager::Instance();
	ShaderManager->Initialize(DefaultShaderName);
	WeQueryManager::Instance()->Initialize();
		
	if(bFullScreen) ShowCursor(false);

	bInitialized = true;

	SetRect(&SourceRect,0,0,ResWidth, ResHeight);
	SetRect(&DestRect,0,0,ResWidth, ResHeight);

	return true;
}


LRESULT WeRenderDevice::MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT tmp = 0;
	return tmp;
}


void WeRenderDevice::ShutDown()
{
	if (!bInitialized) return;

	//WeParticleRenderer::Instance()->ShutDown();
	WeCanvas::Instance()->ShutDown();
	WeQueryManager::Instance()->ShutDown();

	SAFE_RELEASE(lpNotMultisampledDepthSurf);
	SAFE_RELEASE(d3d);
	SAFE_RELEASE(Device);


	bInitialized = false;
}


void WeRenderDevice::Update(WeCamera* ActiveCamera)
{
	if (!bInitialized) return;
	if (!ActiveCamera) return;

	Device->SetTransform(D3DTS_PROJECTION, &ActiveCamera->Projection);
	Device->SetTransform(D3DTS_VIEW, &ActiveCamera->View);

	// TODO: Move these lines to somewhere else instead of setting them every frame
	WeShaderManager::Instance()->SetResHeight((float)ResHeight);
	WeShaderManager::Instance()->SetResWidth((float)ResWidth);
	// ----
	WeShaderManager::Instance()->SetDeltaTime(WeTimer::Instance()->GetDeltaTimeF());
	WeShaderManager::Instance()->SetElapsedTime(WeTimer::Instance()->GetElapsedTimeF());
}


void WeRenderDevice::LoadConfig()
{
}


void WeRenderDevice::SaveConfig()
{
}


void WeRenderDevice::DoRender()
{
	if (!bInitialized) return;
	if (!bDoRendering) return;

	HWND hWindow = GetForegroundWindow();
	if (GetWindowThreadProcessId(hWindow, NULL) != GetCurrentThreadId())
		return;

	HRESULT DeviceState = Device->TestCooperativeLevel();
	D3DPRESENT_PARAMETERS d3dpp;
	if(DeviceState == D3DERR_DEVICENOTRESET)
	{
		d3dpp = PresentParams;
		if(D3D_OK != Device->Reset(&d3dpp))
		{		
			bDeviceLost = true;
			return;
		} else
		{
			OnResetDevice();
			bDeviceLost = false;
			return;
		}
	}
	if(bDeviceLost) return;
	if(DeviceState == D3DERR_DEVICELOST && !bDeviceLost)
	{
		OnLostDevice();
		bDeviceLost = true;
		return;
	}


	//SetRenderState( D3DRS_LIGHTING, FALSE);
	if (AntialiasingFactor > 0)
	{
		Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	} else
	{
		Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	}

	if(GetClearScreen())
		Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, ClearColor, 1, 0);
	else
		Device->Clear(0,NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, ClearColor, 1, 0);

	Device->BeginScene();


	if (RenderCallback)
	{
		RenderCallback();
	}

	static WeFont* Font = WeCanvas::Instance()->MakeFont("Arial", 16);
	if (GetLoadingName().GetLen() != 0)
	{
		WeCanvas::Instance()->Draw2DText(Font, GetLoadingName(), 0, WeRenderDevice::Instance()->GetResHeight() - 20, 300, 20, 0xFFFFFFFF);
		ResetLoadingName();
	}

	Device->EndScene();
    

	if(bFullScreen)
		Device->Present(0,0,hWnd,0);
	else
	{
		if(!BackBufferScale)
			Device->Present(&SourceRect,&DestRect,hWnd,0);
		else
			Device->Present(0,0,hWnd,0);
	}
}



void WeRenderDevice::SaveThumbnail(String PathAndFileName)
{
	IDirect3DSurface9 *SurfScreenCap = NULL;

	Device->GetRenderTarget(0, &SurfScreenCap);
	D3DXSaveSurfaceToFile(PathAndFileName, D3DXIFF_BMP, SurfScreenCap, NULL, NULL);

	SAFE_RELEASE(SurfScreenCap);
}


void WeRenderDevice::SetWireFrame(bool wireframe)
{
	if(wireframe)
	{
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}else{
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	bWireFrame = wireframe;
}



WeCanvas* WeRenderDevice::GetCanvas()
{
	return WeCanvas::Instance();
}


bool WeRenderDevice::SetDisplayMode(int NewX, int NewY, int NewColorDepth, bool NewFullScreen, bool NewVSync, bool Scale)
{
	if (!bInitialized) return false;

	D3DPRESENT_PARAMETERS d3dpp;

	//BackBufferScale = Scale;
	ResWidth = NewX;
	ResHeight = NewY;
	bFullScreen = NewFullScreen;

	MakePresentParams();
	WeShaderManager::Instance()->SetResWidth((float)ResWidth);
	WeShaderManager::Instance()->SetResHeight((float)ResHeight);

	if (NewFullScreen)
		ShowCursor(false);
	else
		ShowCursor(true);


	OnLostDevice();	

	d3dpp = PresentParams;
	if(D3DERR_INVALIDCALL == Device->Reset(&d3dpp))
	{
		if(D3D_OK != Device->Reset(&d3dpp))
		{
			bDeviceLost = true;			
			return false;

		} else bDeviceLost = false;
	}else bDeviceLost = false;

	OnResetDevice();    	
	return true;
}


WeMatrix WeRenderDevice::BuildViewMatrix(WeVector* Pos, WeVector* LookAt, WeVector* Up)
{
	WeMatrix tmp;	
	D3DXMatrixLookAtLH(&tmp, Pos, LookAt, Up);	
	return tmp;
}


WeMatrix WeRenderDevice::BuildProjMatrix(float Fov, float NearClip, float FarClip, float _Aspect)
{
	WeMatrix tmp;
	if (_Aspect)
		D3DXMatrixPerspectiveFovLH(&tmp, Fov, _Aspect, NearClip, FarClip);
	else 
		D3DXMatrixPerspectiveFovLH(&tmp, Fov, Aspect, NearClip, FarClip);

	return tmp;
}


void WeRenderDevice::SetRenderState(D3DRENDERSTATETYPE RenderState, DWORD Value)
{
	Device->SetRenderState(RenderState, Value);
}


void WeRenderDevice::SetRenderTarget(int rtnum, WeRenderTarget *RT)
{
	if (!RT) 
		Device->SetRenderTarget(rtnum, NULL);	
	else
		Device->SetRenderTarget(rtnum, RT->lpSurf[0]);
}

LPDIRECT3DSURFACE9 WeRenderDevice::SaveBackBuffer()
{
	LPDIRECT3DSURFACE9 BackSurface;
	Device->GetRenderTarget(0, &BackSurface);	
	return BackSurface;

}

void WeRenderDevice::RestoreBackBuffer(LPDIRECT3DSURFACE9 BackSurface)
{
	Device->SetRenderTarget(0, BackSurface);
	SAFE_RELEASE( BackSurface );
}





void WeRenderDevice::OnLostDevice()
{
	bDeviceLost = true;

	if(LostDeviceCallback) LostDeviceCallback();

	WeCanvas::Instance()->OnLostDevice();

	WeQueryManager::Instance()->OnLostDevice();
	ShaderManager->OnLostDevice();
	TextureManager->OnLostDevice();
	WeMeshManager::Instance()->OnLostDevice();
	WeRenderTargetManager::Instance()->OnLostDevice();

	//WeParticleRenderer::Instance()->OnLostDevice();
	WePostProcess::Instance()->OnLostDevice();
	//WeTrailRenderer::Instance()->OnLostDevice();

	for (Renderers.First(); !Renderers.End(); Renderers.Next())
	{
		Renderers.GetItem()->OnLostDevice();
	}
}



void WeRenderDevice::OnResetDevice()
{
	if(ResetDeviceCallback) ResetDeviceCallback();

	WeCanvas::Instance()->OnResetDevice();

	WeQueryManager::Instance()->OnResetDevice();
	ShaderManager->OnResetDevice();
	TextureManager->OnResetDevice();
	WeRenderTargetManager::Instance()->OnResetDevice();
	WeMeshManager::Instance()->OnResetDevice();

	WePostProcess::Instance()->OnResetDevice();
	//WeParticleRenderer::Instance()->OnResetDevice();
	//WeTrailRenderer::Instance()->OnResetDevice();

	for (Renderers.First(); !Renderers.End(); Renderers.Next())
	{
		Renderers.GetItem()->OnResetDevice();
	}
}


void WeRenderDevice::SetTexture(int Sampler, WeTexture *Texture)
{
	if (Texture)
		Device->SetTexture(Sampler, Texture->GetTexture());
	else
		Device->SetTexture(Sampler, NULL);
}


void WeRenderDevice::CreateDeclaration(const D3DVERTEXELEMENT9 *pVertexElement, LPDIRECT3DVERTEXDECLARATION9 *lplpDecl)
{
	if (!bInitialized) return;
	Device->CreateVertexDeclaration(pVertexElement, lplpDecl);
}




void WeRenderDevice::SetFog(bool Enable, WeColor &Color, float Density)
{	
	FogEnabled = Enable;
	FogDensity = Density;
	FogColor = Color;

}

void WeRenderDevice::TurnFogOn()
{		
	Device->SetRenderState(D3DRS_FOGENABLE, FogEnabled);
	//Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
	Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	Device->SetRenderState(D3DRS_FOGSTART, * ((DWORD *) (&FogStart)));
	Device->SetRenderState(D3DRS_FOGEND, * ((DWORD *) (&FogEnd)));
	Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	Device->SetRenderState(D3DRS_FOGDENSITY, * ((DWORD *) (&FogDensity)));
	Device->SetRenderState(D3DRS_FOGCOLOR, FogColor);
}

void WeRenderDevice::TurnFogOff()
{
	Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
}


void WeRenderDevice::SetFogStart(float s)
{
	FogStart = s;
}

void WeRenderDevice::SetFogEnd(float e)
{
	FogEnd = e;
}


void WeRenderDevice::SetScissorRect(RECT *Rect, bool test)
{
	Device->SetScissorRect(Rect);
	ScissorRect = *Rect;
	ScissorEnable = test;
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, test);
}

void WeRenderDevice::SetScrissorTest(bool test)
{
	ScissorEnable = test;
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, test);
}


void WeRenderDevice::SetTextureFiltering()
{
	if (GetAnisotropic())
	{
		for (int i = 0; i < 5; i++)
		{
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, AnisotropicFactor);	
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			WeRenderDevice::Instance()->Device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			
		}
	}

	WeRenderDevice::Instance()->Device->SetSamplerState(5, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	WeRenderDevice::Instance()->Device->SetSamplerState(5, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	WeRenderDevice::Instance()->Device->SetSamplerState(5, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	WeRenderDevice::Instance()->Device->SetSamplerState(5, D3DSAMP_MAXANISOTROPY, 4);	

}
