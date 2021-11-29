/*
       File: CWeaponForceFieldMine.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponForceFieldMine_h_
#define _CWeaponForceFieldMine_h_


#include <CWeaponFabric.h>
#include <CString.h>
#include <CWeapon.h>


class CWeaponForceFieldMine: public CWeapon
{
public:
						CWeaponForceFieldMine();
						~CWeaponForceFieldMine();

	void				Tick();

};

#endif