/*
    WePhysicTriangleMesh.cpp

	Autor: Nebojsa Vislavski
	Date: 8.5.2006.
	Version: 1.0



*/

#include <WePhysicTriangleMesh.h>
#include <WeStaticModel.h>
#include <WeMesh.h>
#include <NxCooking.h>
#include <Stream.h>
#include <WePhysics.h>
#include <NxuPhysicsImport.h>

bool WePhysicTriangleMesh::MakeTriangleMesh(WeStaticModel *CollisionModel)
{
	if (!CollisionModel) 
		return false;

	ModelName = CollisionModel->Name;

	
	NxTriangleMeshDesc meshDesc;

	void *points;
	void *indices;

	CollisionModel->Mesh->LockVertexBuffer(&points, D3DLOCK_READONLY);
	CollisionModel->Mesh->LockIndexBuffer(&indices, D3DLOCK_READONLY);



	meshDesc.flags = NX_MF_HARDWARE_MESH;//  0; //NX_CF_FLIPNORMALS;
	meshDesc.points = points;
	meshDesc.pointStrideBytes = CollisionModel->Mesh->GetNumBytesPerVertex();
	meshDesc.triangles = indices;
	meshDesc.triangleStrideBytes = 3*CollisionModel->Mesh->GetNumBytesPerIndex();
	meshDesc.numTriangles = CollisionModel->Mesh->GetNumFaces();
	meshDesc.numVertices = CollisionModel->Mesh->GetNumVertices();

	// init cooking
	NxInitCooking();	
		
	NxCookingParams cookparams;
	NxSetCookingParams(cookparams);
	cookparams.skinWidth = 0.3f;

    MemoryWriteBuffer writeBuffer;	
	int status;
 	status = NxCookTriangleMesh( meshDesc, writeBuffer );
    if ( status	)
	{
		ShapeDesc.meshData = WePhysics::Instance()->GetSDK()->createTriangleMesh( MemoryReadBuffer(writeBuffer.data)	);
    }
	else
	{
	    CollisionModel->Mesh->UnlockIndexBuffer();
		CollisionModel->Mesh->UnlockVertexBuffer();
		return false;
	}


 //   bool status = NxCookTriangleMesh(meshDesc, UserStream("c:\\tmp.bin", false));
	//if (status)
	//{
	//	ShapeDesc.meshData = WePhysics::Instance()->GetSDK()->createTriangleMesh(UserStream("c:\\tmp.bin", true));
	//} else MessageBox(0, "PUko status", 0, 0);

	//// if we have regular mesh data
	//if (!ShapeDesc.meshData) 
	//{		
	//	CollisionModel->Mesh->UnlockIndexBuffer();
	//	CollisionModel->Mesh->UnlockVertexBuffer();
	//	MessageBox(0, "PUKO", 0, 0);
	//	return;
	//}

	CollisionModel->Mesh->UnlockIndexBuffer();
	CollisionModel->Mesh->UnlockVertexBuffer();

	Desc.shapes.pushBack(&ShapeDesc);	

	NxCloseCooking();
	return true;
}