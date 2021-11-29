/*
           File: WeMaterialManager.h
		   Author: Tomislav Kukic
		   Date: 21.09.2005.
		   Version: 1.0
*/





#ifndef _WEMATERIALMANAGER
#define _WEMATERIALMANAGER


#include <SmartList.h>
#include <CString.h>
#include <WindEngine.h>

class WeMaterial;


class WINDENGINE_API WeMaterialManager
{
protected:
	WeMaterialManager(void);
	virtual ~WeMaterialManager(void){}
	
	CSmartList<WeMaterial*> Materials;

private:
	static WeMaterialManager* _instance;

public:
	static WeMaterialManager* Instance();

	virtual void FreeAll();
	virtual void Free(WeMaterial* Material);
			void Tick(); // this will update texture animation

	WeMaterial* Load(StringParam ShaderName, StringParam TextureName);
	WeMaterial* Load(StringParam HoleName, bool ForceLoad = false);

	bool	FreeAllInProgress;
};



#endif