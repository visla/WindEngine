/*
       File: CPickupInvisibleMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupInvisibleMine_h_
#define _CPickupInvisibleMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>



class CPickupInvisibleMine:public CPickUp
{
public:
	CPickupInvisibleMine();
	~CPickupInvisibleMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
