/*
File: CPickupStandardLaser.h
Author: Tomislav Kukic
Date: 23.06.2006
Version: 1.0
*/




#ifndef _CPickupStandardLaser_h_
#define _CPickupStandardLaser_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupStandardLaser:public CPickUp
{
public:
	CPickupStandardLaser();
	~CPickupStandardLaser(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
