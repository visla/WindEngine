/*
       File: CPickupHomingMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupHomingMine_h_
#define _CPickupHomingMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupHomingMine:public CPickUp
{
public:
	CPickupHomingMine();
	~CPickupHomingMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
