/*
       File: CPickupBlurMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupBlurMine_h_
#define _CPickupBlurMine_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupBlurMine:public CPickUp
{
public:
	CPickupBlurMine();
	~CPickupBlurMine(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
