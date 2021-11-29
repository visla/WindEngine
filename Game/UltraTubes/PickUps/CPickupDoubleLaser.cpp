/*
File: CPickupDoubleLaser.cpp
Author: Tomislav Kukic
Date: 23.06.2006
Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupDoubleLaser.h>
#include <CWeaponDescriptionManager.h>



CPickupDoubleLaser::CPickupDoubleLaser()
{
	//ModelName = "PickUps.Models.DoubleLaser";
	NewPreset = 0;
	Type = PICKUPTYPE_DOUBLELASER;
}



void CPickupDoubleLaser::OnTrigger(WeActor* ByObject)
{
	CTuber* tempT = (CTuber*)ByObject;

	tempT->PrimaryWeaponAddonID = WEAPONTYPE_DOUBLELASER;
	tempT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_DOUBLELASER)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

