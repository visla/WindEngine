/*
	Filename:	CPickupActionInvisibility.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ACTION_INVISIBILITY_H
#define _CPICKUP_ACTION_INVISIBILITY_H


#include <CPickupAction.h>


class CTuber;


class CPickupActionInvisibility	: public CPickupAction
{
	friend class CPickupInvisibility;


public:
	CPickupActionInvisibility();
	~CPickupActionInvisibility();

	void				Update(CTuber* Owner);

};

#endif
