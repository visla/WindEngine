/*
	Filename:	CPickupQuadDamage.cpp
	Author:		Goran Bogdanovic
*/


#include <CPickupQuadDamage.h>
#include <ActorTags.h>
#include <CPickupActionDoubleDamage.h>
#include <CPickupActionQuadDamage.h>


CPickupQuadDamage::CPickupQuadDamage()
{
	ModelName = "PickUps.Models.QuadDamage";
	NewPreset = 0;
	Type = PICKUPTYPE_QUADDAMAGE;

	CPickUp::Initialize();
}

CPickupQuadDamage::~CPickupQuadDamage()
{
}


void CPickupQuadDamage::OnTrigger(WeActor* ByObject)
{
	if (ByObject->Type == ACTORTYPE_TUBER)
	{
		CTuber* Tuber = (CTuber*)ByObject;

		bool Found = false;

		for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
		{
            if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionQuadDamage)
			{
				CPickupActionQuadDamage* PickupActionQuad = (CPickupActionQuadDamage*)Tuber->PickupActions.GetItem();
				PickupActionQuad->Timer = 0.0f;
				PickupActionQuad->Validity = true;
				Found = true;
				break;
			}
			if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionDoubleDamage)
			{
				delete Tuber->PickupActions.GetItem();
				Tuber->PickupActions.Delete();
				break;
			}
		}

		if (!Found)
		{
			Tuber->PickupActions.PushBack(new CPickupActionQuadDamage());
		}
	}

	CPickUp::OnTrigger(ByObject);
}
