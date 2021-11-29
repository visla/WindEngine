/*
	Filename:	CPickupActionQuadDamage.h
	Author:		Goran Bogdanovic
*/


#include <CPickupActionQuadDamage.h>
#include <CTuber.h>

#define	LIFETIME	10.0f


CPickupActionQuadDamage::CPickupActionQuadDamage()
{
	ActionType = PickupActionQuadDamage;
	ValidityTime = LIFETIME;
}


CPickupActionQuadDamage::~CPickupActionQuadDamage()
{
	if (OwnerTuber)
	{
		OwnerTuber->StrengthMultiplier = 1.0f;
	}
}


void CPickupActionQuadDamage::Update(CTuber *Owner)
{
	if (Owner)
		Owner->StrengthMultiplier = 4.0f;

	CPickupAction::Update(Owner);
}
