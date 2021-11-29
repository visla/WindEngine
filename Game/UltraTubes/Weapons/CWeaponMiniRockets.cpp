/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CWeaponMiniRockets.h>
#include <CWeaponFabric.h>
#include <WeWorld.h>
#include <WeTimer.h>
#include <WeMaterialManager.h>
#include <CTuber.h>
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


void CWeaponMiniRockets::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, short MyProjectileID, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	this->Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.MiniRocket");

	ProjectileID = MyProjectileID;
	Angle = ((2 * D3DX_PI / 5.0f) * (MyProjectileID + 1));

	WeMatrix SpawnMatrix, TransMatrix, RotMatrix, MiniTransMatrix, MyMatrix;
	Direction = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixRotationAxis(&RotMatrix, &WeVector(0.0f, 0.0f, 1.0f), Angle);
	float TransPos = 2.0f * cos((2 * D3DX_PI) / (ProjectileID + 1));
	D3DXMatrixTranslation(&MiniTransMatrix, TransPos, 0.0f, 0.0f);
	D3DXMatrixTranslation(&TransMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);
	D3DXMatrixTranslation(&SpawnMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);

	D3DXMatrixMultiply(&MyMatrix, &MiniTransMatrix, &RotMatrix);
	D3DXMatrixMultiply(&MyMatrix, &MyMatrix, &SpawnMatrix);
	D3DXMatrixMultiply(&MyMatrix, &MyMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&MyMatrix, &MyMatrix, &TransMatrix);
	SetTransformation(MyMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	PhysicHandle->SetBoxCCDSkeleton(WeBox(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX));
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(MyMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * Direction);

	for (int i = 0; i < 2; i++)
	{
		Trail[i] = new WeTrail();
		Trail[i]->MaxSegments = 20;
		Trail[i]->Location = Location;
		Trail[i]->SegmentCreationTime = .1f;
		Trail[i]->SegmentWidth = 0.2f;
		Trail[i]->SegmentColor.r = 1.0f;
		Trail[i]->SegmentColor.g = 0.0f;
		Trail[i]->SegmentColor.b = 1.0f;
		Trail[i]->Material = WeMaterialManager::Instance()->Load("Weapons.Trails.MiniRocket");
		Trail[i]->Initialize();
		WeaponParent->AddActor(Trail[i]);
	}
	WeMatrix TrailMat;
	Trail[1]->GetTransformation(TrailMat);
	D3DXMatrixRotationZ(&TrailMat, (D3DX_PI / 2));
	Trail[1]->SetTransformation(TrailMat);

	if ((MyProjectileID % 5) == 0)
	{
		Light = new WeLight();
		WeLightState ls;
		ls.Range = 30.0f;
		ls.Diffuse = WeColor(1.0f, 0.0f, 1.0f, 1.0f);
		Light->AddKeyState(ls, 1.0f);
		Light->IsDynamic = true;
		Light->AffectDynamicActors = true;
		Light->Location = Location;
		WeaponParent->AddActor(Light);
	}
	else
		Light = NULL;

	if (MyProjectileID == 1)
	{
		WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.MiniRocketLaunch", true);
		if (FireSound)
		{
			FireSound->Update(WeaponOwner->Location, WeVector(0.0f, 0.0f, 0.0f));
			FireSound->Play(false);
			FireSound->DieOnStop();
		}

		Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.MiniRocketEngine", true);
		if (FireSound)
		{
			Sound->Update(Location, (WeaponDesc.Speed * Direction));
			Sound->Play(true);
		}
	}
	else
	{
		Sound = NULL;
	}
}

CWeaponMiniRockets::CWeaponMiniRockets(CTuber* Owner, short MyProjectileID, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
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
	InitializeWeapon(Owner, RelativeSpawnLoc, MyProjectileID, WeapLocation, WeapDirection, WeapOrientation);
}

CWeaponMiniRockets::~CWeaponMiniRockets()
{
	if (Light)
		Light->LifeTime = 0.0001f;
	for (int i = 0; i < 2; i++)
		Trail[i]->LifeTime = (float)((Trail[i]->MaxSegments / 2) * Trail[i]->SegmentCreationTime);

	SAFE_DELETE(Sound);
}

void CWeaponMiniRockets::Tick()
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
		AngularVelocity *= 1.0f;

		PhysicHandle->SetAngularVelocity(AngularVelocity);
		PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * AZ);
	}
	else
	{
		WeVector Vel;
		WeMatrix VelMatrix;
		float TimeAbs = WeTimer::Instance()->GetElapsedTimeF();

		Vel.x = 4.0f * cos(8 * TimeAbs) * ((ProjectileID % 2 == 0) ? 1 : -1);
		Vel.y = 4.0f * sin(8 * TimeAbs) * ((ProjectileID % 2 == 0) ? 1 : -1);
		Vel.z = WeaponDesc.Speed * 1.0f;

		D3DXMatrixTranslation(&VelMatrix, Vel.x, Vel.y, Vel.z);
		D3DXMatrixMultiply(&VelMatrix, &VelMatrix, &OrientMatrix);
		Vel.x = VelMatrix._41; Vel.y = VelMatrix._42; Vel.z = VelMatrix._43;

		PhysicHandle->SetLinearVelocity(Vel);
	}

	for (int i = 0; i < 2; i++)
		Trail[i]->Location = Location;

	if (Light)
		Light->Location = Location;

	if (Sound)
	{
		Sound->Update(Location, (WeaponDesc.Speed * AZ));
	}\

	CWeapon::Tick();
}

void CWeaponMiniRockets::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	CExplosionLocalized *Explosion;
	Explosion = new CExplosionLocalized((Location - Direction), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionMiniRocket, WEAPONTYPE_MINIROCKETS);
}
