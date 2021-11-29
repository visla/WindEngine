/*
       File: CPickupSlowingRocket.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupSlowingRocket_h_
#define _CPickupSlowingRocket_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupSlowingRocket:public CPickUp
{
public:
	CPickupSlowingRocket();
	~CPickupSlowingRocket(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
