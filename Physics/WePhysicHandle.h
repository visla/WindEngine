/*
    WePhysicHandle.h

	Autor: Nebojsa Vislavski
	Date: 6.10.2005
	Version: 1.0
	Platform: Unix/Win32

	This is main physic handle used for actors. For Unix get self Linux Novodex Libs
    
*/

#ifndef _WEPHYSICHANDLE_H
#define _WEPHYSICHANDLE_H


#include <WeSphere.h>
#include <WindEngine.h>
#include <NxPhysics.h>
#include <WeMatrix.h>
#include <WeQuaternion.h>
#include <WeBox.h>

class WeActor;



class WINDENGINE_API WePhysicHandle
{
friend class WePhysics;

protected:

	WePhysicHandle(): Parent(NULL), nxActor(NULL) {}; // not to be called

public:
	NxActor *nxActor;
	float	Density;


	WePhysicHandle(WeActor *parent);
	virtual ~WePhysicHandle();


	virtual void		AddImpulse(const WeVector &Impulse);
	virtual void		AddVelocity(const WeVector &Velocity);
	virtual void		AddAngularVelocity(const WeVector &Velocity);
	virtual void		SetAngularVelocity(const WeVector &Velocity);
	virtual void		GetTransformation(WeMatrix &outMatrix);
	virtual void		SetTransformation(WeMatrix &TM);
			void		SetOrientationQuat(WeQuaternion &Quat);
	virtual void		GetOrientationQuat(WeQuaternion &Quat);
	virtual void		Destroy();
	virtual void		SetCollision(bool Collision);
	virtual bool		GetCollision();
			void		SetResponse(bool Response);
			void		SetRaycasting(bool Raycasting);
			void		SetCCDThreshold(float Velocity);
			void		SetBoxCCDSkeleton(WeBox &box); // this will set ccd skeleton for physic handle. It will be box-like
			void		SetCollisionGroup(WORD Group);

	virtual void		SetLocation(const WeVector &Location);
	virtual void		SetOrientation(float Yaw, float Pitch, float Roll);
	virtual void		GetLocation(WeVector &outLocation);
	virtual void		OnContactNotify(WePhysicHandle *other, WeVector &NormalForce) {}
			void		SetLinearVelocity(WeVector &Velocity);
			void		GetLinearVelocity(WeVector &Velocity);
			void		PutToSleep();


	// TODO: Implement this
	// methods for shape modification						
			void		AddSphere(WeVector &position, float radius, WORD TriggerType = 0);
			void		AddBox(WeVector &position, WeVector &Dimensions, WORD TriggerType = 0);
			void		AddCapsule(WeVector &position, float radius, float height, WORD TriggerType = 0);
			void		AddConvexMesh(WeVector &position, void *vertices, int verticesCount, void *indices = NULL, int IndicesCount = 0);
			void		AddTriMesh(WeVector &position, void *vertices, int VerticesCount, void *indices = NULL, int IndicesCount = 0);

	// Flags
			void		SetDisableGravity(bool Gravity);
			bool		GetDisableGravity();
			void		SetFrozen(bool Frozen);
			bool		GetFrozen();
			void		SetKinematic(bool Kinematic);
			bool		GetKinematic();

	WeActor *Parent;
};



#endif
