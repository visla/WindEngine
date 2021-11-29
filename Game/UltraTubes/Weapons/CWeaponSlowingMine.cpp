/*
       File: CWeaponSlowingMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponSlowingMine.h>
#include <CWeaponFabric.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionLocalized.h>
#include <WeActor.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <CTrigger.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <CPlayGamePhase.h>

#define LIFETIME			20.0f
#define SLOWING_SPEED		1000.0f



void CWeaponSlowingMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = LIFETIME;

	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.SlowingMine");

	WeMatrix TransMatrix, MineMatrix, TuberMatrix, OrientMatrix;

	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&MineMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&MineMatrix, &MineMatrix, &TuberMatrix);
	Location.x = MineMatrix._41; Location.y = MineMatrix._42; Location.z = MineMatrix._43;

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, SECONDARY_PHYS_BOX, COLLISION_GROUP_TRANSMINE, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(SECONDARY_CCD_BOX_MIN, SECONDARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);

	NotifyTargetArea = new CTrigger(this);
	NotifyTargetArea->Location = Location;
	Owner->GetWorld()->AddActor(NotifyTargetArea);
	NotifyTargetArea->PhysicHandle = WePhysics::Instance()->CreateSphere(NotifyTargetArea, Location, GetEffectiveRadius(WEAPONTYPE_SLOWINGMINE), COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
	NotifyTargetArea->PhysicHandle->SetDisableGravity(true);

	LevitationParam = 0.0f;
	ActivationTime = 0.0f;

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.MineDrop", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.SlowingMine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
}

CWeaponSlowingMine::CWeaponSlowingMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
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

CWeaponSlowingMine::~CWeaponSlowingMine()
{
	NotifyTargetArea->LifeTime = 0.0001f;
	SAFE_DELETE(Sound);
}

void CWeaponSlowingMine::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	ActivationTime += Time;
	LevitationParam += Time;

	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2.0f, 0.0f);

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);
	
	if (Sound)
	{
		Sound->Update(Location, Vel);
	}
	
	CWeapon::Tick();
}

void CWeaponSlowingMine::OnHit(WeActor *Victim)
{
	if (Dying) return;

	if ( (Victim->Type == ACTORTYPE_WEAPON) || (Victim->Type == ACTORTYPE_EXPLOSION) )
	{
		LifeTime = 0.0001f;
		PhysicHandle->SetResponse(false);
		Dying = true;

		CExplosionLocalized *Explosion;
		Explosion = new CExplosionLocalized(Location, WeaponOwner, WeaponDesc.Damage, NULL, LocalizedExplosionStandardRocket, WEAPONTYPE_SLOWINGMINE);
	}
}

void CWeaponSlowingMine::OnNotify(WeActor *Victim)
{
	if (Dying) return;
	if (ActivationTime < GetTimeToActivate(WEAPONTYPE_SLOWINGMINE)) return;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	if (PlayerVictim)
	{
		if (PlayerVictim->CurrentSpeed > PlayerVictim->MaxSpeed / 2.0f)
			PlayerVictim->CurrentSpeed = PlayerVictim->MaxSpeed / 2.0f;
		return;
	}

	if (Victim->PhysicHandle && CPlayGamePhase::ActorPushable(Victim))
	{
		float time = WeTimer::Instance()->GetDeltaTimeF();
		Victim->PhysicHandle->AddVelocity(-Victim->Velocity * (SLOWING_SPEED * time));
	}
}
