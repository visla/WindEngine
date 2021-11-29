/*
	Filename:	CWeaponPreBolt.h
	Author:		Goran Bogdanovic
*/




#ifndef _CWEAPONPREBOLT_H_
#define _CWEAPONPREBOLT_H_

#include <CWeapon.h>


class CWeaponPreBolt: public CWeapon
{
protected:
	WeMatrix			RelativeMatrix;
	WeLight				*Light;
	WeSound				*FireSound;
	
	WeVector			FixedDirection;

	float				PreBoltTimer;
	float				PreBoltTime;


public:
						CWeaponPreBolt(CTuber* Owner, WeVector &RelativeSpawnLoc, CWeaponDescription *Desc);
						~CWeaponPreBolt();

	void				Tick();
};

#endif
