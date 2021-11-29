/*
       File: CPickupAnimationFrontFieldGenerator.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupAnimationFrontFieldGenerator_h_
#define _CPickupAnimationFrontFieldGenerator_h_


#include <CPickUpFactory.h>
#include <CPickUpAnimation.h>
#include <WeActor.h>


class CPickupAnimationFrontFieldGenerator:public CPickUpAnimation
{
public:
	CPickupAnimationFrontFieldGenerator();
	~CPickupAnimationFrontFieldGenerator(){}

	void	Tick();
};



#endif
