/*
       File: CPickupSlowingRocket.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupSlowingRocket.h>
#include <CWeaponDescriptionManager.h>



CPickupSlowingRocket::CPickupSlowingRocket()
{
	ModelName = "PickUps.Models.SlowingRocket";
	NewPreset = 0;
	Type = PICKUPTYPE_SLOWINGROCKET;
}



void CPickupSlowingRocket::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->PrimaryWeaponAddonID = WEAPONTYPE_SLOWINGROCKET;
	tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SLOWINGROCKET)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

