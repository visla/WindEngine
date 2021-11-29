/*
       File: CPickupHomingMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupHomingMine.h>
#include <CWeaponDescriptionManager.h>




CPickupHomingMine::CPickupHomingMine()
{
	ModelName = "Weapons.Models.HomingMine";
	NewPreset = 0;
	Type = PICKUPTYPE_HOMINGMINE;
}



void CPickupHomingMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_HOMINGMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_HOMINGMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}
