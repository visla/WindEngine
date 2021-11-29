/*
       File: CPickupMinigun.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupMinigun.h>
#include <CWeaponDescriptionManager.h>



CPickupMinigun::CPickupMinigun()
{
	ModelName = "PickUps.Models.WeaponModel";
	NewPreset = 6;
	Type = PICKUPTYPE_MINIGUN;
}



void CPickupMinigun::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->PrimaryWeaponAddonID = WEAPONTYPE_MINIGUN;
	tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MINIGUN)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

