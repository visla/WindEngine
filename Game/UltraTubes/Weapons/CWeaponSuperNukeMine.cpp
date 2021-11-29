/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponSuperNukeMine.h>
#include <CTuber.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <ActorTags.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <CExplosionNuke.h>
#include <WeAudioDevice.h>
#include <WeSound.h>


void CWeaponSuperNukeMine::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_MINE;
	LifeTime = NUKE_MINE_LIFETIME;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.SuperNukeMine");

	WeMatrix TransMatrix, MineMatrix, TuberMatrix, OrientMatrix;

	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&MineMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&MineMatrix, &MineMatrix, &TuberMatrix);
	Location.x = MineMatrix._41; Location.y = MineMatrix._42; Location.z = MineMatrix._43;

	PhysicHandle = WePhysics::Instance()->CreateSphere(this, Location, 0.1f, COLLISION_GROUP_TRANSMINE, 0.1f, false);
	PhysicHandle->SetDisableGravity(true);

	LevitationParam = 0.0f;

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.MineDrop", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Mines.SuperNukeMine", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(true);
	}
	
	ExplosionTimer = 0.0f;
}


CWeaponSuperNukeMine::CWeaponSuperNukeMine(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix)
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

CWeaponSuperNukeMine::~CWeaponSuperNukeMine()
{
	SAFE_DELETE(Sound);
}

void CWeaponSuperNukeMine::Tick()
{
	if (Dying) return;

	float Time = WeTimer::Instance()->GetDeltaTimeF();
	ExplosionTimer += Time;

	WeVector Vel, AVel;

	LevitationParam += Time;
	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2, 0.0f);

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	if (Sound)
	{
		Sound->Update(Location, Vel);
	}

	if (ExplosionTimer >= NUKE_MINE_LIFETIME)
	{
		LifeTime = 0.0001f;
		PhysicHandle->SetResponse(false);
		Dying = true;
		new CExplosionNuke(Location, WeaponOwner, NUKE_MINE_SIZE, WeaponDesc.Damage, WEAPONTYPE_SUPERNUKEMINE);
	}

	CWeapon::Tick();
}
