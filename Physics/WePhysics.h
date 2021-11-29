/*
     WePhysics.h

	 Autor: Nebojsa Vislavski
	 Date: 6.10.2005.
	 Version: 1.0

	 This is physic engine class

*/


#ifndef _WEPHYSICS
#define _WEPHYSICS



#include <WeVector.h>
#include <WeMatrix.h>
#include <WeSphere.h>
#include <WindEngine.h>
#include <NxPhysics.h>
#include <ControllerManager.h>
#include <NxController.h>
#include <WeRaycastHit.h>
#include <CString.h>
#include <WePhysicTriangleMesh.h>
#include <WePhysicClothMesh.h>

class WeWorld;
class WeStaticModel;
class WePhysicHandle;
class WeControllerHandle;
class WePhysicModel;
class WeActor;
class WeRigidBody;
class WeJoint;
class Library;
class WeCloth;


#define WE_COLLIOSION_DOWN NXCC_COLLISION_DOWN

#define WE_PHYSICHANDLE_STATIC 0
#define WE_PHYSICHANDLE_DYNAMIC 1
#define WE_PHYSICHANDLE_KINEMATIC 2





enum WeShapesType
{
	WE_SHAPE_TYPE_DYNAMIC = NX_DYNAMIC_SHAPES,
	WE_SHAPE_TYPE_STATIC = NX_STATIC_SHAPES,
	WE_SHAPE_TYPE_ALL = NX_ALL_SHAPES
};



// Local structures needed for physic model creation

struct SRigidBodyID
{
	String ID;
	WeRigidBody *RigidBody;
};

struct SJointIDs
{
	String ID1;
	String ID2;
};

#define DUMMYSHAPE_GROUP	 1

#define WECOLLISION_DISABLE -1
#define WECOLLISION_ON_START_TOUCH NX_NOTIFY_ON_START_TOUCH
#define WECOLLISION_ON_END_TOUCH NX_NOTIFY_ON_END_TOUCH
#define WECOLLISION_ON_TOUCH NX_NOTIFY_ON_TOUCH
#define WECOLLISION_ON_ALL NX_NOTIFY_ALL

#define WETRIGGER_ON_ENTER NX_TRIGGER_ON_ENTER
#define WETRIGGER_ON_LEAVE NX_TRIGGER_ON_LEAVE
#define WETRIGGER_ON_ENABLE NX_TRIGGER_ENABLE
#define WETRIGGER_ON_STAY NX_TRIGGER_ON_STAY


// ---------------------------------------------------
typedef void (*PHYSICS_COLLISIONCALLBACK)(WeActor *Actor1, WeActor *Actor2, WeVector &Normal, WeVector &Tangent, DWORD Flag, WeVector &FirstTouch);
typedef void (*PHYSICS_TRIGGERCALLBACK)(WeActor *TriggerActor, WeActor *OtherActor, DWORD Flag);
typedef void (*PHYSICS_TELEPORTREPORT)(WeActor *Actor1, WeActor *Destination);





class WINDENGINE_API WePhysics
{
friend class WeEngine;
friend class WePhysicModelManager;

private:
	static WePhysics *_instance;

	class TriggerCallback : public NxUserTriggerReport
	{
	public:
		static TriggerCallback  Instance;
		void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
	};

	class CollisionCallback: public NxUserContactReport
	{
	public:
		static CollisionCallback Instance;
		void onContactNotify(NxContactPair& Pair, NxU32 events);
	};



protected:
	bool							Initialized;
	NxPhysicsSDK					*PhysicsSDK;
	NxScene							*Scene;
	NxVec3							Gravity;
	ControllerManager				CM;   // Character controller for our characters


									WePhysics();
	void							UpdateControllers();
	void							MakePhysicModelJoints(WePhysicModel *PhysicModel, Library *pmllib, SJointIDs *JointIDs, SRigidBodyID *RigidBodyIDs); // just support method for CreateModel from PML

	void							SetGroupForShapes(NxActor *Actor, WORD Group);

	PHYSICS_COLLISIONCALLBACK		OnCollision;
	PHYSICS_TRIGGERCALLBACK			OnTrigger;
	PHYSICS_TELEPORTREPORT			OnTeleport;

	CSmartList <WePhysicTriangleMesh *> TriangleMeshes;
	CSmartList <WePhysicClothMesh *>	ClothMeshes;

public:
    static WePhysics				*Instance();



	bool							Initialize();
	void							ShutDown();
	bool							IsHardwareAvailable();

	void							SetGroupCollision(WORD Group1, WORD Group2, WORD CollisionType);
	void							SetGroupCollisionInteract(WORD Group1, WORD Group2, bool Enable);
	void							CreateBoxController(WeControllerHandle *Handle, WeVector &Position, WeVector &Extents, float SlopeLimitDeg, float StepOffset, float SkinWidth = 0.1f);
	void							CreateCapsuleController(WeControllerHandle *Handle, WeVector &Position, float Radius, float Height, float SlopeLimitDeg, float StepOffset, float SkinWidth = 0.1f);
	int								MoveController(WeControllerHandle *Handle, WeVector &Disp, float HeightDelta);
	
	WePhysicHandle*					CreateGroundPlane(WeActor *Owner);
	WePhysicHandle*					CreateBox(WeActor *Owner, const WeVector &Location, const WeVector &Dimensions, WORD Group, float Mass, bool Static, WORD TriggerType = 0, bool UseCCD = false);
	WePhysicHandle*					CreateBox(WeActor *Owner, const WeMatrix &TM, const WeVector &Dimensions, WORD Group, float Mass, bool Static, WORD TriggerType = 0, bool UseCCD = false);

	WePhysicHandle*					CreateSphere(WeActor *Owner, const WeVector &Location, float Radius, WORD Group, float Mass, bool Static, WORD TriggerType = 0);
	WePhysicHandle*					CreateCapsule(WeActor *Owner, const WeVector &Location, float Radius, float Height, WORD Group, float Mass, bool Static, WORD TriggerType = 0);

	// This methods are used to create more complicated physic models
	WePhysicHandle*					CreateFromPhysicModel(WePhysicModel *model, WeActor *Owner, const WeVector &Location);
	// Physic model name consists of PackageName.Group.Name.Index
	WePhysicHandle*					CreateFromPhysicModel(StringParam PhysicModelName, WeActor *Owner, const WeVector &Location);
	WePhysicHandle*					CreateFromPhysicModel(StringParam PhysicModelName, WeActor *Owner, const WeMatrix &Transformation);

	// this will create actors and joints from physic model and place them somewhere
	// these methods are mainly called from editor when you are placing some combination of objects
	void							MakeFromPhysicModel(WePhysicModel *model, WeWorld *World, WeMatrix &Transformation, bool FromEditor = false);
	void							MakeFromPhysicModel(StringParam Name, WeWorld *World, WeMatrix &Transformation, bool FromEditor = false);

	// Joints creation
	// Joint param must be created and this method will fill them
	void							CreateJointLocked(WeJoint *joint, bool Breakable, float BreakForce, float BreakTorque, WePhysicHandle *Actor1, WePhysicHandle *Actor2);
	void							DestroyNxJoint(NxJoint *nxjoint);

	// Creates valid physic handle from supplied collision model 
	// if you supply 0 for density than make static mesh
	void							CreateFromPML(WePhysicModel *PhysicModel, WeActor *Owner, WeVector &Location, StringParam filename);
	WePhysicHandle*					CreateTriangleMesh(WeActor *Owner, const WeVector &Location, WeStaticModel *CollisionModel, float Density = 0, int Type = WE_PHYSICHANDLE_STATIC);
	WePhysicHandle*					CreateTriangleMesh(WeActor *Owner, const WeMatrix &TM, WeStaticModel *CollisionModel, float Density = 0, int Type = WE_PHYSICHANDLE_STATIC);
	WePhysicHandle*					CreateCloth(WeCloth *Owner);

	void							DestroyNxActor(NxActor *nxactor);	
	// returns false if no object is hit by raycasting  	
	bool							RaycastClosest(WeVector &Location, WeVector &Direction, WeShapesType ShapeType, WeRaycastHit &Hit, int Group, float MaxDist, int HintFlags = 0xffffffff);
	// this will set default params for our physic engine. Override this if you want
	// to set different params instead of defaults
	virtual void					SetDefaultParams(); 	
	void							BeginUpdate(); // update physics from this Begin world. 
	void							EndUpdate(); // end update physics from this world
	void							GetWorldMatrix(NxActor *nxActor, WeMatrix &outMatrix);
	void							SetGravity(WeVector &gravity);
	WeVector						GetGravity();
	NxScene							*GetScene() { return Scene; }
	NxPhysicsSDK					*GetSDK() { return PhysicsSDK; } 

	void							SaveSceneStats(StringParam Name);

	void							SetCollisionCallback(PHYSICS_COLLISIONCALLBACK Callback);
	void							SetTriggerCallback(PHYSICS_TRIGGERCALLBACK Callback); 
	void							SetTeleportReport(PHYSICS_TELEPORTREPORT Callback); 
	
};


#endif
