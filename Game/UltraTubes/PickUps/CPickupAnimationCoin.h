/*
	Filename:	CPickupAnimationCoin.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ANIMATION_COIN_H
#define _CPICKUP_ANIMATION_COIN_H


#include <CPickUpAnimation.h>

class CTuber;


class CPickupAnimationCoin : public CPickUpAnimation
{
protected:
	bool					FirstTime;
	DWORD					Type;
	CTuber*					Owner;


public:
	CPickupAnimationCoin(DWORD CoinType, CTuber* ThePicker);
	~CPickupAnimationCoin() {}

	void					Tick();
	void					Render();
};



#endif
