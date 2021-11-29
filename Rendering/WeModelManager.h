/*
    WeModelManager.h


	Autor: Nebojsa Vislavski
	Date: 18.11.2005.
	Version: 1.0

*/


#ifndef _WEMODELMANAGER_H
#define _WEMODELMANAGER_H

#include <SmartList.h>
#include <WindEngine.h>
#include <WeStaticModel.h>




class WINDENGINE_API WeModelManager
{
private:
	static WeModelManager* _instance;

protected:
    
	CSmartList <WeStaticModel *> Models;

public:
	static WeModelManager* Instance();

							WeModelManager();
							~WeModelManager();

	WeStaticModel*			Create();
	WeStaticModel*			Create(StringParam MeshName, StringParam QueryMeshName, bool FromPackage);
			// Material Preset is just signal that meshes should load that Materials			
	WeStaticModel*			Load(StringParam name, bool ForceReload = false); 

	// Clone - just clones anothers meshes. but materials, meshes,... if DeepCopy
	WeStaticModel*			Clone(WeStaticModel*, bool DeepCopy); 
	// this will completely destroy model deeply with his meshes, materials, etc.
	void					Destroy(WeStaticModel *);
	void					Save(StringParam name, StringParam MeshName, StringParam QueryMeshName);
	void					FreeAll();

	bool					FreeAllInProgress;
};




#endif