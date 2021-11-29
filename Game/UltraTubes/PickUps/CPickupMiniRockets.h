/*
       File: CPickupMiniRockets.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupMiniRockets_h_
#define _CPickupMiniRockets_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupMiniRockets:public CPickUp
{
public:
	CPickupMiniRockets();
	~CPickupMiniRockets(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
