/*
       File: CPickupFrontFieldGenerator.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupFrontFieldGenerator_h_
#define _CPickupFrontFieldGenerator_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupFrontFieldGenerator:public CPickUp
{
public:
	CPickupFrontFieldGenerator();
	~CPickupFrontFieldGenerator(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
