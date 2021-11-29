/*
       File: CPickupSlowingMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupSlowingMine_h_
#define _CPickupSlowingMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupSlowingMine:public CPickUp
{
public:
	CPickupSlowingMine();
	~CPickupSlowingMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
