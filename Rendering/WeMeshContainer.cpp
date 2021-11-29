/*
		File: WeMeshContainer.cpp
		Author: Tomislav Kukic
		Date: 11.07.2006
		Version: 1.0
*/



#include <WeMeshContainer.h>
#include <WeMesh.h>
#include <WeFileStream.h>
#include <WePackage.h>





WeMeshContainer::WeMeshContainer()
{
	Name = NULL;
	MeshData.pMesh = NULL;
	pMaterials = NULL;
	pEffects = NULL;
	NumMaterials = 0;
	pAdjacency = NULL;
	pSkinInfo = NULL;
	pNextMeshContainer = NULL;
	Mesh = NULL;
}



WeMeshContainer::~WeMeshContainer()
{	
	NumMaterials = 0;

	delete Name; Name = NULL;
	delete pMaterials; pMaterials = NULL;
	delete pEffects; pEffects = NULL;

	if(MeshData.pMesh)
		MeshData.pMesh->Release();
	if(pSkinInfo)
		pSkinInfo->Release();

	delete pNextMeshContainer; pNextMeshContainer = NULL;
}




void WeMeshContainer::Save(WeFileStream* FS)
{
	for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
	{
		FS->WriteInt(Mesh->MaterialNames[j].GetItemsCount());			
		for(Mesh->MaterialNames[j].First(); !Mesh->MaterialNames[j].End(); Mesh->MaterialNames[j].Next())
		{
			FS->WriteString(Mesh->MaterialNames[j].GetItem());
		}
	}
}





void WeMeshContainer::Load(WeFileStream* FS)
{
	for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
	{
		Mesh->MaterialNames[j].Clear();
		int n = FS->ReadInt();
		for(int i = 0; i < n; i++)
		{
			String MatName = FS->ReadString();
			Mesh->MaterialNames[j].PushBack(MatName);
		}
	}
}


