/*
    WeModelManager.cpp

	Autor: Nebojsa Vislavski
	Date: 18.11.2005.
	Version: 1.0


*/

#include <WeModelManager.h>
#include <WePhysicModel.h>
#include <WePackage.h>
#include <WeMeshManager.h>
#include <WeModel.h>
#include <WeLoger.h>


WeModelManager* WeModelManager::_instance = NULL;


WeModelManager* WeModelManager::Instance()
{
	if(_instance) return _instance;
	_instance = new WeModelManager();
	return _instance;
}





WeModelManager::WeModelManager()
{ 
	FreeAllInProgress = false;
	Models.Clear();
}





WeModelManager::~WeModelManager()
{
	Models.Clear();
}




WeStaticModel *WeModelManager::Create()
{
	WeStaticModel *newModel;
	newModel = new WeStaticModel;
	return newModel;
}





WeStaticModel *WeModelManager::Create(StringParam MeshName, StringParam QueryMeshName, bool FromPackage)
{
	WeStaticModel *newModel;
	newModel = new WeStaticModel;

	newModel->Name = MeshName;
	newModel->Mesh = WeMeshManager::Instance()->Load(MeshName, !FromPackage);
	newModel->QueryMesh = WeMeshManager::Instance()->Load(QueryMeshName, !FromPackage);	
	return newModel;
}





void WeModelManager::FreeAll()
{
	FreeAllInProgress = true;

	for (Models.First(); !Models.End(); Models.Delete())
	{
		SAFE_DELETE(Models.GetItem());
	}

	FreeAllInProgress = false;
}






WeStaticModel* WeModelManager::Load(StringParam name, bool ForceReload)
{
	if (name.GetLen() == 0) return NULL;

	if (!ForceReload)
	{
		for (Models.First(); !Models.End(); Models.Next())
		{
			if (Models.GetItem()->Name == name)
			{
				return Clone(Models.GetItem(), false);
			}
		}
	}


	WeStaticModel* newSModel = NULL;
	WeModel* newModel = NULL;
	WeStaticModel* FinalModel = NULL;

	if (WePackage::Instance()->BeginReading(name, 6))
	{
#ifdef _DEBUG
		WeLoger::DebugLog("LOADING %s\n", name);
#endif

		if (WePackage::Instance()->DataStream.ReadInt() != 11000)
			newSModel = new WeStaticModel;

		WePackage::Instance()->EndReading();
	}
	else if (WePackage::Instance()->BeginReading(name, 10))
	{
#ifdef _DEBUG
		WeLoger::DebugLog("LOADING %s\n", name);
#endif

		if (WePackage::Instance()->DataStream.ReadInt() == 11000)
			newModel = new WeModel;

		WePackage::Instance()->EndReading();
	}


	if (newModel)
	{
		if (!newModel->Load(name, true, ForceReload))
		{
			delete newModel;
			return NULL;
		}

		Models.PushBack(newModel);
		FinalModel = Clone(newModel, false);
	}

	if (newSModel)
	{
		if(!newSModel->Load(name, ForceReload))
		{
			delete newSModel;
			return NULL;
		}

		Models.PushBack(newSModel);
		FinalModel = Clone(newSModel, false);
	}

	return FinalModel;
}




void WeModelManager::Destroy(WeStaticModel *Model)
{
	if (!Model) return;	

	WeMeshManager::Instance()->Free(Model->Mesh);
	WeMeshManager::Instance()->Free(Model->QueryMesh);
	//WeMeshManager::Instance()->Free(Model->ShadowMesh);
}



WeStaticModel* WeModelManager::Clone(WeStaticModel* Model, bool DeepCopy)
{
	if(!Model) return NULL;

	if(Model->IsModel)
	{
		WeModel* newModel = new WeModel();
		newModel->Clone((WeModel*)Model, DeepCopy);
		return (WeStaticModel*)newModel;
	}
	else
	{
		WeStaticModel* newModel = new WeStaticModel();
		newModel->Clone(Model, DeepCopy);
		return newModel;
	}
	
	return NULL;
}



void WeModelManager::Save(StringParam name, StringParam MeshName, StringParam QueryMeshName)
{
}





