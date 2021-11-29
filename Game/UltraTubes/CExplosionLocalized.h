/*
       File: CExplosionLocalized.h
	   Author: Goran Bogdanovic
	   Date: 13.04.2006.
	   Version: 1.0
*/


#ifndef _CExplosionLocalized_h_
#define _CExplosionLocalized_h_

#include <CExplosion.h>


enum LocalizedExplosionType
{
	LocalizedExplosionStandardLaser,
	LocalizedExplosionStandardRocket,
	LocalizedExplosionQuantumLaser,
	LocalizedExplosionMiniRocket,
	LocalizedExplosionStandardBigAss,
	LocalizedExplosionMiniGun,
	LocalizedExplosionTranslocator,
	LocalizedExplosionBlurMine,
	LocalizedExplosionSlowingRocket,
	LocalizedExplosionTuber
};

class WeActor;
class WeLight;
class WePostProcessEffect;
class WeParticleSystem;

class CExplosionLocalized:public CExplosion
{
protected:
	WeActor							*Shockwave;
    WeActor							*BoomSplash[6];
	CTuber							*Victim;
	LocalizedExplosionType			ExplosionType;
	WeLight							*Light;
	float							LightRange;
	float							ShockwaveSize;
	WeVector						PyroX, PyroY, PyroZ;

	WeParticleSystem				*Sparks;

	static WePostProcessEffect		*WaveFX;
	static bool						WaveFXActive;
	bool							WaveFXOwner;

public:
	static WeParticleSystem			*PS;

	CExplosionLocalized(WeVector &MyLocation, CTuber *MyPyromaniac, float MyDamage, CTuber *MyVictim, LocalizedExplosionType MyExplosionType, WEAPONTYPE CreatorID);
	~CExplosionLocalized();

	void							Tick();
	void							ForceAffectVictim();
};	



#endif