/*
       File: CWeaponStandardRocket.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponStandardRocket_h_
#define _CWeaponStandardRocket_h_

#include <CWeapon.h>


class WeParticleSystem;
class WeSound;
class WeRadialTrail;


class CWeaponStandardRocket: public CWeapon
{
protected:
	WeParticleSystem	*PS;
	WeMatrix			OrientMatrix;
	WeLight				*Light;
	WeSound				*Sound;
	WeRadialTrail		*Trail;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);


public:
	static WeParticleSystem *StaticPS;

						CWeaponStandardRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponStandardRocket();

	void				Tick();
	void				OnHit(WeActor *Victim);


};

#endif