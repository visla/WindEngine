/*
       File: CPickupInvisibleMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupInvisibleMine.h>
#include <CWeaponDescriptionManager.h>




CPickupInvisibleMine::CPickupInvisibleMine()
{
	NewPreset = 5;
	Type = PICKUPTYPE_INVISIBLEMINE;
}



void CPickupInvisibleMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_INVISIBLEMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_INVISIBLEMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);
	
}
