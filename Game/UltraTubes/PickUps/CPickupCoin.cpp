/*
       File: CPickupCoin.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupCoin.h>
#include <CWeaponDescriptionManager.h>
#include <ActorTags.h>




CPickupCoin::CPickupCoin()
{
	NewPreset = 0;
}


void CPickupCoin::Initialize()
{
	switch (Type)
	{
	case PICKUPTYPE_COINSMALL:
		ModelName = "PickUps.Models.CoinSmall";
		break;
	case PICKUPTYPE_COINMEDIUM:
		ModelName = "PickUps.Models.CoinMedium";
		break;
	case PICKUPTYPE_COINLARGE:
		ModelName = "PickUps.Models.CoinLarge";
		break;
	}

	CPickUp::Initialize();
}


void CPickupCoin::OnTrigger(WeActor* ByObject)
{
	if (ByObject->Type == ACTORTYPE_TUBER)
	{
		CTuber* tmpT = (CTuber*)ByObject;

		if (tmpT)
		{
			int Value = 0;

			switch (Type)
			{
			case PICKUPTYPE_COINSMALL:
				Value = 1;
				break;
			case PICKUPTYPE_COINMEDIUM:
				Value = 10;
				break;
			case PICKUPTYPE_COINLARGE:
				Value = 100;
				break;
			}

			tmpT->Coins += Value;
			tmpT->Score += (Value * 10);
		}
	}

	CPickUp::OnTrigger(ByObject);
}
