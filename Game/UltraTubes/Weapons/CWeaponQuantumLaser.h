/*
       File: CWeaponQuantumLaser.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponQuantumLaser_h_
#define _CWeaponQuantumLaser_h_

#include <CWeapon.h>

class WeParticleSystem;

class CWeaponQuantumLaser: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	WeLight				*Light;

	WeParticleSystem	*PS;

	WeVector			FixedDirection;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
	static WeParticleSystem	*StaticPS;

						CWeaponQuantumLaser(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOrientation, CWeaponDescription *Desc);
						~CWeaponQuantumLaser();

	void				Tick();
	void				OnHit(WeActor *Victim);

};

#endif