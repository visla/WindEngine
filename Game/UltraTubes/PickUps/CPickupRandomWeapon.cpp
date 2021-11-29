/*
       File: CPickupRandomWeapon.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupRandomWeapon.h>
#include <CWeaponDescriptionManager.h>
#include <WeTimer.h>



CPickupRandomWeapon::CPickupRandomWeapon()
{
	NewPreset = 12;
	Type = PICKUPTYPE_RANDOMWEAPON;
	RandomID = 0;
}



void CPickupRandomWeapon::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpT = (CTuber*)ByObject;

	srand((int)WeTimer::Instance()->GetElapsedTimeF());
	RandomID = (rand()%23) + 10002;

	if(RandomID < 10003) RandomID = 10003;
	if(RandomID > 10025) RandomID = 10025;
	if(RandomID == 10006) RandomID = 10007;
	if(RandomID == 10014) RandomID = 10015;

	switch(RandomID)
	{
	case PICKUPTYPE_BLURMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_BLURMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_BLURMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_DOUBLELASER:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_DOUBLELASER;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_DOUBLELASER)->PickUpAmmo;
		break;
	case PICKUPTYPE_ELECTRICMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_ELECTRICMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_ELECTRICMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_FORCEFIELDMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_FORCEFIELDMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_FORCEFIELDMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_FRONTFIELDGENERATOR:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_FRONTFIELDGENERATOR;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_FRONTFIELDGENERATOR)->PickUpAmmo;
		break;
	case PICKUPTYPE_GRAVITYMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_GRAVITYMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_GRAVITYMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_HOMINGMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_HOMINGMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_HOMINGMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_INVISIBLEMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_INVISIBLEMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_INVISIBLEMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_MINIGUN:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_MINIGUN;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MINIGUN)->PickUpAmmo;
		break;
	case PICKUPTYPE_MINIROCKETS:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_MINIROCKETS;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MINIROCKETS)->PickUpAmmo;
		break;
	case PICKUPTYPE_MOVINGMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_MOVINGMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_MOVINGMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_NUKEROCKET:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_NUKEROCKET;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_NUKEROCKET)->PickUpAmmo;
		break;
	case PICKUPTYPE_QUADLASER:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_QUADLASER;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_QUADLASER)->PickUpAmmo;
		break;
	case PICKUPTYPE_QUANTUMLASER:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_QUANTUMLASER;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_QUANTUMLASER)->PickUpAmmo;
		break;
	case PICKUPTYPE_SLOWINGMINE:
		tmpT->SecondaryWeaponAddonID = WEAPONTYPE_SLOWINGMINE;
		tmpT->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SLOWINGMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_SLOWINGROCKET:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_SLOWINGROCKET;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SLOWINGROCKET)->PickUpAmmo;
		break;
	case PICKUPTYPE_STANDARDLASER:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_STANDARDLASER;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDLASER)->PickUpAmmo;
		break;
	case PICKUPTYPE_STANDARDROCKET:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_STANDARDROCKET;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDROCKET)->PickUpAmmo;
		break;
	case PICKUPTYPE_SUPERNUKEMINE:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_SUPERNUKEMINE;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SUPERNUKEMINE)->PickUpAmmo;
		break;
	case PICKUPTYPE_TRANSLOCATOR:
		tmpT->PrimaryWeaponAddonID = WEAPONTYPE_TRANSLOCATOR;
		tmpT->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_TRANSLOCATOR)->PickUpAmmo;
		break;
	}

	CPickUp::OnTrigger(ByObject);
}

