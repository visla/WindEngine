/*
       File: CPickupSuperNukeMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupSuperNukeMine.h>
#include <CWeaponDescriptionManager.h>




CPickupSuperNukeMine::CPickupSuperNukeMine()
{
	NewPreset = 7;
	Type = PICKUPTYPE_SUPERNUKEMINE;
}



void CPickupSuperNukeMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_SUPERNUKEMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SUPERNUKEMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);

}
