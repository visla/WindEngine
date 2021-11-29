/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponMiniGun.h>
#include <CWeaponFabric.h>
#include <CTuber.h>
#include <WeModelManager.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionLocalized.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <CTuberPartFabric.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeMesh.h>

#define SHELL_LIFETIME	0.75f


void CWeaponMiniGun::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	this->Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.MiniGunAmmo");

	WeMatrix TransMatrix, BeamTransMatrix, RelativeTransMatrix, MyMatrix, TuberMatrix;

	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	if (Primary)
		D3DXMatrixTranslation(&BeamTransMatrix, -0.5f, 0.0f, 0.0f);
	else
		D3DXMatrixTranslation(&BeamTransMatrix, 0.5f, 0.0f, 0.0f);

	D3DXMatrixMultiply(&RelativeTransMatrix, &BeamTransMatrix, &TransMatrix);
	D3DXMatrixMultiply(&MyMatrix, &RelativeTransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&MyMatrix, &MyMatrix, &TuberMatrix);
	SetTransformation(MyMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(MyMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * Direction);

	int ID = (Primary) ? 0 : 1;
	if (!WeaponOwner->Muzzle[ID])
	{
		WeaponOwner->Muzzle[ID] = new CTuberMuzzle(WeaponOwner, ID, RelativeTransMatrix);
		WeaponOwner->Muzzle[ID]->Location = Location;
		WeaponOwner->Muzzle[ID]->Model = WeModelManager::Instance()->Load("Weapons.Models.MiniGunMuzzle");
		WeaponOwner->Muzzle[ID]->Model->Mesh->Materials[0].First();
		WeaponOwner->Muzzle[ID]->LifeTime = (WeaponOwner->Muzzle[ID]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / WeaponOwner->Muzzle[ID]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
	}

	//---create minigun ammo casings---//
	WeVector DX, DY, DZ;
	WeaponOwner->GetLocalAxis(DX, DY, DZ);
	D3DXVec3Normalize(&DY, &DY);
	DY.x *= (1 + (rand() % 5));
	DY.y *= (1 + (rand() % 5));
	CTuberPartFabric::Instance()->CreateShellPart(Location, DY, WeaponParent, SHELL_LIFETIME, false, "Weapons.Models.MiniGunAmmoShell");
	//---------------------------------//

	if (Primary)
	{
		WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.MiniGun", true);
		if (FireSound)
		{
			FireSound->Update(WeaponOwner->Location, WeVector(0.0f, 0.0f, 0.0f));
			FireSound->Play(false);
			FireSound->DieOnStop();
		}
	}
}


CWeaponMiniGun::CWeaponMiniGun(CTuber* Owner, bool IsPrimary, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
{
	Primary = IsPrimary;

	WeVector WeapDirection;
	WeapDirection.x = WeapOwnerMatrix._31; WeapDirection.y = WeapOwnerMatrix._32; WeapDirection.z = WeapOwnerMatrix._33;
	D3DXVec3Normalize(&WeapDirection, &WeapDirection);

	WeVector WeapLocation;
	WeapLocation.x = WeapOwnerMatrix._41; WeapLocation.y = WeapOwnerMatrix._42; WeapLocation.z = WeapOwnerMatrix._43;

	WeMatrix WeapOrientation;
	WeapOrientation = WeapOwnerMatrix;
	WeapOrientation._41 = 0.0f; WeapOrientation._42 = 0.0f; WeapOrientation._43 = 0.0f;

	WeaponDesc = *Desc;
	InitializeWeapon(Owner, RelativeSpawnLoc, WeapLocation, WeapDirection, WeapOrientation);
}

CWeaponMiniGun::~CWeaponMiniGun()
{
}

void CWeaponMiniGun::Tick()
{
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * Direction);

	CWeapon::Tick();
}

void CWeaponMiniGun::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	CExplosionLocalized *Explosion;
	Explosion = new CExplosionLocalized((Location - Direction), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionMiniGun, WEAPONTYPE_MINIGUN);
}
