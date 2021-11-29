/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeaponTranslocator.h>
#include <CWeaponFabric.h>
#include <WeParticleSystem.h>
#include <WeModelManager.h>
#include <WeMaterialManager.h>
#include <WeTimer.h>
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


WeParticleSystem *CWeaponTranslocator::StaticPS[3] = { NULL, NULL, NULL };


void CWeaponTranslocator::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	this->Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	//Model = WeModelManager::Instance()->Load("Weapons.Models.Translocator");

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

	for (int i = 0; i < 3; ++i)
	{
		PS[i] = new WeParticleSystem();
		StaticPS[i]->Clone(PS[i]);
		PS[i]->Initialize();
		PS[i]->Location = Location;
		WeaponParent->AddActor(PS[i]);
	}

	WeLightState LS;
	Light = new WeLight;
	LS.Diffuse = WeColor(1.0f, 0.5f, 0.1f, 1.0f);
	LS.Intensity = 0.7f;
	LS.SpecularIntensity = 0.5f;
	LS.Duration = 1.0f;
	Light->IsDynamic = true;
	Light->Type = LIGHT_POINT;
	Light->Range = 30.0f;
	Light->TM = TM;
	Light->AddLightState(&LS);
	Light->Initialize();
	WeaponParent->AddActor(Light);

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.TranslocatorLaunch", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.TranslocatorEngine", true);
	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * Direction));
		Sound->Play(true);
	}
}

CWeaponTranslocator::CWeaponTranslocator(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
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

CWeaponTranslocator::~CWeaponTranslocator()
{
	Light->LifeTime = 0.0001f;
	for (int i = 0; i < 3; ++i)
	{
		PS[i]->Properties.EmitRate = 0;
		PS[i]->LifeTime = PS[i]->Properties.LifeTime;
	}
	SAFE_DELETE(Sound);
}

void CWeaponTranslocator::Tick()
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
		AngularVelocity *= 2.0f;
	}

	PhysicHandle->SetAngularVelocity(AngularVelocity);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * AZ);

	Light->Location = Location;
	for (int i = 0; i < 3; ++i)
		PS[i]->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * AZ));
	}

	CWeapon::Tick();
}

void CWeaponTranslocator::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	if (!PlayerVictim)
	{
		new CExplosionLocalized((Location - Direction), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionTranslocator, WEAPONTYPE_TRANSLOCATOR);
		return;
	}

	new CExplosionLocalized(Location, WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionTranslocator, WEAPONTYPE_TRANSLOCATOR);
	new CExplosionLocalized(WeaponOwner->Location, WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionTranslocator, WEAPONTYPE_TRANSLOCATOR);

	WeVector Temp;
	Temp = PlayerVictim->Location;
	PlayerVictim->MoveTo(WeaponOwner->Location);
	WeaponOwner->MoveTo(Temp);
}
