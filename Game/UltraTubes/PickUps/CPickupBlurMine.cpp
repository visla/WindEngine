/*
       File: CPickupBlurMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupBlurMine.h>
#include <CWeaponDescriptionManager.h>




CPickupBlurMine::CPickupBlurMine()
{
	ModelName = "PickUps.Models.MineNew";
	NewPreset = 1;
	Type = PICKUPTYPE_BLURMINE;
}



void CPickupBlurMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_BLURMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_BLURMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}


