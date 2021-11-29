/*
       File: CWeaponHomingMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponHomingMine_h_
#define _CWeaponHomingMine_h_


#include <CWeapon.h>

class CTrigger;
class WeTrail;
class WeSound;

class CWeaponHomingMine: public CWeapon
{
protected:
	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

	WeVector			Speed;
	CTrigger			*NotifyTargetArea;
	WeTrail				*Trail[2];
	WeSound				*Sound;

	float				LevitationParam;
	float				ActivationTime;
	float				ChangeTargetTime;

public:
						CWeaponHomingMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponHomingMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
	void				OnNotify(WeActor *Victim);
};

#endif