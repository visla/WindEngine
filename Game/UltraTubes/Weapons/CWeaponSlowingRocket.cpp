/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeaponSlowingRocket.h>
#include <CWeaponFabric.h>
#include <WeParticleSystem.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WeMaterialManager.h>
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

#define SPEED_MULTIPLIER	0.3f

WeParticleSystem *CWeaponSlowingRocket::StaticPS = NULL;


void CWeaponSlowingRocket::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	this->Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.SlowingRocket");

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

	//for (int i = 0; i < 2; i++)
	//{
	//	Trail[i] = new WeTrail();
	//	Trail[i]->MaxSegments = 30;
	//	Trail[i]->Location = Location;
	//	Trail[i]->SegmentCreationTime = .1f;
	//	Trail[i]->SegmentWidth = 0.5;
	//	Trail[i]->SegmentColor.r = 0.0f;
	//	Trail[i]->SegmentColor.g = 0.0f;
	//	Trail[i]->SegmentColor.b = 1.0f;
	//	Trail[i]->Material = WeMaterialManager::Instance()->Load("Weapons.Trails.SlowingRocket");
	//	Trail[i]->Initialize();
	//	WeaponParent->AddActor(Trail[i]);
	//}
	//WeMatrix TrailMat;
	//Trail[1]->GetTransformation(TrailMat);
	//D3DXMatrixRotationZ(&TrailMat, (D3DX_PI / 2));
	//Trail[1]->SetTransformation(TrailMat);

	PS = new WeParticleSystem();
	StaticPS->Clone(PS);
	PS->Initialize();
	PS->Location = Location;
	WeaponParent->AddActor(PS);

	WeLightState LS;
	Light = new WeLight;
	LS.Diffuse = WeColor(0.0f, 1.0f, 1.0f, 1.0f);
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

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.SlowingRocketLaunch", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.SlowingRocketEngine", true);
	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * Direction));
		Sound->Play(true);
	}
}


CWeaponSlowingRocket::CWeaponSlowingRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
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

CWeaponSlowingRocket::~CWeaponSlowingRocket()
{
	Light->LifeTime = 0.0001f;

	//for (int i = 0; i < 2; i++)
	//	Trail[i]->LifeTime = (float)((Trail[i]->MaxSegments / 2) * Trail[i]->SegmentCreationTime);

	PS->Properties.EmitRate = 0;
	PS->LifeTime = PS->Properties.LifeTime;

	SAFE_DELETE(Sound);
}

void CWeaponSlowingRocket::Tick()
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

	//for (int i = 0; i < 2; i++)
	//	Trail[i]->Location = Location;

	PS->Location = Location;
	Light->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
	}

	CWeapon::Tick();
}

void CWeaponSlowingRocket::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	if ( PlayerVictim && (!PlayerVictim->Invulnerable) )
		PlayerVictim->MultiplyVelocity(SPEED_MULTIPLIER);

	new CExplosionLocalized((Location - Direction), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionSlowingRocket, WEAPONTYPE_SLOWINGROCKET);
}
