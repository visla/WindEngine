/*
       File: CWeaponGravityMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponGravityMine_h_
#define _CWeaponGravityMine_h_


#include <CWeapon.h>

class CTrigger;
class WeSound;

class CWeaponGravityMine: public CWeapon
{
protected:
	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

	CTrigger			*NotifyTargetArea;
	WeSound				*Sound;

	float				ActivationTime;
	float				LevitationParam;

public:
						CWeaponGravityMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponGravityMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
	void				OnNotify(WeActor *Victim);

};

#endif