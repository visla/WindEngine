/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeaponNukeRocket.h>
#include <CWeaponFabric.h>
#include <WeParticleSystem.h>
#include <WeMaterialManager.h>
#include <WeTimer.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <CExplosionNuke.h>
#include <ActorTags.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeLight.h>
#include <WeAudioDevice.h>
#include <WeSound.h>

WeParticleSystem *CWeaponNukeRocket::StaticPS = NULL;


void CWeaponNukeRocket::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.BigAssRocket");

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
	PS->Properties.ParticleDirection = WeVector(0.0f, 0.0f, 0.0f);
	WeaponParent->AddActor(PS);

	Light = new WeLight();
	WeLightState ls;
	ls.Range = 80.0f;
	ls.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	Light->AddKeyState(ls, 1.0f);
	Light->IsDynamic = true;
	Light->AffectDynamicActors = true;
	Light->Location = Location;
	WeaponParent->AddActor(Light);

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.NukeRocketLaunch", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.NukeRocketEngine", true);
	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * Direction));
		Sound->Play(true);
	}
}

CWeaponNukeRocket::CWeaponNukeRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
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

CWeaponNukeRocket::~CWeaponNukeRocket()
{
	Light->LifeTime = 0.0001f;
	PS->Properties.EmitRate = 0;
	PS->LifeTime = PS->Properties.LifeTime;
	SAFE_DELETE(Sound);
}

void CWeaponNukeRocket::Tick()
{
	WeVector AngularVelocity = WeVector(0.0f, 0.0f, 0.0f);
	WeVector AX, AY, AZ;

	GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AZ, &AZ);

	if (HomingTarget)
	{
		WeVector VecDistance = HomingTarget->Location - Location;
		D3DXVec3Normalize(&VecDistance, &VecDistance);

		D3DXVec3Cross(&AngularVelocity, &AZ, &VecDistance);
		D3DXVec3Normalize(&AngularVelocity, &AngularVelocity);
		AngularVelocity *= 0.3f;
	}

	PhysicHandle->SetAngularVelocity(AngularVelocity);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * AZ);

	PS->Location = Location;
	Light->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * AZ));
	}

	CWeapon::Tick();
}

void CWeaponNukeRocket::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	new CExplosionNuke(Location, WeaponOwner, NUKE_ROCKET_RADIUS, WeaponDesc.Damage, WEAPONTYPE_NUKEROCKET);
}
