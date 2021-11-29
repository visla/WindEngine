/*
       File: CWeaponInvisibleMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponInvisibleMine_h_
#define _CWeaponInvisibleMine_h_

#include <CWeapon.h>


class CWeaponInvisibleMine: public CWeapon
{
protected:
	float				LevitationParam;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponInvisibleMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponInvisibleMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif