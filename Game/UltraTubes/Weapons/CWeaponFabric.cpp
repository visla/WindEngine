/*
       File: CWeaponFabric.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponFabric.h>
#include <CTuber.h>
#include <CollisionGroups.h>
#include <WePhysics.h>
#include <CWeaponPreBolt.h>
#include <CWeaponStandardLaser.h>
#include <CWeaponDoubleLaser.h>
#include <CWeaponQuadLaser.h>
#include <CWeaponStandardRocket.h>
#include <CWeaponQuantumLaser.h>
#include <CWeaponMiniRockets.h>
#include <CWeaponNukeRocket.h>
#include <CWeaponMiniGun.h>
#include <CWeaponSlowingRocket.h>
#include <CWeaponTranslocator.h>
#include <CWeaponStandardMine.h>
#include <CWeaponBlurMine.h>
#include <CWeaponElectricMine.h>
#include <CWeaponGravityMine.h>
#include <CWeaponHomingMine.h>
#include <CWeaponSlowingMine.h>
#include <CWeaponSuperNukeMine.h>
#include <CWeaponInvisibleMine.h>
#include <CWeaponMovingMine.h>
#include <CWeaponProximityMine.h>
#include <CWeaponSmokeMine.h>
#include <CWeaponDescriptionManager.h>
#include <CUberText.h>
#include <CMainGameInterface.h>


#define DIST_SPAWN_FRONT	5.0f
#define DIST_SPAWN_BACK		8.0f

CWeaponFabric* CWeaponFabric::Instance = new CWeaponFabric();


CWeaponFabric::CWeaponFabric()
{
	Active = true;
}


void CWeaponFabric::BuildWeapon(CTuber* Player, WEAPONTYPE ID, WeMatrix &FullMatrix, bool ServerWeapon)
{
	CSmartList<CWeapon*> NewWeaps;
	static WeVector VecSpawnFront = WeVector(0.0f, 0.0f, DIST_SPAWN_FRONT);
	static WeVector VecSpawnBack = WeVector(0.0f, 0.0f, -DIST_SPAWN_BACK);
	CWeaponDescription *Desc = CWeaponDescriptionManager::Instance->GetWeaponDescription(ID);
	bool HomingWeapon = false;

	switch (ID)
	{
		case WEAPONTYPE_STANDARDLASER:
			NewWeaps.PushBack(new CWeaponStandardLaser(Player, VecSpawnFront, FullMatrix, Desc));
			break;
		case WEAPONTYPE_DOUBLELASER:
			NewWeaps.PushBack(new CWeaponDoubleLaser(Player, true, VecSpawnFront, FullMatrix, Desc));
			NewWeaps.PushBack(new CWeaponDoubleLaser(Player, false, VecSpawnFront, FullMatrix, Desc));
			break;
		case WEAPONTYPE_QUADLASER:
			for (short i = 0; i < 4; i++)
				NewWeaps.PushBack(new CWeaponQuadLaser(Player, i, VecSpawnFront, FullMatrix, Desc));
			break;
		case WEAPONTYPE_STANDARDROCKET:
			NewWeaps.PushBack(new CWeaponStandardRocket(Player, VecSpawnFront, FullMatrix, Desc));
			HomingWeapon = true;
			break;
		case WEAPONTYPE_QUANTUMLASER:
			NewWeaps.PushBack(new CWeaponPreBolt(Player, WeVector(0.0f, 0.0f, DIST_SPAWN_FRONT * 0.6f), Desc));
			//NewWeaps.PushBack(new CWeaponQuantumLaser(Player, VecSpawnFront, FullMatrix, Desc));
			break;
		case WEAPONTYPE_MINIROCKETS:
			for (short i = 0; i < 10; i++)
				NewWeaps.PushBack(new CWeaponMiniRockets(Player, i, VecSpawnFront, FullMatrix, Desc));
			HomingWeapon = true;
			break;
		case WEAPONTYPE_NUKEROCKET:
			NewWeaps.PushBack(new CWeaponNukeRocket(Player, VecSpawnFront, FullMatrix, Desc));
			HomingWeapon = true;
			break;
		case WEAPONTYPE_MINIGUN:
			NewWeaps.PushBack(new CWeaponMiniGun(Player, true, VecSpawnFront, FullMatrix, Desc));
			NewWeaps.PushBack(new CWeaponMiniGun(Player, false, VecSpawnFront, FullMatrix, Desc));
			break;
		case WEAPONTYPE_SLOWINGROCKET:
			NewWeaps.PushBack(new CWeaponSlowingRocket(Player, VecSpawnFront, FullMatrix, Desc));
			HomingWeapon = true;
			break;
		case WEAPONTYPE_TRANSLOCATOR:
			NewWeaps.PushBack(new CWeaponTranslocator(Player, VecSpawnFront, FullMatrix, Desc));
			HomingWeapon = true;
			break;
		case WEAPONTYPE_STANDARDMINE:
			NewWeaps.PushBack(new CWeaponStandardMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_BLURMINE:
			NewWeaps.PushBack(new CWeaponBlurMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_ELECTRICMINE:
			NewWeaps.PushBack(new CWeaponElectricMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_GRAVITYMINE:
			NewWeaps.PushBack(new CWeaponGravityMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_HOMINGMINE:
			NewWeaps.PushBack(new CWeaponHomingMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_SLOWINGMINE:
			NewWeaps.PushBack(new CWeaponSlowingMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_SUPERNUKEMINE:
			NewWeaps.PushBack(new CWeaponSuperNukeMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_INVISIBLEMINE:
			NewWeaps.PushBack(new CWeaponInvisibleMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_MOVINGMINE:
			NewWeaps.PushBack(new CWeaponMovingMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_PROXIMITYMINE:
			NewWeaps.PushBack(new CWeaponProximityMine(Player, VecSpawnBack, FullMatrix));
			break;
		case WEAPONTYPE_SMOKEMINE:
			NewWeaps.PushBack(new CWeaponSmokeMine(Player, VecSpawnBack, FullMatrix));
			break;
		default:
			return;
	}

	for (NewWeaps.First(); !NewWeaps.End(); NewWeaps.Next())
	{
		NewWeaps.GetItem()->WeaponDesc = *Desc;
		NewWeaps.GetItem()->WeaponID = ID;
		if (HomingWeapon)
			LockOnTarget(NewWeaps.GetItem());
		if (NewWeaps.GetItem()->IsSecondaryWeapon())
			NewWeaps.GetItem()->WeaponOwner->DeployedMines.PushBack(NewWeaps.GetItem());
	}

	NewWeaps.Clear();
}

bool CWeaponFabric::LockOnTarget(CWeapon *Weapon)
{
	if ( Weapon->WeaponOwner->EnemyTarget && (Weapon->WeaponOwner->EnemyLockFactor == 1.0f) )
	{
		Weapon->HomingTarget = Weapon->WeaponOwner->EnemyTarget;
		Weapon->WeaponOwner->EnemyTarget->HasWeaponLock = true;
		return true;
	}
	else
		return false;
}

bool CWeaponFabric::Client_MakeWeapon(CTuber *Owner, WEAPONTYPE ID, WeMatrix &OwnerFullMatrix)
{
	BuildWeapon(Owner, ID, OwnerFullMatrix, false);
	return true;
}


WEAPONTYPE CWeaponFabric::Server_MakeWeapon(CTuber *Player, bool AsPrimaryWeapon)
{
	WEAPONTYPE ID = AbleToFire(Player, AsPrimaryWeapon, true);

	if (ID == WEAPONTYPE_NONE) return ID;
	
	WeMatrix FullMatrix;
	Player->GetTransformation(FullMatrix);
	BuildWeapon(Player, ID, FullMatrix, true);

	return ID;
}


WEAPONTYPE CWeaponFabric::AbleToFire(CTuber *Player, bool AsPrimaryWeapon, bool Reset)
{
	const WEAPONTYPE Failure = WEAPONTYPE_NONE;

	if (!Active) return Failure;

	CWeaponDescription *Desc;
	WEAPONTYPE ReturnValue = WEAPONTYPE_NONE;

	//---raycast for weapon room---//
	WeRaycastHit RCHit;
	if (AsPrimaryWeapon)
	{
		if (WePhysics::Instance()->RaycastClosest(Player->Location, Player->Direction, WE_SHAPE_TYPE_STATIC, RCHit, 7, DIST_SPAWN_FRONT + 2.0f))
			return Failure;
	}
	else
	{
		if (WePhysics::Instance()->RaycastClosest(Player->Location, -Player->Direction, WE_SHAPE_TYPE_STATIC, RCHit, 7, DIST_SPAWN_BACK + 4.0f))
			return Failure;
	}
	//-----------------------------//

	if (AsPrimaryWeapon)
	{
		if (Player->PrimaryWeaponAddonID != WEAPONTYPE_NONE)
		{
			Desc = CWeaponDescriptionManager::Instance->GetWeaponDescription(Player->PrimaryWeaponAddonID);
			if (
				(Player->PrimaryWeaponAmmo2 >= Desc->AmmoPerShot) && 
				(Player->PrimaryWeaponFireTime >= (1 / Desc->FireRate)))
				{
					ReturnValue = Player->PrimaryWeaponAddonID;
					if (Reset)
					{
						Player->PrimaryWeaponAmmo2 -= Desc->AmmoPerShot;
						Player->PrimaryWeaponFireTime = 0;
						if (Player->PrimaryWeaponAmmo2 < Desc->AmmoPerShot)
							Player->PrimaryWeaponAddonID = WEAPONTYPE_NONE;
					}
					return ReturnValue;
				}
		}
		else if (Player->PrimaryWeaponID != WEAPONTYPE_NONE)
		{
			Desc = CWeaponDescriptionManager::Instance->GetWeaponDescription(Player->PrimaryWeaponID);
			if (
				(Player->PrimaryWeaponAmmo >= Desc->AmmoPerShot) &&
				(Player->PrimaryWeaponFireTime >= (1 / Desc->FireRate)))
				{
					ReturnValue = Player->PrimaryWeaponID;
					if (Reset)
					{
						Player->PrimaryWeaponAmmo -= Desc->AmmoPerShot;
						Player->PrimaryWeaponFireTime = 0;
					}
					return ReturnValue;
				}
		}
	}
	else	//if (!AsPrimaryWeapon)
	{
		if (Player->SecondaryWeaponAddonID != WEAPONTYPE_NONE)
		{
			Desc = CWeaponDescriptionManager::Instance->GetWeaponDescription(Player->SecondaryWeaponAddonID);
			if (
				(Player->SecondaryWeaponAmmo2 >= Desc->AmmoPerShot) && 
				(Player->SecondaryWeaponFireTime >= (1 / Desc->FireRate)))
				{
					ReturnValue = Player->SecondaryWeaponAddonID;
					if (Reset)
					{
						Player->SecondaryWeaponAmmo2 -= Desc->AmmoPerShot;
						Player->SecondaryWeaponFireTime = 0;
						if (Player->SecondaryWeaponAmmo2 < Desc->AmmoPerShot)
							Player->SecondaryWeaponAddonID = WEAPONTYPE_NONE;
					}
					return ReturnValue;
				}
		}
		else if (Player->SecondaryWeaponID != WEAPONTYPE_NONE)
		{
			Desc = CWeaponDescriptionManager::Instance->GetWeaponDescription(Player->SecondaryWeaponID);
			if (
				(Player->SecondaryWeaponAmmo >= Desc->AmmoPerShot) &&
				(Player->SecondaryWeaponFireTime >= (1 / Desc->FireRate)))
				{
					ReturnValue = Player->SecondaryWeaponID;
					if (Reset)
					{
						Player->SecondaryWeaponAmmo -= Desc->AmmoPerShot;
						Player->SecondaryWeaponFireTime = 0;
					}
					return ReturnValue;
				}
		}
	}

	return Failure;
}


void CWeaponFabric::SetActive(bool BooleanVariableThatActivatesWeaponry)
{
	Active = BooleanVariableThatActivatesWeaponry;
}

bool CWeaponFabric::IsValid()
{
	return Active;
}