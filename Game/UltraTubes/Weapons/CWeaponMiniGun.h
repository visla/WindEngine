/*
       File: CWeaponMiniGun.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponMiniGun_h_
#define _CWeaponMiniGun_h_

#include <CWeapon.h>


class CWeaponMiniGun: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	float				Angle;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);


public:
						CWeaponMiniGun(CTuber* Owner, bool IsPrimary, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponMiniGun();

	bool				Primary;

	void				Tick();
	void				OnHit(WeActor *Victim);

};

#endif