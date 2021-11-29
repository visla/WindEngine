/*
		File: WeModelFrame.cpp
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/




#include <WeModelFrame.h>
#include <WeMesh.h>
#include <WeRenderDevice.h>
#include <WeMeshContainer.h>
#include <WeMaterial.h>
#include <WeMaterialManager.h>
#include <WeBatchItem.h>
#include <WeBatchRenderer.h>
#include <WeEngine.h>
#include <WeActor.h>
#include <WeCamera.h>
#include <WeWorld.h>
#include <WeFileStream.h>
#include <WeShadowVolumeRenderer.h>
#include <WeLoger.h>





WeModelFrame::WeModelFrame()
{
	pFrameSibling = NULL;
	pFrameFirstChild = NULL;
}






WeModelFrame::~WeModelFrame()
{
	SAFE_DELETE(Name);
	SAFE_DELETE(pFrameSibling);
	SAFE_DELETE(pFrameFirstChild);
}







void WeModelFrame::Draw(WeWorld *World, WeCamera *Cam, WeActor* Actor)
{
	WeMeshContainer* meshContainer = (WeMeshContainer*)pMeshContainer;
    while (meshContainer)
    {
		DrawMeshContainer(World, Cam, Actor, meshContainer->Mesh);
        meshContainer = (WeMeshContainer*)meshContainer->pNextMeshContainer;
    }
}



void WeModelFrame::ComputeBoundingBox(WeActor *Owner, WeMatrix &pTM, WeModelFrame *ParentFrame)
{
	if (ParentFrame)
	{
		D3DXMatrixMultiply(&TM, &TransformationMatrix, &ParentFrame->TM);
	}
	else
	{
		D3DXMatrixMultiply(&TM, &TransformationMatrix, &pTM);
	}

	// calculate BBox
	WeVector *Pos, Min, Max;
	char *Data;
	WeMesh *pMesh;
	if (pMeshContainer)
	{
		if(((WeMeshContainer*)pMeshContainer)->Mesh)
		{	
			pMesh = ((WeMeshContainer*)pMeshContainer)->Mesh;			
			pMesh->LockVertexBuffer((void **)&Data, D3DLOCK_READONLY);

			D3DXVECTOR4 tmpV;
			WeMatrix tTM;

			Pos = (WeVector *)&Data[0];
			tTM = TM;
			//tTM._41 = Owner->Location.x;
			//tTM._42 = Owner->Location.y;
			//tTM._43 = Owner->Location.z;		

			D3DXVec3Transform(&tmpV, Pos, &tTM);

			Min.x = tmpV.x; Min.y = tmpV.y; Min.z = tmpV.z;
			Max.x = tmpV.x; Max.y = tmpV.y; Max.z = tmpV.z;

			for (int i = 1; i < pMesh->GetNumVertices(); i++)
			{
				// suppose that vertex is first item
				Pos = (WeVector *)&Data[i*pMesh->GetNumBytesPerVertex()];

				//tTM = Owner->TM;
				//tTM._41 = Owner->Location.x;
				//tTM._42 = Owner->Location.y;
				//tTM._43 = Owner->Location.z;		

				D3DXVec3Transform(&tmpV, Pos, &tTM);
				

				if (Min.x > tmpV.x) Min.x = tmpV.x;
				if (Min.y > tmpV.y) Min.y = tmpV.y;
				if (Min.z > tmpV.z) Min.z = tmpV.z;

				if (Max.x < tmpV.x) Max.x = tmpV.x;
				if (Max.y < tmpV.y) Max.y = tmpV.y;
				if (Max.z < tmpV.z) Max.z = tmpV.z;
			}
		    
			pMesh->UnlockVertexBuffer();

			BBox.Min = Min;
			BBox.Max = Max;	
		}

	}


	if(pFrameSibling)
		((WeModelFrame*)pFrameSibling)->ComputeBoundingBox(Owner, pTM, ParentFrame);

	if(pFrameFirstChild)
		((WeModelFrame*)pFrameFirstChild)->ComputeBoundingBox(Owner, TM, this);
}



void WeModelFrame::Update(WeMatrix &pTM, WeMatrix &pPrevTM, WeModelFrame* ParentFrame)
{	
	if (ParentFrame)
	{
		D3DXMatrixMultiply(&TM, &TransformationMatrix, &ParentFrame->TM);
		D3DXMatrixMultiply(&PrevTM, &TransformationMatrix, &ParentFrame->PrevTM);
	}
	else
	{
		D3DXMatrixMultiply(&TM, &TransformationMatrix, &pTM);
		D3DXMatrixMultiply(&PrevTM, &TransformationMatrix, &pPrevTM);
	}

	if(pFrameSibling)
		((WeModelFrame*)pFrameSibling)->Update(pTM, pPrevTM, ParentFrame);

	if(pFrameFirstChild)
		((WeModelFrame*)pFrameFirstChild)->Update(TM, PrevTM, this);
}





void WeModelFrame::DrawMeshContainer(WeWorld *World, WeCamera *Cam, WeActor* Actor, WeMesh* Mesh)
{
	// TODO: Find mesh you will use for this model. Using Progressive meshes
	if (!Actor) return;

	WeBatchItem *BatchItem;

	CSmartList<WeMaterial*>* Materials;

	// if we havent loaded materials for that preset already then load it
	LoadMaterials(Actor);
	//Materials = &((WeMeshContainer*)(pMeshContainer))->Mesh->Materials[Actor->MaterialPreset];
	Materials = &Mesh->Materials[Actor->MaterialPreset];
	

	CSmartList<WeAttribRange > *Attributes;
	Attributes = &Mesh->MeshAttributes;

	for(Attributes->First(), Materials->First(); !Attributes->End(); Attributes->Next(), Materials->Next())
	{
		BatchItem = new WeBatchItem();

		BatchItem->Mesh = Mesh;
		BatchItem->ShadowMesh = Mesh;
		BatchItem->WorldMatrix = TM;
		BatchItem->PrevWorldMatrix = PrevTM;
		BatchItem->AttribRange = Attributes->GetItem();
		BatchItem->Material = Materials->GetItem();
		BatchItem->Model = (WeStaticModel*)Actor->Model;
		BatchItem->Actor = Actor;
		BatchItem->BSphere = Actor->BSphere;
		BatchItem->Alpha = false;
		WeVector pos;
		pos.x = TM._41;
		pos.y = TM._42;
		pos.z = TM._43;
		BatchItem->BSphere.Position = Actor->BSphere.Position + pos;


		// if we have shader then find nearest lights and also if dynamic lights is turned on
		if (Actor->WantLight) 
		{
			if (Materials->GetItem()->Shader) //&& WeRenderDevice::Instance()->GetLight())
			{		
				World->FindNearestLights(Actor->BSphere, BatchItem->Lights, Materials->GetItem()->Shader->WantLightsCount, Materials->GetItem()->Shader->WantDirectLightsCount, !Actor->IsPrefab);			
				// if Shader want sum light color 
			}
		}


		//// Calculate ZDistance only if this is translucent material
		if (Materials->GetItem()->Type == WEMATERIALTYPE_ALPHABLENDED)
		{
			WeVector tmp;						
			D3DXVec3TransformCoord(&tmp, &pos, &Cam->View);
			BatchItem->ZDist = abs(tmp.z);
			BatchItem->Alpha = true;
		}
		// ------------------------ 		
		// Occlusion Query stuff
		// if we have querymesh use it or use normal mesh
		// ----

		WeBatchRenderer::Instance()->QueueBatchItem(BatchItem);	
		WeShadowVolumeRenderer::Instance()->QueueBatchItem(BatchItem);
	}
}



void WeModelFrame::LoadMaterials(int MaterialPreset)
{
	WeMeshContainer* meshContainer = (WeMeshContainer*)pMeshContainer;
	while (meshContainer)
	{
		WeMesh *tempMesh;
		tempMesh = meshContainer->Mesh;

		if (tempMesh)
		{
			CSmartList<WeMaterial*>* Materials;

			// if we havent loaded materials for that preset already then load it
			Materials = &tempMesh->Materials[MaterialPreset];
			if (Materials->Empty() || tempMesh->GetRefresh())
			{
				int Size;
				WeMaterial *tmpMat;

				//if (!Materials->Empty()) Materials->Clear();
				Size = tempMesh->MeshAttributes.GetItemsCount();

				//tempMesh->MaterialNames[MaterialPreset].First();
				Materials->First();
				for (int i = 0; i < Size; i++)
				{
					tmpMat = WeMaterialManager::Instance()->Load(tempMesh->MaterialNames[MaterialPreset].GetItem());					
					// if not existing material then use default
					if (!tmpMat)
					{				
						tmpMat = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);					
					}
					Materials->PushBack(tmpMat);
					//tempMesh->MaterialNames[MaterialPreset].Next();
				}
			}
		}

		meshContainer = (WeMeshContainer*)meshContainer->pNextMeshContainer;
	}

	if (pFrameSibling)
		((WeModelFrame*)pFrameSibling)->LoadMaterials(MaterialPreset);

	if (pFrameFirstChild)
		((WeModelFrame*)pFrameFirstChild)->LoadMaterials(MaterialPreset);
}



void WeModelFrame::LoadMaterials(WeActor *Owner)
{
	if (Owner)
		LoadMaterials(Owner->MaterialPreset);
}





void WeModelFrame::Save(WeFileStream* FS, WeModelFrame* frame)
{
	if(pMeshContainer)
		((WeMeshContainer*)(pMeshContainer))->Save(FS);

	if(pFrameSibling)
		((WeModelFrame*)pFrameSibling)->Save(FS, frame);

	if(pFrameFirstChild)
		((WeModelFrame*)pFrameFirstChild)->Save(FS, this);	
}





void WeModelFrame::Load(WeFileStream* FS, WeModelFrame* frame)
{
	if(pMeshContainer)
		((WeMeshContainer*)(pMeshContainer))->Load(FS);

	if(pFrameSibling)
		((WeModelFrame*)pFrameSibling)->Load(FS, frame);

	if(pFrameFirstChild)
		((WeModelFrame*)pFrameFirstChild)->Load(FS, this);	
}




void WeModelFrame::Clone(WeModelFrame* Original, bool DeepCopy)
{
	Name = strdup(Original->Name);
	TransformationMatrix = Original->TransformationMatrix;
	if(!Original->pMeshContainer)
	{
		pMeshContainer = NULL;
		return;
	}

	WeMeshContainer* tmpContainer = new WeMeshContainer();
	tmpContainer->pMaterials = NULL;
	tmpContainer->pEffects = NULL;
	tmpContainer->pAdjacency = NULL;
	tmpContainer->pNextMeshContainer = NULL;

	if(Original->pMeshContainer->pSkinInfo)
	{
		Original->pMeshContainer->pSkinInfo->Clone(&tmpContainer->pSkinInfo);

		UINT numBones = tmpContainer->pSkinInfo->GetNumBones();
		tmpContainer->BoneOffsets = new D3DXMATRIX[numBones];
		tmpContainer->FrameCombinedMatrices = new D3DXMATRIX*[numBones];
		
		for (UINT i = 0; i < numBones; i++)
			tmpContainer->BoneOffsets[i] = *(tmpContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	}

	tmpContainer->Name = Original->pMeshContainer->Name;
	tmpContainer->MeshData = Original->pMeshContainer->MeshData;

	if (DeepCopy)
	{
		tmpContainer->Mesh = new WeMesh();
		tmpContainer->Mesh->Clone(((WeMeshContainer*)(Original->pMeshContainer))->Mesh);	
	}
	else
	{
		tmpContainer->Mesh = ((WeMeshContainer*)(Original->pMeshContainer))->Mesh;
	}

	pMeshContainer = (LPD3DXMESHCONTAINER)tmpContainer;
}



void WeModelFrame::SetUseNormalMaps(bool Use)
{
	WeMeshContainer* meshContainer = (WeMeshContainer*)pMeshContainer;
    if (meshContainer && meshContainer->Mesh)
		meshContainer->Mesh->SetNewUseNormalMap(Use);
}


