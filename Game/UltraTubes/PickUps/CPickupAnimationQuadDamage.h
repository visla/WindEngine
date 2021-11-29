/*
	Filename:	CPickupAnimationQuadDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ANIMATION_QUADDAMAGE_H
#define _CPICKUP_ANIMATION_QUADDAMAGE_H


#include <CPickUpAnimation.h>

class CTuber;


class CPickupAnimationQuadDamage : public CPickUpAnimation
{
protected:
	CTuber*					Owner;

public:
	CPickupAnimationQuadDamage(CTuber* ThePicker);
	~CPickupAnimationQuadDamage(){}

	void					Tick();
	void					Render();
};



#endif
