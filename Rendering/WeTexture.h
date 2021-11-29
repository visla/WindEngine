/*
         File: WeTexture.cpp
		 Author: Tomislav Kukic
		 Date: 13.09.2005.
		 Version: 1.0
*/



#ifndef _WETEXTURE
#define _WETEXTURE



#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <CString.h>
#include <WindEngine.h>



#define WETEXTYPE_NONE		0
#define WETEXTYPE_CUBE		1
#define WETEXTYPE_VOLUME	2
#define WETEXTYPE_2D		3


enum WeTextureQuality
{
	TEXTURES_LOW,
	TEXTURES_MEDIUM,
	TEXTURES_HIGH

};

class WINDENGINE_API WeTexture
{
friend class WeTextureManager;

protected:
	WeTexture(void);
	virtual ~WeTexture(void);

	DWORD Type;
	WeTextureQuality	Quality;
	WeTextureQuality	LowestQuality;

	IDirect3DBaseTexture9* Texture;
	bool LoadedFromPackage;

public:
	D3DFORMAT	Format;
	DWORD		Usage;
	D3DPOOL		Pool;

	int Width;
	int Height;
	int Depth;
	bool NoMipmaps;

	bool IsLoaded;

	String Name;

	FILETIME ModifyTime;

	virtual DWORD					GetType(){ return Type; }
	virtual IDirect3DBaseTexture9*	GetTexture(){ if(Texture) return Texture; else return NULL; }

	// TODO: Change LoadFromPackage default param to true when texture editor is finished
	// parameter DWORD Type = WETEXTYPE_2D removed: if name contains "_CUBE" it is CUBE, otherwise it is 2D
	bool							Load(StringParam name = "", bool LoadFromPackage = false);
	bool							CreateBlank(int width, int height, D3DFORMAT Format, DWORD Usage, D3DPOOL Pool);

	// outData = texture data 
	// outSize = number of bytes locked
	// outPitch = pitch allocated
	bool							Lock(void **outData, int &outSize, int &outPitch);
	void							UnLock();

	void OnLostDevice();
	void OnResetDevice();

	void							SetQuality(WeTextureQuality quality);
	WeTextureQuality				GetQuality();

	void							SetLowestQuality(WeTextureQuality quality);
	WeTextureQuality				GetLowestQuality();
};




#endif
