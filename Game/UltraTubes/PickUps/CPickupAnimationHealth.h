/*
       File: CPickupAnimationHealth.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupAnimationHealth_h_
#define _CPickupAnimationHealth_h_


#include <CPickUpFactory.h>
#include <CPickUpAnimation.h>
#include <WeActor.h>


class CPickupAnimationHealth:public CPickUpAnimation
{
public:
	CPickupAnimationHealth();
	~CPickupAnimationHealth(){}

	void	Tick();
};



#endif
