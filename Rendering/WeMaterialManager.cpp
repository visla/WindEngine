/*
           File: WeMaterialManager.h
		   Author: Tomislav Kukic
		   Date: 21.09.2005.
		   Version: 1.0
*/


#include <CString.h>
#include <WeMaterialManager.h>
#include <WeMaterial.h>



WeMaterialManager* WeMaterialManager::_instance = NULL;


WeMaterialManager* WeMaterialManager::Instance()
{
	if(_instance != NULL) return _instance;
	else{
		_instance = new WeMaterialManager();
		return _instance;
	}
}


WeMaterialManager::WeMaterialManager(void)
{
	FreeAllInProgress = false;
}



void WeMaterialManager::FreeAll()
{
	FreeAllInProgress = true;

	WeMaterial* tmpMat = NULL;

	for(Materials.First(); !Materials.End(); Materials.Next())
		delete Materials.GetItem();

	Materials.Clear();

	FreeAllInProgress = false;
}


void WeMaterialManager::Free(WeMaterial* Material)
{
	if(Materials.Empty()) return;

	WeMaterial* tmpMat = NULL;

	for(Materials.First(); !Materials.End(); Materials.Next())
	{
		tmpMat = Materials.GetItem();
		if(tmpMat == Material)
		{
			delete Materials.GetItem();
			Materials.Delete();
			return;
		}
	}
}


WeMaterial* WeMaterialManager::Load(StringParam ShaderName, StringParam TextureName)
{
	WeMaterial* tmp = NULL;
		
	tmp = new WeMaterial();
	tmp->Load(ShaderName, TextureName);
	Materials.PushBack(tmp);

	return tmp;
}




WeMaterial* WeMaterialManager::Load(StringParam HoleName, bool ForceLoad)// Use to load material from package
{
	WeMaterial* tmp = NULL;
	bool IsNew = false;
		
	for(Materials.First(); !Materials.End(); Materials.Next())
	{
		if(Materials.GetItem()->Name == HoleName) 
		{
			if (ForceLoad)
			{
				delete Materials.GetItem();
				Materials.GetItem() = NULL;
				Materials.Delete();
				break;
			}
			else
				return Materials.GetItem();
		}
	}

	if (!tmp)
	{
		tmp = new WeMaterial();
		IsNew = true;
	}
	
	if (tmp->Load(HoleName))
	{
		if (IsNew)
			Materials.PushBack(tmp);
		return tmp;
	}
	else
	{
		if (IsNew)
			delete tmp;
		return NULL;
	}	
}

void WeMaterialManager::Tick()
{
	// TODO: Napraviti i proveriti da slike budu razlicito animirane i da pocinju izpocetka
	for (Materials.First(); !Materials.End(); Materials.Next())
	{
		Materials.GetItem()->Tick();
	}
}