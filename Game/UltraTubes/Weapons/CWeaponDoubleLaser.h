/*
       File: CWeaponDoubleLaser.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponDoubleLaser_h_
#define _CWeaponDoubleLaser_h_

#include <CWeapon.h>

class CWeaponDoubleLaser: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	WeLight				*Light;

	WeVector			FixedDirection;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponDoubleLaser(CTuber* Owner, bool IsPrimary, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponDoubleLaser();

	bool				Primary;

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif