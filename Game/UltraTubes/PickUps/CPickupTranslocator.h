/*
       File: CPickupTranslocator.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupTranslocator_h_
#define _CPickupTranslocator_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupTranslocator:public CPickUp
{
public:
	CPickupTranslocator();
	~CPickupTranslocator(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
