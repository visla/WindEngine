/*
       File: WeTextureManager.cpp
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/



#include <WeTexture.h>
#include <WeAnimTexture.h>
#include <WeTextureManager.h>
#include <WePackage.h>
#include <WeLoger.h>




WeTextureManager* WeTextureManager::_instance = NULL;


WeTextureManager* WeTextureManager::Instance()
{
	if(_instance != NULL) return _instance;
	else{
		_instance = new WeTextureManager();
		return _instance;
	}
}




WeTextureManager::WeTextureManager(void)
{
	FreeAllInProgress = false;
	Quality = TEXTURES_HIGH;
}




void WeTextureManager::Tick()
{
	for(AnimTextures.First(); !AnimTextures.End(); AnimTextures.Next())
	{
		AnimTextures.GetItem()->Tick();
	}
}



WeAnimTexture* WeTextureManager::LoadAnimTexture(StringParam Name)
{
	for(AnimTextures.First(); !AnimTextures.End(); AnimTextures.Next())
	{
		if(AnimTextures.GetItem()->Name == Name)
		{
			return AnimTextures.GetItem();
		}
	}

	WeAnimTexture* tmp = new WeAnimTexture();
	if(WePackage::Instance()->BeginReading(Name, 1))
	{
		tmp->DeSerialize(WePackage::Instance()->DataStream);
		tmp->Name = Name;
		WePackage::Instance()->EndReading();
		AnimTextures.PushBack(tmp);
		return tmp;
	}else{
		return NULL;
	}
}





void WeTextureManager::FreeAll()
{
	FreeAllInProgress = true;

	for(Textures.First(); !Textures.End(); Textures.Next())
	{
		//WeLoger::DebugLog("FreeTexture %s\n", Textures.GetItem()->Name);
		delete Textures.GetItem();
	}

	for(AnimTextures.First(); !AnimTextures.End(); AnimTextures.Next())
		delete AnimTextures.GetItem();

	Textures.Clear();
	AnimTextures.Clear();

	FreeAllInProgress = false;
}




void WeTextureManager::Free(WeTexture* Texture)
{
	if(Textures.Empty()) return;

	WeTexture* tmpTex = NULL;

	for(Textures.First(); !Textures.End(); Textures.Next())
	{
		tmpTex = Textures.GetItem();
		if(tmpTex == Texture)
		{
			delete Textures.GetItem();
			Textures.Delete();
			return;
		}
	}
}




void WeTextureManager::FreeAnimTexture(WeAnimTexture* Texture)
{
	if(Textures.Empty()) return;

	WeAnimTexture* tmpTex = NULL;

	for(AnimTextures.First(); !AnimTextures.End(); AnimTextures.Next())
	{
		tmpTex = AnimTextures.GetItem();
		if(tmpTex == Texture)
		{
			delete AnimTextures.GetItem();
			AnimTextures.Delete();
			return;
		}
	}
}




WeTexture* WeTextureManager::Load(StringParam name, bool LoadFromPackage, bool ForceReload, bool ForceLoad)
{
	WeTexture* tex = NULL;
	bool isNew = false;

	if(!ForceReload)
	{
		for(Textures.First(); !Textures.End(); Textures.Next())
		{
			if(Textures.GetItem()->Name == name)
			{
				if (ForceLoad)
				{
					tex = Textures.GetItem();
					break;
				}
				else
					return Textures.GetItem();
			}
		}
	}
	
	if (!tex)
	{
		tex = new WeTexture();
		isNew = true;
	}

	if(tex->Load(name, LoadFromPackage))
	{
		tex->Name = name;
		tex->SetQuality(Quality);
		if (isNew)
		{
			Textures.PushBack(tex);
		}
		return tex;
	}
	else
	{
		if (isNew)
		{
			delete tex;
		}
		return NULL;
	}
}




WeTexture* WeTextureManager::CreateBlank(int width, int height, D3DFORMAT Format, DWORD Usage, D3DPOOL Pool, bool AutoManaged)
{
	WeTexture* tmpTex = NULL;

    tmpTex = new WeTexture();
	if (tmpTex->CreateBlank(width, height, Format, Usage, Pool))
	{
		if (AutoManaged)
			Textures.PushBack(tmpTex);
	} else
	{
		SAFE_DELETE(tmpTex);
	}	
	return tmpTex;
}




void WeTextureManager::OnLostDevice()
{
	for(Textures.First(); !Textures.End(); Textures.Next())
		Textures.GetItem()->OnLostDevice();
}




void WeTextureManager::OnResetDevice()
{
	for(Textures.First(); !Textures.End(); Textures.Next())
		Textures.GetItem()->OnResetDevice();
}


void WeTextureManager::SetQuality(WeTextureQuality quality)
{
	Quality = quality;
	for (Textures.First(); !Textures.End(); Textures.Next())
		Textures.GetItem()->SetQuality(quality);
}
