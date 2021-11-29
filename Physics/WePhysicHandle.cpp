/*
    WePhysicHandle.cpp

	Autor: Nebojsa Vislavski
	Date: 6.10.2005
	Version: 1.0
	Platform: Unix, Win32

	This is main physic handle used for actors
    
*/

#include <WePhysicHandle.h>
#include <NxPhysics.h>
#include <NxController.h>
#include <WePhysics.h>
#include <WeActor.h>

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////

WePhysicHandle::WePhysicHandle(WeActor *parent)
{
	Parent = parent;	
	nxActor = NULL;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::GetTransformation(WeMatrix &outMatrix)
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
//         Function Name    :  SetTransformation
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  NOT IMPLEMENTED YET
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetTransformation(WeMatrix &TM)
{
	NxMat34 nxm;

	nxm.setColumnMajor44(TM);

	if (nxActor->readBodyFlag(NX_BF_KINEMATIC))
	{
		nxActor->moveGlobalPose(nxm);
	} else
	{
		nxActor->setGlobalPose(nxm);
	}		
}

void WePhysicHandle::SetLocation(const WeVector &Location)
{
	if (!nxActor) return;
	NxVec3 loc;
    loc.x = Location.x; loc.y = Location.y; loc.z = Location.z;

	if (nxActor->readBodyFlag(NX_BF_KINEMATIC))
	{
		nxActor->moveGlobalPosition(loc);
	}
	else
	{
		nxActor->setGlobalPosition(loc);
	}
}


void WePhysicHandle::SetOrientation(float Yaw, float Pitch, float Roll)
{
	if (!nxActor) return;

	WeMatrix M;
	D3DXMatrixRotationYawPitchRoll(&M, Yaw, Pitch, Roll);
	
	NxMat33 nxm;
	
	nxm.setColumnMajorStride4(M);

	if (nxActor->readBodyFlag(NX_BF_KINEMATIC) || nxActor->isDynamic())
	{
		nxActor->moveGlobalOrientation(nxm);
	} else nxActor->setGlobalOrientation(nxm);

	
}

void WePhysicHandle::GetLocation(WeVector &outLocation)
{
	NxVec3 loc;
	loc = nxActor->getGlobalPosition();
	outLocation.x = loc.x; outLocation.y = loc.y; outLocation.z = loc.z; 
}


void WePhysicHandle::GetOrientationQuat(WeQuaternion &Quat)
{
	NxQuat quat;
	quat = nxActor->getGlobalOrientationQuat();
    Quat.x = quat.x;
	Quat.y = quat.y;
	Quat.z = quat.z;
	Quat.w = quat.w;
}

void WePhysicHandle::SetOrientationQuat(WeQuaternion &Quat)
{
	NxQuat quat;
	quat.x = Quat.x;
	quat.y = Quat.y;
	quat.z = Quat.z;
	quat.w = Quat.w;

	if (nxActor->readBodyFlag(NX_BF_KINEMATIC))
	{
		nxActor->moveGlobalOrientationQuat(quat);
	} else nxActor->setGlobalOrientationQuat(quat);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetTransformation
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  NOT IMPLEMENTED YET
//
//////////////////////////////////////////////////////////////////////////
WePhysicHandle::~WePhysicHandle()
{
	if (nxActor)
	{
		nxActor->userData = 0;
		WePhysics::Instance()->DestroyNxActor(nxActor);		
	}
}

void WePhysicHandle::Destroy()
{ 
	WePhysics::Instance()->DestroyNxActor(nxActor);		
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetCollision
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetCollision(bool Collision)
{
	if (!Collision)
		nxActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
	else
		nxActor->clearActorFlag(NX_AF_DISABLE_COLLISION);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetCollision
//         Input Parametars :  
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool WePhysicHandle::GetCollision()
{
	return !(NX_AF_DISABLE_COLLISION == nxActor->readActorFlag(NX_AF_DISABLE_COLLISION));
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetResponse
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will shut off all responses by object
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetResponse(bool Response)
{
	if (!Response)
		nxActor->raiseActorFlag(NX_AF_DISABLE_RESPONSE);
	else
		nxActor->clearActorFlag(NX_AF_DISABLE_RESPONSE);
	
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetRacasting
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will shut off all responses by raycasting
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetRaycasting(bool Raycasting)
{
	for (int i = 0; i < (int)nxActor->getNbShapes(); i++)
	{
		nxActor->getShapes()[i]->setFlag(NX_SF_DISABLE_RAYCASTING, !Raycasting);
	}			
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetCollision
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::AddImpulse(const WeVector &Impulse)
{
	NxVec3 vec;
	vec.x = Impulse.x;
	vec.y = Impulse.y;
	vec.z = Impulse.z;

	nxActor->addForce(vec, NX_IMPULSE);	
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetCollision
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::AddVelocity(const WeVector &Velocity)
{
	NxVec3 vec;
	vec.x = Velocity.x;
	vec.y = Velocity.y;
	vec.z = Velocity.z;

	nxActor->addForce(vec, NX_VELOCITY_CHANGE);

}

void WePhysicHandle::SetLinearVelocity(WeVector &Velocity)
{
	NxVec3 vec;
	vec.x = Velocity.x;
	vec.y = Velocity.y;
	vec.z = Velocity.z;

	nxActor->setLinearVelocity(vec);
}

void WePhysicHandle::GetLinearVelocity(WeVector &Velocity)
{
	NxVec3 vec;
	vec = nxActor->getLinearVelocity();
	Velocity.x = vec.x;
	Velocity.y = vec.y;
	Velocity.z = vec.z;
}


void WePhysicHandle::PutToSleep()
{
	nxActor->putToSleep();
}

void WePhysicHandle::AddAngularVelocity(const WeVector &Velocity)
{
	NxVec3 vec;
	vec.x = Velocity.x;
	vec.y = Velocity.y;
	vec.z = Velocity.z;

	nxActor->addTorque(vec, NX_VELOCITY_CHANGE);
}


void WePhysicHandle::SetAngularVelocity(const WeVector &Velocity)
{
	NxVec3 vec;
	vec.x = Velocity.x;
	vec.y = Velocity.y;
	vec.z = Velocity.z;

	nxActor->setAngularVelocity(vec);
}


void WePhysicHandle::AddBox(WeVector &position, WeVector &dimensions, WORD TriggerType)
{
	NxBoxShapeDesc boxd;
	boxd.dimensions.x = dimensions.x;
	boxd.dimensions.y = dimensions.y;
	boxd.dimensions.z = dimensions.z;
	boxd.localPose.t.x = position.x;
	boxd.localPose.t.y = position.y;
	boxd.localPose.t.z = position.z;
	boxd.shapeFlags |= TriggerType;		
	
	nxActor->createShape(boxd);
	nxActor->updateMassFromShapes(Density, 0);
}

void WePhysicHandle::AddSphere(WeVector &position, float radius, WORD TriggerType)
{
	NxSphereShapeDesc boxd;
	boxd.radius = radius;
	boxd.localPose.t.x = position.x;
	boxd.localPose.t.y = position.y;
	boxd.localPose.t.z = position.z;
	boxd.shapeFlags |= TriggerType;		

	nxActor->createShape(boxd);
	nxActor->updateMassFromShapes(Density, 0);
}

void WePhysicHandle::AddCapsule(WeVector &position, float radius, float height, WORD TriggerType)
{
	NxCapsuleShapeDesc boxd;
	boxd.radius = radius;
	boxd.height = height;
	boxd.localPose.t.x = position.x;
	boxd.localPose.t.y = position.y;
	boxd.localPose.t.z = position.z;
	boxd.shapeFlags |= TriggerType;		

	nxActor->createShape(boxd);
	nxActor->updateMassFromShapes(Density, 0);
}


void WePhysicHandle::SetDisableGravity(bool Gravity)
{
	if (Gravity)
		nxActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	else
		nxActor->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
}

bool WePhysicHandle::GetDisableGravity()
{
	return nxActor->readBodyFlag(NX_BF_DISABLE_GRAVITY);
}

void WePhysicHandle::SetKinematic(bool Kinematic)
{
	if (Kinematic)
		nxActor->raiseBodyFlag(NX_BF_KINEMATIC);
	else
		nxActor->clearBodyFlag(NX_BF_KINEMATIC);
}

bool WePhysicHandle::GetKinematic()
{
	return nxActor->readBodyFlag(NX_BF_KINEMATIC);
}


void WePhysicHandle::SetFrozen(bool Frozen)
{
	if (Frozen)
		nxActor->raiseBodyFlag(NX_BF_FROZEN);
	else
		nxActor->clearBodyFlag(NX_BF_FROZEN);
}


bool WePhysicHandle::GetFrozen()
{
	return nxActor->readBodyFlag(NX_BF_FROZEN);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetRacasting
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will shut off all responses by raycasting
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetBoxCCDSkeleton(WeBox &Box)
{
	if (!nxActor) return;

	NxBox obb; //NxBox(NxVec3(0.0f,0.0f,0.0f),,);
	obb.center = NxVec3(0,0,0);
	obb.extents.x = (Box.Max.x - Box.Min.x) / 2.0f;
	obb.extents.y = (Box.Max.y - Box.Min.y) / 2.0f;
	obb.extents.z = (Box.Max.z - Box.Min.z) / 2.0f;
    obb.rot = NxMat33(NX_IDENTITY_MATRIX);

	int nShapes;

	nShapes = nxActor->getNbShapes();
	NxShape *const *shapes;
	shapes = nxActor->getShapes();

	NxSimpleTriangleMesh triMesh;

			
    triMesh.points=new NxVec3[8];            
	triMesh.numVertices=8;           
	triMesh.pointStrideBytes=sizeof(NxVec3);           
	triMesh.numTriangles=2*6;          
	triMesh.triangles=new NxU32[2*6*3];         
	triMesh.triangleStrideBytes=sizeof(NxU32)*3;       
	triMesh.flags=0;

	NxComputeBoxPoints(obb,(NxVec3 *)triMesh.points);
    memcpy((NxU32 *)triMesh.triangles,NxGetBoxTriangles(),sizeof(NxU32)*2*6*3);    


	NxCCDSkeleton *CCDSkeleton;
	CCDSkeleton = WePhysics::Instance()->GetSDK()->createCCDSkeleton(triMesh);


	int i;
	NxShape *shape;

	for (i = 0; i < nShapes; ++i)
	{		
		shape = shapes[i];		
		shape->setCCDSkeleton(CCDSkeleton);		        
		shape->setFlag(NX_SF_DYNAMIC_DYNAMIC_CCD, true);
	}

	delete[] triMesh.points;
	delete[] triMesh.triangles;	
	WePhysics::Instance()->GetSDK()->releaseCCDSkeleton(*CCDSkeleton);

}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetRacasting
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will shut off all responses by raycasting
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetCCDThreshold(float Velocity)
{
	if (!nxActor) return;

	nxActor->setCCDMotionThreshold(Velocity);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetRacasting
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will shut off all responses by raycasting
//
//////////////////////////////////////////////////////////////////////////
void WePhysicHandle::SetCollisionGroup(WORD Group)
{
	if (!nxActor) return;

	int nShapes = nxActor->getNbShapes();
	NxShape *const *shapes;
	shapes = nxActor->getShapes();

	for (int i = 0; i < nShapes; i++)
	{
		NxShape *shape;
		shape = shapes[i];
		shape->setGroup(Group);
	}
}



