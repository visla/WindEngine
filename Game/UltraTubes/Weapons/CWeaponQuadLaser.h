/*
       File: CWeaponQuadLaser.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponQuadLaser_h_
#define _CWeaponQuadLaser_h_


#include <CWeapon.h>


class CWeaponQuadLaser: public CWeapon
{
protected:
	WeMatrix			OrientMatrix;
	WeLight				*Light;

	WeVector			FixedDirection;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, short MyBeamID, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
						CWeaponQuadLaser(CTuber* Owner, short MyBeamID, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponQuadLaser();

	short				BeamID;

	void				Tick();
	void				OnHit(WeActor *Victim);
};

#endif