/*
 
    WePhysicClothManager.cpp

	Autor: Nebojsa Vislavski
	Date: 23.6.2006.
	Version: 1.0

*/

#include <WePhysicClothMesh.h>
#include <WeMesh.h>
#include <Stream.h>
#include <NxCooking.h>
#include <nxuPhysicsImport.h>
#include <WePhysics.h>


void WePhysicClothMesh::MakeClothMesh(WeStaticModel *Model)
{
	//if (!Model) return;

	//ModelName = Model->Name;

	//NxClothMeshDesc cDesc;

	////WeActualMeshVertex *finpoints;
	//WeNormalMappedMeshVertex *points;
	//DWORD *indices;
	////int  numPoints;

	//

	//Model->Mesh->LockVertexBuffer((void **)&points, D3DLOCK_READONLY);
	//Model->Mesh->LockIndexBuffer((void **)&indices, D3DLOCK_READONLY);

	//cDesc.target = NX_CLOTH_MESH_SOFTWARE;
	//cDesc.numVertices = Model->Mesh->GetNumVertices();
	//cDesc.numTriangles = Model->Mesh->GetNumFaces();
	//cDesc.pointStrideBytes = Model->Mesh->GetNumBytesPerVertex();
	//cDesc.triangleStrideBytes = 3*Model->Mesh->GetNumBytesPerIndex();	
	//cDesc.points = points;
	//cDesc.triangles = indices;
	//cDesc.flags = 0;
	//if (!cDesc.isValid()) return;

	//FILE *f = fopen("nesto.txt", "wt+");
	//for (int i = 0; i < cDesc.numVertices; i++)
	//{
	//	fprintf(f, "%3.3f, %3.3f, %3.3f\n", points[i].Position.x, points[i].Position.y, points[i].Position.z);
	//}
	//fclose(f);

	////char tmp[100];
	////sprintf(tmp, "Num V: %d BPV: %d", Model->Mesh->GetNumVertices(), Model->Mesh->GetNumBytesPerVertex());
	////MessageBox(0, tmp, 0, 0);
	//
 //   

	//// init cooking
	//NxInitCooking();	

	//MemoryWriteBuffer wb;
	//if (!NxCookClothMesh(cDesc, wb))
	//{
	//	return;
	//}
	//	
	//
	//MemoryReadBuffer rb(wb.data);	
	//ClothMesh = WePhysics::Instance()->GetSDK()->createClothMesh( rb);



	////Desc.shapes.pushBack(&ShapeDesc);	
	//Model->Mesh->UnlockIndexBuffer();
	//Model->Mesh->UnlockVertexBuffer();

	//NxCloseCooking();

    int w = 100;
	int h = 100;
	int d = 10;

	/*    ** Generate a uniform cloth patch, w and h are the width and height, d is the distance between vertices.    */  
	NxClothMeshDesc desc;   
	int numX = (int)(w / d) + 1;   
	int numY = (int)(h / d) + 1;    
	desc.numVertices  = (numX+1) * (numY+1);   
	desc.numTriangles = numX*numY*2;    
	desc.pointStrideBytes = sizeof(NxVec3);    
	desc.triangleStrideBytes = 3*sizeof(NxU32);    
	desc.points = (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);    
	desc.triangles = (NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);    
	desc.flags = 0;    
	
	int i,j;    
	NxVec3 *p = (NxVec3*)desc.points;    
	
	for (i = 0; i <= numY; i++) {        
		for (j = 0; j <= numX; j++) {            
			p->set(d*j, 0.0f, d*i);            
			p++;      
		}    
	}   
	
	NxU32 *id = (NxU32*)desc.triangles;   
	for (i = 0; i < numY; i++) 
	{       
		for (j = 0; j < numX; j++) 
		{           
			NxU32 i0 = i * (numX+1) + j;  
			NxU32 i1 = i0 + 1; 
			NxU32 i2 = i0 + (numX+1);    
			NxU32 i3 = i2 + 1;        

			if ((j+i)%2) 
			{              
				*id++ = i0; *id++ = i2; *id++ = i1;     
				*id++ = i1; *id++ = i2; *id++ = i3;   
			}      
			else 
			{           
				*id++ = i0; *id++ = i2; *id++ = i3; 
				*id++ = i0; *id++ = i3; *id++ = i1;  
			}      
		}    
	}

	
	 //...    
	 //Specify that the cooked mesh will not be simulated on the PPU, only in software.   
	 //The software only version is smaller and takes less time to cook.  
	 
	 desc.target = NX_CLOTH_MESH_SOFTWARE;    
	 //Cook the mesh on the fly through a memory stream.   
	 //A file stream could also be used to pre-cook the mesh.  
	 MemoryWriteBuffer wb;    
	 if (!NxCookClothMesh(desc, wb))   
		 return;   	 
	 
	 MemoryReadBuffer rb(wb.data);   
 	 ClothMesh = WePhysics::Instance()->GetSDK()->createClothMesh(rb);

}