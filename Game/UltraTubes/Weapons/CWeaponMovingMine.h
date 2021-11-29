/*
       File: CWeaponMovingMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponMovingMine_h_
#define _CWeaponMovingMine_h_


#include <CWeapon.h>

class WeTrail;
class WeSound;

class CWeaponMovingMine: public CWeapon
{
protected:
	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

	float				LevitationParam;
	float				ActivationTime;

	WeTrail				*Trail[2];
	WeSound				*Sound;

public:
						CWeaponMovingMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponMovingMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif