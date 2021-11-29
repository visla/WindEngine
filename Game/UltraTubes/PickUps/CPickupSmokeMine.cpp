/*
       File: CPickupSmokeMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupSmokeMine.h>
#include <CWeaponDescriptionManager.h>




CPickupSmokeMine::CPickupSmokeMine()
{
	ModelName = "Weapons.Models.BlurMine";
	NewPreset = 0;
	Type = PICKUPTYPE_SMOKEMINE;
}



void CPickupSmokeMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_SMOKEMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SMOKEMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);

}
