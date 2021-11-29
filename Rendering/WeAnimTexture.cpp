/*
		File: WeAnimTexture.cpp
		Author: Tomislav Kukic
		Date: 31.05.2006
		Version: 1.0
*/



#include <WeAnimTexture.h>
#include <WeTimer.h>
#include <WeRenderdevice.h>
#include <WeLoadCounter.h>



WeAnimTexture::WeAnimTexture()
{
	AnimationTime = 0.0f;
	FramesPerSecond = 0;
	ElapsedTime = 0.0f;
	FrameCounter = 0;
	AnimationTimer = 0.0f;
	Looped = false;
	IsPlaying = false;
	IsPaused = false;

	CurrentTexture = NULL;
	TextureColor = D3DCOLOR_RGBA(255,255,255,255);

	Name = (String)"";
}




WeAnimTexture::~WeAnimTexture()
{
	ClearAll();
}





void WeAnimTexture::InsertTextureAtStart(StringParam _Name)
{
	LPDIRECT3DTEXTURE9 temp = LoadTextureFromFile(_Name);
	if(!temp) return;

	Textures.PushFront(temp);
}





void WeAnimTexture::InsertTextureAtEnd(StringParam _Name)
{
	LPDIRECT3DTEXTURE9 temp = LoadTextureFromFile(_Name);
	if(!temp) return;

	Textures.PushBack(temp);
}





LPDIRECT3DTEXTURE9 WeAnimTexture::LoadTextureFromFile(StringParam _Name)
{
	LPDIRECT3DTEXTURE9 Texture2D = NULL;
		
	if(D3D_OK != D3DXCreateTextureFromFileEx(WeRenderDevice::Instance()->Device, _Name, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(0,0,0,0), NULL, NULL, &Texture2D))
	{
		return NULL;
	}

	D3DSURFACE_DESC dd;
	Texture2D->GetLevelDesc(0, &dd);
	Width = dd.Width;
	Height = dd.Height;

	FrameCounter++;
	return Texture2D;
}




void WeAnimTexture::SetAnimationTime(float AnimTime)
{
	AnimationTime = AnimTime;
}




float WeAnimTexture::GetAnimationTime()
{
	return AnimationTime;
}



void WeAnimTexture::SetFramesPerSecond(float Frames)
{
	FramesPerSecond = Frames;
}




float WeAnimTexture::GetFramesPerSecon()
{
	return FramesPerSecond;
}




void WeAnimTexture::SetElapsedTime(float ETime)
{
	ElapsedTime = ETime;
}




int WeAnimTexture::GetCurrentFrameNum()
{
	return FrameCounter;
}



bool WeAnimTexture::GetLooped()
{
	return Looped;
}



bool WeAnimTexture::GetIsPlayin()
{
	return IsPlaying;
}



bool WeAnimTexture::GetIsPaused()
{
	return IsPaused;
}




void WeAnimTexture::Play()
{
	if(IsPlaying) return;

	if(Textures.GetItemsCount() == 0) return;

	Textures.First();
	CurrentTexture = Textures.GetItem();

	IsPlaying = true;
	IsPaused = false;
}




void WeAnimTexture::Tick()
{
	if(Textures.GetItemsCount() == 0) return;
	if(!IsPlaying) return;
	if(IsPaused){ CurrentTexture = Textures.GetItem(); return; }

	AnimationTimer += WeTimer::Instance()->GetDeltaTimeF();
	if(AnimationTimer < AnimationTime) return;

	int tmpCount = (int)(AnimationTimer / AnimationTime);

	if(Looped)
	{
		Textures.SetCircular(true);
		for(int i=0; i<tmpCount; i++)
			Textures.Next();
		Textures.SetCircular(false);
		CurrentTexture = Textures.GetItem();
	}else{
		
		for(int i=0; i<tmpCount; i++)
		{
			Textures.Next();
			if(Textures.End())
			{
				Textures.Prev();
				CurrentTexture = Textures.GetItem();
				IsPlaying = false;
				IsPaused = false;
			}else{
				CurrentTexture = Textures.GetItem();
			}
		}
	}

	AnimationTimer = 0.0f;
}




void WeAnimTexture::SetLooped(bool _Loop)
{ 
	Looped = _Loop; 
}




void WeAnimTexture::Stop()
{
	IsPlaying = false;
	IsPaused = false;
}




void WeAnimTexture::Pause()
{
	if(Textures.GetItemsCount() == 0) return;
	if(!IsPlaying) return;

	IsPaused = true;
}





void WeAnimTexture::StepUp()
{
	if(Textures.GetItemsCount() == 0) return;

	if(Looped)
	{
		Textures.SetCircular(true);
		Textures.Next();
		Textures.SetCircular(false);
	}else
	{
		Textures.Next();
		if(Textures.End())
			Textures.Prev();
	}
}




void WeAnimTexture::StepDown()
{
	if(Textures.GetItemsCount() == 0) return;

	if(Looped)
	{
		Textures.SetCircular(true);
		Textures.Prev();
		Textures.SetCircular(false);
	}else
	{
		Textures.Prev();
		if(Textures.Begin())
			Textures.Next();
	}
}




LPDIRECT3DTEXTURE9 WeAnimTexture::GetCurrentTexture()
{
	return CurrentTexture;
}




LPDIRECT3DTEXTURE9 WeAnimTexture::GetTextureAtTime(float _Time)
{
	if(Textures.GetItemsCount() == 0) return NULL;
	
	LPDIRECT3DTEXTURE9 tmp = NULL;

	Textures.First();
	Textures.SetCircular(true);
	float TimeCounter = _Time / AnimationTime;

	for(float i = 0; i < TimeCounter; i++)
	{
		Textures.Next();
	}
	Textures.SetCircular(false);
	tmp = Textures.GetItem();
	CurrentTexture = Textures.GetItem();

	return tmp;
}




void WeAnimTexture::Serialize(WeFileStream &FS)
{
	LPD3DXBUFFER Buf = NULL;

	FS.WriteFloat(AnimationTime);
	FS.WriteBool(Looped);
	FS.WriteInt(Width);
	FS.WriteInt(Height);

	FS.WriteInt(Textures.GetItemsCount());
	for(Textures.First(); !Textures.End(); Textures.Next())
	{
		D3DXSaveTextureToFileInMemory(&Buf, D3DXIFF_DDS, Textures.GetItem(), NULL);
		FS.WriteDWord(Buf->GetBufferSize());
		FS.WriteData(Buf->GetBufferPointer(), Buf->GetBufferSize());
		SAFE_RELEASE(Buf);
	}
}





void WeAnimTexture::DeSerialize(WeFileStream &FS)
{
	LPDIRECT3DTEXTURE9 tmpTexture = NULL;

	AnimationTime = FS.ReadFloat();
	Looped = FS.ReadBool();
	Width = FS.ReadInt();
	Height = FS.ReadInt();
	FrameCounter = 0;

	DWORD Size = 0;
	char* Data = NULL;
	int Counter = FS.ReadInt();
	
	if(Textures.GetItemsCount() > 0) ClearAll();

	for(int i=0; i<Counter; i++)
	{
		char TempName[100];
		sprintf(TempName, "ANIM TEXTURE \"%s\" [%d/%d]", Name.GetString(), (i + 1), Counter);
		SetLoadingName((String)TempName);

		Size = FS.ReadDWord();
		Data = new char[Size];
		FS.ReadData((void*)Data,(int)Size);
		D3DXCreateTextureFromFileInMemory(WeRenderDevice::Instance()->Device, Data, Size, &tmpTexture);
		Textures.PushBack(tmpTexture);
		FrameCounter++;
		tmpTexture = NULL;
		SAFE_DELETE(Data);

		ResetLoadingName();
		//sprintf(TempName, "+ ANIM TEXTURE \"%s\" [%d/%d]", Name.GetString(), (i + 1), Counter);
		//SetLoadingName((String)TempName);
	}
}






void WeAnimTexture::ClearAll()
{
	for(Textures.First(); !Textures.End(); Textures.Next())
	{
		SAFE_RELEASE(Textures.GetItem());
	}

	Textures.Clear();
}


