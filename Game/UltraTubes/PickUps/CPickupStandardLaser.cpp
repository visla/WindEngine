/*
File: CPickupStandardLaser.cpp
Author: Tomislav Kukic
Date: 23.06.2006
Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupStandardLaser.h>
#include <CWeaponDescriptionManager.h>



CPickupStandardLaser::CPickupStandardLaser()
{
	ModelName = "PickUps.Models.StandardBolt";
	NewPreset = 0;
	Type = PICKUPTYPE_STANDARDLASER;
}



void CPickupStandardLaser::OnTrigger(WeActor* ByObject)
{
	CTuber* tempT = (CTuber*)ByObject;

	tempT->PrimaryWeaponAddonID = WEAPONTYPE_STANDARDLASER;
	tempT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDLASER)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

