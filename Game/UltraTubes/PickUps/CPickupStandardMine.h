/*
       File: CPickupStandardMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupStandardMine_h_
#define _CPickupStandardMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupStandardMine:public CPickUp
{
public:
	CPickupStandardMine();
	~CPickupStandardMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
