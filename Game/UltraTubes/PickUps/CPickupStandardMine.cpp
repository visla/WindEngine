/*
       File: CPickupStandardMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupStandardMine.h>
#include <CWeaponDescriptionManager.h>




CPickupStandardMine::CPickupStandardMine()
{
	NewPreset = 11;
	Type = PICKUPTYPE_STANDARDMINE;
}



void CPickupStandardMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_STANDARDMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);

}
