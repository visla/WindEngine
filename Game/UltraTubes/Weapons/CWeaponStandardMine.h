/*
       File: CWeaponStandardMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponStandardMine_h_
#define _CWeaponStandardMine_h_


#include <CWeapon.h>


class CWeaponStandardMine: public CWeapon
{
protected:
	float				LevitationParam;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponStandardMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponStandardMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif