/*
		File: CExplosionNuke.h
		Author: Goran Bogdanovic
*/


#ifndef _CExplosionNuke_h_
#define _CExplosionNuke_h_

#include <CExplosion.h>
#include <CWeaponDescription.h>

class WeLight;
class CTrigger;


class CExplosionNuke : public CExplosion
{
protected:
	WeActor					*Sphere;
	WeActor					*Ring[2];
	WeLight					*Light;
	CTrigger				*NotifyArea;
	float					Size;
	bool					Flash;

public:
							CExplosionNuke(WeVector &MyLocation, CTuber *MyPyromaniac, float MySize, float MyDamage, WEAPONTYPE CreatorID, bool FlashScreen = true, bool AffectActors = true);
							~CExplosionNuke();

	void					Tick();
	void					OnNotify(WeActor *Victim);
	void					Affect(CTuber *Victim);
};



#endif