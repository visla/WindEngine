/*
       File: CTuberPart.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#ifndef _CTuberPart_h_
#define _CTuberPart_h_

#include <WeActor.h>
#include <CTuber.h>



class WeParticleSystem;

enum TuberPartPSType
{
	TuberPartPSTypeNone,
	TuberPartPSTypeMini,
	TuberPartPSTypeBurnBaby,
	TuberPartPSTypeBurnBabyBURN
};

class CTuberPart : public WeActor
{
protected:
	float					SoundPlayTimer;
	float					SoundPlayTime;

public:
	static WeParticleSystem *StaticPSMini;
	static WeParticleSystem *StaticPSBurnBaby;
	static WeParticleSystem *StaticPSBurnBabyBURNFire;
	static WeParticleSystem *StaticPSBurnBabyBURNSmoke;


	TuberPartPSType			PS;

							CTuberPart(TuberPartPSType PSType = TuberPartPSTypeNone);
							~CTuberPart();

	WeParticleSystem*		Particle;
	WeParticleSystem*		ParticleAlt;

	CTuber*					Tuber;


	void					Tick();
	void					OnCollision(WeActor* Actor);
};



#endif
