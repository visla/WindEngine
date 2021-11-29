/*
	Filename:	CPickupInvisibility.cpp
	Author:		Goran Bogdanovic
*/


#include <CPickupInvisibility.h>
#include <ActorTags.h>
#include <CPickupActionInvisibility.h>


CPickupInvisibility::CPickupInvisibility()
{
	ModelName = "PickUps.Models.Invisibility";
	NewPreset = 0;
	Type = PICKUPTYPE_INVISIBILITY;

	CPickUp::Initialize();
}

CPickupInvisibility::~CPickupInvisibility()
{
}


void CPickupInvisibility::OnTrigger(WeActor* ByObject)
{
	if (ByObject->Type == ACTORTYPE_TUBER)
	{
		CTuber* Tuber = (CTuber*)ByObject;

		bool Found = false;

		for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
		{
			if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionInvisibility)
			{
				CPickupActionInvisibility* PickupAction = (CPickupActionInvisibility*)Tuber->PickupActions.GetItem();
				PickupAction->Timer = 0.0f;
				PickupAction->Validity = true;
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			Tuber->PickupActions.PushBack(new CPickupActionInvisibility());
		}
	}

	CPickUp::OnTrigger(ByObject);
}
