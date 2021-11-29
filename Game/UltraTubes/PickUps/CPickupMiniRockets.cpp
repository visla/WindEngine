/*
       File: CPickupMiniRockets.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupMiniRockets.h>
#include <CTuber.h>
#include <CWeaponDescriptionManager.h>



CPickupMiniRockets::CPickupMiniRockets()
{
	ModelName = "PickUps.Models.WeaponModel";
	NewPreset = 5;
	Type = PICKUPTYPE_MINIROCKETS;
}



void CPickupMiniRockets::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpTuber = (CTuber*)ByObject;

	tmpTuber->PrimaryWeaponAddonID = WEAPONTYPE_MINIROCKETS;
	tmpTuber->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MINIROCKETS)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

