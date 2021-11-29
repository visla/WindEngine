/*
      File: WeActor.h
	  Author: Tomislav Kukic
	  Date: 13.09.2005.
	  Version: 1.0
*/





#ifndef _WEACTOR
#define _WEACTOR

#include <WeVector.h>
#include <WeMatrix.h>
#include <WindEngine.h>
#include <WePhysicHandle.h>
#include <WeSphere.h>
#include <WeBox.h>
#include <WeFileStream.h>
#include <WeLightMap.h>
#include <WePhysicModel.h>


class WeWorld;
class WeStaticModel;
class WePhysicModel;
class WeCamera;
class WeModel;
class WePhysicHandle;
class WeRenderTarget;
class WeZone;
class WeFrustum;
class WeSound;

#define WETYPE_ACTOR			-1
#define WETYPE_LIGHT			-2
#define WETYPE_PREFAB			-3
#define WETYPE_NETWORKACTOR		-4
#define WETYPE_PARTICLESYSTEM	-5
#define WETYPE_PATHNODE			-6
#define WETYPE_SKYBOX			-7
#define WETYPE_TRAIL			-8
#define WETYPE_CLOTH			-9
#define WETYPE_ZONE				-10
#define WETYPE_PORTAL			-11
#define WETYPE_RADIALTRAIL		-12



class WINDENGINE_API WeActor
{
friend class WeWorld;
friend class WeLightMap;

protected:
	WeWorld				*World;		  // one actor can only be in one world	
	DWORD				LastRenderedFrame;
	int					ShadowLightsAffecting; // how many lights are affecting this and dropping shadow

	void				TickPrefab();

public:
						WeActor();
						//WeActor(WeWorld* WorldToBeIn);
	virtual				~WeActor(void);

	void				initialize();

	WeLightMap			LightMap;  // This is lightmap that this actor uses


	int					Version;	// Version of WeActor. Default is 1
	bool				Serializable;   // this object will be saved
	bool				InEditor;
	bool				IsSelection;
	WeActor*			NextActor;   // NOTE! Dont mess with these. For Lists. Dont read or write to these
	WeActor*			PrevActor;   // NOTE! Dont mess with these. For Lists. Dont read or write to these
	// Maps used for actor TODO: Add shadowMap
	WeRenderTarget*		EnvMap; // this is environment map for actor if he needs that	
	WeZone*				CurrentZone; // what is current zone this object is in

	// your custom data
	DWORD				Tag;
	int					Type;
	bool				AutoUpdateTransformations; // when you dont have physic handle should tm be created in tick. default is true. If set to false you must manualy set TM
	DWORD				EngineTag; // some engine markings. Dont use this cause engine may rewrite it
	WeVector			Location; 
	WeVector			Direction;
	WeVector			Velocity;
	WeVector			AngularVelocity;
	// dont write or read from these
	WeVector			OldLocation; 
	WeVector			OldDirection;
	WeVector			OldVelocity;
	// Physics Data
	WePhysicHandle		*PhysicHandle;	
	WeMatrix			TM; // transformation matrix
	WeMatrix			TMWithScale; // transformation matrix with scaling
	WeMatrix			PrevTM;
	int					MaterialPreset;
	bool				UseMaterialParams;  // should it set MaterialParam1, .. 4 for material instead of normal material params. It will override all materials params that this actor has, if it has visible model 
	float				MaterialParam1;
	float				MaterialParam2;
	float				MaterialParam3;
	float				MaterialParam4;
	WeColor				MaterialColor;

	// Sounding
	WeSound				*Sound;       // this is ambient sound played always 
	String				SoundName;
	float				SoundDistance; 



	// shadowing
	bool				CastShadowVolume;
	bool				ReceiveShadowVolume;
	bool				ReceiveProjectors;  // should this actor receive projectors
	bool				LightOccluder;
	float				MaxRenderDistance; // default is 0. which means infinite

	String				ModelName;			// Presentation for model
	String				PhysicModelName;    // Presentation for PhysicModel
	String				Name;				// this is name of object used for different object relations 
	// Teleporters and triggers
	bool				Teleport;
	bool				TeleportRelative; // this means that locations is relative to arrival point
	String				TeleportDestination;  // what is destination object name. If it exists it overrides all other teleport options
	bool				IsTrigger;


	float				Yaw;
	float				Pitch;
	float				Roll;
	// dont write or read from these
	float				OldYaw;
	float				OldPitch;
	float				OldRoll;
	WeVector			Scale;	
	// Aging and life properties
	float				LifeTime;			// In seconds before Actor is destroyed. 0 - for eternity
	float				ObjectAge;
	bool				AutoDestroy;

	virtual String		GetInfo() { return ""; }

	WeSphere			BSphere;		// bounding sphere - this is custom data. You set this and use this
	WeBox				BBox;			// bounding box - this is mainly user data. You set this and use this	

	bool				WantLight;			// def. true - if you want this object to find lights or to be illuminated
	bool				GhostObject;		// It is visible but not interactive
	bool				StaticObject;		// It is static object like map decoration
	bool				PassFlares;			// true if flares are visible through this actor (default: false)
	// Visibility checks
	// This is Time this object is occluded in seconds. If 0 then object is not occluded. 
	// Counted in Actor->tick and reseted in BatchRenderer
	float				OccludedTime;			
	// NEVER SET THIS 	
	bool				IsLight; 
	bool				IsParticleSystem; 
	bool				IsCloth;  	
	bool				IsPrefab;			// Is this prefab, NEVER SET THIS
	bool				IsNetworkActor;		// NEVER SET THIS	
	bool				Renderable;		// is this model renderable. On server side it is not renderable
	bool				Editor_Renderable; // is this model renderable in editor
	bool				HasTicked;			// if this object has ticked already. Don`t modify this 			

	WeStaticModel*		Model;
	WePhysicModel		PhysicModel;	 // model describing physic properties. Default type is no physics	

	virtual void		SetWorld(WeWorld* World);
	WeWorld*			GetWorld() { return World; }
	virtual bool		IntersectFrustum(WeFrustum *frustum); // check to see if this object is in frustum
	// Call this reposition you object. If you use Physic handle than call this very rarely	
	void				MoveTo(WeVector &newlocation);
	void				SetScaling(WeVector &scale);
	void				SetOrientation(float Yaw, float Pitch, float Roll);
	void				SetTransformation(WeMatrix &NewTransform);
	void				GetTransformation(WeMatrix &outM);
	void				GetOrientation(float &Yaw, float &Pitch, float &Roll);
	void				GetLocalAxis(WeVector &XAxis, WeVector &YAxis, WeVector &ZAxis);
	virtual void		OnTeleported();
	// ----------------------------------------------------------------

	virtual void		Tick();
	// Saves objects state
	virtual void		Serialize(WeFileStream &FS);
	// Loads objects state and initialize object. Calls SetTransformation after loading
	virtual void		Deserialize(WeFileStream &FS);

	// This initialize will make his model, and physic model like his query mesh
	// NOTE! This doesnt adds actor to world
	virtual void		Initialize();
	virtual void		InitializeSound();

	// this will create new object and make him have the same properties.
	// Aftet this it will call Initialize
	// NOTE! You have to create Clone object yourself and they have to be same type
	// if they are not same type clone will do nothing
	// Call this function in editor. 
	virtual void		Clone(WeActor *Destination, bool DeepCopy = false); 
	virtual void		OnRender(WeCamera* Cam);
	virtual void		PreRender(WeCamera* Cam);
	virtual void		PostRender(WeCamera* Cam);
			void		ReInsertIntoWorld(); // this will reinsert object into world. This is called when changing IsPrefab

	void				Editor_Show(); // this will show object if in editor
	void				Editor_Hide(); // this will hide object if in editor and disable collision

	float				GetFinalAge(); // total object age

	virtual void		destroy();
};




#endif
