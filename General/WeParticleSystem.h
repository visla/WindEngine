/*

     WeParticleSystem.h

	 Autor: Nebojsa Vislavski
	 Date: 8.11.2005.
	 Version:1.0

	 This is main particle system class. Use this and if you want you can inherit it.
	 You should almost always set BSphere radius or you will suffer great performance penality

*/

#ifndef _WEPARTICLESYSTEM_H
#define _WEPARTICLESYSTEM_H


#include <WeActor.h>
#include <WeTexture.h>
#include <WeParticle.h>
//#include <list>
#include <WindEngine.h>
#include <WeMaterial.h>
#include <WePlane.h>
#include <WeFileStream.h>

//using namespace std;

#define WE_PARTICLE_SPAWNTYPE_POINT 0
#define WE_PARTICLE_SPAWNTYPE_LINE  1
#define WE_PARTICLE_SPAWNTYPE_HORIZPLANE 2
#define WE_PARTICLE_SPAWNTYPE_SPHERE 3


// used for defining direction when spawning
#define WE_PARTICLE_MOVETYPE_NONE		 0  // not movable particles	
#define WE_PARTICLE_MOVETYPE_DIRECTIONAL 1  // in one line
#define WE_PARTICLE_MOVETYPE_SPHERICAL	 2  // like fireworks explosion
#define WE_PARTICLE_MOVETYPE_CHAOTIC	 3  // changing direction always


enum WEPARTICLE_BLEND_TYPE
{
	WEPARTICLE_BLEND_ADDITIVE,
	WEPARTICLE_BLEND_STANDARD
};

enum WEPARTICLE_ANGLE_TYPE
{
	WEPARTICLE_ANGLE_ALIGN_TO_MOTION,
	WEPARTICLE_ANGLE_RANDOM,
	WEPARTICLE_ANGLE_SPECIFY	
};

enum WEPARTICLE_ANIM_TYPE
{
	WEPARTICLE_ANIM_NONE,
	WEPARTICLE_ANIM_RANDOM,
	WEPARTICLE_ANIM_ANIM,
};



class WINDENGINE_API WeParticleSystem: public WeActor
{
protected:

	void		AddParticle(int index);	
	float		NumToSpawn;

public:

	class SProperties
	{
	public:
		struct  ColorState
		{
			WeColor Color;
			float	GradientPoint;  // between 0..1
		};

		SProperties();

		void		Serialize(WeFileStream &FS);
		void		Deserialize(WeFileStream &FS);
		
		int			Version;								// properties version
		int			MaxParticles;
		int			ParticleCount;							// how many particles we have now	
		
		int			EmitRate;								// particles per second
		int			EmitRateRandomness;						// variable emitting per second

		int			SpawnType;								// how particles are supposed to create
		int			MoveType;								// how particles are supposed to move
		// params like dimension, radius, width, height for different spawn types
		float		SpawnParam1;
		float		SpawnParam2;
		float		SpawnParam3;
		float		SpawnParam4;
		float		VisibleSphereRadius;

		bool		RelativePosition;						// this identifies if particles are in relative position with particle emmiter. 

		// Deprecated
		float		SpawnVolumeSize;						// this is radius if spawn from sphere 
		WeVector	SpawnLine;								// if it uses spawn line. this is vector from location on which particles are spawn
		float		SpawnPlaneWidth;
		float		SpawnPlaneHeight;
		// --------------

		// visual representations params	
		WEPARTICLE_BLEND_TYPE  BlendType;
		WeMaterial*		Material;						// this is material used for particles. It has shader and textures
		String			MaterialName;
		CSmartList <ColorState> Colors;				// how colors are changed in time
		WeColor			ColorFrom;
		WeColor			ColorTo;

		float	  SpinFrom;
		float	  SpinTo;
		float	  SpinRandomness;  
		bool	  SpinningInLife;

		float     AlphaFrom;
		float     AlphaTo;
		float	  AlphaRandomness;

		// other params
		float     SizeFrom;							// size from
		float     SizeTo;   
		float	  SizeRandomenss;

		WeVector  Gravity;
		WeVector  ParticleDirection;
		float	  StartVelocity;					// speed in m/s
		float	  StartVelocityRandomness;			// variation of velocity

		float	  Spread;							// random spreading ... from 0..1 ... 

		float	  LifeTime;
		float	  LifeTimeRandomness;

		WEPARTICLE_ANGLE_TYPE AngleType;
		float	  ParticleAngle;					// if type is angle_specify

		WEPARTICLE_ANIM_TYPE  AnimType;
	};


						WeParticleSystem();
	virtual				~WeParticleSystem() { Reset(); }

	int					Version;
	SProperties			Properties;

	// Particles
	//-------------------
	WeParticle*			ParticleData;				// this is array of particle data	
	WeParticleIndex*	ParticleIndex;				// this is index array for particles
	WePhysicHandle**	ParticlePhysicHandles;	// this is array of pointers for physic handles
	int					LastIndex;						// last index 
	WeActor*			ParticlePlane;
	//-------------------	


	virtual	void		Clone(WeActor *Destination, bool DeepCopy = false); 
			void		Initialize();
			void		Serialize(WeFileStream &FS);
			void		Deserialize(WeFileStream &FS);
			// if you wish you can load particle system settings from some package. Use particle system editor
			// to save something in package for later load.
			bool		LoadPreset(StringParam _Name);
			bool		SavePreset(StringParam _Name, bool Replace = false);

			void		setMaxParticles(int MaxParticles);
	virtual void		Reset();    // this resets particle system
	virtual void		Tick();     // this will move particles around
	virtual void		OnRender(WeCamera *Cam);  // this will batch this particle system for drawing
	// this is function u can override for custom spawning directions of particles
	virtual void		SpawnDirection(WeVector &ParticleDirection); 
	// you can override this if you wish to make your own spawning code
	virtual void		SpawnPosition(WeVector &ParticlePosition); 
	bool				IntersectFrustum(WeFrustum *frustum);


};

#endif