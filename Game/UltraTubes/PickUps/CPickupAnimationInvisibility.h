/*
	Filename:	CPickupAnimationInvisibility.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ANIMATION_INVISIBILITY_H
#define _CPICKUP_ANIMATION_INVISIBILITY_H


#include <CPickUpAnimation.h>

class CTuber;

class CPickupAnimationInvisibility : public CPickUpAnimation
{
protected:
	CTuber*					Owner;

public:
	CPickupAnimationInvisibility(CTuber* ThePicker);
	~CPickupAnimationInvisibility(){}

	void					Tick();
	void					Render();
};



#endif
