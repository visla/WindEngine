/*
	Filename:	CPickupActionQuadDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ACTION_QUADDAMAGE_H
#define _CPICKUP_ACTION_QUADDAMAGE_H


#include <CPickupAction.h>


class CTuber;


class CPickupActionQuadDamage : public CPickupAction
{
	friend class CPickupQuadDamage;


public:
	CPickupActionQuadDamage();
	~CPickupActionQuadDamage();

	void				Update(CTuber* Owner);

};

#endif
