/*
       File: CPickupFrontFieldGenerator.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupFrontFieldGenerator.h>
#include <CWeaponDescriptionManager.h>



CPickupFrontFieldGenerator::CPickupFrontFieldGenerator()
{
	ModelName = "PickUps.Models.Weapon7";
	Type = PICKUPTYPE_FRONTFIELDGENERATOR;
}



void CPickupFrontFieldGenerator::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->PrimaryWeaponID = WEAPONTYPE_FRONTFIELDGENERATOR;
	tmpT->PrimaryWeaponAmmo = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_FRONTFIELDGENERATOR)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

