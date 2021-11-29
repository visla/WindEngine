/*
	Filename:	CPickupDoubleDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_DOUBLEDAMAGE_H
#define _CPICKUP_DOUBLEDAMAGE_H


#include <CPickUp.h>

class WeActor;


class CPickupDoubleDamage : public CPickUp
{
public:
	CPickupDoubleDamage();
	~CPickupDoubleDamage();


	void			OnTrigger(WeActor* ByObject);

};


#endif
