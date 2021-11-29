/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponDoubleLaser.h>
#include <CWeaponFabric.h>
#include <WeActor.h>
#include <WeTimer.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <WePhysicModel.h>
#include <ActorTags.h>
#include <CollisionGroups.h>
#include <CExplosionLocalized.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeLight.h>
#include <WeAudioDevice.h>
#include <WeSound.h>


void CWeaponDoubleLaser::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.StandardLaser");

	WeMatrix TransMatrix, BeamTransMatrix, LaserMatrix, TuberMatrix;

	FixedDirection = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	if (Primary)
		D3DXMatrixTranslation(&BeamTransMatrix, -1.0f, 0.0f, 0.0f);
	else
		D3DXMatrixTranslation(&BeamTransMatrix, 1.0f, 0.0f, 0.0f);

	D3DXMatrixMultiply(&LaserMatrix, &BeamTransMatrix, &TransMatrix);
	D3DXMatrixMultiply(&LaserMatrix, &LaserMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&LaserMatrix, &LaserMatrix, &TuberMatrix);
	SetTransformation(LaserMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(LaserMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);

	Light = new WeLight();
	WeLightState ls;
	ls.Range = 20.0f;
	ls.Diffuse = WeColor(1.0f, 0.0f, 0.0f, 1.0f);
	Light->AddKeyState(ls, 1.0f);
	Light->IsDynamic = true;
	Light->AffectDynamicActors = true;
	Light->Location = Location;
	WeaponParent->AddActor(Light);

	if (Primary)
	{
		WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.DoubleBolt", true);
		if (FireSound)
		{
			FireSound->Update(WeaponOwner->Location, WeVector(0.0f, 0.0f, 0.0f));
			FireSound->Play(false);
			FireSound->DieOnStop();
		}
	}
}

CWeaponDoubleLaser::CWeaponDoubleLaser(CTuber* Owner, bool IsPrimary, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
{
	Primary = IsPrimary;

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

CWeaponDoubleLaser::~CWeaponDoubleLaser()
{
	Light->LifeTime = 0.0001f;
}

void CWeaponDoubleLaser::Tick()
{
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);

	Light->Location = Location;

	CWeapon::Tick();
}

void CWeaponDoubleLaser::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	CExplosionLocalized *Explosion;
	Explosion = new CExplosionLocalized((Location - FixedDirection), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionStandardLaser, WEAPONTYPE_DOUBLELASER);
}
