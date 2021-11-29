/*
File: CPickupDoubleLaser.h
Author: Tomislav Kukic
Date: 23.06.2006
Version: 1.0
*/




#ifndef _CPickupDoubleLaser_h_
#define _CPickupDoubleLaser_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupDoubleLaser:public CPickUp
{
public:
	CPickupDoubleLaser();
	~CPickupDoubleLaser(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
