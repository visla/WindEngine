/*
		File: WeZone.cpp
		Author: Tomislav Kukic
		Date: 05.07.2006
		Version: 1.0
*/



#include <WeZone.h>
#include <WeRenderTree.h>
#include <WeWorld.h>
#include <WePortal.h>
#include <WeStaticModel.h>
#include <WePhysics.h>


WeZone::WeZone()
{
	//SpecialModelName = "MapEditor.Geometry.Box";
	Type = WETYPE_ZONE;
	UID = 0;
	UsedFrame = -1;
	RenderTree = NULL;
	ZoneVer = 1;
	ObjectsCount = 0;
	Box.Min = WeVector(-5, -5, -5);
	Box.Max = WeVector(5, 5, 5);
	WorldBox.Min = Box.Min;
	WorldBox.Max = Box.Max;
	//CanBePrefab = false;
}



WeZone::~WeZone()
{
}


bool WeZone::AddActor(WeActor *actor)
{	
	if (actor->Name == (String)"Test")
	{
			char tmp[1000];
			sprintf(tmp, "WorldBox Min: %3.3f, %3.3f, %3.3f, Max: %3.3f, %3.3f, %3.3f\n \
						 Box: Min: %3.3f, %3.3f, %3.3f, Max: %3.3f, %3.3f, %3.3f\n Name: %s", WorldBox.Min.x, WorldBox.Min.y, WorldBox.Min.z,
						 WorldBox.Max.x, WorldBox.Max.y, WorldBox.Max.z, 
						 actor->BBox.Min.x, actor->BBox.Min.y, actor->BBox.Min.z,
						 actor->BBox.Max.x, actor->BBox.Max.y, actor->BBox.Max.z, actor->Name.GetString());
			MessageBox(0, tmp, 0, 0);		
	}


	if (WorldBox.Intersect(&actor->BBox))
	{
		actor->CurrentZone = this;
		if (actor->IsPrefab)
		{
			Actors.PushBack(actor);
		}
		ObjectsCount++;
		return true;
	} else
	{
		return false;
	}
}

void WeZone::Initialize()
{
	if (InEditor)
	{
		WeActor::Initialize();

		D3DXMatrixIdentity(&TM);

		TM._11 = 1.0f;
		TM._22 = 1.0f;
		TM._33 = 1.0f;
		TM._41 = Location.x;
		TM._42 = Location.y;
		TM._43 = Location.z;
		Model->ComputeBoundingBox(this);
		// make this sclaing be 10m
		OrigScale.x = 10.0f / (BBox.Max.x - BBox.Min.x);
		OrigScale.y = 10.0f / (BBox.Max.y - BBox.Min.y);
		OrigScale.z = 10.0f / (BBox.Max.z - BBox.Min.z);

		SAFE_DELETE(PhysicHandle);
		WeVector v;
		v = (Box.Max - Box.Min) / 2.0f;
		PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, v, 10, 10, false, 0, false);		
		PhysicHandle->SetKinematic(true);

	} else
	{
		Renderable = false;        
	}
	WorldBox.Min = Box.Min + Location;
	WorldBox.Max = Box.Max + Location;

}





void WeZone::Clone(WeActor* Destination, bool DeepCopy)
{
	WeActor::Clone(Destination, DeepCopy);
    
	WeZone *tmpD;
	tmpD = (WeZone *)Destination;
	tmpD->Box = Box;	
}





void WeZone::Tick()
{
	if (InEditor)
	{
		Scale.x = OrigScale.x * ((Box.Max.x - Box.Min.x) / 10.0f);
		Scale.y = OrigScale.y * ((Box.Max.y - Box.Min.y) / 10.0f);
		Scale.z = OrigScale.z * ((Box.Max.z - Box.Min.z) / 10.0f);

		if (PrevScale != Scale)
		{
			// reinitialize object
			SAFE_DELETE(PhysicHandle);
			WeVector v;
			v = (Box.Max - Box.Min) / 2.0f;
			PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, v, 10, 10, false, 0, false);		
			PhysicHandle->SetKinematic(true);
			Model->ComputeBoundingBox(this);
		}
		PrevScale = Scale;
		WeActor::Tick();

		// Reset rotation 
		D3DXMatrixIdentity(&TM);		
		TM._11 = Scale.x;
		TM._22 = Scale.y;
		TM._33 = Scale.z;
		TM._41 = Location.x;
		TM._42 = Location.y;
		TM._43 = Location.z;		
		TMWithScale = TM;
		if (Model)	Model->UpdateTransformations(TM, PrevTM);
		WorldBox.Min = Box.Min + Location;
		WorldBox.Max = Box.Max + Location;

	}

	Frustums.Clear();
}


void WeZone::MakeRenderTree()
{
	Actors.Clear(); // erase all actors currently in list.

	SAFE_DELETE(RenderTree);
	RenderTree = new WeRenderTree();
	WeBox box;
	box.Min = WorldBox.Min*4;
	box.Max = WorldBox.Max*4;
	RenderTree->Initialize(World, 10, box);
}




void WeZone::Serialize(WeFileStream &FS)
{
	WeActor::Serialize(FS);

	FS.WriteInt(ZoneVer);
	FS.WriteInt(UID);
	FS.WriteVector(Box.Min);
	FS.WriteVector(Box.Max);	
}


void WeZone::Deserialize(WeFileStream &FS)
{
	WeActor::Deserialize(FS);

	int v;
	v = FS.ReadInt();
	if (v == 1)
	{
		UID = FS.ReadInt();
		FS.ReadVector(Box.Min);
		FS.ReadVector(Box.Max);		
		
	} // end loading zone ver 1
}


bool WeZone::IntersectFrustum(WeFrustum *frustum)
{
	WeSphere s;
	s = BSphere;
	s.Position = Location;					
	return frustum->Collide(s);
}