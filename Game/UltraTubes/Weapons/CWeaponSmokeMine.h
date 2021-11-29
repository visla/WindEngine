/*
       File: CWeaponSmokeMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponSmokeMine_h_
#define _CWeaponSmokeMine_h_


#include <CWeapon.h>

class WeParticleSystem;
class CTrigger;
class WeSound;


class CWeaponSmokeMine: public CWeapon
{
protected:
	float				LevitationParam;
	float				ActivationTime;

	WeParticleSystem*	PS;
	WeSound*			Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
	static WeParticleSystem *StaticPS;

						CWeaponSmokeMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponSmokeMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif