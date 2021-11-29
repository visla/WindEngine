/*
       File: CPickupGravityMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupGravityMine.h>
#include <CWeaponDescriptionManager.h>




CPickupGravityMine::CPickupGravityMine()
{
	NewPreset = 3;
	Type = PICKUPTYPE_GRAVITYMINE;
}



void CPickupGravityMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_GRAVITYMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_GRAVITYMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
}

