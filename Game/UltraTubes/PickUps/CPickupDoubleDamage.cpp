/*
	Filename:	CPickupDoubleDamage.cpp
	Author:		Goran Bogdanovic
*/


#include <CPickupDoubleDamage.h>
#include <ActorTags.h>
#include <CPickupActionDoubleDamage.h>
#include <CPickupActionQuadDamage.h>


CPickupDoubleDamage::CPickupDoubleDamage()
{
	NewPreset = 0;
	Type = PICKUPTYPE_DOUBLEDAMAGE;
}

CPickupDoubleDamage::~CPickupDoubleDamage()
{
}


void CPickupDoubleDamage::OnTrigger(WeActor* ByObject)
{
	if (ByObject->Type == ACTORTYPE_TUBER)
	{
		CTuber* Tuber = (CTuber*)ByObject;

		bool Found = false;

		for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
		{
            if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionDoubleDamage)
			{
				CPickupActionDoubleDamage* PickupActionDouble = (CPickupActionDoubleDamage*)Tuber->PickupActions.GetItem();
				PickupActionDouble->Timer = 0.0f;
				PickupActionDouble->Validity = true;
				Found = true;
				break;
			}
			if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionQuadDamage)
			{
				delete Tuber->PickupActions.GetItem();
				Tuber->PickupActions.Delete();
				break;
			}
		}

		if (!Found)
		{
			Tuber->PickupActions.PushBack(new CPickupActionDoubleDamage());
		}
	}

	CPickUp::OnTrigger(ByObject);
}
