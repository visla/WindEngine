/*
       File: CWeaponStandardLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponStandardLaser.h>
#include <CWeaponFabric.h>
#include <WeTimer.h>
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
#include <WeLightState.h>
#include <WeSound.h>
#include <WePhysics.h>
#include <CMainGamePhase.h>
#include <Helper.h>


void CWeaponStandardLaser::InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation)
{
	Type = ACTORTYPE_WEAPON;
	WeaponOwner = Owner;
  	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.StandardLaser");

	WeMatrix TransMatrix, LaserMatrix, TuberMatrix;

	FixedDirection = WeapDirection;
	OrientMatrix = WeapOrientation;

	D3DXMatrixTranslation(&TransMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	D3DXMatrixTranslation(&TuberMatrix, WeapLocation.x, WeapLocation.y, WeapLocation.z);

	D3DXMatrixMultiply(&LaserMatrix, &TransMatrix, &OrientMatrix);
	D3DXMatrixMultiply(&LaserMatrix, &LaserMatrix, &TuberMatrix);
	SetTransformation(LaserMatrix);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, LaserMatrix, PRIMARY_PHYS_BOX, COLLISION_GROUP_WEAPON, 0.1f, false);
	WeBox Box(PRIMARY_CCD_BOX_MIN, PRIMARY_CCD_BOX_MAX);
	PhysicHandle->SetBoxCCDSkeleton(Box);
	PhysicHandle->SetCCDThreshold(0);
	PhysicHandle->SetDisableGravity(true);
	PhysicHandle->SetTransformation(LaserMatrix);
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);

	Light = NULL;
	//WeLightState LS;
	//Light = new WeLight;
	//LS.Diffuse = WeColor(0.0f, 0.0f, 1.0f, 1.0f);
	//LS.Intensity = 0.5f;
	//LS.SpecularIntensity = 0.5f;
	//LS.Duration = 1.0f;
	//Light->IsDynamic = true;
	//Light->Type = LIGHT_POINT;
	//Light->Range = 30.0f;
	//Light->TM = TM;
	//Light->AddLightState(&LS);
	//Light->Initialize();
	//WeaponParent->AddActor(Light);

	char Visla[100];
	sprintf(Visla, "Sounds.Weapons.StandardBolt%d", Rnd(1, 3));
	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto(String(Visla), true);
	if (FireSound)
	{
		FireSound->Update(WeaponOwner->Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}
}

CWeaponStandardLaser::CWeaponStandardLaser(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc)
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

	WeVector AX, AY;
	AX.x = WeapOwnerMatrix._11; AX.y = WeapOwnerMatrix._12; AX.z = WeapOwnerMatrix._13;
	AY.x = WeapOwnerMatrix._21; AY.y = WeapOwnerMatrix._22; AY.z = WeapOwnerMatrix._23;
	WeVector WeapLocation;
	WeapLocation.x = WeapOwnerMatrix._41; WeapLocation.y = WeapOwnerMatrix._42; WeapLocation.z = WeapOwnerMatrix._43;
	WeapLocation += -(0.5f * AX) + ((float)(rand() % 1000) * AX / 1000.0f);
	WeapLocation += -(0.5f * AY) + ((float)(rand() % 1000) * AY / 1000.0f);

	WeMatrix WeapOrientation;
	WeapOrientation = WeapOwnerMatrix;
	WeapOrientation._41 = 0.0f; WeapOrientation._42 = 0.0f; WeapOrientation._43 = 0.0f;

	WeaponDesc = *Desc;
	InitializeWeapon(Owner, RelativeSpawnLoc, WeapLocation, WeapDirection, WeapOrientation);
}

CWeaponStandardLaser::~CWeaponStandardLaser()
{
	if (Light)
		Light->LifeTime = 0.0001f;
}

void CWeaponStandardLaser::Tick()
{
	PhysicHandle->SetLinearVelocity(WeaponDesc.Speed * FixedDirection);
	//if (WeaponDesc.Speed == 0 || (FixedDirection.x == 0 && FixedDirection.y == 0 && FixedDirection.z == 0))
	//{
	//	FILE *f = fopen("weaponslog.txt", "at+");
	//	fprintf(f, "LASER 000\n");
	//	fflush(f);
	//	fclose(f);	
	//}

	if (Light)
		Light->Location = Location;

	CWeapon::Tick();	
}

void CWeaponStandardLaser::OnHit(WeActor *Victim)
{
	if (Dying) return;

	LifeTime = 0.0001f;
	PhysicHandle->SetResponse(false);
	Dying = true;

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	new CExplosionLocalized((Location - FixedDirection), WeaponOwner, WeaponDesc.Damage, PlayerVictim, LocalizedExplosionStandardLaser, WEAPONTYPE_STANDARDLASER);
}
