/*
       File: CExplosionSplash.h
	   Author: Goran Bogdanovic
	   Date: 14.04.2006.
	   Version: 1.0
*/


#ifndef _CExplosionSplash_h_
#define _CExplosionSplash_h_

#include <CExplosion.h>
#include <CWeaponDescription.h>

class WeLight;
class CTrigger;


enum SplashExplosionType
{
	SplashExplosionStandardSplash,
	SplashExplosionHomingMine
};


class CExplosionSplash : public CExplosion
{
protected:
	SplashExplosionType		ExplosionType;
	WeActor					*BoomSplash[3];
	WeLight					*Light;
	CTrigger				*NotifyArea;
	float					Size;

public:
							CExplosionSplash(WeVector &MyLocation, CTuber *MyPyromaniac, float MySize, float MyDamage, SplashExplosionType MyExplosionType, WEAPONTYPE CreatorID);
							~CExplosionSplash();

	void					Tick();
	void					OnNotify(WeActor *Victim);
	void					Affect(CTuber *Victim);
};



#endif