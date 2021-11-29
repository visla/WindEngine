/*
       File: CPickupTranslocator.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupTranslocator.h>
#include <CWeaponDescriptionManager.h>



CPickupTranslocator::CPickupTranslocator()
{
	ModelName = "PickUps.Models.Translocator";
	NewPreset = 0;
	Type = PICKUPTYPE_TRANSLOCATOR;
}



void CPickupTranslocator::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->PrimaryWeaponAddonID = WEAPONTYPE_TRANSLOCATOR;
	tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_TRANSLOCATOR)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

