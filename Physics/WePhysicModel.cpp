/*
    WePhysicModel.cpp


	Autor: Nebojsa Vislavski
	Date: 5.12.2005.
	Version: 1.0


	This is model containing all physic handles for simulation

*/

#include <WePhysicModel.h>
#include <stdio.h>
#include <WePhysics.h>
#include <WeStaticModel.h>

//#include <NxuStreamFactory.h>
#include <WePackage.h>
#include <WeActor.h>


WePhysicModel::WePhysicModel()
{
	Version = 1;
	Type = PHYSICMODEL_STATIC_SHAPE;
	Owner = NULL;
	Mass = 10;
	TriggerType = 0;
	ShapeGroup = 0;
	BoxDimensions = WeVector(1.0,1.0,1.0);
	SphereRadius = 1.0f;
	CapsuleRadius = 1.0f;
	CapsuleHeight = 1.0f;
	SleepOnStart = true;
}

WePhysicModel::~WePhysicModel()
{
	
}


WePhysicHandle* WePhysicModel::MakePhysicHandle(WeActor *Parent)
{
	if (Parent == NULL) return NULL;

	Owner = Parent;
	WePhysicHandle *Handle = NULL;

	switch (Type)
	{
	case PHYSICMODEL_STATIC_SHAPE:
		if (Owner->InEditor)
		{
			if (Parent->Model)
			{
				// if it is no animated model
				if (!Owner->Model->IsModel)
				{
					Handle = WePhysics::Instance()->CreateTriangleMesh(Owner, Owner->Location, Owner->Model);		
				}
				if (!Handle)
				{
					WeVector v;				
					v = (Owner->BBox.Max - Owner->BBox.Min) / 2.0f;	

					Handle = WePhysics::Instance()->CreateBox(Owner, Owner->TM, v, 0, 10, false, 0, false);		
					if (Handle)	Handle->SetKinematic(true);
				}
			}
		} else
		{
			if (Parent->Model)
			{
				// if it is no animated model
				if (!Owner->Model->IsModel)
				{
					Handle = WePhysics::Instance()->CreateTriangleMesh(Owner, Owner->Location, Owner->Model);		
				}
				if (!Handle)
				{
					WeVector v;				
					v = (Owner->BBox.Max - Owner->BBox.Min) / 2.0f;	

					Handle = WePhysics::Instance()->CreateBox(Owner, Owner->TM, v, 0, 10, true);		
				}
			}
		}
		
		break;
	case PHYSICMODEL_DYNAMIC_BOX:
		Handle = WePhysics::Instance()->CreateBox(Owner, Owner->TM, BoxDimensions, ShapeGroup, Mass, false, TriggerType);		
		break;
	case PHYSICMODEL_DYNAMIC_SPHERE:
		Handle = WePhysics::Instance()->CreateSphere(Owner, Owner->Location, SphereRadius, ShapeGroup, Mass, false, TriggerType);		
		break;
	case PHYSICMODEL_DYNAMIC_CYLINDER:
		Handle = WePhysics::Instance()->CreateCapsule(Owner, Owner->Location, CapsuleRadius, CapsuleHeight, ShapeGroup, Mass, false, TriggerType);		
		break;
	case PHYSICMODEL_DYNAMIC_SHAPE:
		// TODO: Implement this later
		break;

	}

	if (Handle && SleepOnStart) Handle->PutToSleep();
	return Handle;
}


void WePhysicModel::Serialize(WeFileStream &FS)
{

	FS.WriteInt(Version);
	FS.WriteInt(Type);

	FS.WriteBool(SleepOnStart);
	FS.WriteVector(BoxDimensions);
	FS.WriteFloat(SphereRadius);
	FS.WriteFloat(CapsuleRadius);
	FS.WriteFloat(CapsuleHeight);
	FS.WriteWord(ShapeGroup);
	FS.WriteWord(TriggerType);
	FS.WriteFloat(Mass);
	
}


void WePhysicModel::Deserialize(WeFileStream &FS)
{
   int v = FS.ReadInt();
   
   if (v == 1)
   {
	   Type = (PHYSICMODEL_TYPE)FS.ReadInt();

	   SleepOnStart = FS.ReadBool();
	   FS.ReadVector(BoxDimensions);
	   SphereRadius = FS.ReadFloat();
	   CapsuleRadius = FS.ReadFloat();
	   CapsuleHeight = FS.ReadFloat();
	   ShapeGroup = FS.ReadWord();
	   TriggerType = FS.ReadWord();
	   Mass = FS.ReadFloat();
   } // end of version 1
}
