/*
       File: CPickupStandardRocket.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupStandardRocket_h_
#define _CPickupStandardRocket_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupStandardRocket:public CPickUp
{
public:
	CPickupStandardRocket();
	~CPickupStandardRocket(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
