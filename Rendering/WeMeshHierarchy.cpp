/*
		File: WeMeshHierarchy.cpp
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/



#include <WeMeshHierarchy.h>
#include <WeMeshContainer.h>
#include <WeModelFrame.h>
#include <WeMesh.h>
#include <stdio.h>
#include <CString.h>
#include <WeRenderDevice.h>
#include <weSphere.h>



char EmptyName[10] = "<empty>";


HRESULT WeMeshHierarchy::CreateFrame( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
	*ppNewFrame = NULL;

    WeModelFrame *newFrame = new WeModelFrame;
	ZeroMemory(newFrame, sizeof(WeModelFrame));

	D3DXMatrixIdentity(&newFrame->TM);
	D3DXMatrixIdentity(&newFrame->PrevTM);

	if (Name)
	{
		newFrame->Name = strdup(Name);
	}
	else
	{
		newFrame->Name = strdup(EmptyName);
	}

	*ppNewFrame = (LPD3DXFRAME)newFrame;
    	
	 return S_OK;
}



HRESULT WeMeshHierarchy::CreateMeshContainer( THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData, 
                            CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
                            DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER * ppNewMeshContainer )
{
	WeMeshContainer *newMeshContainer = new WeMeshContainer;
	ZeroMemory(newMeshContainer, sizeof(WeMeshContainer));

	*ppNewMeshContainer = NULL;

	if (Name)
	{
		newMeshContainer->Name = strdup(Name);
	}
	else
	{
		newMeshContainer->Name = strdup(EmptyName);
	}

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		DestroyMeshContainer(newMeshContainer);
		return E_FAIL;
	}

	newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	
	//DWORD dwFaces = pMeshData->pMesh->GetNumFaces();
	//newMeshContainer->pAdjacency = new DWORD[dwFaces*3];
	//memcpy(newMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaces*3);
	

	//D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
	//if (FAILED(pMeshData->pMesh->GetDeclaration(Declaration)))
	//{
	//	return E_FAIL;
	//}


	pMeshData->pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_32BIT, NormalMappedMeshVertexElements, WeRenderDevice::Instance()->Device, &newMeshContainer->MeshData.pMesh);
	//pMeshData->pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_32BIT, Declaration, WeRenderDevice::Instance()->Device, &newMeshContainer->MeshData.pMesh);

	DWORD *Adjancency;
	
	Adjancency = new DWORD[3 * newMeshContainer->MeshData.pMesh->GetNumFaces()];
	newMeshContainer->MeshData.pMesh->GenerateAdjacency(0.3f, Adjancency);
	newMeshContainer->MeshData.pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, Adjancency, NULL, NULL, NULL);

	if (FAILED(D3DXComputeTangent(newMeshContainer->MeshData.pMesh, 0, 0, 0, 0, Adjancency)))
	{
		MessageBox(0, "Error computing tangent", 0, 0);
		return false;
	}

	delete [] Adjancency;

	//============= Dodatak za skined animacije ==========================  31.07.2006. ==========
	if(pSkinInfo)
	{
		pSkinInfo->Clone(&newMeshContainer->pSkinInfo);
		pSkinInfo->AddRef();

		int numBones = pSkinInfo->GetNumBones();

	    newMeshContainer->BoneOffsets = new D3DXMATRIX[numBones];
		newMeshContainer->FrameCombinedMatrices = new D3DXMATRIX*[numBones];

	    for (int i = 0; i < numBones; i++)
	        newMeshContainer->BoneOffsets[i] = *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	}
	else
	{
		newMeshContainer->pSkinInfo = NULL;
		newMeshContainer->BoneOffsets = NULL;
		newMeshContainer->FrameCombinedMatrices = NULL;
	}
	// ==========================================================================================

	newMeshContainer->NumMaterials = NumMaterials;

	newMeshContainer->Mesh = new WeMesh();
	newMeshContainer->Mesh->SetMesh(newMeshContainer->MeshData.pMesh, true, NumMaterials);
	newMeshContainer->Mesh->BSphere.Position = WeVector(0.0,0.0,0.0);
	newMeshContainer->Mesh->BSphere.Radius = 200.0f;


	*ppNewMeshContainer = newMeshContainer;

	 return S_OK;
}



HRESULT WeMeshHierarchy::DestroyFrame( THIS_ LPD3DXFRAME pFrameToFree )
{
	WeModelFrame *pFrame = (WeModelFrame*)pFrameToFree;

	SAFE_DELETE(pFrame->Name);
    SAFE_DELETE(pFrame);
	
	return S_OK;
}



HRESULT WeMeshHierarchy::DestroyMeshContainer( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	WeMeshContainer* pMeshContainer = (WeMeshContainer*)pMeshContainerToFree;
	
	//// name
	SAFE_DELETE(pMeshContainer->Name)

	//// adjacency data
	SAFE_DELETE(pMeshContainer->pAdjacency) 
	
	// release the main mesh
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	
	// release skin information
	SAFE_RELEASE(pMeshContainer->pSkinInfo);

	SAFE_DELETE(pMeshContainer->BoneOffsets);

	SAFE_DELETE(pMeshContainer->FrameCombinedMatrices);

	//SAFE_DELETE(pMeshContainer->SkinMesh);

	//SAFE_DELETE(pMeshContainer->Mesh);
	
	// finally delete the mesh container itself
	SAFE_DELETE(pMeshContainer);

	return S_OK;
}
