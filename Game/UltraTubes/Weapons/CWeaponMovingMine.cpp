/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponMovingMine.h>
#include <CWeaponFabric.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionSplash.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <CTrigger.h>
#include <WeMaterialManager.h>
#include <WeTrail.h>
#include <WeAudioDevice.h>
#include <WeSound.h>

#define LIFETIME				20.0f
#define BOOM_SIZE				20.0f


void CWeaponMovingMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = LIFETIME;

	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.MovingMine");

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
		Trail[i]->Material = WeMaterialManager::Instance()->Load("Weapons.Trails.MovingMine");
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

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.MovingMine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
}


CWeaponMovingMine::CWeaponMovingMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
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


CWeaponMovingMine::~CWeaponMovingMine()
{
	for (int i = 0; i < 2; i++)
		Trail[i]->LifeTime = (float)((Trail[i]->MaxSegments / 2) * Trail[i]->SegmentCreationTime);

	SAFE_DELETE(Sound);
}

void CWeaponMovingMine::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	ActivationTime += Time;
	LevitationParam += Time;

	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2, 0.0f);

	if (ActivationTime >= GetTimeToActivate(WEAPONTYPE_MOVINGMINE))
	{
		const float range = 10.0f;
		const float speed = 1.0f;

		float vx = range * sin(speed * LevitationParam);
		float vy = range * cos(speed * LevitationParam);
		float vz = range * sin(speed * LevitationParam) * cos(speed * LevitationParam);
		Vel.x += vx;
		Vel.y += vy;
		Vel.z += vz;
	}

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	for (int i = 0; i < 2; i++)
		Trail[i]->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, Vel);
	}

	CWeapon::Tick();
}

void CWeaponMovingMine::OnHit(WeActor *Victim)
{
	if (Dying) return;
	if (Victim->IsPrefab) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetCollision(false);
	Dying = true;

	CExplosionSplash *Explosion;
	Explosion = new CExplosionSplash(Location, WeaponOwner, BOOM_SIZE, WeaponDesc.Damage, SplashExplosionStandardSplash, WEAPONTYPE_MOVINGMINE);
}
