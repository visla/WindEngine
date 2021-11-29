/*
       File: CPickupSlowingMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupSlowingMine.h>
#include <CWeaponDescriptionManager.h>




CPickupSlowingMine::CPickupSlowingMine()
{
	NewPreset = 9;
	Type = PICKUPTYPE_SLOWINGMINE;
}



void CPickupSlowingMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_SLOWINGMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SLOWINGMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);

}
