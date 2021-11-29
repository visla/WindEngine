/*
          File: WeMesh.cpp
		  Author: Tomislav Kukic
		  Date: 12.09.2005
		  Version: 1.0
*/



#include <WeMesh.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <WeMaterialManager.h>
#include <WeMaterial.h>
#include <WePackage.h>
#include <WeEngine.h>
#include <WeLightMapper.h>
#include <WeLoadCounter.h>


D3DVERTEXELEMENT9 MeshVertexElements[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0, 
	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	D3DDECL_END()
};

D3DVERTEXELEMENT9 ActualMeshVertexElements[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1, 
	D3DDECL_END()
};

/*D3DVERTEXELEMENT9 ActualMeshVertexElements2[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0, 
	D3DDECL_END()
};*/


D3DVERTEXELEMENT9 NormalMappedOldMeshVertexElements[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0, 
	0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	0, 44, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1, 
	D3DDECL_END()
};


D3DVERTEXELEMENT9 NormalMappedMeshVertexElements[] = 
{
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },  
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },	
	{ 0, 44, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	D3DDECL_END()
};


D3DVERTEXELEMENT9 ShadowMeshVertexElement[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	D3DDECL_END()
};



/*D3DVERTEXELEMENT9 NormalMappedMeshVertexElements2[] = 
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0, 
	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0, 
	0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0, 
	0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0, 
	D3DDECL_END()
};*/



LPDIRECT3DVERTEXDECLARATION9 lpActualVertexDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9 lpNormaMappedVertexDecl = NULL;



WeMesh::WeMesh(void)
{
	HardwareMesh = NULL;	
	ShadowMesh = NULL;
	lpVB = NULL;
	lpIB = NULL;

	VerticesCount = 0;
	FacesCount = 0;
	Dynamic = false;
	Loaded = false;
	HasAttrTable = true;
	Version = 5;
	LightMapSize = (int)LIGHTMAP_SIZE;
	RefreshPresets = false;
	UseNormalMap = true;
	NewUseNormalMap = false;
	Unwrapped = false;
	MaxLODFactor = 0.8f;
	MaxLODDistance = 1000.0f;
	MaxLODLevels = 10;
	HardwareMeshes = NULL;
	
	OnlyVisibleAttribute = -1;
}




WeMesh::~WeMesh(void)
{
	SAFE_RELEASE(HardwareMesh);
	SAFE_RELEASE(ShadowMesh);
	SAFE_RELEASE(lpVB);
	SAFE_RELEASE(lpIB);

	if (HardwareMeshes)
	{
		for (int i = 1; i < MaxLODLevels; ++i)
			SAFE_RELEASE(HardwareMeshes[i]);
	}

	ClearAll();
}




void WeMesh::SetMesh(LPD3DXMESH Mesh, bool CreateAttribs, UINT MaterialNum)
{
	if (!Mesh) return;
	HardwareMesh = Mesh;
	ComputeBoundings();
	if(!CreateAttribs) return;



	D3DXATTRIBUTERANGE *AttrRange;
	DWORD AttrSize;

	HardwareMesh->GetAttributeTable(NULL, &AttrSize);	
	if (AttrSize == 0) HasAttrTable = false;
	AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
	HardwareMesh->GetAttributeTable(AttrRange, &AttrSize);

    

	WeAttribRange tmpAttribs;
	
	
	String tmpTexName = WeEngine::Instance()->Desc.DefaultTextureName;
	String tmpShaderName = WeEngine::Instance()->Desc.DefaultShaderName;
	String tmpDefaultMaterial = WeEngine::Instance()->Desc.DefaultMaterialName;

	//char tmp[200];


	for(int i=0; i < (int)MaterialNum; i++)
	{
		// Fill Attributes
		if (i < (int)AttrSize)
		{
			tmpAttribs.AttribId = AttrRange[i].AttribId;
			tmpAttribs.FaceCount= AttrRange[i].FaceCount;
			tmpAttribs.FaceStart = AttrRange[i].FaceStart;
			tmpAttribs.VertexCount = AttrRange[i].VertexCount;
			tmpAttribs.VertexStart = AttrRange[i].VertexStart;
		}
		tmpAttribs.AttribId = i;
		MeshAttributes.PushBack(tmpAttribs);
	}

	// for all other material names that dont exist fill default material
	for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
	{
		if (MaterialNames[j].GetItemsCount() == 0)
		{
			for (int i = 0; i < (int)AttrSize; i++)
			{
				MaterialNames[j].PushBack(tmpDefaultMaterial);
			}
		}	
	}

	delete[] AttrRange;


	//LPD3DXMESH tmpMesh;
	
	HardwareMesh->GetVertexBuffer(&lpVB);
	HardwareMesh->GetIndexBuffer(&lpIB);
	

	if (GenerateShadowMesh(WeRenderDevice::Instance()->Device, HardwareMesh, &ShadowMesh) != S_OK)
	{		
	}
	if (!ShadowMesh) MessageBox(0, "ShadowMesh not created!", 0, 0);
    
	

	VerticesCount = GetNumVertices();
	FacesCount = GetNumFaces();
}


void WeMesh::SimplifyMesh()
{
	if (!HardwareMesh) return;

	HardwareMeshes = new LPD3DXMESH[MaxLODLevels];
	
	HardwareMeshes[0] = HardwareMesh;
	DWORD Faces = HardwareMesh->GetNumFaces();

	for (int i = 1; i < MaxLODLevels; ++i)
	{
		Faces = (DWORD)((float)Faces * MaxLODFactor);
		if (FAILED(D3DXSimplifyMesh(HardwareMesh, Adjancency, NULL, NULL, Faces, D3DXMESHSIMP_FACE, &HardwareMeshes[i])))
		{
			MessageBox(0, "Error simplifying mesh", 0, 0);		
		}
		//char tmp[100];
		//sprintf(tmp, "Faces for %d, is %d", i, HardwareMeshes[i]->GetNumFaces());
		//MessageBox(0, tmp, 0, 0);
	}
	
	CurrentMesh = HardwareMeshes[0];
}


bool WeMesh::Optimize()
{	
	Adjancency = new DWORD[3 * GetNumFaces()];
	HardwareMesh->GenerateAdjacency(0.01f, Adjancency);
	if (!Unwrapped)
		HardwareMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, Adjancency, NULL, NULL, NULL);    
	return true;
}




void WeMesh::DrawSubset(WeAttribRange* SubAttr, float ZDist)
{
	//if (HasAttrTable)
	//{
	//	WeRenderDevice::Instance()->Device->SetIndices(lpIB);
	//	if (UseNormalMap)
	//	{
	//		WeRenderDevice::Instance()->Device->SetStreamSource(0, lpVB, 0, 52);
	//		WeRenderDevice::Instance()->Device->SetVertexDeclaration(GetNormaMappedVertexDecl());
	//	}
	//	else
	//	{
	//		WeRenderDevice::Instance()->Device->SetStreamSource(0, lpVB, 0, 40);
	//		WeRenderDevice::Instance()->Device->SetVertexDeclaration(GetActualVertexDecl());
	//	}
	//	//WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, SubAttr->FaceStart, SubAttr->VertexCount, SubAttr->VertexStart, SubAttr->FaceCount);
	//	WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, SubAttr->VertexStart, SubAttr->VertexCount, SubAttr->FaceStart*3, SubAttr->FaceCount);
	//	if (FAILED(WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, SubAttr->VertexCount, SubAttr->VertexStart, SubAttr->FaceCount)))
	//	{
	//		printf("FAILED\n");
	//	}
	//} else 
	//{
	//HardwareMesh->DrawSubset(SubAttr->AttribId);
	if (HardwareMeshes)
	{
		if (ZDist >= MaxLODDistance) CurrentMesh = HardwareMeshes[MaxLODLevels-1];
		else if (ZDist < 0) CurrentMesh = HardwareMeshes[0];
		else
		{
			int tmp;
			tmp = (int)(MaxLODDistance / MaxLODLevels);
			tmp = (int)ZDist / tmp;		
			CurrentMesh = HardwareMeshes[tmp];
		}
	} else CurrentMesh = HardwareMesh;
	
	CurrentMesh->DrawSubset(SubAttr->AttribId);
	//}
}



void WeMesh::DrawShadowSubset(WeAttribRange* SubAttr)
{
	//if (HasAttrTable)
	//{
	//	if (!lpActualVertexDecl)
	//	{
	//		WeRenderDevice::Instance()->Device->CreateVertexDeclaration(ActualMeshVertexElements2, &lpActualVertexDecl);
	//		
	//	}
	//	if (!lpNormaMappedVertexDecl)
	//	{
	//		WeRenderDevice::Instance()->Device->CreateVertexDeclaration(NormalMappedMeshVertexElements2, &lpNormaMappedVertexDecl);
	//		
	//	}

	//	WeRenderDevice::Instance()->Device->SetIndices(lpIB);
	//	if (UseNormalMap)
	//	{
	//		WeRenderDevice::Instance()->Device->SetStreamSource(0, lpVB, 0, 44);
	//		WeRenderDevice::Instance()->Device->SetVertexDeclaration(lpNormaMappedVertexDecl);	
	//	} else
	//	{
	//		WeRenderDevice::Instance()->Device->SetStreamSource(0, lpVB, 0, 32);
	//		WeRenderDevice::Instance()->Device->SetVertexDeclaration(lpActualVertexDecl);	
	//	}
	//	//WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, SubAttr->FaceStart, SubAttr->VertexCount, SubAttr->VertexStart, SubAttr->FaceCount);
	//	WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, SubAttr->VertexStart, SubAttr->VertexCount, SubAttr->FaceStart*3, SubAttr->FaceCount);
	//	//if (FAILED(WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, SubAttr->VertexCount, SubAttr->VertexStart, SubAttr->FaceCount)))
	//	//{
	//	//	printf("FAILED\n");
	//	//}
	//} else 
	//{
		ShadowMesh->DrawSubset(SubAttr->AttribId);
	//}
}




void WeMesh::Clone(WeMesh* Mesh)
{
	BoundingBox = Mesh->BoundingBox;
	BSphere.Radius = Mesh->BSphere.Radius;
	FacesCount = Mesh->FacesCount;

	D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
	Mesh->HardwareMesh->GetDeclaration(Declaration);
	LPD3DXMESH tmpMesh = NULL;
	Mesh->HardwareMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_32BIT, Declaration, WeRenderDevice::Instance()->Device, &tmpMesh);

	for(Mesh->MaterialNames->First(); !Mesh->MaterialNames->End(); Mesh->MaterialNames->Next())
		MaterialNames->PushBack(Mesh->MaterialNames->GetItem());

	MeshAttributes = Mesh->MeshAttributes;
	Name = Mesh->Name;
	UseNormalMap = Mesh->UseNormalMap;
	NewUseNormalMap = Mesh->NewUseNormalMap;
	Version = Mesh->Version;
	VerticesCount = Mesh->VerticesCount;

	SetMesh(tmpMesh, true);
	ShadowMesh = Mesh->HardwareMesh;
}




void WeMesh::SetMaterial(StringParam MaterialName, int forMaterialPreset)
{
	if (forMaterialPreset > MAX_MATERIAL_PRESETS2) forMaterialPreset = 0;
	Materials[forMaterialPreset].Clear();
	WeMaterial *tmpM;

	tmpM = WeMaterialManager::Instance()->Load(MaterialName);
	if (tmpM)
	{
		for (MaterialNames[forMaterialPreset].First(); !MaterialNames[forMaterialPreset].End(); MaterialNames[forMaterialPreset].Next())
		{
			Materials[forMaterialPreset].PushBack(tmpM);
			MaterialNames[forMaterialPreset].GetItem() = MaterialName;	
		}
	}
}



void WeMesh::DeOptimize()
{
	int NumFaces;
	int NumVertices;
	LPD3DXMESH tmpMesh;
	CSmartList<WeAttribRange> MeshAttributes2;
	WeAttribRange attr;

	NumFaces = HardwareMesh->GetNumFaces();
	NumVertices = HardwareMesh->GetNumFaces()*3;
	
	if (FAILED(D3DXCreateMesh(NumFaces, NumVertices, D3DXMESH_32BIT | D3DXMESH_MANAGED, NormalMappedMeshVertexElements, WeRenderDevice::Instance()->Device, &tmpMesh)))
	{
		MessageBox(0, "Error deoptimizing mesh", 0, 0);			
		return;
	}

	WeNormalMappedMeshVertex *Data, *Data2;
	DWORD *IndexData, *IndexData2, *AttrData;

	HardwareMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&Data);
	HardwareMesh->LockIndexBuffer(D3DLOCK_READONLY, (void **)&IndexData);
	

	tmpMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void **)&Data2);
	tmpMesh->LockIndexBuffer(D3DLOCK_DISCARD, (void **)&IndexData2);
	tmpMesh->LockAttributeBuffer(D3DLOCK_DISCARD, &AttrData);

	int j = 0;
	int k = 0;
	int attrid = 0;
	for (MeshAttributes.First(); !MeshAttributes.End(); MeshAttributes.Next())
	{
		attr.AttribId = MeshAttributes.GetItem().AttribId;
		attr.FaceStart = j / 3;
		attr.FaceCount = MeshAttributes.GetItem().FaceCount;
		attr.VertexStart = j;
		attr.VertexCount = MeshAttributes.GetItem().FaceCount*3;
		MeshAttributes2.PushBack(attr);

		//int tmp1, tmp2;
		//tmp1 = MeshAttributes.GetItem().FaceStart;
		//tmp2 = MeshAttributes.GetItem().FaceCount;
		//char tmp[100];
		//sprintf(tmp, "atr: %d, FS: %d, FC: %d", attr.AttribId, tmp1, tmp2);
		//MessageBox(0, tmp, 0, 0);

		for (unsigned int i = MeshAttributes.GetItem().FaceStart*3; i < MeshAttributes.GetItem().FaceStart*3 + MeshAttributes.GetItem().FaceCount*3; ++i)
		{
			Data2[j] = Data[IndexData[i]];
			j++;
		}

		// fill attribute buffer for this attribute
		for (unsigned int i = 0; i < attr.FaceCount; ++i)
		{
			AttrData[k] = attr.AttribId;
			k++;
		}		
	}

	// make index buffer and attribute buffer
	for (int i = 0; i < NumFaces*3; ++i)
	{
		IndexData2[i] = i;
	}

	tmpMesh->UnlockIndexBuffer();
	tmpMesh->UnlockVertexBuffer();
	tmpMesh->UnlockAttributeBuffer();
	HardwareMesh->UnlockIndexBuffer();
	HardwareMesh->UnlockVertexBuffer();

	MeshAttributes.Clear();
	D3DXATTRIBUTERANGE *AttrRange;
	AttrRange = new D3DXATTRIBUTERANGE[MeshAttributes2.GetItemsCount()];

	j = 0;
	for (MeshAttributes2.First(); !MeshAttributes2.End(); MeshAttributes2.Next())
	{
		AttrRange[j].AttribId = MeshAttributes2.GetItem().AttribId;
		AttrRange[j].FaceCount = MeshAttributes2.GetItem().FaceCount;
		AttrRange[j].FaceStart = MeshAttributes2.GetItem().FaceStart;
		AttrRange[j].VertexCount = MeshAttributes2.GetItem().VertexCount;
		AttrRange[j].VertexStart= MeshAttributes2.GetItem().VertexStart;
		MeshAttributes.PushBack(MeshAttributes2.GetItem());
		j++;
	}
	
	SAFE_RELEASE(HardwareMesh);
	HardwareMesh = tmpMesh;
	HardwareMesh->SetAttributeTable(AttrRange, MeshAttributes2.GetItemsCount());
	delete[] AttrRange;

}


void WeMesh::UnwrapLightMap()
{
	if (!HardwareMesh) return;

	//WeLoger::DebugLog("Unwrap %s\n", Name);

	int NumVertices;
	int NumFaces;

	NumVertices = HardwareMesh->GetNumVertices();
	NumFaces = HardwareMesh->GetNumFaces();

	//if (GetUseNormalMap())
	//{
		LPD3DXMESH newMesh;
		LPD3DXBUFFER buf, buf2;
		float s;
		UINT p;

		DWORD AttrSize;
		D3DXATTRIBUTERANGE *AttrRange;

		HardwareMesh->GetAttributeTable(NULL, &AttrSize);	
		if (AttrSize == 0) HasAttrTable = false;
		AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
		HardwareMesh->GetAttributeTable(AttrRange, &AttrSize);

		delete[] Adjancency;
		Adjancency = new DWORD[3 * GetNumFaces()];
		HardwareMesh->GenerateAdjacency(0.01f, Adjancency);

		//D3DXWeldVertices(HardwareMesh, D3DXWELDEPSILONS_WELDALL, NULL, NULL, NULL, NULL, NULL);

		HRESULT res = D3DXUVAtlasCreate(HardwareMesh, 0, 1.0f, LightMapSize, LightMapSize, 5, 1, 
			Adjancency, NULL, NULL, NULL, 0, NULL, &newMesh, &buf, &buf2, &s, &p);
		if (FAILED(res))
		{
			char tmp[100];
			sprintf(tmp, "D3DXUVAtlasCreate %s: %.8x", Name.GetString(), res);
			MessageBox(0, tmp, "Error", 0);		
			return;
		}

		SAFE_RELEASE(HardwareMesh);
		HardwareMesh = newMesh;
		HardwareMesh->SetAttributeTable(AttrRange, AttrSize);
		//WeLightMapper::Instance()->MakeUV(this);

		//WeNormalMappedMeshVertex *Vertices, *Orig;
		//DWORD *Faces;

		//Orig = (WeNormalMappedMeshVertex*)malloc(sizeof(WeNormalMappedMeshVertex)*NumVertices);
		//LockVertexBuffer((void **)&Vertices, D3DLOCK_READONLY);
		//memcpy(Orig, Vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
		//UnlockVertexBuffer();

		//

		//LockIndexBuffer((void **)&Faces, D3DLOCK_READONLY);		

		//// Modify normal and make uv`s
		//for (int i = 0; i < NumFaces; i++)
		//{
		//							
		//}		


		//UnlockIndexBuffer();
		//free(Orig);
	//} else
	//{
	//	WeActualMeshVertex *Vertices, *orig;

	//	orig = (WeActualMeshVertex*)malloc(sizeof(WeActualMeshVertex)*NumVertices);
	//	LockVertexBuffer((void **)&Vertices, D3DLOCK_READONLY);
	//	memcpy(orig, Vertices, sizeof(WeActualMeshVertex)*NumVertices);
	//	UnlockVertexBuffer();

	//	LockVertexBuffer((void **)&Vertices, D3DLOCK_DISCARD);
	//	for (int i = 0; i < NumVertices; ++i)
	//	{
	//		Vertices[i] = orig[i];
	//		Vertices[i] = orig[i];
	//		Vertices[i].lu = orig[i].u;
	//		Vertices[i].lv = orig[i].v;
	//	}
	//	UnlockVertexBuffer();
	//	free(orig);
	//}		
}

bool WeMesh::Load(StringParam name, bool MeshIsX)
{
	LPD3DXBUFFER tmpMaterials;
	String tmpMaterialName;
	DWORD numMaterials = 0;
	//WeMaterial *tmpMat;
	Name = name;
	Unwrapped = false;

	char TempName[100];
	sprintf(TempName, "MESH \"%s\"", name.GetString());
	SetLoadingName((String)TempName);

	SAFE_RELEASE(HardwareMesh);
	if (HardwareMeshes)
	{
		for (int i = 0; i < MaxLODLevels; ++i)
			SAFE_RELEASE(HardwareMeshes[i]);
	}

	SAFE_RELEASE(ShadowMesh);
	SAFE_RELEASE(lpVB);
	SAFE_RELEASE(lpIB);
	ClearAll();

	if(!MeshIsX)
	{
		if (WePackage::Instance()->BeginReading(name, 5))
		{
			int v;
			v = WePackage::Instance()->DataStream.ReadInt();
			if (v == 1)
			{
				int fSize = (int)WePackage::Instance()->DataStream.ReadDWord();
				char* MeshData;
				if (fSize > 1000000)
				{
					WePackage::Instance()->EndReading();
					HardwareMesh = NULL;
					ResetLoadingName();
					return false;
				}

				MeshData = new char[fSize];
				WePackage::Instance()->DataStream.ReadData((void*)MeshData, fSize);

				if(D3D_OK != D3DXLoadMeshFromXInMemory((void*)MeshData, fSize, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &numMaterials, &HardwareMesh))
				{
					// Error handler need to be here
					WePackage::Instance()->EndReading();
					HardwareMesh = NULL;
					ResetLoadingName();
					return false;				
				}
				delete[] MeshData;

				String s;
				for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
				{
					int Count = WePackage::Instance()->DataStream.ReadInt();				
					for(int i = 0; i < Count; i++)
					{	
						s = WePackage::Instance()->DataStream.ReadString();					
						MaterialNames[j].PushBack(s);
					}
				}
				WePackage::Instance()->EndReading();
			}

			else if (v == 2)
			{
				UseNormalMap = WePackage::Instance()->DataStream.ReadBool();
				UseNormalMap = true;
				NewUseNormalMap = UseNormalMap;
				int fSize = WePackage::Instance()->DataStream.ReadDWord();
				char* MeshData;
				MeshData = new char[fSize];
				WePackage::Instance()->DataStream.ReadData((void*)MeshData, fSize);

				if(D3D_OK != D3DXLoadMeshFromXInMemory((void*)MeshData, fSize, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &numMaterials, &HardwareMesh))
				{
					// Error handler need to be here
					WePackage::Instance()->EndReading();				
					HardwareMesh = NULL;
					ResetLoadingName();
					return false;				
				}
				delete[] MeshData;

				String s;
				for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
				{
					int Count = WePackage::Instance()->DataStream.ReadInt();
					for(int i = 0; i < Count; i++)
					{	
						s = WePackage::Instance()->DataStream.ReadString();
						MaterialNames[j].PushBack(s);
					}
				}
				WePackage::Instance()->EndReading();
			} // end of version 2

			else if (v == 3)
			{
				LightMapSize = WePackage::Instance()->DataStream.ReadInt();
				UseNormalMap = WePackage::Instance()->DataStream.ReadBool();
				UseNormalMap = true;
				NewUseNormalMap = UseNormalMap;
				int fSize = WePackage::Instance()->DataStream.ReadDWord();
				char* MeshData;
				MeshData = new char[fSize];
				WePackage::Instance()->DataStream.ReadData((void*)MeshData, fSize);

				if(D3D_OK != D3DXLoadMeshFromXInMemory((void*)MeshData, fSize, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &numMaterials, &HardwareMesh))
				{
					// Error handler need to be here
					WePackage::Instance()->EndReading();				
					HardwareMesh = NULL;
					return false;				
				}
				delete[] MeshData;

				String s;
				for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
				{
					int Count = WePackage::Instance()->DataStream.ReadInt();
					for(int i = 0; i < Count; i++)
					{	
						s = WePackage::Instance()->DataStream.ReadString();
						MaterialNames[j].PushBack(s);
					}
				}
				WePackage::Instance()->EndReading();
			} // end of version 3



			else if (v == 4)
			{
				int NumVertices, NumFaces;
				WeFileStream *FS;
				FS = &WePackage::Instance()->DataStream;
				

				NumVertices = FS->ReadInt();
				NumFaces = FS->ReadInt();				
				
				WeNormalMappedMeshVertex *vertices;
				DWORD *indices, *attr;
				vertices = new WeNormalMappedMeshVertex[NumVertices];
				indices = new DWORD[NumFaces*3];
				attr = new DWORD[NumFaces];

				
				FS->ReadData(vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
				FS->ReadData(indices, sizeof(DWORD)*NumFaces*3);
				FS->ReadData(attr, sizeof(DWORD)*NumFaces);

				LightMapSize = WePackage::Instance()->DataStream.ReadInt();
				UseNormalMap = true;

				if (FAILED(D3DXCreateMesh(NumFaces, NumVertices, D3DXMESH_32BIT | D3DXMESH_MANAGED, NormalMappedOldMeshVertexElements, WeRenderDevice::Instance()->Device, &HardwareMesh)))
				{
					WePackage::Instance()->EndReading();				
					HardwareMesh = NULL;
					delete[] vertices;
					delete[] indices;
					delete[] attr;
					return false;
				}

				WeNormalMappedMeshVertex *vertices2;
				DWORD *indices2, *attr2;

				// read mesh data
				LockVertexBuffer((void **)&vertices2, D3DLOCK_DISCARD);
				LockIndexBuffer((void **)&indices2, D3DLOCK_DISCARD);
				LockAttributeBuffer(&attr2, D3DLOCK_DISCARD);

				memcpy(vertices2, vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
				memcpy(indices2, indices, sizeof(DWORD)*NumFaces*3);
				memcpy(attr2, attr, sizeof(DWORD)*NumFaces);				
				
				UnlockVertexBuffer();
				UnlockIndexBuffer();
				UnlockAttributeBuffer();

				// read attribute table
				D3DXATTRIBUTERANGE *AttrRange;
				DWORD AttrSize;
				AttrSize = FS->ReadInt();
				AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
				FS->ReadData(AttrRange, sizeof(D3DXATTRIBUTERANGE)*AttrSize);
				HardwareMesh->SetAttributeTable(AttrRange, AttrSize);
	
				// read material settings
				String s;
				for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
				{
					int Count = WePackage::Instance()->DataStream.ReadInt();
					for(int i = 0; i < Count; i++)
					{	
						s = WePackage::Instance()->DataStream.ReadString();
						MaterialNames[j].PushBack(s);
					}
				}
				WePackage::Instance()->EndReading();

				Unwrapped = false;
			} // end of version 4



			else if (v == 5)
			{
				int NumVertices, NumFaces;
				WeFileStream *FS;
				FS = &WePackage::Instance()->DataStream;


				NumVertices = FS->ReadInt();
				NumFaces = FS->ReadInt();				

				WeNormalMappedMeshVertex *vertices;
				DWORD *indices, *attr;
				vertices = new WeNormalMappedMeshVertex[NumVertices];
				indices = new DWORD[NumFaces*3];
				attr = new DWORD[NumFaces];


				FS->ReadData(vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
				FS->ReadData(indices, sizeof(DWORD)*NumFaces*3);
				FS->ReadData(attr, sizeof(DWORD)*NumFaces);

				LightMapSize = WePackage::Instance()->DataStream.ReadInt();
				UseNormalMap = true;

				if (FAILED(D3DXCreateMesh(NumFaces, NumVertices, D3DXMESH_32BIT | D3DXMESH_MANAGED, NormalMappedMeshVertexElements, WeRenderDevice::Instance()->Device, &HardwareMesh)))
				{
					WePackage::Instance()->EndReading();				
					HardwareMesh = NULL;
					delete[] vertices;
					delete[] indices;
					delete[] attr;
					return false;
				}

				WeNormalMappedMeshVertex *vertices2;
				DWORD *indices2, *attr2;

				// read mesh data
				LockVertexBuffer((void **)&vertices2, D3DLOCK_DISCARD);
				LockIndexBuffer((void **)&indices2, D3DLOCK_DISCARD);
				LockAttributeBuffer(&attr2, D3DLOCK_DISCARD);

				memcpy(vertices2, vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
				memcpy(indices2, indices, sizeof(DWORD)*NumFaces*3);
				memcpy(attr2, attr, sizeof(DWORD)*NumFaces);				

				UnlockVertexBuffer();
				UnlockIndexBuffer();
				UnlockAttributeBuffer();

				// read attribute table
				D3DXATTRIBUTERANGE *AttrRange;
				DWORD AttrSize;
				AttrSize = FS->ReadInt();
				AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
				FS->ReadData(AttrRange, sizeof(D3DXATTRIBUTERANGE)*AttrSize);
				HardwareMesh->SetAttributeTable(AttrRange, AttrSize);

				// read material settings
				String s;
				for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
				{
					int Count = WePackage::Instance()->DataStream.ReadInt();
					for(int i = 0; i < Count; i++)
					{	
						s = WePackage::Instance()->DataStream.ReadString();
						MaterialNames[j].PushBack(s);
					}
				}
				WePackage::Instance()->EndReading();

				Unwrapped = true;
			} // end of version 5

		}
		else
		{ 
			this->HardwareMesh = NULL; 
			ResetLoadingName();
			return false; 
		}	

	}
	else
	{
		if (D3D_OK != D3DXLoadMeshFromX(name, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &numMaterials, &HardwareMesh))
		{
			ResetLoadingName();
			return false;
		}
		HasAttrTable = true;
	}
	

	Optimize();


	D3DXATTRIBUTERANGE *AttrRange;
	DWORD AttrSize;
	HardwareMesh->GetAttributeTable(NULL, &AttrSize);	
	if (AttrSize == 0) HasAttrTable = false;
	AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
	HardwareMesh->GetAttributeTable(AttrRange, &AttrSize);

    
	WeAttribRange tmpAttribs;		
	String tmpTexName = WeEngine::Instance()->Desc.DefaultTextureName;
	String tmpShaderName = WeEngine::Instance()->Desc.DefaultShaderName;
	String tmpDefaultMaterial = WeEngine::Instance()->Desc.DefaultMaterialName;


	int aSize;
	aSize = AttrSize;

	for(int i=0; i<(int)aSize; i++)
	{
		// Fill Attributes
		if (i < (int)AttrSize)
		{
			tmpAttribs.AttribId = AttrRange[i].AttribId;
			tmpAttribs.FaceCount= AttrRange[i].FaceCount;
			tmpAttribs.FaceStart = AttrRange[i].FaceStart;
			tmpAttribs.VertexCount = AttrRange[i].VertexCount;
			tmpAttribs.VertexStart = AttrRange[i].VertexStart;
		} 
		//tmpAttribs.AttribId = i;
		MeshAttributes.PushBack(tmpAttribs);

		// Set up material names if they are loaded from x 
		if(MeshIsX)
		{			
			for (int j = 0; j < MAX_MATERIAL_PRESETS; j++)
			{
				MaterialNames[j].PushBack(tmpDefaultMaterial);
			}
		}
		else
		{
			//for (int j = 0; j < MAX_MATERIAL_PRESETS; j++)
			//{
				//WeMaterial *tmpMat = WeMaterialManager::Instance()->Load(OldMaterialNames[j].GetItem());
				//Materials[j].PushBack(tmpMat);
				//OldMaterialNames[j].Next();	
			//}
		}
	}

	// for all other material names that dont exist fill default material
	for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
	{
		int ii = MaterialNames[j].GetItemsCount();
		for (int i = ii; i < (int)aSize; ++i)
		{
			MaterialNames[j].PushBack(tmpDefaultMaterial);
		}
	}


	delete[] AttrRange;

	ComputeBoundings();

//	 Clone mesh and make it be ActualMeshVertexDeclaration
	LPD3DXMESH tmpMesh;
	//if (!UseNormalMap)
	//{
	//	if (FAILED(HardwareMesh->CloneMesh(D3DXMESH_32BIT | D3DXMESH_MANAGED, ActualMeshVertexElements, WeRenderDevice::Instance()->Device, &tmpMesh)))
	//	{
	//		MessageBox(0, "Error cloning mesh", 0, 0);
	//	}
	//	SAFE_RELEASE(HardwareMesh);
	//	HardwareMesh = tmpMesh;
	//}
	//else
	//{
	

	if (!Unwrapped)
	{
		if (FAILED(HardwareMesh->CloneMesh(D3DXMESH_32BIT | D3DXMESH_MANAGED, NormalMappedMeshVertexElements, WeRenderDevice::Instance()->Device, &tmpMesh)))
		{
			MessageBox(0, "Error cloning mesh for normal mapped", 0, 0);
			ResetLoadingName();
			return false;
		} 


		SAFE_RELEASE(HardwareMesh);
		HardwareMesh = tmpMesh;

// novo roberto dodao //
		DWORD *rgdwAdjacency = NULL;
		rgdwAdjacency = new DWORD[HardwareMesh->GetNumFaces() * 3];

		if( rgdwAdjacency == NULL )
			return FALSE;

		HardwareMesh->ConvertPointRepsToAdjacency(NULL, rgdwAdjacency);

	//	if (FAILED(D3DXComputeTangent(HardwareMesh, 0, 0, 0, 0, Adjancency)))
		
		if (FAILED(	D3DXComputeTangentFrameEx(	HardwareMesh,
												D3DDECLUSAGE_TEXCOORD,
												0,
												D3DDECLUSAGE_TANGENT,
												0,
												D3DX_DEFAULT,
												0,
												D3DX_DEFAULT, //D3DDECLUSAGE_NORMAL,
												0,
												D3DXTANGENT_GENERATE_IN_PLACE, // | D3DXTANGENT_CALCULATE_NORMALS,
												NULL, //rgdwAdjacency,
												-1.01f,
												0.0f,
												-1.01f,
												NULL,
												NULL)
					)
				)
				
		
		{
			MessageBox(0, "Error computing tangent", 0, 0);
			ResetLoadingName();
			return false;
		}
		delete []rgdwAdjacency;
	}

	if (!Unwrapped)	UnwrapLightMap();

	HardwareMesh->GetVertexBuffer(&lpVB);
	HardwareMesh->GetIndexBuffer(&lpIB);

	GenerateShadowMesh(WeRenderDevice::Instance()->Device, HardwareMesh, &ShadowMesh);

	if (!Unwrapped)	DeOptimize();


	HardwareMesh->GetVertexBuffer(&lpVB);
	HardwareMesh->GetIndexBuffer(&lpIB);
	VerticesCount = GetNumVertices();
	FacesCount = GetNumFaces();

			


	//D3DXWeldVertices(HardwareMesh, D3DXWELDEPSILONS_WELDALL, NULL, NULL, NULL, NULL, NULL);
	//SimplifyMesh();
	Loaded = true;

	//MessageBox(0, Name, 0, 0);

	//sprintf(TempName, "+ MESH \"%s\"", name.GetString());
	//SetLoadingName((String)TempName);
	ResetLoadingName();

	delete [] Adjancency;	
	Adjancency = NULL;
	return true;
}




void WeMesh::CreateDynamic(int NumFaces, int NumVertices, WeMaterial *Material, bool DeclarationWithColor_)
{
	SAFE_RELEASE ( HardwareMesh );
	Materials[0].Clear();
	MeshAttributes.Clear();

	Materials[0].PushBack(Material);
	WeAttribRange attr;
	attr.AttribId = 0;
	attr.FaceCount = NumFaces;
	attr.FaceStart = 0;
	attr.VertexCount = NumVertices;
	attr.VertexStart = 0;

	HasAttrTable = true;

	MeshAttributes.PushBack(attr);		
	Dynamic = true;
	DynamicFaces = NumFaces;
	DynamicVertices = NumVertices;
	DynamicMaterial = Material;

	DeclarationWithColor = DeclarationWithColor_;

	if (DeclarationWithColor)
	{
		D3DXCreateMesh(NumFaces, NumVertices, D3DXMESH_32BIT | D3DXMESH_DYNAMIC, MeshVertexElements, WeRenderDevice::Instance()->Device, &HardwareMesh);	
	} else
	{
		D3DXCreateMesh(NumFaces, NumVertices, D3DXMESH_32BIT | D3DXMESH_DYNAMIC, ActualMeshVertexElements, WeRenderDevice::Instance()->Device, &HardwareMesh);	
	}

	HardwareMesh->GetVertexBuffer(&lpVB);
	HardwareMesh->GetIndexBuffer(&lpIB);

}

void WeMesh::ComputeNormals()
{
	if (!HardwareMesh) return;

	D3DXComputeNormals(HardwareMesh, NULL);
}


void WeMesh::ComputeBoundings()
{
	WeVector *Point, Min, Max, Center;
	float radius;


	HardwareMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&Point);
	D3DXComputeBoundingBox(Point, HardwareMesh->GetNumVertices(), HardwareMesh->GetNumBytesPerVertex(), &Min, &Max);
	D3DXComputeBoundingSphere(Point, HardwareMesh->GetNumVertices(), HardwareMesh->GetNumBytesPerVertex(), &Center, &radius);
	HardwareMesh->UnlockVertexBuffer();

	

//	SAFE_DELETE(BoundingBox);
//	SAFE_DELETE(BSphere);
	
	BoundingBox.Min = Min; BoundingBox.Max = Max;
	BSphere.Position = Center; BSphere.Radius = radius;
	//BSphere = new WeSphere(Center, radius);
}


void WeMesh::ClearAll()
{
	MeshAttributes.Clear();
	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		Materials[i].Clear();
		MaterialNames[i].Clear();
	}

//	for(Materials.First(); !Materials.End(); Materials.Next())
//		delete Materials.GetItem();
}




void WeMesh::LockIndexBuffer(void **data, DWORD Flags)
{
	HardwareMesh->LockIndexBuffer(Flags, data);
}

void WeMesh::LockAttributeBuffer(DWORD **data, DWORD Flags)
{
	HardwareMesh->LockAttributeBuffer(Flags, data);
}

void WeMesh::UnlockAttributeBuffer()
{
	HardwareMesh->UnlockAttributeBuffer();
}

void WeMesh::LockVertexBuffer(void **data, DWORD Flags)
{
	HardwareMesh->LockVertexBuffer(Flags, data);
}



void WeMesh::UnlockVertexBuffer()
{
	HardwareMesh->UnlockVertexBuffer();	
}



void WeMesh::UnlockIndexBuffer()
{
	HardwareMesh->UnlockIndexBuffer();
}



int WeMesh::GetNumBytesPerVertex()
{
	return HardwareMesh->GetNumBytesPerVertex();
}




int WeMesh::GetNumBytesPerIndex()
{
	if (!HardwareMesh) return 0;
	if (HardwareMesh->GetOptions() & D3DXMESH_32BIT) return 4;
	else return 2;	
}



int WeMesh::GetNumVertices()
{
	if (!HardwareMesh) return 0;
	return HardwareMesh->GetNumVertices();
}




int WeMesh::GetNumFaces()
{
	if (!HardwareMesh) return 0;
	return HardwareMesh->GetNumFaces();	
}


void WeMesh::OnLostDevice()
{
	if (Dynamic)
		SAFE_RELEASE(HardwareMesh);
}

void WeMesh::OnResetDevice()
{
	if (Dynamic)
		CreateDynamic(DynamicFaces, DynamicVertices, DynamicMaterial, DeclarationWithColor);
}


// Export mesh vertices to OBJ
void WeMesh::ExportVerticesToOBJ(WeTextStream &TS, WeMatrix &TM)
{
	TS.WriteString("# vertices from \"");
	TS.WriteString(Name);
	TS.WriteString("\", number of vertices: ");
	TS.WriteInt(VerticesCount);
	TS.WriteString("\n");

	if (!UseNormalMap)
	{
		WeNormalMappedMeshVertex *Vertices;
		LockVertexBuffer((void**)&Vertices, D3DLOCK_READONLY);

		for (int iVertex = 0; iVertex < VerticesCount; iVertex++)
		{
			D3DXVECTOR4 pos;
			D3DXVec3Transform(&pos, &Vertices[iVertex].Position, &TM);

			TS.WriteString("v ");
			TS.WriteFloat(pos.x);
			TS.WriteString(" ");
			TS.WriteFloat(pos.y);
			TS.WriteString(" ");
			TS.WriteFloat(-pos.z);
			TS.WriteString("\n");
		}

		UnlockVertexBuffer();
	}
	else
	{
		WeNormalMappedMeshVertex *Vertices;
		LockVertexBuffer((void**)&Vertices, D3DLOCK_READONLY);

		for (int iVertex = 0; iVertex < VerticesCount; iVertex++)
		{
			D3DXVECTOR4 pos;
			D3DXVec3Transform(&pos, &Vertices[iVertex].Position, &TM);

			TS.WriteString("v ");
			TS.WriteFloat(pos.x);
			TS.WriteString(" ");
			TS.WriteFloat(pos.y);
			TS.WriteString(" ");
			TS.WriteFloat(-pos.z);
			TS.WriteString("\n");
		}

		UnlockVertexBuffer();
	}
}


String DespaceName(String name)
{
	String Result = "";
	for (int i = 0; i < name.GetLen(); i++)
		Result += (name[i] == ' ' ? '_' : name[i]);
	return Result;
}


// Export mesh faces to OBJ, starting vertex index is FirstVertex
void WeMesh::ExportFacesToOBJ(WeTextStream &TS, int GroupIndex, int FirstVertex)
{
	TS.WriteString("# faces from \"");
	TS.WriteString(Name);
	TS.WriteString("\", number of faces: ");
	TS.WriteInt(FacesCount);
	TS.WriteString("\n");

	TS.WriteString("g ");
	TS.WriteString(DespaceName(Name));
	TS.WriteString("___");
	TS.WriteInt(GroupIndex);
	TS.WriteString("\n");

	DWORD *Indices;
	LockIndexBuffer((void**)&Indices, D3DLOCK_READONLY);

	for (int iFace = 0; iFace < FacesCount; iFace++)
	{
		TS.WriteString("f ");
		TS.WriteDWord(FirstVertex + Indices[iFace * 3 + 0]);
		TS.WriteChar(' ');
		TS.WriteDWord(FirstVertex + Indices[iFace * 3 + 1]);
		TS.WriteChar(' ');
		TS.WriteDWord(FirstVertex + Indices[iFace * 3 + 2]);
		TS.WriteChar('\n');
	}

	UnlockIndexBuffer();
}





struct CEdgeMapping
{
    int m_anOldEdge[2];  // vertex index of the original edge
    int m_aanNewEdge[2][2]; // vertex indexes of the new edge
                            // First subscript = index of the new edge
                            // Second subscript = index of the vertex for the edge

public:
    CEdgeMapping()
    {
        FillMemory( m_anOldEdge, sizeof(m_anOldEdge), -1 );
        FillMemory( m_aanNewEdge, sizeof(m_aanNewEdge), -1 );
    }
};

#define ADJACENCY_EPSILON 0.0001f
#define EXTRUDE_EPSILON 0.1f


struct SHADOWVERT
{
    WeVector Position;
    WeVector Normal;    
};



int FindEdgeInMappingTable( int nV1, int nV2, CEdgeMapping *pMapping, int nCount )
{
    for( int i = 0; i < nCount; ++i )
    {
        // If both vertex indexes of the old edge in mapping entry are -1, then
        // we have searched every valid entry without finding a match.  Return
        // this index as a newly created entry.
        if( ( pMapping[i].m_anOldEdge[0] == -1 && pMapping[i].m_anOldEdge[1] == -1 ) ||

            // Or if we find a match, return the index.
            ( pMapping[i].m_anOldEdge[1] == nV1 && pMapping[i].m_anOldEdge[0] == nV2 ) )
        {
            return i;
        }
    }

    return -1;  // We should never reach this line
}


HRESULT WeMesh::GenerateShadowMesh( IDirect3DDevice9 *pd3dDevice, ID3DXMesh *pMesh, ID3DXMesh **ppOutMesh )
{
    HRESULT hr = S_OK;
    ID3DXMesh *pInputMesh;

    if( !ppOutMesh )
        return E_INVALIDARG;
    *ppOutMesh = NULL;

    // Convert the input mesh to a format same as the output mesh using 32-bit index.
	if (FAILED(HardwareMesh->CloneMesh(D3DXMESH_32BIT | D3DXMESH_MANAGED, ShadowMeshVertexElement, WeRenderDevice::Instance()->Device, &pInputMesh)))
	{
		MessageBox(0, "Error cloning mesh", 0, 0);
	}


    // Generate adjacency information
    DWORD *pdwAdj = new DWORD[3 * pInputMesh->GetNumFaces()];
    DWORD *pdwPtRep = new DWORD[pInputMesh->GetNumVertices()];
    if( !pdwAdj || !pdwPtRep )
    {
        delete[] pdwAdj; delete[] pdwPtRep;
        pInputMesh->Release();
        return E_OUTOFMEMORY;
    }

    hr = pInputMesh->GenerateAdjacency( ADJACENCY_EPSILON, pdwAdj );
    if( FAILED( hr ) )
    {
        delete[] pdwAdj; delete[] pdwPtRep;
        pInputMesh->Release();
        return hr;
    }

    pInputMesh->ConvertAdjacencyToPointReps( pdwAdj, pdwPtRep );
    delete[] pdwAdj;

    SHADOWVERT *pVBData = NULL;
    DWORD *pdwIBData = NULL;

    pInputMesh->LockVertexBuffer( 0, (LPVOID*)&pVBData );
    pInputMesh->LockIndexBuffer( 0, (LPVOID*)&pdwIBData );

    if( pVBData && pdwIBData )
    {
        // Maximum number of unique edges = Number of faces * 3
        DWORD dwNumEdges = pInputMesh->GetNumFaces() * 3;
        CEdgeMapping *pMapping = new CEdgeMapping[dwNumEdges];
        if( pMapping )
        {
            int nNumMaps = 0;  // Number of entries that exist in pMapping

            // Create a new mesh
            ID3DXMesh *pNewMesh;
            hr = D3DXCreateMesh( pInputMesh->GetNumFaces() + dwNumEdges * 2,
                                 pInputMesh->GetNumFaces() * 3,
								 D3DXMESH_32BIT,
								 ShadowMeshVertexElement,
                                 pd3dDevice,
                                 &pNewMesh );
            if( SUCCEEDED( hr ) )
            {
                SHADOWVERT *pNewVBData = NULL;
                DWORD *pdwNewIBData = NULL;

                pNewMesh->LockVertexBuffer( 0, (LPVOID*)&pNewVBData );
                pNewMesh->LockIndexBuffer( 0, (LPVOID*)&pdwNewIBData );

                // nNextIndex is the array index in IB that the next vertex index value
                // will be store at.
                int nNextIndex = 0;

                if( pNewVBData && pdwNewIBData )
                {
                    ZeroMemory( pNewVBData, pNewMesh->GetNumVertices() * pNewMesh->GetNumBytesPerVertex() );
                    ZeroMemory( pdwNewIBData, sizeof(DWORD) * pNewMesh->GetNumFaces() * 3 );

                    // pNextOutVertex is the location to write the next
                    // vertex to.
                    SHADOWVERT *pNextOutVertex = pNewVBData;

                    // Iterate through the faces.  For each face, output new
                    // vertices and face in the new mesh, and write its edges
                    // to the mapping table.

                    for( UINT f = 0; f < pInputMesh->GetNumFaces(); ++f )
                    {
                        // Copy the vertex data for all 3 vertices
                        CopyMemory( pNextOutVertex, pVBData + pdwIBData[f * 3], sizeof(SHADOWVERT) );
                        CopyMemory( pNextOutVertex + 1, pVBData + pdwIBData[f * 3 + 1], sizeof(SHADOWVERT) );
                        CopyMemory( pNextOutVertex + 2, pVBData + pdwIBData[f * 3 + 2], sizeof(SHADOWVERT) );

                        // Write out the face
                        pdwNewIBData[nNextIndex++] = f * 3;
                        pdwNewIBData[nNextIndex++] = f * 3 + 1;
                        pdwNewIBData[nNextIndex++] = f * 3 + 2;

                        // Compute the face normal and assign it to
                        // the normals of the vertices.
                        D3DXVECTOR3 v1, v2;  // v1 and v2 are the edge vectors of the face
                        D3DXVECTOR3 vNormal;
                        v1 = *(D3DXVECTOR3*)(pNextOutVertex + 1) - *(D3DXVECTOR3*)pNextOutVertex;
                        v2 = *(D3DXVECTOR3*)(pNextOutVertex + 2) - *(D3DXVECTOR3*)(pNextOutVertex + 1);
                        D3DXVec3Cross( &vNormal, &v1, &v2 );
                        D3DXVec3Normalize( &vNormal, &vNormal );

                        pNextOutVertex->Normal = vNormal;
                        (pNextOutVertex + 1)->Normal = vNormal;
                        (pNextOutVertex + 2)->Normal = vNormal;

                        pNextOutVertex += 3;

                        // Add the face's edges to the edge mapping table

                        // Edge 1
                        int nIndex;
                        int nVertIndex[3] = { pdwPtRep[pdwIBData[f * 3]],
                                              pdwPtRep[pdwIBData[f * 3 + 1]],
                                              pdwPtRep[pdwIBData[f * 3 + 2]] };
                        nIndex = FindEdgeInMappingTable( nVertIndex[0], nVertIndex[1], pMapping, dwNumEdges );

                        // If error, we are not able to proceed, so abort.
                        if( -1 == nIndex )
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if( pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1 )
                        {
                            // No entry for this edge yet.  Initialize one.
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[0];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[1];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3 + 1;

                            ++nNumMaps;
                        } else
                        {
                            // An entry is found for this edge.  Create
                            // a quad and output it.
                            assert( nNumMaps > 0 );

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3;      // For clarity
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3 + 1;

                            // First triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // Second triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // pMapping[nIndex] is no longer needed. Copy the last map entry
                            // over and decrement the map count.

                            pMapping[nIndex] = pMapping[nNumMaps-1];
                            FillMemory( &pMapping[nNumMaps-1], sizeof( pMapping[nNumMaps-1] ), 0xFF );
                            --nNumMaps;
                        }

                        // Edge 2
                        nIndex = FindEdgeInMappingTable( nVertIndex[1], nVertIndex[2], pMapping, dwNumEdges );

                        // If error, we are not able to proceed, so abort.
                        if( -1 == nIndex )
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if( pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1 )
                        {
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[1];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[2];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3 + 1;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3 + 2;

                            ++nNumMaps;
                        } else
                        {
                            // An entry is found for this edge.  Create
                            // a quad and output it.
                            assert( nNumMaps > 0 );

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3 + 1;
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3 + 2;

                            // First triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // Second triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // pMapping[nIndex] is no longer needed. Copy the last map entry
                            // over and decrement the map count.

                            pMapping[nIndex] = pMapping[nNumMaps-1];
                            FillMemory( &pMapping[nNumMaps-1], sizeof( pMapping[nNumMaps-1] ), 0xFF );
                            --nNumMaps;
                        }

                        // Edge 3
                        nIndex = FindEdgeInMappingTable( nVertIndex[2], nVertIndex[0], pMapping, dwNumEdges );

                        // If error, we are not able to proceed, so abort.
                        if( -1 == nIndex )
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if( pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1 )
                        {
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[2];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[0];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3 + 2;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3;

                            ++nNumMaps;
                        } else
                        {
                            // An entry is found for this edge.  Create
                            // a quad and output it.
                            assert( nNumMaps > 0 );

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3 + 2;
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3;

                            // First triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // Second triangle
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // pMapping[nIndex] is no longer needed. Copy the last map entry
                            // over and decrement the map count.

                            pMapping[nIndex] = pMapping[nNumMaps-1];
                            FillMemory( &pMapping[nNumMaps-1], sizeof( pMapping[nNumMaps-1] ), 0xFF );
                            --nNumMaps;
                        }
                    }

                    // Now the entries in the edge mapping table represent
                    // non-shared edges.  What they mean is that the original
                    // mesh has openings (holes), so we attempt to patch them.
                    // First we need to recreate our mesh with a larger vertex
                    // and index buffers so the patching geometry could fit.


                    // Create a mesh with large enough vertex and
                    // index buffers.

                    SHADOWVERT *pPatchVBData = NULL;
                    DWORD *pdwPatchIBData = NULL;

                    ID3DXMesh *pPatchMesh = NULL;
                    // Make enough room in IB for the face and up to 3 quads for each patching face
                    hr = D3DXCreateMesh( nNextIndex / 3 + nNumMaps * 7,
                                         ( pInputMesh->GetNumFaces() + nNumMaps ) * 3,
                                         D3DXMESH_32BIT,
										 ShadowMeshVertexElement,
                                         pd3dDevice,
                                         &pPatchMesh );

                    if( FAILED( hr ) )
                        goto cleanup;

                    hr = pPatchMesh->LockVertexBuffer( 0, (LPVOID*)&pPatchVBData );
                    if( SUCCEEDED( hr ) )
                        hr = pPatchMesh->LockIndexBuffer( 0, (LPVOID*)&pdwPatchIBData );

                    if( pPatchVBData && pdwPatchIBData )
                    {
                        ZeroMemory( pPatchVBData, sizeof(SHADOWVERT) * ( pInputMesh->GetNumFaces() + nNumMaps ) * 3 );
                        ZeroMemory( pdwPatchIBData, sizeof(DWORD) * ( nNextIndex + 3 * nNumMaps * 7 ) );

                        // Copy the data from one mesh to the other

                        CopyMemory( pPatchVBData, pNewVBData, sizeof(SHADOWVERT) * pInputMesh->GetNumFaces() * 3 );
                        CopyMemory( pdwPatchIBData, pdwNewIBData, sizeof(DWORD) * nNextIndex );
                    } else
                    {
                        // Some serious error is preventing us from locking.
                        // Abort and return error.

                        pPatchMesh->Release();
                        goto cleanup;
                    }

                    // Replace pNewMesh with the updated one.  Then the code
                    // can continue working with the pNewMesh pointer.

                    pNewMesh->UnlockVertexBuffer();
                    pNewMesh->UnlockIndexBuffer();
                    pNewVBData = pPatchVBData;
                    pdwNewIBData = pdwPatchIBData;
                    pNewMesh->Release();
                    pNewMesh = pPatchMesh;

                    // Now, we iterate through the edge mapping table and
                    // for each shared edge, we generate a quad.
                    // For each non-shared edge, we patch the opening
                    // with new faces.

                    // nNextVertex is the index of the next vertex.
                    int nNextVertex = pInputMesh->GetNumFaces() * 3;

                    for( int i = 0; i < nNumMaps; ++i )
                    {
                        if( pMapping[i].m_anOldEdge[0] != -1 &&
                            pMapping[i].m_anOldEdge[1] != -1 )
                        {
                            // If the 2nd new edge indexes is -1,
                            // this edge is a non-shared one.
                            // We patch the opening by creating new
                            // faces.
                            if( pMapping[i].m_aanNewEdge[1][0] == -1 ||  // must have only one new edge
                                pMapping[i].m_aanNewEdge[1][1] == -1 )
                            {
                                // Find another non-shared edge that
                                // shares a vertex with the current edge.
                                for( int i2 = i + 1; i2 < nNumMaps; ++i2 )
                                {
                                    if( pMapping[i2].m_anOldEdge[0] != -1 &&       // must have a valid old edge
                                        pMapping[i2].m_anOldEdge[1] != -1 &&
                                        ( pMapping[i2].m_aanNewEdge[1][0] == -1 || // must have only one new edge
                                        pMapping[i2].m_aanNewEdge[1][1] == -1 ) )
                                    {
                                        int nVertShared = 0;
                                        if( pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1] )
                                            ++nVertShared;
                                        if( pMapping[i2].m_anOldEdge[1] == pMapping[i].m_anOldEdge[0] )
                                            ++nVertShared;

                                        if( 2 == nVertShared )
                                        {
                                            // These are the last two edges of this particular
                                            // opening. Mark this edge as shared so that a degenerate
                                            // quad can be created for it.

                                            pMapping[i2].m_aanNewEdge[1][0] = pMapping[i].m_aanNewEdge[0][0];
                                            pMapping[i2].m_aanNewEdge[1][1] = pMapping[i].m_aanNewEdge[0][1];
                                            break;
                                        }
                                        else
                                        if( 1 == nVertShared )
                                        {
                                            // nBefore and nAfter tell us which edge comes before the other.
                                            int nBefore, nAfter;
                                            if( pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1] )
                                            {
                                                nBefore = i;
                                                nAfter = i2;
                                            } else
                                            {
                                                nBefore = i2;
                                                nAfter = i;
                                            }

                                            // Found such an edge. Now create a face along with two
                                            // degenerate quads from these two edges.

                                            pNewVBData[nNextVertex] = pNewVBData[pMapping[nAfter].m_aanNewEdge[0][1]];
                                            pNewVBData[nNextVertex+1] = pNewVBData[pMapping[nBefore].m_aanNewEdge[0][1]];
                                            pNewVBData[nNextVertex+2] = pNewVBData[pMapping[nBefore].m_aanNewEdge[0][0]];
                                            // Recompute the normal
                                            D3DXVECTOR3 v1 = pNewVBData[nNextVertex+1].Position - pNewVBData[nNextVertex].Position;
                                            D3DXVECTOR3 v2 = pNewVBData[nNextVertex+2].Position - pNewVBData[nNextVertex+1].Position;
                                            D3DXVec3Normalize( &v1, &v1 );
                                            D3DXVec3Normalize( &v2, &v2 );
                                            D3DXVec3Cross( &pNewVBData[nNextVertex].Normal, &v1, &v2 );
                                            pNewVBData[nNextVertex+1].Normal = pNewVBData[nNextVertex+2].Normal = pNewVBData[nNextVertex].Normal;

                                            pdwNewIBData[nNextIndex] = nNextVertex;
                                            pdwNewIBData[nNextIndex+1] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex+2] = nNextVertex + 2;

                                            // 1st quad

                                            pdwNewIBData[nNextIndex+3] = pMapping[nBefore].m_aanNewEdge[0][1];
                                            pdwNewIBData[nNextIndex+4] = pMapping[nBefore].m_aanNewEdge[0][0];
                                            pdwNewIBData[nNextIndex+5] = nNextVertex + 1;

                                            pdwNewIBData[nNextIndex+6] = nNextVertex + 2;
                                            pdwNewIBData[nNextIndex+7] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex+8] = pMapping[nBefore].m_aanNewEdge[0][0];

                                            // 2nd quad

                                            pdwNewIBData[nNextIndex+9] = pMapping[nAfter].m_aanNewEdge[0][1];
                                            pdwNewIBData[nNextIndex+10] = pMapping[nAfter].m_aanNewEdge[0][0];
                                            pdwNewIBData[nNextIndex+11] = nNextVertex;

                                            pdwNewIBData[nNextIndex+12] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex+13] = nNextVertex;
                                            pdwNewIBData[nNextIndex+14] = pMapping[nAfter].m_aanNewEdge[0][0];

                                            // Modify mapping entry i2 to reflect the third edge
                                            // of the newly added face.

                                            if( pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1] )
                                            {
                                                pMapping[i2].m_anOldEdge[0] = pMapping[i].m_anOldEdge[0];
                                            } else
                                            {
                                                pMapping[i2].m_anOldEdge[1] = pMapping[i].m_anOldEdge[1];
                                            }
                                            pMapping[i2].m_aanNewEdge[0][0] = nNextVertex + 2;
                                            pMapping[i2].m_aanNewEdge[0][1] = nNextVertex;

                                            // Update next vertex/index positions

                                            nNextVertex += 3;
                                            nNextIndex += 15;

                                            break;
                                        }
                                    }
                                }
                            } else
                            {
                                // This is a shared edge.  Create the degenerate quad.

                                // First triangle
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][1];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][0];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][0];

                                // Second triangle
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][1];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][0];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][0];
                            }
                        }
                    }
                }

cleanup:;
                if( pNewVBData )
                {
                    pNewMesh->UnlockVertexBuffer();
                    pNewVBData = NULL;
                }
                if( pdwNewIBData )
                {
                    pNewMesh->UnlockIndexBuffer();
                    pdwNewIBData = NULL;
                }

                if( SUCCEEDED( hr ) )
                {
                    // At this time, the output mesh may have an index buffer
                    // bigger than what is actually needed, so we create yet
                    // another mesh with the exact IB size that we need and
                    // output it.  This mesh also uses 16-bit index if
                    // 32-bit is not necessary.


                    bool bNeed32Bit = ( pInputMesh->GetNumFaces() + nNumMaps ) * 3 > 65535;
                    ID3DXMesh *pFinalMesh;
                    hr = D3DXCreateMesh( nNextIndex / 3,  // Exact number of faces
                                         ( pInputMesh->GetNumFaces() + nNumMaps ) * 3,
										 D3DXMESH_MANAGED | ( bNeed32Bit ? D3DXMESH_32BIT : 0 ),
										 ShadowMeshVertexElement,
                                         pd3dDevice,
                                         &pFinalMesh );
                    if( SUCCEEDED( hr ) )
                    {
                        pNewMesh->LockVertexBuffer( 0, (LPVOID*)&pNewVBData );
                        pNewMesh->LockIndexBuffer( 0, (LPVOID*)&pdwNewIBData );

                        SHADOWVERT *pFinalVBData = NULL;
                        WORD *pwFinalIBData = NULL;

                        pFinalMesh->LockVertexBuffer( 0, (LPVOID*)&pFinalVBData );
                        pFinalMesh->LockIndexBuffer( 0, (LPVOID*)&pwFinalIBData );

                        if( pNewVBData && pdwNewIBData && pFinalVBData && pwFinalIBData )
                        {
                            CopyMemory( pFinalVBData, pNewVBData, sizeof(SHADOWVERT) * ( pInputMesh->GetNumFaces() + nNumMaps ) * 3 );

                            if( bNeed32Bit )
                                CopyMemory( pwFinalIBData, pdwNewIBData, sizeof(DWORD) * nNextIndex );
                            else
                            {
                                for( int i = 0; i < nNextIndex; ++i )
                                    pwFinalIBData[i] = (WORD)pdwNewIBData[i];
                            }
                        }

                        if( pNewVBData )
                            pNewMesh->UnlockVertexBuffer();
                        if( pdwNewIBData )
                            pNewMesh->UnlockIndexBuffer();
                        if( pFinalVBData )
                            pFinalMesh->UnlockVertexBuffer();
                        if( pwFinalIBData )
                            pFinalMesh->UnlockIndexBuffer();

                        // Release the old
                        pNewMesh->Release();
                        pNewMesh = pFinalMesh;
                    }

                    *ppOutMesh = pNewMesh;
                }
                else
                    pNewMesh->Release();
            }
            delete[] pMapping;
        } else
            hr = E_OUTOFMEMORY;
    } else
        hr = E_FAIL;

    if( pVBData )
        pInputMesh->UnlockVertexBuffer();

    if( pdwIBData )
        pInputMesh->UnlockIndexBuffer();

    delete[] pdwPtRep;
    pInputMesh->Release();

    return hr;
}


























/*
void WeMesh::Load(StringParam name, bool MeshIsX, int MaterialPreset)
{
	LPD3DXBUFFER tmpMaterials;
	//Strings MaterialNames[MAX_MATERIAL_PRESETS];
	String tmpMaterialName;
	DWORD Size = 0;
	WeMaterial *tmpMat;



	if (MaterialPreset > MAX_MATERIAL_PRESETS) MaterialPreset = 0;
	if (Loaded)
	{
		// Load material for supplied preset.
		Size = MeshAttributes.GetItemsCount();
		Materials[MaterialPreset].Clear();	
		MaterialNames[MaterialPreset].First();

		for (int i = 0; i < Size; i++)
		{			
			tmpMat = WeMaterialManager::Instance()->Load(MaterialNames[MaterialPreset].GetItem());			
			Materials[MaterialPreset].PushBack(tmpMat);
			MaterialNames[MaterialPreset].Next();
		}		
		return;
	}

	if(!MeshIsX)
	{
		if (WePackage::Instance()->BeginReading(name, 5))
		{
			int fSize = WePackage::Instance()->DataStream.ReadDWord();
			char* MeshData;
			MeshData = new char[fSize];
			WePackage::Instance()->DataStream.ReadData((void*)MeshData, fSize);

			if(D3D_OK != D3DXLoadMeshFromXInMemory((void*)MeshData, fSize, D3DXMESH_32BIT | D3DXMESH_VB_MANAGED | D3DXMESH_IB_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &Size, &HardwareMesh))
			{
				// Error handler need to be here
			}
		    delete[] MeshData;
		} else
		{ 
			this->HardwareMesh = NULL; return; 
		}

		for (int j = 0; j < MAX_MATERIAL_PRESETS; j++)
		{
			int Count = WePackage::Instance()->DataStream.ReadInt();
			
			for(int i = 0; i < Count; i++)
			{				
				MaterialNames[j].PushBack(WePackage::Instance()->DataStream.ReadString());
			}
		}
		WePackage::Instance()->EndReading();

	} else
	{
	    if(D3D_OK != D3DXLoadMeshFromX(name, D3DXMESH_32BIT | D3DXMESH_VB_MANAGED | D3DXMESH_IB_MANAGED, WeRenderDevice::Instance()->Device, NULL, &tmpMaterials, NULL, &Size, &HardwareMesh))
		{
			return;
		}
		HasAttrTable = true;
	}
	

	D3DXMATERIAL *tmpMaterial = NULL;
	if (MeshIsX)
	{
		tmpMaterial = (D3DXMATERIAL*)tmpMaterials->GetBufferPointer();
	}

	D3DXATTRIBUTERANGE *AttrRange;
	DWORD AttrSize;

	HardwareMesh->GetAttributeTable(NULL, &AttrSize);
	if (AttrSize == 0) HasAttrTable = false;
	AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
	HardwareMesh->GetAttributeTable(AttrRange, &AttrSize);

    

	WeAttribRange tmpAttribs;
	
	
	String tmpTexName = WeEngine::Instance()->Desc.DefaultTextureName;
	String tmpShaderName = WeEngine::Instance()->Desc.DefaultShaderName;
	String tmpDefaultMaterial = WeEngine::Instance()->Desc.DefaultMaterialName;

	char tmp[200];


	MaterialNames[MaterialPreset].First();	
	for(int i=0; i<Size; i++)
	{

		// Fill Attributes
		if (!Loaded)
		{
			if (i < AttrSize)
			{
				tmpAttribs.AttribId = AttrRange[i].AttribId;
				tmpAttribs.FaceCount= AttrRange[i].FaceCount;
				tmpAttribs.FaceStart = AttrRange[i].FaceStart;
				tmpAttribs.VertexCount = AttrRange[i].VertexCount;
				tmpAttribs.VertexStart = AttrRange[i].VertexStart;
			}
			tmpAttribs.AttribId = i;
			MeshAttributes.PushBack(tmpAttribs);
		}

		// Set up materials
		if(MeshIsX)
		{
			if(tmpMaterial[i].pTextureFilename != NULL)
				tmpTexName = tmpMaterial[i].pTextureFilename;
			else
				tmpTexName = "";
			// Load Material and use it
			tmpMat = WeMaterialManager::Instance()->Load(tmpDefaultMaterial);
			for (int j = 0; j < MAX_MATERIAL_PRESETS; j++)
			{
				Materials[j].PushBack(tmpMat);				
				MaterialNames[j].PushBack(tmpDefaultMaterial);
			}
		}else
		{
			// THIS WILL Load Materials if havent already loaded for this preset
			tmpMat = WeMaterialManager::Instance()->Load(MaterialNames[MaterialPreset].GetItem());
			Materials[MaterialPreset].PushBack(tmpMat);
			MaterialNames[MaterialPreset].Next();	

			// for all other material names that dont exist fill default material
			for (int j = 0; j < MAX_MATERIAL_PRESETS; j++)
			{
				if (MaterialNames[j].GetItemsCount() == 0)
				{
					MaterialNames[j].PushBack(tmpDefaultMaterial);                    
				}
			}
		}
	}

	delete[] AttrRange;

	ComputeBoundings();

//	 Clone mesh and make it be ActualMeshVertexDeclaration
	LPD3DXMESH tmpMesh;
	if (FAILED(HardwareMesh->CloneMesh(D3DXMESH_32BIT | D3DXMESH_MANAGED, ActualMeshVertexElements, WeRenderDevice::Instance()->Device, &tmpMesh)))
	{
		MessageBox(0, "Error", 0, 0);
	}
	HardwareMesh->Release();
	HardwareMesh = tmpMesh;

	HardwareMesh->GetVertexBuffer(&lpVB);
	HardwareMesh->GetIndexBuffer(&lpIB);


	Name = name;
	Loaded = true;
}
*/



LPDIRECT3DVERTEXDECLARATION9 WeMesh::GetActualVertexDecl()
{
	if (!lpActualVertexDecl)
	{
		WeRenderDevice::Instance()->Device->CreateVertexDeclaration(ActualMeshVertexElements, &lpActualVertexDecl);
	}
	return lpActualVertexDecl;
}


LPDIRECT3DVERTEXDECLARATION9 WeMesh::GetNormaMappedVertexDecl()
{
	if (!lpNormaMappedVertexDecl)
	{
		WeRenderDevice::Instance()->Device->CreateVertexDeclaration(NormalMappedMeshVertexElements, &lpNormaMappedVertexDecl);
	}
	return lpNormaMappedVertexDecl;
}

