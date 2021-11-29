/*
       File: CPickupHealth.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupHealth_h_
#define _CPickupHealth_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupHealth:public CPickUp
{
public:
	CPickupHealth();
	~CPickupHealth(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
