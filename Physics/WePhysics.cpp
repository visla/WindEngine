/*
     WePhysics.cpp

	 Autor: Nebojsa Vislavski
	 Date: 6.10.2005.
	 Version: 1.0

	 This is physic engine class

*/


#include <WePhysics.h>
#include <WeActor.h>
#include <WeWorld.h>
#include <WeTimer.h>
#include <WePhysicHandle.h>
#include <WeControllerHandle.h>
#include <WeStaticModel.h>
#include <WeMesh.h>

#include <NxBoxController.h>
//#include <NxCapsuleController.h>
#include <NxCooking.h>
#include <Stream.h>
//#include <NxuPhysicsImport.h>
//#include <NxuStreamFactory.h>
//#include <NxuStream.h>
#include <Nxarray.h>

#include <WePhysicModel.h>
#include <WePhysicModelManager.h>
#include <WeModelManager.h> // from rendering
#include <WeCloth.h>
#include <NxMeshData.h>
#include <WeClothHandle.h>


WePhysics *WePhysics::_instance = NULL;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WePhysics *WePhysics::Instance()
{
	if (_instance) return _instance;
	_instance = new WePhysics;
	return _instance;
}






void WePhysics::TriggerCallback::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)        
{
	if (WePhysics::Instance()->OnTrigger)
	{
		WePhysicHandle *ph;
		WeActor *a1, *a2;
		ph = (WePhysicHandle* )triggerShape.getActor().userData;
		a1 = ph->Parent;
		ph = (WePhysicHandle* )otherShape.getActor().userData;
		a2 = ph->Parent;
                		
		WePhysics::Instance()->OnTrigger(a1, a2, status);
	}
    
}

WePhysics::CollisionCallback WePhysics::CollisionCallback::Instance;
WePhysics::TriggerCallback WePhysics::TriggerCallback::Instance;

void WePhysics::CollisionCallback::onContactNotify(NxContactPair& Pair, NxU32 events)
{
	// do teleporting if we need to 
	WePhysicHandle *ph;
	WeActor *a1, *a2;
	WeVector tmp;
	ph = (WePhysicHandle* )Pair.actors[0]->userData;
	a1 = ph->Parent;
	ph = (WePhysicHandle* )Pair.actors[1]->userData;
	a2 = ph->Parent;

	
	// if one of them is teleport then move other to teleport location
	if (events == NX_NOTIFY_ON_START_TOUCH)
	{
		WeActor *Teleport = NULL;
		WeActor *Dude = NULL;

		if (a1->Teleport)
		{
			Teleport = a1;
			Dude = a2;
		}
		else if (a2->Teleport)
		{
			Teleport = a2;
			Dude = a1;
		}

		if (Teleport)
		{
			bool Teleported = false;

			if (Teleport->TeleportDestination.GetLen() > 0)
			{
				// we have teleport destination object. Just check if there is any destionation object with that
				// name in the teleports world
				WeActor *dest;
				dest = Teleport->GetWorld()->FindActor(Teleport->TeleportDestination);
				if (dest)
				{
					if (Teleport->TeleportRelative)
					{
						WeVector RelLocation;
						float RelDistance;

						RelLocation= Dude->Location - Teleport->Location;
						RelDistance = D3DXVec3Length(&RelLocation);

						if ((Teleport->TeleportRadius > 0.001f) && (RelDistance > Teleport->TeleportRadius))
						{
							D3DXVec3Normalize(&RelLocation, &RelLocation);
							RelLocation *= Teleport->TeleportRadius;
						}

						Dude->SetTransformation(dest->TM);
						Dude->MoveTo(dest->Location + RelLocation);
					}
					else
					{
						Dude->SetTransformation(dest->TM);
					}

					Teleported = true;
					if (WePhysics::Instance()->OnTeleport)
					{
						WePhysics::Instance()->OnTeleport(Dude, NULL);
					}
				}
			}


/*
			if (Teleport->TeleportDestination.GetLen() > 0)
			{
				// we have teleport destination object. Just check if there is any destionation object with that
				// name in the teleports world
				WeActor *dest;
				dest = Teleport->GetWorld()->FindActor(Teleport->TeleportDestination);
				if (dest)
				{
					if (WePhysics::Instance()->OnTeleport) WePhysics::Instance()->OnTeleport(Dude, dest);
					Dude->SetTransformation(dest->TM);
					Teleported = true;
				}
			} else
			{
				if (!Teleport->TeleportRelative)	
				{
					if (WePhysics::Instance()->OnTeleport) WePhysics::Instance()->OnTeleport(Dude, NULL);
					Dude->MoveTo(Teleport->TeleportPoint);
					Teleported = true;
				}
				else
				{
					tmp = Teleport->TeleportPoint + Dude->Location - Teleport->Location;
					if (WePhysics::Instance()->OnTeleport) WePhysics::Instance()->OnTeleport(Dude, NULL);
					Dude->MoveTo(tmp);
					Teleported = true;
				}

				if (Teleport->TeleportChangeDirection)
				{
					//change direction
					//
					//----------------

					//change velocity
					Dude->PhysicHandle->GetLinearVelocity(Dude->Velocity);
					Dude->Velocity = D3DXVec3Length(&Dude->Velocity) * Teleport->TeleportDirection;
					Dude->PhysicHandle->SetLinearVelocity(Dude->Velocity);
					Teleported = true;
					//----------------
				}
			}
*/

			if (Teleported)
				Dude->OnTeleported();
			return;
		}
	}
	

	if (WePhysics::Instance()->OnCollision)
	{
		WeVector Normal;
		WeVector Tangent;
		WeVector FirstTouchPoint;

		NxContactStreamIterator i(Pair.stream);
    
		if (i.goNextPair())
		{
			if (i.goNextPatch())
			{
				if (i.goNextPoint())
				{
					FirstTouchPoint.x = i.getPoint().x;
					FirstTouchPoint.y = i.getPoint().y;
					FirstTouchPoint.z = i.getPoint().z;
				}
			}
		}

		Normal.x = Pair.sumNormalForce.x;
		Normal.y = Pair.sumNormalForce.y;
		Normal.z = Pair.sumNormalForce.z;

		Tangent.x = Pair.sumFrictionForce.x;
		Tangent.y = Pair.sumFrictionForce.y;
		Tangent.z = Pair.sumFrictionForce.z;

		WePhysics::Instance()->OnCollision(a1, a2, Normal, Tangent, events, FirstTouchPoint);
	}
}







/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Intialize
//         Input Parametars :  N/A
//         Output Parametars:  bool
//         Comments         :  Initialize 
//
//////////////////////////////////////////////////////////////////////////
bool WePhysics::Initialize()
{
	if (Initialized) return true;

	PhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if (!PhysicsSDK) return false;

	//NxuPhysicsImport::SDK = PhysicsSDK;
	PhysicsSDK->setParameter(NX_CONTINUOUS_CD, 1);

	SetDefaultParams();

	// Create NxScene cause we`ll have only one scene in engine
	// TODO: Maybe add methods for creating many scenes but later	
	Gravity = NxVec3(0, -9.81f, 0.0f);

	NxSceneDesc SceneDesc;
	SceneDesc.gravity = Gravity;
	Scene = PhysicsSDK->createScene(SceneDesc);	
	
	Scene->setUserContactReport(&WePhysics::CollisionCallback::Instance);
	Scene->setUserTriggerReport(&WePhysics::TriggerCallback::Instance);


	// Create the default material
    NxMaterial* defaultMaterial = Scene->getMaterialFromIndex(0); 
    defaultMaterial->setRestitution(0.0f);
    defaultMaterial->setStaticFriction(0.5f);
    defaultMaterial->setDynamicFriction(0.5f);

    NxMaterial* mat = Scene->getMaterialFromIndex(2); 
    mat->setRestitution(0.0f);
    mat->setStaticFriction(0.001f);
    mat->setDynamicFriction(0.001f);


	Scene->setTiming(1.0f/60.0f, 8, NX_TIMESTEP_FIXED);
	PhysicsSDK->setParameter(NX_CCD_EPSILON, 0.01f);

	// Disable collision with DummyShapeGroup
	//Scene->setGroupCollisionFlag(DUMMYSHAPE_GROUP, 0, false);
	//Scene->setGroupCollisionFlag(DUMMYSHAPE_GROUP, DUMMYSHAPE_GROUP, false);
	for (int i = 0; i < 32; i++)
		Scene->setGroupCollisionFlag(DUMMYSHAPE_GROUP, i, false);


	Initialized = true;
	return true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetDefaultParams()
{
	// nothing for now
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WePhysics::WePhysics()
{
	Initialized = false;
	OnCollision = NULL;
	OnTrigger = NULL;
	OnTeleport = NULL;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::ShutDown()
{
	if (!Initialized) return;
	
	for (TriangleMeshes.First(); !TriangleMeshes.End(); TriangleMeshes.Delete())
	{
		SAFE_DELETE(TriangleMeshes.GetItem());
	}


	PhysicsSDK->releaseScene(*Scene);
	PhysicsSDK->release();

	Initialized = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetCollisionCallback(PHYSICS_COLLISIONCALLBACK Callback)
{
	if (!Initialized) return;

	OnCollision = Callback;	
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetTriggerCallback(PHYSICS_TRIGGERCALLBACK Callback)
{
	if (!Initialized) return;

	OnTrigger = Callback;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetTeleportReport(PHYSICS_TELEPORTREPORT Callback)
{
	if (!Initialized) return;

	OnTeleport = Callback;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetGroupCollision(WORD Group1, WORD Group2, WORD CollisionType)
{
	if (!Initialized) return;

	Scene->setActorGroupPairFlags(Group1, Group2, CollisionType);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::SetGroupCollisionInteract(WORD Group1, WORD Group2, bool Enable)
{
	if (!Initialized) return;

	Scene->setGroupCollisionFlag(Group1, Group2, Enable);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::BeginUpdate()
{
	if (!Initialized) return;
	//Scene->simulate(1.0f/60.0f);
	//if (WeTimer::Instance()->GetDeltaTimeF() > 0)
	//{
		Scene->simulate(WeTimer::Instance()->GetDeltaTimeF());
		Scene->flushStream();
	//}

	//UpdateControllers();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::EndUpdate()
{
	if (!Initialized) return;
	Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	UpdateControllers();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::UpdateControllers()
{
	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;
	Scene->getTiming(maxTimestep, maxIter, method, &numSubSteps);

	if(numSubSteps)	CM.updateControllers();
	
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::GetWorldMatrix(NxActor *nxActor, WeMatrix &outMatrix)
{
	NxMat34 P;	
	P = nxActor->getGlobalPose();

	NxMat33 Orient = P.M;
	NxVec3 Trans = P.t;

	Orient.getColumnMajorStride4(&(outMatrix[0]));
	Trans.get(&(outMatrix[12]));

	outMatrix[3] = outMatrix[7] = outMatrix[11] = 0.0f;
	outMatrix[15] = 1.0f;

	

}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle *WePhysics::CreateGroundPlane(WeActor *Owner)
{
	NxPlaneShapeDesc PlaneDesc;
	NxActorDesc ActorDesc;
	if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);

	ActorDesc.shapes.pushBack(&PlaneDesc);
	Handle->nxActor = Scene->createActor(ActorDesc);
	Handle->nxActor->userData = Handle;
	return Handle;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle *WePhysics::CreateBox(WeActor *Owner, const WeVector &Location, const WeVector &Dimensions, WORD Group, float Mass, bool Static, WORD TriggerType, bool UseCCD)
{
	NxBoxShapeDesc BoxShapeDesc, Dummy;
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;


	if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);


	BoxShapeDesc.dimensions.set(Dimensions.x,Dimensions.y,Dimensions.z);	
	BoxShapeDesc.shapeFlags |= TriggerType;		
	
	//BoxShapeDesc.mass = Mass;


	BodyDesc.mass = Mass;
	if (!Static) 
	{
		ActorDesc.body = &BodyDesc;
	}
	
	if (TriggerType)
	{
		// we must have dummy shape in some group that is not collissioned
		Dummy.dimensions.set(Dimensions.x,Dimensions.y,Dimensions.z);	
		Dummy.group = DUMMYSHAPE_GROUP;			
		ActorDesc.shapes.pushBack(&Dummy);
	}

	ActorDesc.shapes.pushBack(&BoxShapeDesc);

	//ActorDesc.density = 10.0f;
	ActorDesc.globalPose.t = NxVec3(Location.x, Location.y, Location.z);


	//Handle->Density = 10;
	Handle->nxActor = Scene->createActor(ActorDesc);
	if (Handle->nxActor)
	{
		Handle->nxActor->setGroup(Group);	
		SetGroupForShapes(Handle->nxActor, Group);
		Handle->nxActor->getShapes()[0]->setMaterial(2);
		Handle->nxActor->userData = Handle;
	}

	return Handle;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle *WePhysics::CreateBox(WeActor *Owner, const WeMatrix &TM, const WeVector &Dimensions, WORD Group, float Mass, bool Static, WORD TriggerType, bool UseCCD)
{
	NxBoxShapeDesc BoxShapeDesc, Dummy;
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;


	if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);


	BoxShapeDesc.dimensions.set(Dimensions.x,Dimensions.y,Dimensions.z);	
	BoxShapeDesc.shapeFlags |= TriggerType;		
	//BoxShapeDesc.mass = Mass;


	BodyDesc.mass = Mass;
	if (!Static) 
	{
		ActorDesc.body = &BodyDesc;
	}
	
	if (TriggerType)
	{
		// we must have dummy shape in some group that is not collissioned
		Dummy.dimensions.set(Dimensions.x,Dimensions.y,Dimensions.z);	
		Dummy.group = DUMMYSHAPE_GROUP;			
		ActorDesc.shapes.pushBack(&Dummy);
	}

	ActorDesc.shapes.pushBack(&BoxShapeDesc);

	//ActorDesc.density = 10.0f;
	ActorDesc.globalPose.setColumnMajor44((float *)&TM);
    

	//Handle->Density = 10;
	Handle->nxActor = Scene->createActor(ActorDesc);
	if (Handle->nxActor)
	{
		Handle->nxActor->setGroup(Group);	
		SetGroupForShapes(Handle->nxActor, Group);
		Handle->nxActor->getShapes()[0]->setMaterial(2);
		Handle->nxActor->userData = Handle;
	}

	return Handle;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle *WePhysics::CreateSphere(WeActor *Owner, const WeVector &Location, float Radius, WORD Group, float Mass, bool Static, WORD TriggerType)
{
	NxSphereShapeDesc SphereShapeDesc, Dummy;
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;


	if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);

	SphereShapeDesc.radius = Radius;
	SphereShapeDesc.shapeFlags |= TriggerType;

	if (TriggerType)
	{
		// we must have dummy shape in some group that is not collissioned
		Dummy.radius = Radius;
		Dummy.group = DUMMYSHAPE_GROUP;			
		ActorDesc.shapes.pushBack(&Dummy);
	}


	BodyDesc.mass = Mass;
	if (!Static) 
	{
		ActorDesc.body = &BodyDesc;
	}

	ActorDesc.shapes.pushBack(&SphereShapeDesc);
	ActorDesc.globalPose.t = NxVec3(Location.x, Location.y, Location.z);

	Handle->nxActor = Scene->createActor(ActorDesc);
	if (!TriggerType)
	{
		Handle->nxActor->setGroup(Group);
		SetGroupForShapes(Handle->nxActor, Group);
	}

	Handle->nxActor->userData = Handle;

	return Handle;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle *WePhysics::CreateCapsule(WeActor *Owner, const WeVector &Location, float Radius, float Height, WORD Group, float Mass, bool Static, WORD TriggerType)
{
	NxCapsuleShapeDesc CapsuleShapeDesc, Dummy;
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;


	if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);

	CapsuleShapeDesc.radius = Radius;
	CapsuleShapeDesc.height = Height;
	CapsuleShapeDesc.shapeFlags |= TriggerType;

	if (TriggerType)
	{
		// we must have dummy shape in some group that is not collissioned
		Dummy.radius = Radius;
		Dummy.height = Height;
		Dummy.group = DUMMYSHAPE_GROUP;			
		ActorDesc.shapes.pushBack(&Dummy);
	}


	BodyDesc.mass = Mass;
	if (!Static) 
	{
		ActorDesc.body = &BodyDesc;
	}

	ActorDesc.shapes.pushBack(&CapsuleShapeDesc);
	ActorDesc.globalPose.t = NxVec3(Location.x, Location.y, Location.z);

	Handle->nxActor = Scene->createActor(ActorDesc);
	if (!TriggerType)
	{
		Handle->nxActor->setGroup(Group);
		SetGroupForShapes(Handle->nxActor, Group);
	}

	Handle->nxActor->userData = Handle;

	return Handle;

}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Gets new position 
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::DestroyNxActor(NxActor *nxactor)
{
	Scene->releaseActor(*nxactor);
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  CreateBoxController
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Creates box controller for specified handle
//
//////////////////////////////////////////////////////////////////////////
void WePhysics::CreateBoxController(WeControllerHandle *Handle, WeVector &Position, WeVector &Extents, float SlopeLimitDeg, float StepOffset, float SkinWidth)
{
	if (!Initialized) return;
	if (!Handle) return;

	NxBoxControllerDesc desc;
	NxVec3 Pos, Dim;
	Pos.x = Position.x;  Pos.y = Position.y; Pos.z = Position.z;
	Dim.x = Extents.x; Dim.y = Extents.y; Dim.z = Extents.z;

	//desc.position		= Pos;
	desc.extents		= Dim;
	desc.upDirection	= NX_Y;	
	desc.slopeLimit		= cosf(NxMath::degToRad(SlopeLimitDeg));
	desc.skinWidth		= SkinWidth;
	desc.stepOffset		= StepOffset;
	desc.callback		= &Handle->ControllerReport;

	Handle->nxController = CM.createController(Scene, desc);
	Handle->nxActor = Handle->nxController->getActor();
	Handle->nxActor->userData = Handle;	

}


void WePhysics::CreateCapsuleController(WeControllerHandle *Handle, WeVector &Position, float Radius, float Height, float SlopeLimit, float StepOffset, float SkinWidth)
{
	if (!Initialized) return;
	if (!Handle) return;

	///*NxCapsuleControllerDesc desc;
	//NxVec3 Pos;
	//Pos.x = Position.x;  Pos.y = Position.y; Pos.z = Position.z;


	//desc.position		= Pos;
	//desc.radius			= Radius;
	//desc.height			= Height;
	//desc.upDirection	= NX_Y;	
	//desc.slopeLimit		= cosf(NxMath::degToRad(SlopeLimit));
	//desc.skinWidth		= SkinWidth;
	//desc.stepOffset		= StepOffset;
	//desc.callback		= &Handle->ControllerReport;

	//Handle->nxController = CM.createController(Scene, desc);	
	//Handle->nxActor = Handle->nxController->getActor();
	//Handle->nxActor->userData = Handle;	*/


}



int WePhysics::MoveController(WeControllerHandle *Handle, WeVector &Disp, float HeightDelta)
{
	if (!Handle) return 0;
	NxVec3 d;
	NxU32 CollisionFlags;


	d.x = Disp.x;  d.y = Disp.y;  d.z = Disp.z;
	d += Gravity;
	if (HeightDelta != 0) d.y += HeightDelta;
	d = d * WeTimer::Instance()->GetDeltaTimeF();



	Handle->nxController->move(d, 7, 0.001f, CollisionFlags, 1.0f);

	return CollisionFlags;
}



void WePhysics::SetGravity(WeVector &gravity)
{
	Gravity.x = gravity.x;
	Gravity.y = gravity.y;
	Gravity.z = gravity.z;
	Scene->setGravity(Gravity);
}


WeVector WePhysics::GetGravity()
{
	WeVector Res;
	Res.x = Gravity.x;
	Res.y = Gravity.y;
	Res.z = Gravity.z;
	return Res;
}


WePhysicHandle*	WePhysics::CreateCloth(WeCloth *Owner)
{
	//MessageBox(0, "CreateCloth", 0, 0);
	if (!Owner->Model) return NULL;

	NxClothDesc ClothDesc;

	NxClothMesh *cmDesc = NULL;

	for (ClothMeshes.First(); !ClothMeshes.End(); ClothMeshes.Next())
	{
		if (ClothMeshes.GetItem()->ModelName == Owner->Model->Name) 
		{
			cmDesc = ClothMeshes.GetItem()->GetClothMeshDesc();
			break;
		}
	}

	
	if (!cmDesc) 
	{
		WePhysicClothMesh *cm;
		cm = new WePhysicClothMesh;
		cm->MakeClothMesh(Owner->Model);
		ClothMeshes.PushBack(cm);
		cmDesc = cm->GetClothMeshDesc();	
	}
	
	ClothDesc.clothMesh = cmDesc;
	ClothDesc.globalPose.setColumnMajor44((float *)&Owner->TM);

	ClothDesc.meshData.verticesPosBegin = Owner->Vertices;
	ClothDesc.meshData.verticesPosByteStride = Owner->Model->Mesh->GetNumBytesPerVertex();
	ClothDesc.meshData.verticesNormalBegin = &Owner->Vertices[0].Normal;
	ClothDesc.meshData.verticesNormalByteStride = Owner->Model->Mesh->GetNumBytesPerVertex();
	ClothDesc.meshData.numVerticesPtr = (NxU32*)&Owner->NumVertices; // malloc(sizeof(NxU32));
	ClothDesc.meshData.maxVertices = Owner->Model->Mesh->GetNumVertices()*2;
	Owner->NumOriginalVertices = Owner->Model->Mesh->GetNumVertices();
	//ClothDesc.meshData.flags = NX_MDF_INDEXED_MESH;

	//char tmps[100];
	//sprintf(tmps, "maxV %d, V stride %d", ClothDesc.meshData.maxVertices, ClothDesc.meshData.verticesPosByteStride);
	//MessageBox(0, tmps, 0, 0);

		
	ClothDesc.meshData.maxIndices = Owner->Model->Mesh->GetNumFaces()*3*2;
	ClothDesc.meshData.indicesByteStride = Owner->Model->Mesh->GetNumBytesPerIndex();
	ClothDesc.meshData.indicesBegin = Owner->Indices;
	ClothDesc.meshData.numIndicesPtr =  (NxU32*)&Owner->NumIndices; //malloc(sizeof(NxU32));

	//sprintf(tmps, "maxIndices %d, indices stride %d, numIndices %d", ClothDesc.meshData.maxIndices, ClothDesc.meshData.indicesByteStride, *ClothDesc.meshData.numTrianglesPtr);
	//MessageBox(0, tmps, 0, 0);
	//   
	ClothDesc.meshData.maxParentIndices = Owner->Model->Mesh->GetNumFaces()*3*2;
	ClothDesc.meshData.numParentIndicesPtr = (NxU32*)&Owner->NumParentIndices; // malloc(sizeof(NxU32));
	ClothDesc.meshData.parentIndicesBegin = Owner->ParentIndices;
	ClothDesc.meshData.parentIndicesByteStride = sizeof(DWORD);
	
	ClothDesc.thickness = Owner->Thickness;
	ClothDesc.bendingStiffness = Owner->BendingStiffness;
	ClothDesc.stretchingStiffness = Owner->StretchingStiffness;
	ClothDesc.dampingCoefficient = Owner->DampingCoef;
	ClothDesc.friction = Owner->Friction;
	ClothDesc.pressure = Owner->Pressure;
	ClothDesc.tearFactor = Owner->TearFactor;
	ClothDesc.attachmentResponseCoefficient = Owner->AttachmentResponseCoef;
	ClothDesc.collisionResponseCoefficient = Owner->CollisionResponseCoef;
	ClothDesc.density = Owner->Density;
	ClothDesc.collisionGroup = Owner->Group;
		
	ClothDesc.flags |= NX_CLF_BENDING;
	//ClothDesc.flags |= NX_CLF_COLLISION_TWOWAY;
	if (Owner->Gravity)
		ClothDesc.flags |= NX_CLF_GRAVITY;
	if (Owner->Tearable)
		ClothDesc.flags |= NX_CLF_TEARABLE;

	//ClothDesc.flags |= NX_CLF_STATIC;


	WeClothHandle *ch = new WeClothHandle(Owner);
	ch->Cloth = Scene->createCloth(ClothDesc);
	//if (!ch->Cloth) MessageBox(0, "Nije napravljen", 0, 0);
	//ch->Cloth->attachToCollidingShapes(NX_CLOTH_INTERACTION_ONEWAY);
	//ch->Cloth->setFlags(NX_CLF_STATIC);
	//ch->Cloth->setExternalAcceleration(NxVec3(0,0, 2));

	// clear his mesh and create new dynamic which is going to be used for this
    // make uv coords
	void *points;

	Owner->Model->Mesh->LockVertexBuffer((void **)&points, D3DLOCK_READONLY);
	memcpy(Owner->Vertices, points, Owner->Model->Mesh->GetNumVertices()*sizeof(WeNormalMappedMeshVertex));
	memcpy(&Owner->Vertices[Owner->Model->Mesh->GetNumVertices()], points, Owner->Model->Mesh->GetNumVertices()*sizeof(WeNormalMappedMeshVertex));
	Owner->Model->Mesh->UnlockVertexBuffer();

	
	Owner->ClothMesh = new WeMesh;
	Owner->ClothMesh->Materials[0].Clear();	
	Owner->ClothMesh->CreateDynamic(Owner->Model->Mesh->GetNumFaces()*6, Owner->Model->Mesh->GetNumVertices()*6, NULL, false);
	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		Owner->ClothMesh->MaterialNames[i] = Owner->Model->Mesh->MaterialNames[i];
		Owner->ClothMesh->Materials[i]  	= Owner->Model->Mesh->Materials[i];
	}
	Owner->ClothMesh->BSphere = Owner->Model->Mesh->BSphere;
	Owner->ClothMesh->BSphere.Radius *= 2.0f;

	Owner->Model->Mesh = Owner->ClothMesh;	
	Owner->PhysicHandle = ch;
	return ch;
}



WePhysicHandle* WePhysics::CreateTriangleMesh(WeActor *Owner, const WeVector &Location, WeStaticModel *CollisionModel, float Density, int Type)
{	
	if (!CollisionModel) return NULL;	
	if (CollisionModel->IsModel) return NULL;

	NxActorDesc *actorDesc = NULL;
    
	for (TriangleMeshes.First(); !TriangleMeshes.End(); TriangleMeshes.Next())
	{
		if (TriangleMeshes.GetItem()->ModelName == CollisionModel->Name) 
		{
			actorDesc = TriangleMeshes.GetItem()->GetActorDesc();
			break;
		}
	}

	if (!actorDesc)
	{
		WePhysicTriangleMesh *tm;
		tm = new WePhysicTriangleMesh;
		if (!tm->MakeTriangleMesh(CollisionModel)) 
		{
			delete tm;
			return NULL;
		}
		TriangleMeshes.PushBack(tm);
		actorDesc = tm->GetActorDesc();		
	}


    
    WePhysicHandle *Handle = new WePhysicHandle(Owner);
	NxBodyDesc body;
	if (Type == WE_PHYSICHANDLE_DYNAMIC || Type == WE_PHYSICHANDLE_KINEMATIC)
	{
		actorDesc->body = &body;
		actorDesc->density = 10;
	} else 	actorDesc->body = NULL;

	actorDesc->globalPose.t.x  = Location.x;
	actorDesc->globalPose.t.y  = Location.y;
	actorDesc->globalPose.t.z  = Location.z;

	
    Handle->nxActor = Scene->createActor(*actorDesc);
	if (!Handle->nxActor) 
	{
		SAFE_DELETE(Handle);
		return NULL;
	}
	Handle->nxActor->userData = Handle;	

	if (Type == WE_PHYSICHANDLE_KINEMATIC)
	{
		Handle->nxActor->raiseBodyFlag(NX_BF_KINEMATIC);
	}

	return Handle;
}




WePhysicHandle* WePhysics::CreateTriangleMesh(WeActor *Owner, const WeMatrix &TM, WeStaticModel *CollisionModel, float Density, int Type)
{	
	if (!CollisionModel) return NULL;

	NxActorDesc *actorDesc = NULL;
    
	for (TriangleMeshes.First(); !TriangleMeshes.End(); TriangleMeshes.Next())
	{
		if (TriangleMeshes.GetItem()->ModelName == CollisionModel->Name) 
		{
			actorDesc = TriangleMeshes.GetItem()->GetActorDesc();
			break;
		}
	}

	if (!actorDesc)
	{
		WePhysicTriangleMesh *tm;
		tm = new WePhysicTriangleMesh;
		tm->MakeTriangleMesh(CollisionModel);
		TriangleMeshes.PushBack(tm);
		actorDesc = tm->GetActorDesc();		
	}


    
    WePhysicHandle *Handle = new WePhysicHandle(Owner);
	NxBodyDesc body;
	if (Type == WE_PHYSICHANDLE_DYNAMIC || Type == WE_PHYSICHANDLE_KINEMATIC)
	{
		actorDesc->body = &body;
		actorDesc->density = 10;
	} else 	actorDesc->body = NULL;

	actorDesc->globalPose.setColumnMajor44((float *)&TM);

    Handle->nxActor = Scene->createActor(*actorDesc);
	Handle->nxActor->userData = Handle;	

	if (Type == WE_PHYSICHANDLE_KINEMATIC)
	{
		Handle->nxActor->raiseBodyFlag(NX_BF_KINEMATIC);
	}

	return Handle;
}


bool WePhysics::RaycastClosest(WeVector &Location, WeVector &Direction, WeShapesType ShapeType, WeRaycastHit &Hit, int Group, float MaxDist, int HintFlags)
{
	NxRay Ray;
	NxRaycastHit hit;
	NxShape *shape = NULL;
	Ray.orig.x = Location.x; Ray.orig.y = Location.y; Ray.orig.z = Location.z;
	Ray.dir.x = Direction.x; Ray.dir.y = Direction.y; Ray.dir.z = Direction.z;

	shape = Scene->raycastClosestShape(Ray, (NxShapesType)ShapeType, hit, Group, MaxDist, HintFlags); 
	if (shape)
	{
		Hit.ActorHit = ((WePhysicHandle *)shape->getActor().userData)->Parent;
		Hit.Distance = hit.distance;
		Hit.FaceID = hit.faceID;
		Hit.MaterialIndex = hit.materialIndex;
		Hit.u = hit.u;
		Hit.v = hit.v;
		Hit.WorldImpact.x = hit.worldImpact.x; Hit.WorldImpact.y = hit.worldImpact.y; Hit.WorldImpact.z = hit.worldImpact.z;
		Hit.WorldNormal.x = hit.worldNormal.x; Hit.WorldNormal.y = hit.worldNormal.y; Hit.WorldNormal.z = hit.worldNormal.z;		
		return true;
	}
	return false;
}




void WePhysics::CreateFromPML(WePhysicModel *PhysicModel, WeActor *Owner, WeVector &Location, StringParam filename)
{
	//SJointIDs *JointIDs = NULL;
	//SRigidBodyID *RigidBodyIDs;

	//if (!PhysicModel) return;
	//if (!Owner) return;

	//delete[] PhysicModel->Joints;
	//delete[] PhysicModel->RigidBodies;


	//NxVec3 nxLocation;
	//nxLocation.x = Location.x;
	//nxLocation.y = Location.y;
	//nxLocation.z = Location.z;

	//Library *pmllib;
	//pmllib = pmlLoadScene(filename);

	//// use only model[0] cause we are importing only one model
	//PhysicModel->RigidBodyCount = pmllib->model[0]->rigidbody.count;
	//if (PhysicModel->RigidBodyCount) 
	//{ 
	//	PhysicModel->RigidBodies = new WeRigidBody[PhysicModel->RigidBodyCount];
	//	RigidBodyIDs = new SRigidBodyID[PhysicModel->RigidBodyCount];
	//}
	//PhysicModel->JointCount = pmllib->model[0]->joint.count;
 //   if (PhysicModel->JointCount) 
	//{
	//	PhysicModel->Joints = new WeJoint[PhysicModel->JointCount];
	//	JointIDs = new SJointIDs[PhysicModel->JointCount];
	//}


	//

	//// ------------------------------------
	//// make joint ids
	//for (int i = 0; i < PhysicModel->JointCount; i++)
	//{
	//	// TODO: add joints
	//	JointIDs[i].ID1 = pmllib->model[0]->joint[i]->child->id.buf;
	//	JointIDs[i].ID2 = pmllib->model[0]->joint[i]->parent->id.buf;		
	//}



	//// ----------------------
	//// Create Rigid Bodies
	//for (int i = 0; i < PhysicModel->RigidBodyCount; i++)
	//{
	//	NxActorDesc ad;
	//	NxBodyDesc bd;
	//	NxQuat quat;
	//	NxVec3 tmpVec,p0,p1;						
	//	
	//	bd.mass = pmllib->model[0]->rigidbody[i]->mass;
	//	
	//	if (pmllib->model[0]->rigidbody[i]->dynamic) ad.body = &bd;
	//	else ad.body = NULL;

	//	RigidBodyIDs[i].ID = pmllib->model[0]->rigidbody[i]->id.buf;
	//	RigidBodyIDs[i].RigidBody = &PhysicModel->RigidBodies[i];
	//	PhysicModel->RigidBodies[i].Name = pmllib->model[0]->rigidbody[i]->id.buf;



	//	ad.density = 10;
	//	ad.globalPose.t.x = pmllib->model[0]->rigidbody[i]->position.x;
	//	ad.globalPose.t.y = pmllib->model[0]->rigidbody[i]->position.z;
	//	ad.globalPose.t.z = pmllib->model[0]->rigidbody[i]->position.y;
	//	ad.globalPose.t += nxLocation;

	//	quat.x = pmllib->model[0]->rigidbody[i]->orientation.x;
	//	quat.y = pmllib->model[0]->rigidbody[i]->orientation.y;
	//	quat.z = pmllib->model[0]->rigidbody[i]->orientation.z;
	//	quat.w = pmllib->model[0]->rigidbody[i]->orientation.w;
	//	ad.globalPose.M.fromQuat(quat);




	//	// Make shapes
	//	// -----------------------------------
	//	RigidBody *rigidbody;
	//	rigidbody = pmllib->model[0]->rigidbody[i];
	//	for (int j = 0; j < rigidbody->shape.count; j++)
	//	{
	//		NxBoxShapeDesc *bsd;				
	//		NxSphereShapeDesc ssd;
	//		NxCapsuleShapeDesc csd;
	//		NxConvexShapeDesc convexsd;
	//		

	//		switch (rigidbody->shape[j]->geometry->GetType())
	//		{
	//		case GT_BOX:
	//			bsd = new NxBoxShapeDesc;
	//			bsd->dimensions.x = ((Box *)(rigidbody->shape[j]->geometry))->size.x / 2.0f;
	//			bsd->dimensions.y = ((Box *)(rigidbody->shape[j]->geometry))->size.z / 2.0f;
	//			bsd->dimensions.z = ((Box *)(rigidbody->shape[j]->geometry))->size.y / 2.0f;

	//			//printf("Size: %3.3f %3.3f %3.3f\n", bsd.dimensions.x, bsd.dimensions.y, bsd.dimensions.z);

	//			quat.x = rigidbody->shape[j]->orientation.x;
	//			quat.y = rigidbody->shape[j]->orientation.y;
	//			quat.z = rigidbody->shape[j]->orientation.z;
	//			quat.w = rigidbody->shape[j]->orientation.w;
	//			//bsd.materialIndex = rigidbody->shape[j]->material->index;				
	//			bsd->localPose.t.x = rigidbody->shape[j]->position.x;
	//			bsd->localPose.t.y = rigidbody->shape[j]->position.z;
	//			bsd->localPose.t.z = rigidbody->shape[j]->position.y;

	//			bsd->localPose.M.fromQuat(quat);
	//			ad.shapes.pushBack(bsd);

	//			break;
	//		case GT_CAPSULE:
	//			p0.x =  ((Capsule *)(rigidbody->shape[j]->geometry))->p0.x;
	//			p0.y =  ((Capsule *)(rigidbody->shape[j]->geometry))->p0.y;
	//			p0.z =  ((Capsule *)(rigidbody->shape[j]->geometry))->p0.z;

	//			p1.x =  ((Capsule *)(rigidbody->shape[j]->geometry))->p1.x;
	//			p1.y =  ((Capsule *)(rigidbody->shape[j]->geometry))->p1.y;
	//			p1.z =  ((Capsule *)(rigidbody->shape[j]->geometry))->p1.z;
	//			tmpVec = p0-p1;

	//			csd.radius = ((Capsule *)(rigidbody->shape[j]->geometry))->radius;
	//			csd.height = tmpVec.magnitude();
	//			

	//			csd.localPose.t.x = rigidbody->shape[j]->position.x;
	//			csd.localPose.t.y = rigidbody->shape[j]->position.z;
	//			csd.localPose.t.z = rigidbody->shape[j]->position.y;
	//			quat.x = rigidbody->shape[j]->orientation.x;
	//			quat.y = rigidbody->shape[j]->orientation.y;
	//			quat.z = rigidbody->shape[j]->orientation.z;
	//			quat.w = rigidbody->shape[j]->orientation.w;
	//			//csd.materialIndex = rigidbody->shape[j]->material->index;				
	//			csd.localPose.M.fromQuat(quat);
	//			ad.shapes.pushBack(&csd);
	//			break;
	//		case GT_SPHERE:
	//			ssd.radius = ((Sphere *)(rigidbody->shape[j]->geometry))->radius;

	//			ssd.localPose.t.x = rigidbody->shape[j]->position.x;
	//			ssd.localPose.t.y = rigidbody->shape[j]->position.y;
	//			ssd.localPose.t.z = rigidbody->shape[j]->position.z;
	//			quat.x = rigidbody->shape[j]->orientation.x;
	//			quat.y = rigidbody->shape[j]->orientation.y;
	//			quat.z = rigidbody->shape[j]->orientation.z;
	//			quat.w = rigidbody->shape[j]->orientation.w;
	//			//ssd.materialIndex = rigidbody->shape[j]->material->index;				
	//			ssd.localPose.M.fromQuat(quat);
	//			ssd.localPose.t.x = rigidbody->shape[j]->position.x;
	//			ssd.localPose.t.y = rigidbody->shape[j]->position.z;
	//			ssd.localPose.t.z = rigidbody->shape[j]->position.y;


	//			ad.shapes.pushBack(&ssd);
	//			break;

	//		case GT_CONVEX:
	//			// TODO: Add support for this convex objects
	//			break;

	//		case GT_TRIMESH:
	//			// TODO: Add support for tri meshes
	//			break;
	//		}	
	//	} // MAKE SHAPES END


	//	// Make nxActor
	//	PhysicModel->RigidBodies[i].PhysicHandle = new WePhysicHandle(Owner);
 //       
	//	PhysicModel->RigidBodies[i].PhysicHandle->nxActor = Scene->createActor(ad);		
	//	PhysicModel->RigidBodies[i].PhysicHandle->nxActor->setLinearDamping(1.5f);
	//	PhysicModel->RigidBodies[i].PhysicHandle->nxActor->userData = PhysicModel->RigidBodies[i].PhysicHandle;		
	//}

	//if (PhysicModel->JointCount)
	//	MakePhysicModelJoints(PhysicModel, pmllib, JointIDs, RigidBodyIDs);

}



void WePhysics::MakePhysicModelJoints(WePhysicModel *PhysicModel, Library *pmllib, SJointIDs *JointIDs, SRigidBodyID *RigidBodyIDs)
{
	/*WeRigidBody *Child, *Parent;
	WePhysicHandle *ChildH, *ParentH;


	for (int i = 0; i < PhysicModel->JointCount; i++)
	{
		Child = NULL;
		Parent = NULL;
		ChildH = NULL;
		ParentH = NULL;

		for (int j = 0; j < PhysicModel->RigidBodyCount; j++)
		{
			if (RigidBodyIDs[j].ID == JointIDs[i].ID1) Child = RigidBodyIDs[j].RigidBody;
			if (RigidBodyIDs[j].ID == JointIDs[i].ID2) Parent= RigidBodyIDs[j].RigidBody;
		}
		if (Child) ChildH = Child->PhysicHandle;
		if (Parent) ParentH = Parent->PhysicHandle;

				
		CreateJointLocked(&PhysicModel->Joints[i], pmllib->model[0]->joint[i]->breakable, 
			pmllib->model[0]->joint[i]->breakforce, pmllib->model[0]->joint[i]->breaktorque,
			ChildH, ParentH);
	}*/
}



void WePhysics::DestroyNxJoint(NxJoint *nxjoint)
{
	Scene->releaseJoint(*nxjoint);	
}

void WePhysics::CreateJointLocked(WeJoint *joint, bool Breakable, float BreakForce, float BreakTorque, WePhysicHandle *Actor1, WePhysicHandle *Actor2)
{
	//NxFixedJointDesc fixedDesc;
	////NxD6JointDesc fixedDesc;
	//if (Actor1) fixedDesc.actor[0] = Actor1->nxActor;
	//else fixedDesc.actor[0] = NULL;

	//if (Actor2) fixedDesc.actor[1] = Actor2->nxActor;
	//else fixedDesc.actor[1] = NULL;

	//if (Breakable)
	//{
	//	fixedDesc.maxForce = BreakForce;
	//	fixedDesc.maxTorque = BreakTorque;
	//}
	//fixedDesc.jointFlags =  NX_JF_COLLISION_ENABLED;
	//joint->nxJoint = Scene->createJoint(fixedDesc);
}


WePhysicHandle* WePhysics::CreateFromPhysicModel(WePhysicModel *model, WeActor *Owner, const WeVector &Location)
{
	return NULL;
	/*if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	WePhysicHandle *Handle = new WePhysicHandle(Owner);

	NxActorDesc desc;
	desc = *model->Collection.scenes[0]->mModel->mActors[0];
    
	desc.globalPose.t = NxVec3(Location.x, Location.y, Location.z);   	
	Handle->nxActor = Scene->createActor(desc);
	if (Handle->nxActor)
	{
		if (model->SleepOnStart) Handle->nxActor->putToSleep();
		Handle->nxActor->setGroup(model->Group);
		SetGroupForShapes(Handle->nxActor, model->Group);
		Handle->SetBoxCCDSkeleton(WeBox(WeVector(-1.0f, -1.0f, -1.0f), WeVector(1.0f, 1.0f, 1.0f)));
		Handle->SetCCDThreshold(30);
	} else
	{
		return NULL;
	}
	Handle->nxActor->userData = Handle;
	return Handle;			            */
}



WePhysicHandle* WePhysics::CreateFromPhysicModel(StringParam HoleName, WeActor *Owner, const WeVector &Location)
{
	return NULL;
	//NxActorDesc desc;
	//WePhysicModel *model;


	//String s;
	//s = HoleName;
	//s = s.Extract('.', 3);
	//int index = 0;	
	//index = atoi(s.GetString());

	//if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	//WePhysicHandle *Handle = new WePhysicHandle(Owner);

	////delete model;
	//model = WePhysicModelManager::Instance()->Load(HoleName, true);	
	//desc = *model->GetActorDesc(index);
 //   
	//desc.globalPose.t = NxVec3(Location.x, Location.y, Location.z);   	
	//Handle->nxActor = Scene->createActor(desc);
	//if (Handle->nxActor)
	//{
	//	Handle->nxActor->setGroup(model->Group);	
	//	SetGroupForShapes(Handle->nxActor, model->Group);
	//	if (model->SleepOnStart) Handle->nxActor->putToSleep();
	//	Handle->SetBoxCCDSkeleton(WeBox(WeVector(-1.0f, -1.0f, -1.0f), WeVector(1.0f, 1.0f, 1.0f)));
	//	Handle->SetCCDThreshold(30);
	//	//Handle->nxActor->getShapes()[0]->setMaterial(2);
	//} else
	//{		
	//	return NULL;
	//}
	//Handle->nxActor->userData = Handle;
	//return Handle;			            
}



WePhysicHandle* WePhysics::CreateFromPhysicModel(StringParam HoleName, WeActor *Owner, const WeMatrix &Transformation)
{
	return NULL;
	//NxActorDesc desc;
	//WePhysicModel *model;


	//String s;
	//s = HoleName;
	//s = s.Extract('.', 3);
	//int index = 0;	
	//index = atoi(s.GetString());


	////delete model;
	//model = WePhysicModelManager::Instance()->Load(HoleName, true);	
	//if (!model) 
	//{
	//	return NULL;
	//}
	//desc = *model->GetActorDesc(index);

	//if (Owner) SAFE_DELETE(Owner->PhysicHandle);
	//WePhysicHandle *Handle = new WePhysicHandle(Owner);
 //   
	//WeMatrix M;

	//desc.globalPose.getColumnMajor44((float *)&M);

	//Handle->nxActor = Scene->createActor(desc);
	//if (Handle->nxActor)
	//{
	//	Handle->nxActor->setGroup(model->Group);	
	//	SetGroupForShapes(Handle->nxActor, model->Group);
	//	if (model->SleepOnStart) Handle->nxActor->putToSleep();
	//	//Handle->nxActor->getShapes()[0]->setMaterial(2);
	//	Handle->SetBoxCCDSkeleton(WeBox(WeVector(-1.0f, -1.0f, -1.0f), WeVector(1.0f, 1.0f, 1.0f)));
	//	Handle->SetCCDThreshold(30);
	//} else
	//{		
	//	return NULL;
	//}
	//M = Transformation*M;
	//Handle->SetTransformation(M);
	//Handle->nxActor->userData = Handle;
	//return Handle;			            
}


void WePhysics::MakeFromPhysicModel(StringParam Name, WeWorld *World, WeMatrix &Transformation, bool FromEditor)
{
	//if (!World) return;

	//WePhysicModel *model;
	//model = WePhysicModelManager::Instance()->Load(Name, true);
	//if (!model) return;

	//WeActor *tmpA;
	//NxActorDesc desc;
	//WeMatrix M;

	//for (int i = 0; i < model->GetActorsCount(); i++)
	//{
	//	desc = *model->GetActorDesc(i);


	//	tmpA = new WeActor;
	//	tmpA->InEditor = FromEditor;
	//	tmpA->ModelName = model->ModelNames[i];

	//	desc.globalPose.getColumnMajor44((float *)&M);

	//	if (FromEditor)
	//	{
	//		tmpA->Initialize();
	//	} else
	//	{
	//		tmpA->PhysicHandle = new WePhysicHandle(tmpA);
	//		tmpA->PhysicHandle->nxActor = Scene->createActor(desc);
	//		if (tmpA->PhysicHandle->nxActor)
	//		{
	//			tmpA->PhysicHandle->nxActor->setGroup(model->Group);
	//			SetGroupForShapes(tmpA->PhysicHandle->nxActor, model->Group);
	//			if (model->SleepOnStart) tmpA->PhysicHandle->nxActor->putToSleep();
	//			tmpA->PhysicHandle->nxActor->userData = tmpA->PhysicHandle;
	//			tmpA->PhysicHandle->SetBoxCCDSkeleton(WeBox(WeVector(-1.0f, -1.0f, -1.0f), WeVector(1.0f, 1.0f, 1.0f)));
	//			tmpA->PhysicHandle->SetCCDThreshold(30);
	//		} else
	//		{			
	//			delete tmpA;			
	//			return;
	//		}
	//		tmpA->Model     = WeModelManager::Instance()->Load(tmpA->ModelName);			

	//	}

	//	tmpA->PhysicModelName = model->Name + ".";
	//	char tmp[10];
	//	itoa(i, tmp, 10);
	//	tmpA->PhysicModelName = tmpA->PhysicModelName + tmp;

	//	M = M*Transformation;
	//	tmpA->SetTransformation(M);
	//	World->AddActor(tmpA);
	//}	
}


void WePhysics::MakeFromPhysicModel(WePhysicModel *model, WeWorld *World, WeMatrix &Transformation, bool FromEditor)
{
	/*
	if (!World) return;
	if (!model) return;

    WeActor *tmpA;
	NxActorDesc desc;
	WeMatrix M;

	for (int i = 0; i < model->GetActorsCount(); i++)
	{
		desc = *model->GetActorDesc(i);
		
        tmpA = new WeActor;
		tmpA->InEditor = FromEditor;
		tmpA->ModelName = model->ModelNames[i];

		desc.globalPose.getColumnMajor44((float *)&M);
		if (FromEditor)
		{
			tmpA->Initialize();
		} else
		{
			tmpA->PhysicHandle = new WePhysicHandle(tmpA);
			tmpA->PhysicHandle->nxActor = Scene->createActor(desc);			
			if (tmpA->PhysicHandle->nxActor)
			{
				tmpA->PhysicHandle->nxActor->setGroup(model->Group);
				SetGroupForShapes(tmpA->PhysicHandle->nxActor, model->Group);
				if (model->SleepOnStart) tmpA->PhysicHandle->nxActor->putToSleep();
				tmpA->PhysicHandle->nxActor->userData = tmpA->PhysicHandle;
				tmpA->PhysicHandle->SetBoxCCDSkeleton(WeBox(WeVector(-1.0f, -1.0f, -1.0f), WeVector(1.0f, 1.0f, 1.0f)));
				tmpA->PhysicHandle->SetCCDThreshold(30);

			} else
			{			
				delete tmpA;			
				return;
			}

			tmpA->Model     = WeModelManager::Instance()->Load(tmpA->ModelName);			
		}
		M = Transformation*M;
		tmpA->SetTransformation(M);
		World->AddActor(tmpA);
	}	*/

}



void WePhysics::SaveSceneStats(StringParam Name)
{
	if (!Initialized) return;

	NxSceneStats stats;
	Scene->getStats(stats);
	FILE *f;
	f = fopen(Name, "wt+");

	
	fprintf(f, "Max Actors: %d\n", stats.maxActors);
	fprintf(f, "maxAxisConstraints: %d\n", stats.maxAxisConstraints);
	fprintf(f, "maxContacts: %d\n", stats.maxContacts);
	fprintf(f, "maxDynamicActors: %d\n", stats.maxDynamicActors);
	fprintf(f, "maxDynamicActorsInAwakeGroups: %d\n", stats.maxDynamicActorsInAwakeGroups);
	fprintf(f, "maxDynamicShapes: %d\n", stats.maxDynamicShapes);
	fprintf(f, "maxJoints: %d\n", stats.maxJoints);
	fprintf(f, "maxPairs: %d\n", stats.maxPairs);
	fprintf(f, "maxSolverBodies: %d\n", stats.maxSolverBodies);
	fprintf(f, "maxStaticShapes: %d\n", stats.maxStaticShapes);
	fprintf(f, "numActors: %d\n", stats.numActors);
	fprintf(f, "numAxisConstraints: %d\n", stats.numAxisConstraints);
	fprintf(f, "numContacts: %d\n", stats.numContacts);
	fprintf(f, "numDynamicActors: %d\n", stats.numDynamicActors);
	fprintf(f, "numDynamicActorsInAwakeGroups: %d\n", stats.numDynamicActorsInAwakeGroups);
	fprintf(f, "numDynamicShapes: %d\n", stats.numDynamicShapes);
	fprintf(f, "numJoints: %d\n", stats.numJoints);
	fprintf(f, "numPairs: %d\n", stats.numPairs);
	fprintf(f, "numSolverBodies: %d\n", stats.numSolverBodies);
	fprintf(f, "numStaticShapes: %d\n", stats.numStaticShapes);
	fprintf(f, "numClothes: %d\n", Scene->getNbCloths());

	fflush(f);
	fclose(f);
}

bool WePhysics::IsHardwareAvailable()
{
	if (!Initialized) return false;
	return PhysicsSDK->getHWVersion() != NX_HW_VERSION_NONE;    
}


void WePhysics::SetGroupForShapes(NxActor *Actor, WORD Group)
{
	for (int i = 0; i < (int)Actor->getNbShapes(); i++)
	{
		Actor->getShapes()[i]->setGroup(Group);
	}
}
