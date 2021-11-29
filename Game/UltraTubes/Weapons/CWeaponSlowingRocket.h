/*
       File: CWeaponSlowingRocket.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponSlowingRocket_h_
#define _CWeaponSlowingRocket_h_


#include <CWeapon.h>


class WeParticleSystem;
class WeTrail;
class WeSound;

class CWeaponSlowingRocket: public CWeapon
{
protected:
	WeTrail				*Trail[2];
	WeMatrix			OrientMatrix;
	WeParticleSystem	*PS;
	WeLight				*Light;
	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);


public:
	static WeParticleSystem *StaticPS;

						CWeaponSlowingRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponSlowingRocket();

	void				Tick();
	void				OnHit(WeActor *Victim);

};

#endif