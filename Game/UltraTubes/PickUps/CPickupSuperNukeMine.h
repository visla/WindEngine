/*
       File: CPickupSuperNukeMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupSuperNukeMine_h_
#define _CPickupSuperNukeMine_h_


#include <CPickUpFactory.h>
#include <CpickUp.h>
#include <WeActor.h>


class CPickupSuperNukeMine:public CPickUp
{
public:
	CPickupSuperNukeMine();
	~CPickupSuperNukeMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
