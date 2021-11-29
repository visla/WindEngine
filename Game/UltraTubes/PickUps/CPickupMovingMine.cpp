/*
       File: CPickupMovingMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupMovingMine.h>
#include <CWeaponDescriptionManager.h>




CPickupMovingMine::CPickupMovingMine()
{
	NewPreset = 6;
	Type = PICKUPTYPE_MOVINGMINE;
}



void CPickupMovingMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_MOVINGMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MOVINGMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}
