/*
	Filename:	CPickupAnimationDoubleDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ANIMATION_DOUBLEDAMAGE_H
#define _CPICKUP_ANIMATION_DOUBLEDAMAGE_H


#include <CPickUpAnimation.h>

class CTuber;


class CPickupAnimationDoubleDamage : public CPickUpAnimation
{
protected:
	CTuber*					Owner;


public:
	CPickupAnimationDoubleDamage(CTuber* ThePicker);
	~CPickupAnimationDoubleDamage(){}

	void					Tick();
	void					Render();
};



#endif
