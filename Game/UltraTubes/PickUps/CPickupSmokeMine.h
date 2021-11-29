/*
       File: CPickupSmokeMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupSmokeMine_h_
#define _CPickupSmokeMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupSmokeMine:public CPickUp
{
public:
	CPickupSmokeMine();
	~CPickupSmokeMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
