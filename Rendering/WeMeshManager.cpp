/*
    WeMeshManager.cpp

	Autor: Nebojsa Vislavski
	Date: 18.11.2005.
	Version: 1.0


     

*/

#include <WeMeshManager.h>
#include <WeMesh.h>


WeMeshManager* WeMeshManager::_instance = NULL;


WeMeshManager* WeMeshManager::Instance()
{
	if(_instance) return _instance;
	_instance = new WeMeshManager();
	return _instance;
}

WeMeshManager::WeMeshManager()
{
	FreeAllInProgress = false;
}



void WeMeshManager::FreeAll()
{
	FreeAllInProgress = true;

	for (Meshes.First(); !Meshes.End(); )
	{
		SAFE_DELETE(Meshes.GetItem());
		Meshes.Delete();
	}

	FreeAllInProgress = false;
}

void WeMeshManager::Free(WeMesh* Mesh)
{
	for (Meshes.First(); !Meshes.End(); Meshes.Next())
	{
		if (Meshes.GetItem() == Mesh)
		{
			delete Meshes.GetItem();
			Meshes.Delete();
			return;
		}
	}
}


WeMesh* WeMeshManager::CreateDynamic(int NumFaces, int NumVertices, WeMaterial *Material)
{
	WeMesh *newMesh;
	newMesh = new WeMesh;
	newMesh->CreateDynamic(NumFaces, NumVertices, Material);
	Meshes.PushBack(newMesh);
	return newMesh;   
}




WeMesh* WeMeshManager::Load(StringParam name, bool FromX, bool ForceReload, bool ForceLoad)
{
	WeMesh *newMesh = NULL;
	bool isNew = false;

	if (!ForceReload)
	{
		for (Meshes.First(); !Meshes.End(); Meshes.Next())
		{
			if (Meshes.GetItem()->Name == name) 
			{
				if (ForceLoad)
				{
					newMesh = Meshes.GetItem();
					break;
				}
				else
					return Meshes.GetItem();
			}
		}
	}

	
	if (!newMesh)
	{
		newMesh = new WeMesh;
		isNew = true;
	}

	if (newMesh->Load(name, FromX))
	{
		if (isNew) // mozda da se doda && !ForceReload
		{
			Meshes.PushBack(newMesh);
		}
		return newMesh;
	}
	else
	{
		if (isNew)
		{
			delete newMesh;
		}
		return NULL;
	}
}


void WeMeshManager::OnLostDevice()
{
	for (Meshes.First(); !Meshes.End(); Meshes.Next())
	{		
		Meshes.GetItem()->OnLostDevice();
	}
}

void WeMeshManager::OnResetDevice()
{
	for (Meshes.First(); !Meshes.End(); Meshes.Next())
	{
		Meshes.GetItem()->OnResetDevice();
	}
}
