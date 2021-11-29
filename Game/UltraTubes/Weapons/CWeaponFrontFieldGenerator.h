/*
       File: CWeaponFrontFieldGenerator.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponFrontFieldGenerator_h_
#define _CWeaponFrontFieldGenerator_h_


#include <CString.h>
#include <CWeapon.h>


class CWeaponFrontFieldGenerator: public CWeapon
{
public:
						CWeaponFrontFieldGenerator();
						~CWeaponFrontFieldGenerator();

	void				Tick();

};

#endif