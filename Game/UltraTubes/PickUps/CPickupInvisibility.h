/*
	Filename:	CPickupInvisibility.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_INVISIBILITY_H
#define _CPICKUP_INVISIBILITY_H


#include <CPickUp.h>

class WeActor;


class CPickupInvisibility : public CPickUp
{
public:
	CPickupInvisibility();
	~CPickupInvisibility();


	void			OnTrigger(WeActor* ByObject);

};


#endif
