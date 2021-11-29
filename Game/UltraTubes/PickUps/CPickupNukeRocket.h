/*
       File: CPickupNukeRocket.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupNukeRocket_h_
#define _CPickupNukeRocket_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupNukeRocket:public CPickUp
{
public:
	CPickupNukeRocket();
	~CPickupNukeRocket(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
