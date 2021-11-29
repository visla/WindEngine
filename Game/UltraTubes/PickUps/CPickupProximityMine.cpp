/*
       File: CPickupProximityMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupProximityMine.h>
#include <CWeaponDescriptionManager.h>




CPickupProximityMine::CPickupProximityMine()
{
	NewPreset = 8;
	Type = PICKUPTYPE_PROXIMITYMINE;
}



void CPickupProximityMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_PROXIMITYMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_PROXIMITYMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);

}

