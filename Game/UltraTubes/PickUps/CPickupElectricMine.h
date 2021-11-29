/*
       File: CPickupElectricMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupElectricMine_h_
#define _CPickupElectricMine_h_


#include <CPickUpFactory.h>
#include <CPickup.h>
#include <WeActor.h>


class CPickupElectricMine:public CPickUp
{
public:
	CPickupElectricMine();
	~CPickupElectricMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
