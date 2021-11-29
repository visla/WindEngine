/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponHomingMine.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionSplash.h>
#include <WeActor.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <CTrigger.h>
#include <WeMaterialManager.h>
#include <WeTrail.h>
#include <WeAudioDevice.h>
#include <WeSound.h>

#define LIFETIME				20.0f
#define BOOM_SIZE				20.0f
#define TARGETING_AREA_SIZE		30.0f
#define CHANGE_TARGET_TIME		5.0f
#define MAX_SPEED				10.0f


void CWeaponHomingMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = LIFETIME;
	ChangeTargetTime = CHANGE_TARGET_TIME;

	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.HomingMine");

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
	ActivationTime = 0.0f;

	NotifyTargetArea = new CTrigger(this);
	NotifyTargetArea->Location = Location;
	Owner->GetWorld()->AddActor(NotifyTargetArea);
	NotifyTargetArea->PhysicHandle = WePhysics::Instance()->CreateSphere(NotifyTargetArea, Location, TARGETING_AREA_SIZE, COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
	NotifyTargetArea->PhysicHandle->SetDisableGravity(true);

	for (int i = 0; i < 2; i++)
	{
		Trail[i] = new WeTrail();
		Trail[i]->MaxSegments = 20;
		Trail[i]->Location = Location;
		Trail[i]->StartFadeFactor = 0.9f;
		Trail[i]->SegmentCreationTime = 0.1f;
		Trail[i]->SegmentWidth = 0.2f;
		Trail[i]->SegmentColor.r = 1.0f;
		Trail[i]->SegmentColor.g = 0.0f;
		Trail[i]->SegmentColor.b = 0.0f;
		Trail[i]->Material = WeMaterialManager::Instance()->Load("Weapons.Trails.HomingMine");
		Trail[i]->Initialize();
		WeaponParent->AddActor(Trail[i]);
	}
	WeMatrix TrailMat;
	Trail[1]->GetTransformation(TrailMat);
	D3DXMatrixRotationZ(&TrailMat, (D3DX_PI / 2));
	Trail[1]->SetTransformation(TrailMat);

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.MineDrop", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.HomingMine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
}


CWeaponHomingMine::CWeaponHomingMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
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

CWeaponHomingMine::~CWeaponHomingMine()
{
	NotifyTargetArea->LifeTime = 0.0001f;
	for (int i = 0; i < 2; i++)
		Trail[i]->LifeTime = (float)((Trail[i]->MaxSegments / 2) * Trail[i]->SegmentCreationTime);
	SAFE_DELETE(Sound);
}

void CWeaponHomingMine::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	ActivationTime += Time;
	LevitationParam += Time;
	ChangeTargetTime += Time;

	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2, 0.0f);

	if ( (HomingTarget) && WeaponOwner->ValidateTarget(HomingTarget) )
	{
		WeVector SpeedVector;

		D3DXVec3Normalize(&SpeedVector, &(HomingTarget->Location - Location));
		SpeedVector *= WeaponDesc.Speed;

		Vel += SpeedVector;
	}

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	NotifyTargetArea->Location = Location;
	for (int i = 0; i < 2; i++)
		Trail[i]->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
	}

	CWeapon::Tick();
}

void CWeaponHomingMine::OnHit(WeActor *Victim)
{
	if (Dying) return;
	if (Victim->IsPrefab) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	new CExplosionSplash(Location, WeaponOwner, BOOM_SIZE, WeaponDesc.Damage, SplashExplosionHomingMine, WEAPONTYPE_HOMINGMINE);
}

void CWeaponHomingMine::OnNotify(WeActor *Victim)
{
	if ( (ActivationTime >= GetTimeToActivate(WEAPONTYPE_HOMINGMINE)) && (ChangeTargetTime >= CHANGE_TARGET_TIME) && (Victim != HomingTarget) && (Victim->Type == ACTORTYPE_TUBER) && WeaponOwner->ValidateTarget((CTuber*)Victim) )
	{
		ChangeTargetTime = 0.0f;
		HomingTarget = (CTuber*)Victim;
	}
}
