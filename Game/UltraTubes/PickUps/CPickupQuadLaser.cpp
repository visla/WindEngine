/*
File: CPickupQuadLaser.cpp
Author: Tomislav Kukic
Date: 23.06.2006
Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupQuadLaser.h>
#include <CWeaponDescriptionManager.h>



CPickupQuadLaser::CPickupQuadLaser()
{
	//ModelName = "PickUps.Models.QuadLaser";
	NewPreset = 0;
	Type = PICKUPTYPE_QUADLASER;
}



void CPickupQuadLaser::OnTrigger(WeActor* ByObject)
{
	CTuber* tempT = (CTuber*)ByObject;

	tempT->PrimaryWeaponAddonID = WEAPONTYPE_QUADLASER;
	tempT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_QUADLASER)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

