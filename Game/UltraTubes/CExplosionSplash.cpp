/*
       File: CExplosionSplash.cpp
	   Author: Goran Bogdanovic
	   Date: 14.04.2006.
	   Version: 1.0
*/


#include <CExplosionSplash.h>
#include <CTuber.h>
#include <WeParticleSystem.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <CollisionGroups.h>
#include <WeModelManager.h>
#include <CTuberPartFabric.h>
#include <CTrigger.h>
#include <ActorTags.h>
#include <CMainGamePhase.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeMesh.h>
#include <CPlayGamePhase.h>

#define LIGHT_RANGE	30.0f


CExplosionSplash::CExplosionSplash(WeVector &MyLocation, CTuber *MyPyromaniac, float MySize, float MyDamage, SplashExplosionType MyExplosionType, WEAPONTYPE CreatorID)
{
	Location = MyLocation;
	Pyromaniac = MyPyromaniac;
	Size = MySize;
	Damage = MyDamage;
	ExplosionType = MyExplosionType;
	Creator = CreatorID;
	World = Pyromaniac->GetWorld();
	World->AddActor(this);

	PhysicHandle = WePhysics::Instance()->CreateSphere(this, Location, Size, COLLISION_GROUP_EXPLOSION, 10.0f, false, WETRIGGER_ON_ENABLE);
	PhysicHandle->SetDisableGravity(true);

	NotifyArea = new CTrigger(this);
	NotifyArea->Location = Location;
	World->AddActor(NotifyArea);
	NotifyArea->PhysicHandle = WePhysics::Instance()->CreateSphere(NotifyArea, Location, Size, COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
	NotifyArea->PhysicHandle->SetDisableGravity(true);

	WeSound *FireSound = NULL;
	if ((rand() % 2) == 0)
		FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.HomingMineExplosion1", true);
	else
		FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.HomingMineExplosion2", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	for (int i = 0; i < 2; i++)
		CTuberPartFabric::Instance()->CreateTuberPart(this, true, ((rand() % 3) == 0) ? true : false);

	WeColor ExplosionColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);

	switch(ExplosionType)
	{
	case SplashExplosionHomingMine:
		BoomSplash[0] = new WeActor();
		BoomSplash[0]->Model = new WeStaticModel();
		BoomSplash[0]->Model->Clone(WeModelManager::Instance()->Load("Weapons.Explosions.HomingMine"), false);
		BoomSplash[0]->Model->Mesh->Materials[0].First();
		BoomSplash[0]->LifeTime = (BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		BoomSplash[0]->Location = Location;
		BoomSplash[0]->SetScaling(WeVector(30.0f, 30.0f, 30.0f));
		World->AddActor(BoomSplash[0]);

		ExplosionColor = WeColor(1.0f, 0.0f, 0.0f, 1.0f);
		LifeTime = 1.2f;
		AddExplosiveRefraction(20.0f, LifeTime);
		AddDistantExplosion();
		break;

	case SplashExplosionStandardSplash:
		WeStaticModel *StaticBigAss[3];
		StaticBigAss[0] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket1");
		StaticBigAss[1] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket2");
		StaticBigAss[2] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket3");
		for (int i = 0; i < 3; i++)
		{
            BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			BoomSplash[i]->Model->Clone(StaticBigAss[(rand() % 3)], false);
			BoomSplash[i]->Model->Mesh->Materials[0].First();
			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
			BoomSplash[i]->Location = (Location + WeVector((3.0f / ((rand() % 10) + 1)), (3.0f / ((rand() % 10) + 1)), (3.0f / ((rand() % 10) + 1))));
			World->AddActor(BoomSplash[i]);
		}

		ExplosionColor = WeColor(1.0f, 1.0f, 0.2f, 1.0f);
		LifeTime = 0.8f;
		AddExplosiveRefraction(20.0f, LifeTime);
		AddDistantExplosion();
		break;
	}

	WeLightState LS;
	Light = new WeLight;
	LS.Diffuse = ExplosionColor;
	LS.Intensity = 0.8f;
	LS.SpecularIntensity = 0.5f;
	LS.Duration = 1.0f;
	Light->IsDynamic = true;
	Light->Type = LIGHT_POINT;
	Light->Range = LIGHT_RANGE;
	Light->TM = TM;
	Light->AddLightState(&LS);
	Light->Initialize();
	World->AddActor(Light);
}


CExplosionSplash::~CExplosionSplash()
{
	NotifyArea->LifeTime = 0.0001f;
	Light->LifeTime = 0.0001f;
}


void CExplosionSplash::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();
	float DecStep = ((LIGHT_RANGE / LifeTime) * Time);

	if (Light->Range - DecStep >= 0.0f)
		Light->Range -= DecStep;

	CExplosion::Tick();
}

void CExplosionSplash::OnNotify(WeActor *Victim)
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	WeVector DistanceVector, AccelVector;

	DistanceVector = (Victim->Location - Location);
	D3DXVec3Normalize(&AccelVector, &DistanceVector);
	AccelVector *= (100.0f * Time);

	if (PlayerVictim && PlayerVictim->IsPushable())
	{
		PlayerVictim->Velocity += AccelVector;
		return;
	}

	if (Victim->PhysicHandle && CPlayGamePhase::ActorPushable(Victim))
		Victim->PhysicHandle->AddVelocity(AccelVector);
}


void CExplosionSplash::Affect(CTuber *Victim)
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	CExplosion::Affect(Victim, (Damage * time));

	if (Victim == CMainGamePhase::Instance->InputPlayer && Victim->IsPushable())
	{
		CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShaking;
		CMainGamePhase::Instance->Camera.StateDefaultingTime = 0.1f;
	}
}
