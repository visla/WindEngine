/*
	Filename:	CPickupQuadDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_QUADDAMAGE_H
#define _CPICKUP_QUADDAMAGE_H


#include <CPickUp.h>

class WeActor;


class CPickupQuadDamage : public CPickUp
{
public:
	CPickupQuadDamage();
	~CPickupQuadDamage();


	void			OnTrigger(WeActor* ByObject);

};


#endif
