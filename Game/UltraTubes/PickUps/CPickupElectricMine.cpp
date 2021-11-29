/*
       File: CPickupElectricMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupElectricMine.h>
#include <CWeaponDescriptionManager.h>




CPickupElectricMine::CPickupElectricMine()
{
	NewPreset = 1;
	Type = PICKUPTYPE_ELECTRICMINE;
}



void CPickupElectricMine::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	tmpT->SecondaryWeaponAddonID = WEAPONTYPE_ELECTRICMINE;
	tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_ELECTRICMINE)->PickUpAmmo;

	CPickUp::OnTrigger(ByObject);


}

