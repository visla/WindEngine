/*
         File: WeTexture.h
		 Author: Tomislav Kukic
		 Date: 13.09.2005.
		 Version: 1.0
*/



#include <d3dx9.h>
#include <WeTexture.h>
#include <WePackage.h>
#include <WerenderDevice.h>
#include <WeLoadCounter.h>
#include <WeLoger.h>




WeTexture::WeTexture(void)
{
	Width = 0;
	Height = 0;
	Depth = 0;

	IsLoaded = false;

	Type = WETEXTYPE_NONE;
	Texture = NULL;
	LoadedFromPackage = false;

	LowestQuality = TEXTURES_LOW;
	Quality = TEXTURES_HIGH;
	NoMipmaps = false;

	ModifyTime.dwLowDateTime = 0;
	ModifyTime.dwHighDateTime = 0;
}

WeTexture::~WeTexture()
{
	SAFE_RELEASE(Texture);
}


bool WeTexture::Load(StringParam name, bool LoadFromPackage)
{
	char TempName[1000];
	sprintf(TempName, "TEXTURE \"%s\"", name.GetString());
	SetLoadingName((String)TempName);

	Name = name;

	if (LoadFromPackage)
	{
		// TODO: Add here code for loading from package
		if (WePackage::Instance()->BeginReading(name, 1))
		{
			WeFileStream *FS = &WePackage::Instance()->DataStream;
			int ver = 0;

			int fSize = FS->ReadInt();
			if (fSize < 0) // we use versioned textures!
			{
				fSize = -fSize;
				ver = FS->ReadInt();
			}

			if (ver == 0)
			{
				char* TexData;
				TexData = new char[fSize];
				FS->ReadData((void*)TexData, (int)fSize);

				FS->ReadString();
				Type = FS->ReadDWord();

				SAFE_RELEASE(Texture);

				if(Type == WETEXTYPE_2D)
				{
					LPDIRECT3DTEXTURE9 tmpTex;
					D3DXCreateTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				else if(Type == WETEXTYPE_CUBE)
				{
					LPDIRECT3DCUBETEXTURE9 tmpTex;
					D3DXCreateCubeTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				delete [] TexData;

				Format = (D3DFORMAT)FS->ReadDWord();
				Usage = FS->ReadDWord();
				Pool = (D3DPOOL)FS->ReadDWord();

				Width = FS->ReadInt();
				Height = FS->ReadInt();
				Depth = FS->ReadInt();
			}

			else if (ver == 1)
			{
				char* TexData;
				TexData = new char[fSize];
				FS->ReadData((void*)TexData, (int)fSize);

				FS->ReadString();
				Type = FS->ReadDWord();

				SAFE_RELEASE(Texture);

				if(Type == WETEXTYPE_2D)
				{
					LPDIRECT3DTEXTURE9 tmpTex;
					D3DXCreateTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				else if(Type == WETEXTYPE_CUBE)
				{
					LPDIRECT3DCUBETEXTURE9 tmpTex;
					D3DXCreateCubeTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				delete[] TexData;

				Format = (D3DFORMAT)FS->ReadDWord();
				Usage = FS->ReadDWord();
				Pool = (D3DPOOL)FS->ReadDWord();

				Width = FS->ReadInt();
				Height = FS->ReadInt();
				Depth = FS->ReadInt();

				LowestQuality = (WeTextureQuality)FS->ReadDWord();
			} // end of version 1

			else if (ver == 2)
			{
				char* TexData;
				TexData = new char[fSize];
				FS->ReadData((void*)TexData, (int)fSize);

				FS->ReadString();
				Type = FS->ReadDWord();

				SAFE_RELEASE(Texture);

				if(Type == WETEXTYPE_2D)
				{
					LPDIRECT3DTEXTURE9 tmpTex;
					D3DXCreateTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				else if(Type == WETEXTYPE_CUBE)
				{
					LPDIRECT3DCUBETEXTURE9 tmpTex;
					D3DXCreateCubeTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
					Texture = tmpTex;
				}

				delete[] TexData;

				Format = (D3DFORMAT)FS->ReadDWord();
				Usage = FS->ReadDWord();
				Pool = (D3DPOOL)FS->ReadDWord();

				Width = FS->ReadInt();
				Height = FS->ReadInt();
				Depth = FS->ReadInt();	

				LowestQuality = (WeTextureQuality)FS->ReadInt();

				ModifyTime.dwLowDateTime = FS->ReadDWord();
				ModifyTime.dwHighDateTime = FS->ReadDWord();
			} // end of version 2

			else if (ver == 3)
			{
				Type = FS->ReadDWord();

				Format = (D3DFORMAT)FS->ReadDWord();
				Usage = FS->ReadDWord();
				Pool = (D3DPOOL)FS->ReadDWord();

				Width = FS->ReadInt();
				Height = FS->ReadInt();
				Depth = FS->ReadInt();

				LowestQuality = (WeTextureQuality)FS->ReadInt();

				ModifyTime.dwLowDateTime = FS->ReadDWord();
				ModifyTime.dwHighDateTime = FS->ReadDWord();

				NoMipmaps = FS->ReadBool();

				if (NoMipmaps && (Type == WETEXTYPE_2D))
				{
					LPDIRECT3DTEXTURE9 Tex = NULL;
					D3DXCreateTexture(WeRenderDevice::Instance()->Device, Width, Height, 1, Usage, Format, D3DPOOL_MANAGED, &Tex);

					RECT r;
					r.left = 0;
					r.top = 0;
					r.right = Width;
					r.bottom = Height;

					int Pitch;
					Pitch = FS->ReadInt();

					D3DLOCKED_RECT lr;
					Tex->LockRect(0, &lr, &r, 0);
					WePackage::Instance()->DataStream.ReadData(lr.pBits, Height * Pitch);
					Tex->UnlockRect(0);

					Texture = Tex;
				}
				else
				{
					char* TexData;
					TexData = new char[fSize];
					FS->ReadData((void*)TexData, (int)fSize);

					SAFE_RELEASE(Texture);

					if(Type == WETEXTYPE_2D)
					{
						LPDIRECT3DTEXTURE9 tmpTex;
						D3DXCreateTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL,&tmpTex);
						Texture = tmpTex;
					}

					else if(Type == WETEXTYPE_CUBE)
					{
						LPDIRECT3DCUBETEXTURE9 tmpTex;
						D3DXCreateCubeTextureFromFileInMemoryEx(WeRenderDevice::Instance()->Device, (void*)TexData, fSize,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &tmpTex);
						Texture = tmpTex;
					}

					delete[] TexData;
				}
			} // end of version 3
		}
		else
		{ 
			Texture = NULL; 
			IsLoaded = false;
			ResetLoadingName();
			return false; 
		}
		WePackage::Instance()->EndReading();	

		IsLoaded = true;
		ResetLoadingName();
		LoadedFromPackage = true;
		return true;
	}

	String tmpS = name;
	if (!tmpS.ContainsCaseInsensitive((String)"_CUBE."))
	{
		LPDIRECT3DTEXTURE9 Texture2D = NULL;
		
		if(D3D_OK != D3DXCreateTextureFromFileEx(WeRenderDevice::Instance()->Device, name,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &Texture2D))
		{
			Texture = NULL;
			ResetLoadingName();
			return false;
		}
		Texture = Texture2D;
		Type = WETEXTYPE_2D;
		D3DSURFACE_DESC dd;
		Texture2D->GetLevelDesc(0, &dd);
		Width = dd.Width;
		Height = dd.Height;
		Format = dd.Format;
		Usage = dd.Usage;	
	}
	else
	{
		LPDIRECT3DCUBETEXTURE9 CubeTexture = NULL;
		
		if(D3D_OK != D3DXCreateCubeTextureFromFileEx(WeRenderDevice::Instance()->Device, name,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL, &CubeTexture))
		{
			Texture = NULL;
			ResetLoadingName();
			return false;
		}
		Texture = CubeTexture;
		Type = WETEXTYPE_CUBE;
		D3DSURFACE_DESC dd;
		CubeTexture->GetLevelDesc(0, &dd);
		Width = dd.Width;
		Height = dd.Height;
		Format = dd.Format;
		Usage = dd.Usage;	
	}

	FILE *f = fopen(Name, "rb");
	if (f)
	{
		GetFileTime(f, NULL, NULL, &ModifyTime);
		fclose(f);
	}


	ResetLoadingName();
	//sprintf(TempName, "+ TEXTURE \"%s\"", name.GetString());
	//SetLoadingName((String)TempName);

	IsLoaded = true;
	return true;
}

bool WeTexture::Lock(void **outData, int &outSize, int &outPitch)
{
	if (!Texture) return false;
	if (Type != WETEXTYPE_2D) return false;


	LPDIRECT3DTEXTURE9 Tex;
	Tex = (LPDIRECT3DTEXTURE9)Texture;
	D3DLOCKED_RECT Rect;
	if (FAILED(Tex->LockRect(0, &Rect, NULL, D3DLOCK_DISCARD)))
	{
		return false;
	}

	if (outData) *outData = Rect.pBits;
	outPitch = Rect.Pitch;
	outSize = Width*Height*(Rect.Pitch/Width);
	return true;
}

void WeTexture::UnLock()
{
	if (!Texture) return;
	if (Type != WETEXTYPE_2D) return;

	LPDIRECT3DTEXTURE9 Tex;
	Tex = (LPDIRECT3DTEXTURE9)Texture;	
	Tex->UnlockRect(0);	
}

bool WeTexture::CreateBlank(int width, int height, D3DFORMAT Format, DWORD Usage, D3DPOOL Pool)
{
	this->Width =	width;
	this->Height =	height;
	this->Format =	Format;
	this->Usage =	Usage;
	this->Pool =	Pool;
	this->Type = WETEXTYPE_2D;
	//this->LowestQuality = TEXTURES_HIGH;

	LPDIRECT3DTEXTURE9 Texture2D = NULL;
	if (FAILED(D3DXCreateTexture(WeRenderDevice::Instance()->Device, width, height, 0, Usage, Format, Pool, &Texture2D)))
	{
		IsLoaded = false;
		Texture = NULL;
		return false;
	}

	Texture = Texture2D;
	IsLoaded = true;
	return true;
}


void WeTexture::OnLostDevice()
{	
	//SAFE_RELEASE(Texture);
	//IsLoaded = false;
}


void WeTexture::OnResetDevice()
{
	//Load(Name,LoadedFromPackage,Type);
}



WeTextureQuality WeTexture::GetQuality()
{
	return Quality;
}



void WeTexture::SetQuality(WeTextureQuality quality)
{
	Quality = quality;

	WeTextureQuality qual;
	if (quality < LowestQuality)
		qual = LowestQuality;
	else
		qual = Quality;

	Texture->SetLOD(2-qual);
}


WeTextureQuality WeTexture::GetLowestQuality()
{
	return LowestQuality;
}


void WeTexture::SetLowestQuality(WeTextureQuality quality)
{
	LowestQuality = quality;
	SetQuality(Quality);
}


