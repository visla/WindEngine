/*
       File: CPickupMinigun.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupMinigun_h_
#define _CPickupMinigun_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupMinigun:public CPickUp
{
public:
	CPickupMinigun();
	~CPickupMinigun(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
