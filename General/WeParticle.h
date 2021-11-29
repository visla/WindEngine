/*
    WeParticle.h

	Autor: Nebojsa Vislavski
	Date: 8.11.2005.
	Version: 1.0

	This is only one particle.


*/

#ifndef _WEPARTICLE_H
#define _WEPARTICLE_H

#include <WeVector.h>
#include <WePhysicHandle.h>

#define WeColor D3DXCOLOR

class WeParticleVertex
{
public:
	WeVector Position;  // w holds - living or not
	float    Alive;
	WeVector PPosition; // position of particle in world space. Used for rendering
	float	 Size;
	float    Age;
	float    LifeTime;  // this is life span or time :)
	float	 SizeFrom;
	DWORD	 Color;
	float	 u,v;
	WeVector Velocity;
	WeVector YAxis;
	float	 SpinTo;
	WeVector PPositionRel; // position relative to particle system
};

class WeParticleIndex
{
public:
	DWORD tri1[3];
	DWORD tri2[3];
};



class WeParticleSystem;



class WeParticle
{
public:
	WeParticleVertex Particle[4];	
	void Update(WeParticleSystem *Ps, float fDeltaTime);
};

#endif