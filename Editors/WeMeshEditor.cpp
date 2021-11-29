/*

       File: WeMeshEditor.cpp
	   Author: Nebojsa Vislavski
	   Date: 09.11.2005.
	   Version: 1.0
*/


#include <direct.h>
#include <WePackage.h>
#include <WeMeshEditor.h>
#include <WeMaterial.h>




WeMeshEditor::WeMeshEditor()
{
}



WeMeshEditor::~WeMeshEditor()
{
	Destroy();
}



bool WeMeshEditor::Save(StringParam SaveName, bool Replace)
{
	//DWORD Size;
	//char* MeshData;


	//FILE *f = fopen(Name, "rb+");
	//if (f)
	//{
	//	fseek(f,0,SEEK_END);
	//	Size = ftell(f);

	//	fseek(f,0,SEEK_SET);
	//	MeshData = new char[Size];
	//	fread((void*)MeshData, Size, 1, f);
	//	fflush(f);
	//	fclose(f);
	//}
	//else if (WePackage::Instance()->BeginReading(Nam, 5))
	//{
	//	int Ver;
	//	bool b;
	//	Ver = WePackage::Instance()->DataStream.ReadInt(); 
	//	
	//	if (Ver == 2)
	//	{
	//		b = WePackage::Instance()->DataStream.ReadBool();
	//	}
	//	if (Ver == 3)
	//	{
	//		int tmp;
	//		tmp = WePackage::Instance()->DataStream.ReadInt();
	//		b = WePackage::Instance()->DataStream.ReadBool();			
	//	}


	//	Size = WePackage::Instance()->DataStream.ReadDWord();
	//	if (Size >= 1000000)
	//	{
	//		WePackage::Instance()->EndReading();
	//		return false;
	//	}

	//	MeshData = new char[Size];
	//	WePackage::Instance()->DataStream.ReadData(MeshData, Size);
	//	WePackage::Instance()->EndReading();
	//}
	//else
	//{
	//	return false;
	//}	
	UnwrapLightMap();

	if(WePackage::Instance()->BeginWriting(SaveName, 5, true))
	{

		WeNormalMappedMeshVertex *vertices;	
		DWORD *indices;
		DWORD *Attr;

		LockVertexBuffer((void **)&vertices, D3DLOCK_READONLY);
		LockIndexBuffer((void **)&indices, D3DLOCK_READONLY);
		LockAttributeBuffer(&Attr, D3DLOCK_READONLY);

		WeFileStream *FS;
		FS = &WePackage::Instance()->DataStream;

		FS->WriteInt(Version);
		FS->WriteInt(GetNumVertices());
		FS->WriteInt(GetNumFaces());

		//char tmp[100];
		//sprintf(tmp, "Snimam: %d, %d, %d", GetNumVertices(), GetNumFaces(), MeshAttributes.GetItemsCount());
		//MessageBox(0, tmp, 0, 0);

		FS->WriteData(vertices, sizeof(WeNormalMappedMeshVertex)*GetNumVertices());
		FS->WriteData(indices, sizeof(DWORD)*GetNumFaces()*3);
		FS->WriteData(Attr, sizeof(DWORD)*GetNumFaces());	

		UnlockAttributeBuffer();
		UnlockIndexBuffer();
		UnlockVertexBuffer();

		WePackage::Instance()->DataStream.WriteInt(LightMapSize);


		// save attribute table
		D3DXATTRIBUTERANGE *AttrRange;
		DWORD AttrSize;

		HardwareMesh->GetAttributeTable(NULL, &AttrSize);			
		AttrRange = new D3DXATTRIBUTERANGE[AttrSize];
		HardwareMesh->GetAttributeTable(AttrRange, &AttrSize);
		FS->WriteInt(AttrSize);
		FS->WriteData(AttrRange, sizeof(D3DXATTRIBUTERANGE)*AttrSize);		

		// save materials
		for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
		{
			WePackage::Instance()->DataStream.WriteInt(MaterialNames[j].GetItemsCount());			
			for(MaterialNames[j].First(); !MaterialNames[j].End(); MaterialNames[j].Next())
			{
				WePackage::Instance()->DataStream.WriteString(MaterialNames[j].GetItem());				
			}
		}

		WePackage::Instance()->EndWriting();
	}
	else
	{
		return false;
	}

	//delete[] MeshData;

	return true;
}



void WeMeshEditor::Destroy()
{
	SAFE_RELEASE(HardwareMesh);
}


