/*
       File: CPickupStandardRocket.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupStandardRocket.h>
#include <CTuber.h>
#include <CWeaponDescriptionManager.h>



CPickupStandardRocket::CPickupStandardRocket()
{
	ModelName = "PickUps.Models.StandardRocket";
	NewPreset = 0;
	Type = PICKUPTYPE_STANDARDROCKET;
}



void CPickupStandardRocket::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpTuber = (CTuber*)ByObject;

	tmpTuber->PrimaryWeaponAddonID = WEAPONTYPE_STANDARDROCKET;
	tmpTuber->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDROCKET)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}


