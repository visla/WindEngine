/*
       File: CPickupForceFieldMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupForceFieldMine_h_
#define _CPickupForceFieldMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupForceFieldMine:public CPickUp
{
public:
	CPickupForceFieldMine();
	~CPickupForceFieldMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
