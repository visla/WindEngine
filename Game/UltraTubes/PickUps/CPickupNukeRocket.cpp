/*
       File: CPickupNukeRocket.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupNukeRocket.h>
#include <CWeaponDescriptionManager.h>



CPickupNukeRocket::CPickupNukeRocket()
{
	ModelName = "PickUps.Models.WeaponModel";
	NewPreset = 4;
	Type = PICKUPTYPE_NUKEROCKET;
}



void CPickupNukeRocket::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpTuber = (CTuber*)ByObject;

	tmpTuber->PrimaryWeaponAddonID = WEAPONTYPE_NUKEROCKET;
	tmpTuber->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_NUKEROCKET)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

