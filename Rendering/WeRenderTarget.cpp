/*
    WeRenderTarget.cpp

	Autor: Nebojsa Vislavski
	Date: 2.11.2005.
	Version: 1.0
	Platform: Win32

    Thsi is render target class 
*/


#include <WeRenderTarget.h>
#include <WeRenderDevice.h>


WeRenderTarget::WeRenderTarget()
{
	Initialized = false;
	for (int i = 0; i < 6; i++) lpSurf[i] = NULL;
	for (int i = 0; i < 6; i++) lpTextureSurf[i] = NULL;
}

bool WeRenderTarget::Initialize(int w, int h, D3DFORMAT format, D3DMULTISAMPLE_TYPE MultiSample)
{
	if (Initialized) return true;

	LPDIRECT3DTEXTURE9 lpTmp;

	SAFE_RELEASE(lpSurf[0]);
	if (MultiSample != D3DMULTISAMPLE_NONE)
	{
		if (FAILED(WeRenderDevice::Instance()->Device->CreateRenderTarget(w, h, format, MultiSample, 0, false, &lpSurf[0], NULL)))
		{
			Initialized = false;
			return false;
		} else Initialized = true;
	}


	if (FAILED(WeRenderDevice::Instance()->Device->CreateTexture(w, h, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &lpTmp, NULL)))
	{
		Initialized = false;
		return false;
	} else Initialized = true;


	// If we are using multisampling then surface of texture goes in different texturesurf. 
	// and if not using multisampling then surface goes to regular surface
	if (MultiSample != D3DMULTISAMPLE_NONE)
	{
		lpTmp->GetSurfaceLevel(0, &lpTextureSurf[0]);    
	} else
	{
		lpTmp->GetSurfaceLevel(0, &lpSurf[0]);
	}
    
	Width = w;
	Height = h;
	Format = format;
	Type = WERT_2D;	
	lpRT = lpTmp;
	MType = MultiSample;
	return Initialized;
}



void WeRenderTarget::Clear()
{
	for (int i = 0; i < 6; i++)
	{
		if (lpSurf[i])
		{
			//lpSurf[i]->LockRect(

		}
	}
}

bool WeRenderTarget::InitializeCube(int size, D3DFORMAT format)
{
	if (Initialized) return true;

	LPDIRECT3DCUBETEXTURE9 lpTmp;

	

	if (FAILED(WeRenderDevice::Instance()->Device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &lpTmp, NULL)))
	{
		Initialized = false;
		return false;
	} else Initialized = true;

	for (int i = 0; i < 6; i++)
	{
		lpTmp->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &lpSurf[i]);
	}
    
	Size = size;
	Format = format;
	Type = WERT_CUBE;	
	lpRT = lpTmp;
	return Initialized;
}




void WeRenderTarget::ShutDown()
{
	if (!Initialized) return;

	for (int i = 0; i < 6; i++) SAFE_RELEASE( lpSurf[i] );
	for (int i = 0; i < 6; i++) SAFE_RELEASE( lpTextureSurf[i] );
	SAFE_RELEASE(lpRT);
	Initialized = false;
}

void WeRenderTarget::BeginScene(int Rtnum, int CubeFace)
{
	if (Type == WERT_2D)
	{
		WeRenderDevice::Instance()->Device->SetRenderTarget(Rtnum, lpSurf[0]);
	}
	else
	{
		WeRenderDevice::Instance()->Device->SetRenderTarget(Rtnum, lpSurf[CubeFace]);
	}
}


void WeRenderTarget::OnResetDevice()
{
	if (Type == WERT_2D)
		Initialize(Width, Height, Format, MType);
	else 
		InitializeCube(Size, Format);
}


void WeRenderTarget::OnLostDevice()
{
	ShutDown();
}