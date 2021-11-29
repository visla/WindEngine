/*
	Filename:	CPickupActionInvisibility.h
	Author:		Goran Bogdanovic
*/


#include <CPickupActionInvisibility.h>
#include <CTuber.h>

#define	LIFETIME	10.0f


CPickupActionInvisibility::CPickupActionInvisibility()
{
	ActionType = PickupActionInvisibility;
	ValidityTime = LIFETIME;
}


CPickupActionInvisibility::~CPickupActionInvisibility()
{
	if (OwnerTuber)
	{
		OwnerTuber->Invisible = false;
		OwnerTuber->MaterialPreset = TuberMaterialPresetNormal;
	}
}


void CPickupActionInvisibility::Update(CTuber *Owner)
{
	if (Owner)
	{
		Owner->Invisible = true;
		Owner->MaterialPreset = TuberMaterialPresetInvisibility;
	}

	CPickupAction::Update(Owner);
}
