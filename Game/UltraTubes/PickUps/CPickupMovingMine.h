/*
       File: CPickupMovingMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupMovingMine_h_
#define _CPickupMovingMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupMovingMine:public CPickUp
{
public:
	CPickupMovingMine();
	~CPickupMovingMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
