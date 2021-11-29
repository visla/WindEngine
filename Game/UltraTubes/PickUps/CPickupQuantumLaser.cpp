/*
       File: CPickupQuantumLaser.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupQuantumLaser.h>
#include <CWeaponDescriptionManager.h>



CPickupQuantumLaser::CPickupQuantumLaser()
{
	ModelName = "PickUps.Models.QuantumLaser";
	NewPreset = 0;
	Type = PICKUPTYPE_QUANTUMLASER;
}



void CPickupQuantumLaser::OnTrigger(WeActor* ByObject)
{
	CTuber* tempT = (CTuber*)ByObject;

	tempT->PrimaryWeaponAddonID = WEAPONTYPE_QUANTUMLASER;
	tempT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_QUANTUMLASER)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

