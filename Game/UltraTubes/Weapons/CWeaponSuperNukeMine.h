/*
       File: CWeaponSuperNukeMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponSuperNukeMine_h_
#define _CWeaponSuperNukeMine_h_

#include <CWeapon.h>

#define NUKE_MINE_LIFETIME		5.0f
#define NUKE_MINE_SIZE			300.0f

class WeSound;

class CWeaponSuperNukeMine: public CWeapon
{
protected:
	float				LevitationParam;
	float				ExplosionTimer;

	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponSuperNukeMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix);
						~CWeaponSuperNukeMine();

	void				Tick();
};

#endif