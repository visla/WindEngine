/*
       File: WeStatixModel.cpp
	   Author: Vislavski Nebojsa
	   Date: 12.09.2005.
	   Version: 1.0
*/



#include <CString.h>
#include <WeStaticModel.h>
#include <SmartList.h>
#include <WeBatchRenderer.h>
#include <WeAttribRange.h>
#include <WeMaterial.h>
#include <WeBatchItem.h>
#include <WeMesh.h>
#include <WeActor.h>
#include <WeCamera.h>
#include <WeQueryManager.h>
#include <WeMeshManager.h>
#include <WeEngine.h>
#include <WeRenderDevice.h>
#include <WePackage.h>
#include <WeShadowVolumeRenderer.h>

#include <d3dx9.h>



WeStaticModel::WeStaticModel(void)
{
	Mesh =	NULL;
	QueryMesh = NULL;
	//ShadowMesh = NULL;
	IsModel = false;
	Version = 1;
	
	D3DXMatrixIdentity(&TM);
}


void WeStaticModel::UpdateTransformations(WeMatrix &parentTM, WeMatrix &prevTM)
{
	TM = parentTM;
	PrevTM = prevTM;
}


void WeStaticModel::ComputeBoundingBox(WeActor *Owner)
{
	if (!Owner) return;
	if (!Owner->Model) return;
	if (!Owner->Model->Mesh) return;

	char *Data;
	WeVector Min, Max;
	WeVector *Pos;

	Owner->Model->Mesh->LockVertexBuffer((void **)&Data, D3DLOCK_READONLY);

	D3DXVECTOR4 tmpV;
	WeMatrix tTM;

	Pos = (WeVector *)&Data[0];
	tTM = Owner->TM;
	tTM._41 = Owner->Location.x;
	tTM._42 = Owner->Location.y;
	tTM._43 = Owner->Location.z;		

	D3DXVec3Transform(&tmpV, Pos, &tTM);

	Min.x = tmpV.x; Min.y = tmpV.y; Min.z = tmpV.z;
	Max.x = tmpV.x; Max.y = tmpV.y; Max.z = tmpV.z;

	for (int i = 1; i < Owner->Model->Mesh->GetNumVertices(); i++)
	{
		// suppose that vertex is first item
		Pos = (WeVector *)&Data[i*Owner->Model->Mesh->GetNumBytesPerVertex()];

		tTM = Owner->TM;
		tTM._41 = Owner->Location.x;
		tTM._42 = Owner->Location.y;
		tTM._43 = Owner->Location.z;		

		D3DXVec3Transform(&tmpV, Pos, &tTM);
		

		if (Min.x > tmpV.x) Min.x = tmpV.x;
		if (Min.y > tmpV.y) Min.y = tmpV.y;
		if (Min.z > tmpV.z) Min.z = tmpV.z;

		if (Max.x < tmpV.x) Max.x = tmpV.x;
		if (Max.y < tmpV.y) Max.y = tmpV.y;
		if (Max.z < tmpV.z) Max.z = tmpV.z;
	}
    
	Owner->Model->Mesh->UnlockVertexBuffer();

	Owner->BBox.Min = Min;
	Owner->BBox.Max = Max;	

	WeVector SPos;

	SPos = (Max-Min) / 2.0f;
	

	Owner->BSphere.Position = Min + SPos;
	Owner->BSphere.Radius = D3DXVec3Length(&SPos);

}



void WeStaticModel::Clone(WeStaticModel *Original, bool DeepCopy)
{
	if (!Original) return;

	Name = Original->Name;
	MeshName = Original->MeshName;
	QueryMeshName = Original->QueryMeshName;
	//ShadowMeshName = Original->ShadowMeshName;

	if (!DeepCopy)
	{
		Mesh = Original->Mesh;
		QueryMesh = Original->QueryMesh;
		//ShadowMesh = Original->ShadowMesh;
	} 
	else
	{
		Mesh = WeMeshManager::Instance()->Load(MeshName, false, true);
		QueryMesh = WeMeshManager::Instance()->Load(QueryMeshName, false, true);
		//ShadowMesh = WeMeshManager::Instance()->Load(ShadowMeshName, false, true);
	}
}


void WeStaticModel::LoadMaterials(int MaterialPreset)
{
	if (!Mesh) return;

	CSmartList<WeMaterial*>* Materials;

	// if we havent loaded materials for that preset already then load it
	Materials = &Mesh->Materials[MaterialPreset];
	if (Materials->Empty() || Mesh->GetRefresh())
	{
		int Size;
		WeMaterial *tmpMat;

		if (!Materials->Empty()) Materials->Clear();
		Size = Mesh->MeshAttributes.GetItemsCount();

		Mesh->MaterialNames[MaterialPreset].First();
		for (int i = 0; i < Size; i++)
		{	
			tmpMat = WeMaterialManager::Instance()->Load(Mesh->MaterialNames[MaterialPreset].GetItem());					
			// if not existing material then use default
			if (!tmpMat)
			{				
				tmpMat = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);					
			}
			Materials->PushBack(tmpMat);
			Mesh->MaterialNames[MaterialPreset].Next();
		}
	}
}




void WeStaticModel::LoadMaterials(WeActor *Owner)
{
	if (Owner)
		LoadMaterials(Owner->MaterialPreset);
}



void WeStaticModel::Draw(WeWorld* World, WeCamera* Cam, WeActor* Owner)
{
	// TODO: Find mesh you will use for this model. Using Progressive meshes
	if (!Owner) return;
	if (!Mesh) return;

	// Check to see if this is visible
	WeSphere s;
	WeVector pos;
	pos.x = TM._41;
	pos.y = TM._42;
	pos.z = TM._43;

	if (Owner->Type == WETYPE_ZONE)
	{
		s = Owner->BSphere;
		s.Position = pos;
		
	}
	else if (Owner->IsPrefab)
	{
		// If Owner is prefab then use Owner->BSPhere
		s = Owner->BSphere;		
	}
	else
	{
		s = Mesh->BSphere;
		s.Position += pos;			
	}		
	//s = Owner->BSphere;		
	//if (!Cam->SphereInFrustum(s)) return;


	WeBatchItem *BatchItem;

	CSmartList<WeMaterial*>* Materials;

	// if we havent loaded materials for that preset already then load it
	LoadMaterials(Owner);
	Materials = &Mesh->Materials[Owner->MaterialPreset];
	//if (Materials->Empty() || Mesh->GetRefresh())
	//{
	//	int Size;
	//	WeMaterial *tmpMat;

	//	if (!Materials->Empty()) Materials->Clear();
	//	Size = Mesh->MeshAttributes.GetItemsCount();	

	//	Mesh->MaterialNames[Owner->MaterialPreset].First();
	//	for (int i = 0; i < Size; i++)
	//	{	
	//		tmpMat = WeMaterialManager::Instance()->Load(Mesh->MaterialNames[Owner->MaterialPreset].GetItem());						
	//		// if not existing material then use default
	//		if (!tmpMat)
	//		{				
	//			tmpMat = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);							
	//		}
	//		Materials->PushBack(tmpMat);
	//		Mesh->MaterialNames[Owner->MaterialPreset].Next();
	//	}		
	//}


	CSmartList<WeAttribRange > *Attributes;
	Attributes = &Mesh->MeshAttributes;
	int iAttribute = 0;

	for(Attributes->First(), Materials->First();
		!Attributes->End();
		Attributes->Next(), Materials->Next(), ++iAttribute)
	{
		if ((Mesh->OnlyVisibleAttribute < 0) || (Mesh->OnlyVisibleAttribute == iAttribute))
		{
			BatchItem = new WeBatchItem();

			BatchItem->Mesh = Mesh;
			//BatchItem->ShadowMesh = ShadowMesh;
			BatchItem->ShadowMesh = NULL;
			BatchItem->WorldMatrix = TM;
			BatchItem->PrevWorldMatrix = PrevTM;
			BatchItem->AttribRange = Attributes->GetItem();
			BatchItem->Material = Materials->GetItem();
			BatchItem->Model = this;
			BatchItem->Actor = Owner;
			BatchItem->BSphere = s;
			BatchItem->Alpha = false;

			// if we have shader then find nearest lights and also if dynamic lights is turned on
			if (Owner->WantLight) 
			{
				if (Materials->GetItem()->Shader) //&& WeRenderDevice::Instance()->GetLight())
				{		
					World->FindNearestLights(s, BatchItem->Lights, Materials->GetItem()->Shader->WantLightsCount, Materials->GetItem()->Shader->WantDirectLightsCount, !Owner->IsPrefab);							
					// if Shader want sum light color 
					if (Materials->GetItem()->Shader->WantSumLightColor)
						World->FindSumColorLights(pos, BatchItem->SumLightColor);
				}
			}
			else
			{
				BatchItem->SumLightColor.r = BatchItem->SumLightColor.g = BatchItem->SumLightColor.b = 0.0f;
			}


			WeVector tmp;						
			D3DXVec3TransformCoord(&tmp, &pos, &Cam->View);
			BatchItem->ZDist = tmp.z;

			// Calculate ZDistance only if this is translucent material
			if ((Materials->GetItem()->Type == WEMATERIALTYPE_ALPHABLENDED) || (Materials->GetItem()->Type == WEMATERIALTYPE_FOREGROUND))
			{
				BatchItem->Alpha = true;
			}
			// ------------------------ 		
			// Occlusion Query stuff
			// if we have querymesh use it or use normal mesh
			if (!QueryMesh)
				BatchItem->QueryMesh = Mesh;
			else
				BatchItem->QueryMesh = QueryMesh;
			BatchItem->Query = WeQueryManager::Instance()->GetNextQuery();
			// ----

			WeBatchRenderer::Instance()->QueueBatchItem(BatchItem);	
			WeShadowVolumeRenderer::Instance()->QueueBatchItem(BatchItem);
		}
	}
}


bool WeStaticModel::Load(StringParam name, bool ForceReload)
{

	if(WePackage::Instance()->BeginReading(name, 6))
	{		
		int Ver;
		Ver = WePackage::Instance()->DataStream.ReadInt();
		if (Ver == 1)
		{
			MeshName = WePackage::Instance()->DataStream.ReadString();
			QueryMeshName = WePackage::Instance()->DataStream.ReadString();
			WePackage::Instance()->DataStream.ReadString(); // ShadowMeshName

		}
		else
		{
			WePackage::Instance()->DataStream.Seek(-4, SEEK_CUR);			
			MeshName = WePackage::Instance()->DataStream.ReadString();
			QueryMeshName = WePackage::Instance()->DataStream.ReadString();
			//ShadowMeshName = MeshName;
		}
		if (QueryMeshName == (String)"")
		{
			QueryMeshName = MeshName;
		}
		//if (ShadowMeshName == (String)"")
		//{
		//	ShadowMeshName = MeshName;
		//}
	}
	else 
	{		
		return false;
	}
	WePackage::Instance()->EndReading();

	Mesh = WeMeshManager::Instance()->Load(MeshName, false, ForceReload);
	QueryMesh = WeMeshManager::Instance()->Load(QueryMeshName, false, ForceReload);
	//ShadowMesh = WeMeshManager::Instance()->Load(ShadowMeshName, false, ForceReload);

	Name = name;

	if (!Mesh)
	{
		char temp[1000];
		sprintf(temp, "Model %s\nMesh %s", Name.GetString(), MeshName.GetString());
		MessageBox(0, temp, "ERROR", 0);
	}

	return true;
}





bool WeStaticModel::Save(StringParam HoleName, bool Replace)
{
	//if(Replace) WePackage::Instance()->Remove(HoleName);

	if(WePackage::Instance()->BeginWriting(HoleName, 6, true))
	{
		WePackage::Instance()->DataStream.WriteInt(Version);
		WePackage::Instance()->DataStream.WriteString(MeshName);
		WePackage::Instance()->DataStream.WriteString(QueryMeshName);
		WePackage::Instance()->DataStream.WriteString((String)""); // (ShadowMeshName);
	}
	else
		return false;
	WePackage::Instance()->EndWriting();

	return true;
}




String WeStaticModel::GetCurrentMaterialName(WeActor *Owner)
{
	String s;
	if (!Owner) return s;

	if (Mesh) 
	{
		Mesh->MaterialNames[Owner->MaterialPreset].First();
		s = Mesh->MaterialNames[Owner->MaterialPreset].GetItem();
	}
	return s;
}




void WeStaticModel::SetMaterial(StringParam MaterialName, int forMaterialPreset)
{
	if(Mesh)
	{
		Mesh->SetMaterial(MaterialName, forMaterialPreset);
	}
}


