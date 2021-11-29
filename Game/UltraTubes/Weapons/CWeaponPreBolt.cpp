/*
	Filename:	CWeaponPreBolt.cpp
	Author:		Goran Bogdanovic
*/



#include <CWeaponPreBolt.h>
#include <WeTimer.h>
#include <CTuber.h>
#include <ActorTags.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeLight.h>
#include <CWeaponQuantumLaser.h>
#include <WeMesh.h>
#include <WeAudioDevice.h>
#include <WeSound.h>


CWeaponPreBolt::CWeaponPreBolt(CTuber* Owner, WeVector &RelativeSpawnLoc, CWeaponDescription *Desc)
{
	Type = ACTORTYPE_WEAPON;
	LifeTime = 0.0f;
	WeaponOwner = Owner;
	D3DXMatrixTranslation(&RelativeMatrix, RelativeSpawnLoc.x, RelativeSpawnLoc.y, RelativeSpawnLoc.z);
	WeaponDesc = *Desc;

	WeaponParent = Owner->GetWorld();
	WeaponParent->AddActor(this);

	Model = WeModelManager::Instance()->Load("Weapons.Models.QuantumPrefire");

	Light = new WeLight();
	WeLightState ls;
	ls.Range = 10.0f;
	ls.Intensity = 0.0f;
	ls.Diffuse = WeColor(0.0f, 1.0f, 0.0f, 1.0f);
	Light->AddKeyState(ls, 1.0f);
	Light->IsDynamic = true;
	Light->AffectDynamicActors = true;
	Light->Location = Location;
	WeaponParent->AddActor(Light);

	WeMatrix MyMatrix, OwnerMatrix;
	WeaponOwner->GetTransformation(OwnerMatrix);

	D3DXMatrixMultiply(&MyMatrix, &RelativeMatrix, &OwnerMatrix);
	SetTransformation(MyMatrix);

	//Model->Mesh->Materials[0].First();
	//
	//static bool firstshit = false;
	//if (!firstshit)
	//{
	//	firstshit = true;
	//	Model->Mesh->Materials[0].GetItem()->TextureAnimParam4 *= 5.0f;
	//}

	PreBoltTime = 1.0f; //(Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
	PreBoltTimer = 0.0f;

	FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Weapons.QuantumPrefire", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
	}

	SetScaling(WeVector(10.0f, 10.0f, 10.0f));
}

CWeaponPreBolt::~CWeaponPreBolt()
{
	if (Light)
		Light->LifeTime = 0.0001f;

	if (FireSound)
	{
		FireSound->Stop();
		FireSound->DieOnStop();
	}
}

void CWeaponPreBolt::Tick()
{
	PreBoltTimer += WeTimer::Instance()->GetDeltaTimeF();

	if (FireSound)
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));

	if (PreBoltTimer >= PreBoltTime)
	{
		WeVector RelativeSpawnLoc;
		RelativeSpawnLoc.x = RelativeMatrix._41; RelativeSpawnLoc.y = RelativeMatrix._42; RelativeSpawnLoc.z = RelativeMatrix._43;
		WeMatrix FullMatrix;
		WeaponOwner->GetTransformation(FullMatrix);
		CWeaponQuantumLaser *NewBolt = new CWeaponQuantumLaser(WeaponOwner, RelativeSpawnLoc, FullMatrix, &WeaponDesc);
		NewBolt->WeaponID = WEAPONTYPE_QUANTUMLASER;

		LifeTime = 0.0001f;
	}
	else
	{
		SetScaling( WeVector(1.0f, 1.0f, 1.0f) * (10.0f - (7.0f * (PreBoltTimer / PreBoltTime))) );
		Light->LightStates.First();
		Light->LightStates.GetItem().Intensity = 0.7f * (PreBoltTimer / PreBoltTime);

		WeMatrix MyMatrix, OwnerMatrix;
		WeaponOwner->GetTransformation(OwnerMatrix);

		D3DXMatrixMultiply(&MyMatrix, &RelativeMatrix, &OwnerMatrix);
		SetTransformation(MyMatrix);

		if (Light)
			Light->Location = Location;
	}

	CWeapon::Tick();
}
