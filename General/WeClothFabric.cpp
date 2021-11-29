/*
    WeClothFabric.cpp

	Autor: Nebojsa Vislavski
	Date: 30.11.2005.
	Version: 1.0


	This is cloth fabric class used to create clothes.


*/


#include <WeClothFabric.h>
#include <WeCloth.h>
#include <WeMeshManager.h>
#include <WeWorld.h>
#include <WeModelManager.h>

WeClothFabric *WeClothFabric::_instance = NULL;

WeClothFabric *WeClothFabric::Instance()
{
	if (_instance) return _instance;
	_instance = new WeClothFabric;
	return _instance;
}



WeCloth *WeClothFabric::CreateCloth(WeWorld *WorldToBeIn, WeVector &Location, WeMaterial *clothMaterial, 
									int Width, int Height, float TextureTileW, float TextureTileH,
									int VerticesW, int VerticesH)
{
   //WeCloth *Cloth;


   //Cloth = new WeCloth;
   //int numFaces = (VerticesW-1)*2 * (VerticesH-1);
   //int numVertices = (VerticesW * VerticesH);


   //Cloth->Model = WeModelManager::Instance()->Create();
   //Cloth->Model->Mesh = WeMeshManager::Instance()->CreateDynamic(numFaces, numVertices, clothMaterial);
   ////Cloth->Model->QueryMesh = Cloth->Model->Mesh;

   //   
   //
   //float PatchSizeX = (float)Width /  (float)(VerticesW-1);
   //float PatchSizeY = (float)Height / (float)(VerticesH-1);
   //int PatchesX = VerticesW-1;
   //int PatchesY = VerticesH-1;
   //
   //float  StartX = Location.x - ((float)Width/2.0f);
   //float  StartY = Location.y + ((float)Height/2.0f);
   //float  Z = 0;

   //WeMeshVertex *Data;
   //DWORD *Index;
   //Cloth->Model->Mesh->LockVertexBuffer((void **)&Data, 0);
   //Cloth->Model->Mesh->LockIndexBuffer((void **)&Index, 0);

   //for (int j = 0; j < VerticesH;  j++)
   //{
	  // for (int i = 0; i < VerticesW; i++)
	  // {
		 //  Data[j*VerticesW + i].Position.x = StartX + (i*PatchSizeX);
		 //  Data[j*VerticesW + i].Position.y = StartY - (j*PatchSizeY);
		 //  Data[j*VerticesW + i].Position.z = Location.z;

		 //  Data[j*VerticesW + i].u = (TextureTileW / (float)(VerticesW-1))*(float)(i);
		 //  Data[j*VerticesW + i].v = (TextureTileH / (float)(VerticesH-1))*(float)(j);
		 //  Data[j*VerticesW + i].Color = D3DCOLOR_XRGB(0,255,255);           
	  // }
   //}

   //int k = 0;
   //for (int j = 0; j < VerticesH-1; j++)
   //{
	  // for (int i = 0; i < VerticesW-1; i++)
	  // {
		 //  Index[k] = (j*VerticesW) + i;
		 //  Index[k+1] = (j*VerticesW) + i + 1;
		 //  Index[k+2] = ((j+1)*VerticesW) + i;
	
		 //  Index[k+3] = ((j+1)*VerticesW) + i;
		 //  Index[k+4] = (j*VerticesW) + i + 1;
		 //  Index[k+5] = ((j+1)*VerticesW) + i+1;
		 //  k+=6;
	  // }
   //}

   //Cloth->Model->Mesh->UnlockIndexBuffer();
   //Cloth->Model->Mesh->UnlockVertexBuffer();
   //Cloth->Model->Mesh->ComputeNormals();
   //

   //Cloth->Model->Mesh->BSphere.Position = Location;
   //Cloth->Model->Mesh->BSphere.Radius = sqrtf((float)Width*(float)Width + (float)Height*(float)Height);

   //Cloth->Location = Location;
   //Cloth->VerticesW = VerticesW;
   //Cloth->VerticesH = VerticesH;
   //Cloth->TextureTileX = TextureTileW;
   //Cloth->TextureTileY = TextureTileH;
   //Cloth->Width = Width;
   //Cloth->Height = Height;


   //Cloth->BSphere = Cloth->Model->Mesh->BSphere;  
   //Cloth->ClothMaterial = clothMaterial;
   //WorldToBeIn->AddActor(Cloth);

   //// We have settled mesh now. We Only have to make pathes and joints in physics

   return NULL;	
}

