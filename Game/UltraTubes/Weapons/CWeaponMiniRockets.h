/*
       File: CWeaponMiniRockets.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponMiniRockets_h_
#define _CWeaponMiniRockets_h_

#include <CWeapon.h>

class WeTrail;
class WeSound;

class CWeaponMiniRockets: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	float				Angle;
	WeTrail				*Trail[2];
	WeLight				*Light;
	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, short MyProjectileID, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponMiniRockets(CTuber* Owner, short MyProjectileID, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponMiniRockets();

	short				ProjectileID;

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif