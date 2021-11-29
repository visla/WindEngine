/*
       File: CPickupRandomWeapon.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupRandomWeapon_h_
#define _CPickupRandomWeapon_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>




class CPickupRandomWeapon:public CPickUp
{
public:
	CPickupRandomWeapon();
	~CPickupRandomWeapon(){}

	int			RandomID;

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
