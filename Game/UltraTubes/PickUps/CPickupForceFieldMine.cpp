/*
       File: CPickupForceFieldMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupForceFieldMine.h>
#include <CWeaponDescriptionManager.h>




CPickupForceFieldMine::CPickupForceFieldMine()
{
	NewPreset = 2;
	Type = PICKUPTYPE_FORCEFIELDMINE;
}



void CPickupForceFieldMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_FORCEFIELDMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_FORCEFIELDMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
	
}

