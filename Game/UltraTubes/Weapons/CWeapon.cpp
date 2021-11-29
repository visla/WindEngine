/*
       File: CWeapon.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeapon.h>
#include <CMainGamePhase.h>
#include <WeTimer.h>
#include <ActorTags.h>
#include <CTuber.h>
#include <CWeaponSuperNukeMine.h>


#define DEFAULT_LIFETIME	10.0f	//sec


CWeapon::CWeapon()
{
	Dying	 =	false;
	LifeTime =	DEFAULT_LIFETIME;
	HomingTarget = NULL;
	MineLockFactor = 0.0f;
	PassFlares = true;
}

CWeapon::~CWeapon()
{
	if (!CMainGamePhase::Instance->Map.IsUnloading())
	{
		if (HomingTarget)
		{
			HomingTarget->HasWeaponLock = false;
			HomingTarget->WeaponLockDistance = -1.0f;
			if (HomingTarget->ClosestLockedWeapon == this)
				HomingTarget->ClosestLockedWeapon = NULL;
		}
		if (IsSecondaryWeapon())
		{
			for (WeaponOwner->DeployedMines.First(); !WeaponOwner->DeployedMines.End(); WeaponOwner->DeployedMines.Next())
				if (WeaponOwner->DeployedMines.GetItem() == this)
				{
					WeaponOwner->DeployedMines.Delete();
					break;
				}
		}
	}
	HomingTarget = NULL;
}


void CWeapon::Tick()
{
	if (HomingTarget)
	{
		HomingTarget->HasWeaponLock = true;

		if (HomingTarget->Dead)
		{
			HomingTarget->HasWeaponLock = false;
			HomingTarget = NULL;
		}
		else
		{
			float Distance = abs(D3DXVec3Length(&(HomingTarget->Location - Location)));
			if ( (HomingTarget->WeaponLockDistance < 0.0f) || (Distance < HomingTarget->WeaponLockDistance) )
			{
				HomingTarget->WeaponLockDistance = Distance;
				HomingTarget->ClosestLockedWeapon = this;
			}
			else if (HomingTarget->ClosestLockedWeapon == this)
			{
				HomingTarget->WeaponLockDistance = Distance;
			}		
		}
	}

	WeActor::Tick();
}


float CWeapon::GetTimeToActivate(WEAPONTYPE ID)
{
	switch (ID)
	{
	case WEAPONTYPE_SLOWINGMINE:
	case WEAPONTYPE_MOVINGMINE:
	case WEAPONTYPE_GRAVITYMINE:
	case WEAPONTYPE_ELECTRICMINE:
	case WEAPONTYPE_BLURMINE:
	case WEAPONTYPE_HOMINGMINE:
		return 1.0f;
	case WEAPONTYPE_PROXIMITYMINE:
		return 2.0f;
	case WEAPONTYPE_SUPERNUKEMINE:
		return NUKE_MINE_LIFETIME;
	default:
		return 0.0f;
	}
}

float CWeapon::GetEffectiveRadius(WEAPONTYPE ID)
{
	switch (ID)
	{
	case WEAPONTYPE_STANDARDMINE:
	case WEAPONTYPE_SMOKEMINE:
	case WEAPONTYPE_MOVINGMINE:
	case WEAPONTYPE_INVISIBLEMINE:
	case WEAPONTYPE_HOMINGMINE:
		return 1.0f;
	case WEAPONTYPE_SLOWINGMINE:
		return 50.0f;
	case WEAPONTYPE_PROXIMITYMINE:
		return 20.0f;
	case WEAPONTYPE_GRAVITYMINE:
		return 50.0f;
	case WEAPONTYPE_ELECTRICMINE:
	case WEAPONTYPE_BLURMINE:
		return 10.0f;
	default:
		return 0.0f;
	}
}


bool CWeapon::IsPrimaryWeapon()
{
	return (Type == ACTORTYPE_WEAPON);
}

bool CWeapon::IsSecondaryWeapon()
{
	return (Type == ACTORTYPE_MINE);
}
