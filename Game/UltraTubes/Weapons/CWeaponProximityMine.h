/*
       File: CWeaponProximityMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/



#ifndef _CWeaponProximityMine_h_
#define _CWeaponProximityMine_h_


#include <CWeapon.h>

class CTrigger;
class WeSound;

class CWeaponProximityMine: public CWeapon
{
protected:
	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);
	
	CTrigger			*NotifyTargetArea;
	WeLight				*Light;
	WeSound				*Sound;

	float				LevitationParam;
	float				ActivationTime;

	void				Explode();

public:
						CWeaponProximityMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponProximityMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
	void				OnNotify(WeActor *Victim);
};


#endif