/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponQuantumLaser.h>
#include <CWeaponFabric.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <CExplosionLocalized.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeLight.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeTextureManager.h>
#include <WeParticleSystem.h>


WeParticleSystem *CWeaponQuantumLaser::StaticPS = NULL;

void CWeaponQuantumLaser::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.QuantumBolt");

	WeMatrix TransMatrix, LaserMatrix, TuberMatrix;

	FixedDirection = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&LaserMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&LaserMatrix, &LaserMatrix, &TuberMatrix);
	SetTransformation(LaserMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(LaserMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);

	Light = new WeLight;

	Light->IsDynamic = true;
	Light->Type = LIGHT_POINT;
	Light->Range = 80.0f;
	Light->TM = TM;

	////////////////////////////////////////////////////////////////////////////
	//// light flare
	//Light->Corona = true;
	//Light->CoronaTextureName = "Flare.Flare3Quantumlaser.Corona";
	//Light->CoronaFixedSize = true;
	//Light->CoronaRotation = true;
	//Light->CoronaSizeX = 0.2f;
	//Light->CoronaSizeY = 0.1f;

	//Light->Flares = true;
	//char gabl[100];
	//for (int i = 0; i < 5; ++i)
	//{
	//	sprintf(gabl, "Flare.Flare3Quantumlaser.Ring%d", (i + 1));
	//	Light->FlareTextureNames[i] = String(gabl);
	//}
	//Light->FlareSizeX = 0.3f;
	//Light->FlareSizeY = 0.3f;

	//Light->FadeInTime = 0.1f;
	//Light->FadeOutTime = 0.1f;
	////////////////////////////////////////////////////////////////////////////

	WeLightState LS;
	LS.Diffuse = WeColor(0.0f, 1.0f, 0.0f, 1.0f);
	LS.Intensity = 1.1f;
	LS.SpecularIntensity = 0.5f;
	LS.Duration = 1.0f;
	//LS.CoronaAlpha = 1.0f;
	//LS.FlareAlpha[0] = 0.1f;
	//for (int i = 1; i < 5; ++i)
	//	LS.FlareAlpha[i] = 0.5f;
	Light->AddLightState(&LS);
	Light->Initialize();
	WeaponParent->AddActor(Light);

	PS = new WeParticleSystem();
	StaticPS->Clone(PS);
	PS->Initialize();
	PS->Location = Location;
	WeaponParent->AddActor(PS);

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.QuantumBolt", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}
}

CWeaponQuantumLaser::CWeaponQuantumLaser(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
{
	WeVector WeapDirection;
	CTuber* Target = Owner->GetBestEnemyTargetLock(BOLT_LOCK_DISTANCE, BOLT_LOCK_MINDOT);
	if (Target)
	{
		WeapDirection = (Target->Location - Owner->Location);
	}
	else
	{
        WeapDirection.x = WeapOwnerMatrix._31; WeapDirection.y = WeapOwnerMatrix._32; WeapDirection.z = WeapOwnerMatrix._33;
	}
	D3DXVec3Normalize(&WeapDirection, &WeapDirection);

	WeVector WeapLocation;
	WeapLocation.x = WeapOwnerMatrix._41; WeapLocation.y = WeapOwnerMatrix._42; WeapLocation.z = WeapOwnerMatrix._43;

	WeMatrix WeapOrientation;
	WeapOrientation = WeapOwnerMatrix;
	WeapOrientation._41 = 0.0f; WeapOrientation._42 = 0.0f; WeapOrientation._43 = 0.0f;

	WeaponDesc = *Desc;
	InitializeWeapon(Owner, RelativeSpawnLoc, WeapLocation, WeapDirection, WeapOrientation);
}

CWeaponQuantumLaser::~CWeaponQuantumLaser()
{
	Light->LifeTime = 0.0001f;

	PS->Properties.EmitRate = 0;
	PS->LifeTime = PS->Properties.LifeTime;
}

void CWeaponQuantumLaser::Tick()
{
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);

	PS->Location = Location;
	Light->Location = Location;

	CWeapon::Tick();
}

void CWeaponQuantumLaser::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	CExplosionLocalized *Explosion;
	Explosion = new CExplosionLocalized((Location - FixedDirection), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionQuantumLaser, WEAPONTYPE_QUANTUMLASER);
}
