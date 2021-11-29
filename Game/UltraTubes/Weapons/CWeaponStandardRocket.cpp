/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeaponStandardRocket.h>
#include <CWeaponFabric.h>
#include <WeWorld.h>
#include <WeParticleSystem.h>
#include <WeTimer.h>
#include <WeMaterialManager.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <CExplosionLocalized.h>
#include <ActorTags.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeLight.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeCamera.h>
#include <WeVector.h>
#include <CGame.h>
#include <WeRadialTrail.h>
#include <CConfig.h>

#define	LIFETIME					20.0f

WeParticleSystem *CWeaponStandardRocket::StaticPS = NULL;

void CWeaponStandardRocket::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_WEAPON;
	LifeTime = LIFETIME;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.StandardRocket");

	WeMatrix TransMatrix, RocketMatrix, TuberMatrix;

	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&RocketMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&RocketMatrix, &RocketMatrix, &TuberMatrix);
	SetTransformation(RocketMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(RocketMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * Direction);

	PS = new WeParticleSystem();
	StaticPS->Clone(PS);
	PS->Initialize();
	PS->Location = Location;
	WeaponParent->AddActor(PS);

	WeLightState LS;
	Light = new WeLight;
	LS.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	LS.Intensity = 0.5f;
	LS.SpecularIntensity = 0.5f;
	LS.Duration = 1.0f;
	Light->IsDynamic = true;
	Light->Type = LIGHT_POINT;
	Light->Range = 30.0f;
	Light->TM = TM;
	Light->AddLightState(&LS);
	Light->Initialize();
	WeaponParent->AddActor(Light);

	Trail = NULL;
	if (GetConfigInt("tuber.RocketTrail.Enabled", 0) != 0)
	{
		Trail = new WeRadialTrail();
		Trail->Location = Location;
		Trail->Material = WeMaterialManager::Instance()->Load("Materials.Trails.Rocket");
		Trail->Initialize();

		Trail->MaxSegments = GetConfigInt("tuber.RocketTrail.MaxSegments", 10);
		Trail->SegmentWidth = GetConfigFloat("tuber.RocketTrail.SegmentWidth", 1.0f);
		Trail->StartFadeFactor = GetConfigFloat("tuber.RocketTrail.StartFadeFactor", 0.0f);
		Trail->SegmentCreationTime = GetConfigFloat("tuber.RocketTrail.SegmentCreationTime", 0.3f);
		Trail->RingSegmentCount = GetConfigInt("tuber.RocketTrail.RingSegmentCount", 8);
		Trail->FromAlpha = GetConfigFloat("tuber.RocketTrail.FromAlpha", 1.0f);
		Trail->ToAlpha = GetConfigFloat("tuber.RocketTrail.ToAlpha", 0.0f);
		Trail->FromRadius = GetConfigFloat("tuber.RocketTrail.FromRadius", 1.0f);
		Trail->ToRadius = GetConfigFloat("tuber.RocketTrail.ToRadius", 0.1f);

		Trail->Initialize();
		WeaponParent->AddActor(Trail);
	}

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.StandardRocketLaunch", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.StandardRocketEngine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
}



CWeaponStandardRocket::CWeaponStandardRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
{
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


CWeaponStandardRocket::~CWeaponStandardRocket()
{
	Light->LifeTime = 0.0001f;
	PS->Properties.EmitRate = 0;
	PS->LifeTime = PS->Properties.LifeTime;
	SAFE_DELETE(Sound);

	if (Trail)
		Trail->LifeTime = (float)((Trail->MaxSegments / 2) * Trail->SegmentCreationTime);
}


void CWeaponStandardRocket::Tick()
{
	WeVector AngularVelocity = WeVector(0.0f, 0.0f, 0.0f);
	WeVector AX, AY, AZ;

	GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AZ, &AZ);

	if (HomingTarget && WeaponOwner->ValidateTarget(HomingTarget))
	{
		WeVector VecDistance = HomingTarget->Location - Location;
		D3DXVec3Normalize(&VecDistance, &VecDistance);

		D3DXVec3Cross(&AngularVelocity, &AZ, &VecDistance);
		D3DXVec3Normalize(&AngularVelocity, &AngularVelocity);
		AngularVelocity *= 1.0f;
	}

	PhysicHandle->SetAngularVelocity(AngularVelocity);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * AZ);

	PS->Location = Location;
	PS->Properties.Gravity = -Direction * 0.3f;
	Light->Location = Location;
	if (Trail)
		Trail->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * AZ));
	}

	CWeapon::Tick();
}


void CWeaponStandardRocket::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	new CExplosionLocalized((Location - Direction), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionStandardRocket, WEAPONTYPE_STANDARDROCKET);
}
