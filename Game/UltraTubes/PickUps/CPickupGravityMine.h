/*
       File: CPickupGravityMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupGravityMine_h_
#define _CPickupGravityMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupGravityMine:public CPickUp
{
public:
	CPickupGravityMine();
	~CPickupGravityMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
