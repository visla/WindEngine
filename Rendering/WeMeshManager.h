/*
    WeMeshManager.cpp

	Autor: Nebojsa Vislavski
	Date: 18.11.2005.
	Version: 1.0

	This menager is responsible for loading meshes and caring about duplicated 

     

*/


#ifndef _WEMESHMANAGER_H
#define _WEMESHMANAGER_H

#include <SmartList.h>
#include <WindEngine.h>
#include <WeMesh.h>

class WeMaterial;

class WINDENGINE_API WeMeshManager
{
protected:
	virtual ~WeMeshManager(void){ FreeAll(); }

	CSmartList<WeMesh*> Meshes;

private:
	static WeMeshManager* _instance;

public:
	static WeMeshManager* Instance();

	WeMeshManager();

	virtual void FreeAll();
	virtual void Free(WeMesh* Mesh);
	// this will completely deeply destroy mesh, releasing memory for materials, effects, textures,
	// you should use this only when meshes are loaded with ForceReload.
			void Destroy(WeMesh *Mesh); 

	// Loads mesh and uses cashing. If you are not familiar with params leave them to defaults
	virtual WeMesh* Load(StringParam name, bool FromX = true, bool ForceReload = false, bool ForceLoad = false);	
	virtual WeMesh* CreateDynamic(int NumFaces, int NumVertices, WeMaterial *Material); 

	void OnLostDevice();  
	void OnResetDevice();

	bool		FreeAllInProgress;
	
};



#endif