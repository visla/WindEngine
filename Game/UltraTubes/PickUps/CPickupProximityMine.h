/*
       File: CPickupProximityMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupProximityMine_h_
#define _CPickupProximityMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupProximityMine:public CPickUp
{
public:
	CPickupProximityMine();
	~CPickupProximityMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
