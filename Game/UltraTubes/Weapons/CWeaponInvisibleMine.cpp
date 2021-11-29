/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponInvisibleMine.h>
#include <CWeaponFabric.h>
#include <WeTimer.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionSplash.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeAudioDevice.h>
#include <WeSound.h>

#define LIFETIME				20.0f
#define BOOM_SIZE				20.0f


void CWeaponInvisibleMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = LIFETIME;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.InvisibleMine");

	WeMatrix TransMatrix, MineMatrix, TuberMatrix, OrientMatrix;

	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&MineMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&MineMatrix, &MineMatrix, &TuberMatrix);
	Location.x = MineMatrix._41; Location.y = MineMatrix._42; Location.z = MineMatrix._43;

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, SECONDARY_PHYS_BOX, COLLISION_GROUP_MINE, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(SECONDARY_CCD_BOX_MIN, SECONDARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);

	LevitationParam = 0.0f;

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.MineDrop", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}
}


CWeaponInvisibleMine::CWeaponInvisibleMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
{
	WeVector WeapDirection;
	WeapDirection.x = WeapOwnerMatrix._31; WeapDirection.y = WeapOwnerMatrix._32; WeapDirection.z = WeapOwnerMatrix._33;
	D3DXVec3Normalize(&WeapDirection, &WeapDirection);

	WeVector WeapLocation;
	WeapLocation.x = WeapOwnerMatrix._41; WeapLocation.y = WeapOwnerMatrix._42; WeapLocation.z = WeapOwnerMatrix._43;

	WeMatrix WeapOrientation;
	WeapOrientation = WeapOwnerMatrix;
	WeapOrientation._41 = 0.0f; WeapOrientation._42 = 0.0f; WeapOrientation._43 = 0.0f;

	InitializeWeapon(Owner, RelativeSpawnLoc, WeapLocation, WeapDirection, WeapOrientation);
}

CWeaponInvisibleMine::~CWeaponInvisibleMine()
{
}

void CWeaponInvisibleMine::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	LevitationParam += Time;
	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2, 0.0f);

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	CWeapon::Tick();
}

void CWeaponInvisibleMine::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CExplosionSplash *Explosion;
	Explosion = new CExplosionSplash(Location, WeaponOwner, BOOM_SIZE, WeaponDesc.Damage, SplashExplosionStandardSplash, WEAPONTYPE_INVISIBLEMINE);
}
