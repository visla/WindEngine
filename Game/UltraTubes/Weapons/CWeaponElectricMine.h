/*
       File: CWeaponElectricMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponElectricMine_h_
#define _CWeaponElectricMine_h_


#include <CWeapon.h>

class CTrigger;
class WeSound;

class CWeaponElectricMine: public CWeapon
{
protected:
	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

	CTrigger			*NotifyTargetArea;
	WeSound				*Sound;

	float				LevitationParam;
	float				ActivationTime;

public:
						CWeaponElectricMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponElectricMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
	void				OnNotify(WeActor *Victim);
};

#endif