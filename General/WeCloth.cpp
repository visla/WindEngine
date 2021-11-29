/*
    WeCloth.cpp

	Autor: Nebojsa vislavski
	Date: 30.11.2005.
	Version: 1.0
	Platform: Win32/Linux

    
*/


#include <WeCloth.h>
#include <WeCamera.h>
#include <WeBatchItem.h>
#include <WeBatchRenderer.h>
#include <WeMaterial.h>
#include <WeModelManager.h>
#include <WePhysics.h>
#include <WeClothHandle.h>
#include <WeTimer.h>
#include <NxPhysics.h>
#include <WeMaterialManager.h>
#include <WeEngine.h>

WeCloth::WeCloth() 
{
	IsCloth = true;
	Type = WETYPE_CLOTH;
	NumVertices = 0;
	NumIndices = 0;
	NumParentIndices = 0;
	ClothMesh = NULL;
	Vertices = NULL;
	Indices = NULL;
	ParentIndices = NULL;
	LocalVersion = 1;

	NxClothDesc cd;
	cd.setToDefault();

	BendingStiffness = cd.bendingStiffness;
	StretchingStiffness =  cd.stretchingStiffness;
	DampingCoef = cd.dampingCoefficient;
	Friction = cd.friction;
	Density = cd.density;
	TearFactor = cd.tearFactor;
	Pressure = cd.pressure;
	Thickness = cd.thickness;
	AttachmentResponseCoef = cd.attachmentResponseCoefficient;
	CollisionResponseCoef = cd.collisionResponseCoefficient;
	Group = cd.collisionGroup;
	AutoAttachToColliding = true;
	Tearable = false;
	Gravity = true;
	Simulating = true;
	OnlyHardware = false;
}


WeCloth::~WeCloth()
{	
	SAFE_DELETE(ClothMesh);
}

void WeCloth::Initialize()
{	
	if (Model) SAFE_DELETE(Model);
	if (PhysicHandle) SAFE_DELETE(PhysicHandle);

	Model = WeModelManager::Instance()->Load(ModelName, false);		

    if (InEditor)
	{
		//obican triangle mesh
		if (Model) 
		{					
			Model->ComputeBoundingBox(this);
			Model->Mesh->Materials[0].Clear();
			WeMaterial *m;
			m = WeMaterialManager::Instance()->Load("Editor.Materials.DefaultCloth");
			if (!m)
				m = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);
			Model->Mesh->Materials[0].PushBack(m);
			//PhysicHandle = WePhysics::Instance()->CreateTriangleMesh(this, Location, Model, 10, WE_PHYSICHANDLE_KINEMATIC);		        			

			WeVector dim;
			dim.x = BBox.Max.x - BBox.Min.x;
			dim.y = BBox.Max.y - BBox.Min.y;
			dim.z = BBox.Max.z - BBox.Min.z;

			if (dim.x == 0.0f) dim.x = 0.1f;
			if (dim.y == 0.0f) dim.y = 0.1f;
			if (dim.z == 0.0f) dim.z = 0.1f;
			PhysicHandle = WePhysics::Instance()->CreateBox(this, TM, dim, 0, 100, false);
			//PhysicHandle = WePhysics::Instance()->CreateSphere(this, Location, BSphere.Radius, 0, 100, false);
			PhysicHandle->SetKinematic(true);
		}
		
	} else
	{
		if ((OnlyHardware && WePhysics::Instance()->IsHardwareAvailable()) || OnlyHardware == false)
		{
			if (Model)
			{
				Model->ComputeBoundingBox(this);

				delete[] Vertices;
				delete[] Indices;
				delete[] ParentIndices;

				Vertices = new WeNormalMappedMeshVertex[Model->Mesh->GetNumVertices()*3*5];
				Indices = new DWORD[Model->Mesh->GetNumFaces()*3*3*5];
				ParentIndices = new DWORD[Model->Mesh->GetNumFaces()*3*3*5];
	            
				// Make simple receive arrays for cloth
				PhysicHandle = WePhysics::Instance()->CreateCloth(this);
				//((WeClothHandle *)PhysicHandle)->SetStatic(false);				
				Simulating = false;
				
			}
		} else
		{
			Renderable = false;
		}
	}
}



void WeCloth::Clone(WeActor *Destination, bool DeepCopy)
{
	WeActor::Clone(Destination, DeepCopy);

	WeCloth *c;
	c = (WeCloth*)Destination;

	c->BendingStiffness = BendingStiffness;
	c->LowModelName = LowModelName;
	c->MediumModelName = MediumModelName;
	c->HighModelName = HighModelName;
	c->StretchingStiffness = StretchingStiffness;
	c->DampingCoef = DampingCoef;
	c->Friction = Friction;
	c->Pressure = Pressure;
	c->TearFactor = TearFactor;
	c->Thickness = Thickness;
	c->AttachmentResponseCoef = AttachmentResponseCoef;
	c->CollisionResponseCoef = CollisionResponseCoef;
	c->Group = Group;
	c->AutoAttachToColliding = AutoAttachToColliding;
	c->Tearable = Tearable;
	c->Density = Density;
	c->Gravity = Gravity;
	c->OnlyHardware = OnlyHardware;
}


void WeCloth::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Serialize(FS);

	FS.WriteInt(LocalVersion);
	FS.WriteString(LowModelName);
	FS.WriteString(MediumModelName);
	FS.WriteString(HighModelName);

	FS.WriteFloat(BendingStiffness);
	FS.WriteFloat(StretchingStiffness);
	FS.WriteFloat(DampingCoef);
	FS.WriteFloat(Friction);
	FS.WriteFloat(Pressure);
	FS.WriteFloat(TearFactor);
	FS.WriteFloat(Thickness);
	FS.WriteFloat(AttachmentResponseCoef);
	FS.WriteFloat(CollisionResponseCoef);
	FS.WriteInt(Group);
	FS.WriteFloat(Density);
	FS.WriteBool(AutoAttachToColliding);
	FS.WriteBool(Tearable);
	FS.WriteBool(Gravity);
	FS.WriteBool(OnlyHardware);


}

void WeCloth::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Deserialize(FS);

	int lv;
	lv = FS.ReadInt();
	if (lv == 1)
	{
		LowModelName = FS.ReadString();
		MediumModelName = FS.ReadString();
		HighModelName = FS.ReadString();

		BendingStiffness = FS.ReadFloat();
		StretchingStiffness = FS.ReadFloat();
		DampingCoef = FS.ReadFloat();
		Friction = FS.ReadFloat();
		Pressure = FS.ReadFloat();
		TearFactor = FS.ReadFloat();
		Thickness = FS.ReadFloat();
		AttachmentResponseCoef = FS.ReadFloat();
		CollisionResponseCoef = FS.ReadFloat();
		Group = FS.ReadInt();
		Density = FS.ReadFloat();
		AutoAttachToColliding = FS.ReadBool();
		Tearable = FS.ReadBool();
		Gravity = FS.ReadBool();
		OnlyHardware = FS.ReadBool();
	}
}

void WeCloth::Tick()
{
	
	if (InEditor) WeActor::Tick();
	else
	{
		if ((OnlyHardware && WePhysics::Instance()->IsHardwareAvailable()) || OnlyHardware == false)
		{
			PrevTM = TM;
			ObjectAge += WeTimer::Instance()->GetDeltaTimeF();

			// suppose this actor is occluded. If it is not BatchRenderer will reset this to 0
			OccludedTime += WeTimer::Instance()->GetDeltaTimeF(); 
			if (LifeTime != 0.0f)
			{
				LifeTime -= WeTimer::Instance()->GetDeltaTimeF();
				if (LifeTime <= 0.0f)
				{
					delete this;
					return;
				}
			}
			Location = Vertices[0].Position;
			D3DXMatrixIdentity(&TM);
			TM._41 = Location.x;
			TM._42 = Location.y;
			TM._43 = Location.z;
			Model->UpdateTransformations(TM, PrevTM);
		}
	}
}

void WeCloth::OnRender(WeCamera *Cam)
{
	//if ((OnlyHardware && WePhysics::Instance()->IsHardwareAvailable()) || OnlyHardware == false)
	//{
	//	if (OccludedTime > 3.0f)
	//	{
	//		if (!InEditor && Simulating) 
	//		{
	//			((WeClothHandle *)PhysicHandle)->SetStatic(true);
	//			Simulating = false;
	//		}
	//	} else
	//	{
	//		if (!InEditor && !Simulating) 
	//		{
	//			((WeClothHandle *)PhysicHandle)->SetStatic(false);
	//			Simulating = true;
	//		}
	//	}
	//}


	if ((OnlyHardware && WePhysics::Instance()->IsHardwareAvailable()) || OnlyHardware == false)
	{
		if (!InEditor)
		{
			void *index;
			void *points;
			ClothMesh->LockIndexBuffer(&index, D3DLOCK_DISCARD);
			ClothMesh->LockVertexBuffer(&points, D3DLOCK_DISCARD);

			

			for (int i = 0; i < (int)NumParentIndices; i++)
			{
				Vertices[i].u = Vertices[ParentIndices[i]].u;
				Vertices[i].v = Vertices[ParentIndices[i]].v;			
			}

			memcpy(index, Indices, NumIndices*sizeof(DWORD)*3);
			memcpy(points, Vertices, NumVertices*sizeof(WeNormalMappedMeshVertex));


			ClothMesh->UnlockIndexBuffer();
			ClothMesh->UnlockVertexBuffer();

			ClothMesh->MeshAttributes.First();
			ClothMesh->MeshAttributes.GetItem().FaceCount = NumIndices;
			ClothMesh->MeshAttributes.GetItem().VertexCount = NumVertices;
			ClothMesh->MeshAttributes.GetItem().FaceStart = 0;
			ClothMesh->MeshAttributes.GetItem().VertexStart = 0;
		}  
	}
	//if (InEditor)
	WeActor::OnRender(Cam);
}
