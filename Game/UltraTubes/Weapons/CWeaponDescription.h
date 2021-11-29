/*
       File: CWeaponDescription.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponDescription_h_
#define _CWeaponDescription_h_


#include <CString.h>
#include <stdio.h>


enum WEAPONTYPE
{
	WEAPONTYPE_NONE = -1,
	WEAPONTYPE_STANDARDLASER,
	WEAPONTYPE_DOUBLELASER,
	WEAPONTYPE_QUADLASER,
	WEAPONTYPE_STANDARDROCKET,
	WEAPONTYPE_NUKEROCKET,
	WEAPONTYPE_QUANTUMLASER,
	WEAPONTYPE_MINIROCKETS,
	WEAPONTYPE_SLOWINGROCKET,
	WEAPONTYPE_TRANSLOCATOR,
	WEAPONTYPE_MINIGUN,
	WEAPONTYPE_STANDARDMINE,
	WEAPONTYPE_SMOKEMINE,
	WEAPONTYPE_PROXIMITYMINE,
	WEAPONTYPE_SLOWINGMINE,
	WEAPONTYPE_HOMINGMINE,
	WEAPONTYPE_GRAVITYMINE,
	WEAPONTYPE_SUPERNUKEMINE,
	WEAPONTYPE_BLURMINE,
	WEAPONTYPE_MOVINGMINE,
	WEAPONTYPE_INVISIBLEMINE,
	WEAPONTYPE_ELECTRICMINE,
	WEAPONTYPE_FRONTFIELDGENERATOR,
	WEAPONTYPE_FORCEFIELDMINE
};


class CWeaponDescription
{
public:
								CWeaponDescription();
								~CWeaponDescription(){}

	float						FireRate;
	int							PickUpAmmo;
	int							AmmoPerShot;
	float						Damage;
	float						Speed;
	float						HomingLockTime;
	String						Name;
	String						InterfaceImage;
	WEAPONTYPE					Type;


	void						Serialize(FILE *f); 
	void						Deserialize(FILE *f);
};




#endif
