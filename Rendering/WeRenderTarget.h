/*
    WeRenderTarget.h

	Autor: Nebojsa Vislavski
	Date: 2.11.2005.
	Version: 1.0
	Platform: Win32

    Thsi is render target class 
*/

#ifndef _WERENDERTARGET_H
#define _WERENDERTARGET_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WindEngine.h>


#define WERT_2D	  0
#define WERT_CUBE 1




class WINDENGINE_API WeRenderTarget
{
friend class WeRenderDevice;
friend class WeRenderTargetManager;

protected:
	LPDIRECT3DBASETEXTURE9	lpRT;

	IDirect3DSurface9	*lpSurf[6]; // for all normal and cube faces
	IDirect3DSurface9	*lpTextureSurf[6]; // for texture surfaces
	bool				Initialized;
	int					Width;
	int					Height;
	int					Size;
	D3DFORMAT			Format;
	D3DMULTISAMPLE_TYPE MType;
	int					Type;    // 2D, CUBE

	WeRenderTarget();
	~WeRenderTarget() { ShutDown(); }

	bool	Initialize(int w, int h, D3DFORMAT Format, D3DMULTISAMPLE_TYPE Type);
	bool	InitializeCube(int size, D3DFORMAT Format);
	void	ShutDown();

public:	

	LPDIRECT3DBASETEXTURE9	GetTexture() { return lpRT; }
	LPDIRECT3DSURFACE9		GetSurface(int index) { return lpSurf[index]; }
	LPDIRECT3DSURFACE9		GetTextureSurface(int index) { return lpTextureSurf[index]; }

	// will initialize capturing to render target. 
	// NOTE: It will not restore previous render target 
	// Depending on type CubePlane
	void BeginScene(int Rtnum = 0, int CubeFace = D3DCUBEMAP_FACE_POSITIVE_X);
	void Clear();
	
	void OnLostDevice();
	void OnResetDevice();
};


#endif