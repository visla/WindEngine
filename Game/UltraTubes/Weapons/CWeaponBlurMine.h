/*
       File: CWeaponBlurMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponBlurMine_h_
#define _CWeaponBlurMine_h_


#include <CWeapon.h>

class CTrigger;
class WeSound;


class CWeaponBlurMine: public CWeapon
{
protected:
	float				LevitationParam;
	float				ActivationTime;

	CTrigger			*NotifyTargetArea;
	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponBlurMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponBlurMine();

	void				Tick();
	void				OnHit(WeActor *Victim);
	void				OnNotify(WeActor *Victim);
};

#endif