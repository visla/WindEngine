/*
	Filename:	CPickupActionDoubleDamage.h
	Author:		Goran Bogdanovic
*/


#include <CPickupActionDoubleDamage.h>
#include <CTuber.h>

#define	LIFETIME	10.0f


CPickupActionDoubleDamage::CPickupActionDoubleDamage()
{
	ActionType = PickupActionDoubleDamage;
	ValidityTime = LIFETIME;
}


CPickupActionDoubleDamage::~CPickupActionDoubleDamage()
{
	if (OwnerTuber)
	{
		OwnerTuber->StrengthMultiplier = 1.0f;
	}
}


void CPickupActionDoubleDamage::Update(CTuber *Owner)
{
	if (Owner)
		Owner->StrengthMultiplier = 2.0f;

	CPickupAction::Update(Owner);
}
