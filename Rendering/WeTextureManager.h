/*
       File: WeTextureManager.h
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/





#ifndef _WETEXTUREMANAGER
#define _WETEXTUREMANAGER



#include <CString.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <WindEngine.h>
#include <WeTexture.h>
#include <WeAnimTexture.h>



class WINDENGINE_API WeTextureManager
{
protected:
	WeTextureManager(void);
	virtual ~WeTextureManager(void){ FreeAll(); }

	CSmartList<WeTexture*> Textures;
	CSmartList<WeAnimTexture*> AnimTextures;

private:
	static WeTextureManager*	_instance;
	WeTextureQuality			Quality;


public:
	static WeTextureManager*	Instance();

	virtual void				FreeAll();
	virtual void				Free(WeTexture* Texture);
	virtual void				FreeAnimTexture(WeAnimTexture* Texture);

	void						Tick();
	WeAnimTexture*				LoadAnimTexture(StringParam Name);
	
	// If you supply no default param loading is performed from package instead from file
	// TODO: Change this default param when texture editor is finished	
	virtual WeTexture*			 Load(StringParam name, bool LoadFromPackage = false, bool ForceReload = false, bool ForceLoad = false);
	// auto managed means texture manager is responsible of deleting and removing texture. else you need to delete it by yourself
	// ex. lightmaps are created with automanaging
	virtual WeTexture*			 CreateBlank(int width, int height, D3DFORMAT Format, DWORD Usage, D3DPOOL Pool, bool AutoManaged = false);

	void						SetQuality(WeTextureQuality quality); 
	WeTextureQuality			GetQuality() { return Quality; }

	void						OnLostDevice();
	void						OnResetDevice();

	bool						FreeAllInProgress;
};



#endif
