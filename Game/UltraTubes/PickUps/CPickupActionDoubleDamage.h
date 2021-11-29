/*
	Filename:	CPickupActionDoubleDamage.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ACTION_DOUBLEDAMAGE_H
#define _CPICKUP_ACTION_DOUBLEDAMAGE_H


#include <CPickupAction.h>


class CTuber;


class CPickupActionDoubleDamage	: public CPickupAction
{
	friend class CPickupDoubleDamage;


public:
	CPickupActionDoubleDamage();
	~CPickupActionDoubleDamage();

	void				Update(CTuber* Owner);

};

#endif
