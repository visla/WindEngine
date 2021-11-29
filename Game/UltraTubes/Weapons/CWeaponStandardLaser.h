/*
       File: CWeaponStandardLaser.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponStandardLaser_h_
#define _CWeaponStandardLaser_h_

#include <CWeapon.h>


class CWeaponStandardLaser: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	WeLight				*Light;
	
	WeVector			FixedDirection;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);


public:
						CWeaponStandardLaser(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponStandardLaser();

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif
