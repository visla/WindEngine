/*
       File: CWeaponTranslocator.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponTranslocator_h_
#define _CWeaponTranslocator_h_


#include <CWeapon.h>

class WeParticleSystem;
class WeSound;

class CWeaponTranslocator: public CWeapon
{
	WeParticleSystem	*PS[3];
	WeMatrix			OrientMatrix;
	WeLight				*Light;
	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);


public:
	static WeParticleSystem *StaticPS[3];

						CWeaponTranslocator(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponTranslocator();

	void				Tick();
	void				OnHit(WeActor *Victim);

};

#endif