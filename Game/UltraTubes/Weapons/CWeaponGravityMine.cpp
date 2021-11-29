/*
       File: CWeaponGravityMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponGravityMine.h>
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



void CWeaponGravityMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = LIFETIME;

	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.GravityMine");

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
	NotifyTargetArea->PhysicHandle = WePhysics::Instance()->CreateSphere(NotifyTargetArea, Location, GetEffectiveRadius(WEAPONTYPE_GRAVITYMINE), COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
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

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.GravityMine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
}

CWeaponGravityMine::CWeaponGravityMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
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

CWeaponGravityMine::~CWeaponGravityMine()
{
	NotifyTargetArea->LifeTime = 0.0001f;
	SAFE_DELETE(Sound);
}

void CWeaponGravityMine::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	ActivationTime += Time;
	LevitationParam += Time;

	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(D3DX_PI / 2.0f, 0.0f, 0.0f);

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	if (Sound)
	{
		Sound->Update(Location, Vel);
	}

	CWeapon::Tick();
}

void CWeaponGravityMine::OnHit(WeActor *Victim)
{
	if (Dying) return;

	if ( (Victim->Type == ACTORTYPE_WEAPON) || (Victim->Type == ACTORTYPE_EXPLOSION) )
	{
		Dying = true;
		LifeTime = 0.0001f;
		PhysicHandle->SetResponse(false);

		CExplosionLocalized *Explosion;
		Explosion = new CExplosionLocalized(Location, WeaponOwner, WeaponDesc.Damage, NULL, LocalizedExplosionStandardRocket, WEAPONTYPE_GRAVITYMINE);
	}
}


void CWeaponGravityMine::OnNotify(WeActor *Victim)
{
	if (Dying) return;
	if (ActivationTime < GetTimeToActivate(WEAPONTYPE_GRAVITYMINE)) return;

	float Time = WeTimer::Instance()->GetDeltaTimeF();

	WeVector DistanceVector, GravityVector;
	DistanceVector = (Location - Victim->Location);
	if (D3DXVec3Length(&DistanceVector) < 4.0f) return;
	D3DXVec3Normalize(&GravityVector, &DistanceVector);
	GravityVector *= (4000.0f * Time);

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	if (PlayerVictim)
	{
		PlayerVictim->Velocity += GravityVector;
		return;
	}

	if (Victim->PhysicHandle && CPlayGamePhase::ActorPushable(Victim))
		Victim->PhysicHandle->AddVelocity(GravityVector);
}
