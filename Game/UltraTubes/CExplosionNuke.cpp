/*
       File: CExplosionNuke.cpp
	   Author: Goran Bogdanovic
*/


#include <CExplosionNuke.h>
#include <CTuber.h>
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
#include <CPlayGamePhase.h>

#define LIFETIME		2.0f
#define LIGHT_RANGE		10000.0f


CExplosionNuke::CExplosionNuke(WeVector &MyLocation, CTuber *MyPyromaniac, float MySize, float MyDamage, WEAPONTYPE CreatorID, bool FlashScreen, bool AffectActors)
{
	Location = MyLocation;
	Pyromaniac = MyPyromaniac;
	Size = MySize;
	Damage = MyDamage;
	Creator = CreatorID;
	World = Pyromaniac->GetWorld();
	World->AddActor(this);
	Flash = FlashScreen;

	LifeTime = LIFETIME;
	PhysicHandle = WePhysics::Instance()->CreateSphere(this, Location, Size * 2, COLLISION_GROUP_EXPLOSION, 10.0f, false, WETRIGGER_ON_ENABLE);
	PhysicHandle->SetDisableGravity(true);

	if (AffectActors)
	{
		NotifyArea = new CTrigger(this);
		NotifyArea->Location = Location;
		World->AddActor(NotifyArea);
		NotifyArea->PhysicHandle = WePhysics::Instance()->CreateSphere(NotifyArea, Location, Size * 2, COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
		NotifyArea->PhysicHandle->SetDisableGravity(true);
	}
	else
		NotifyArea = NULL;

    Sphere = new WeActor();
	Sphere->Model = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionNuke");
	Sphere->Location = Location;
	World->AddActor(Sphere);

	WeStaticModel *StaticRing[2];
	StaticRing[0] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionPlaneShockwave1");
	StaticRing[1] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionPlaneShockwave2");
	for (int i = 0; i < 2; i++)
	{
		Ring[i] = new WeActor();
		Ring[i]->Model = StaticRing[i];
		Ring[i]->Location = Location;
		World->AddActor(Ring[i]);
	}

	//Light = new WeLight();
	//WeLightState ls;
	//ls.Range = 0.0f;
	//ls.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	//Light->AddKeyState(ls, 1.0f);
	//Light->IsDynamic = true;
	//Light->AffectDynamicActors = true;
	//Light->Location = Location;
	//World->AddActor(Light);

	WeSound *FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.NukeRocket", true);
	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}
}


CExplosionNuke::~CExplosionNuke()
{
	if (NotifyArea)
		NotifyArea->LifeTime = 0.0001f;
	Sphere->LifeTime = 0.0001f;
	for (int i = 0; i < 2; i++)
		Ring[i]->LifeTime = 0.0001f;
	//Light->LifeTime = 0.0001f;
}


void CExplosionNuke::Tick()
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	float Step = ((LIGHT_RANGE / LifeTime) * time);
	//Light->Range += Step;

	float Move = (Size / LifeTime) * time;
	Sphere->Scale = WeVector(Sphere->Scale.x + Move, Sphere->Scale.y + Move, Sphere->Scale.z + Move);
	Ring[0]->Scale = WeVector(Ring[0]->Scale.x + Move, Ring[0]->Scale.y, Ring[0]->Scale.z + Move);
	Ring[1]->Scale = WeVector(Ring[1]->Scale.x, Ring[1]->Scale.y + Move, Ring[1]->Scale.z + Move);

	if ( Flash && (ObjectAge >= (LIFETIME / 2)))
	{
		Flash = false;
		WeVector Distance = (CMainGamePhase::Instance->InputPlayer->Location - Location);
		if (D3DXVec3Length(&Distance) <= (Size * 4))
			CMainGamePhase::Instance->InputPlayer->CurrentFlash = TuberFlashNuke;
	}

	CExplosion::Tick();
}

void CExplosionNuke::OnNotify(WeActor *Victim)
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();

	CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	WeVector DistanceVector, AccelVector;

	DistanceVector = (Victim->Location - Location);
	D3DXVec3Normalize(&AccelVector, &DistanceVector);
	AccelVector *= (4000.0f * Time);

	if (PlayerVictim && PlayerVictim->IsPushable())
	{
		WeVector tmpV, xV, Yv, Zv;
		PlayerVictim->GetLocalAxis(xV, Yv, Zv);
		D3DXVec3Cross(&tmpV, &Yv, &AccelVector);
		PlayerVictim->Velocity += AccelVector;
		//PlayerVictim->AngularVelocity += tmpV / 40.0f;
		return;
	}

	if (Victim->PhysicHandle && CPlayGamePhase::ActorPushable(Victim))
		Victim->PhysicHandle->AddVelocity(AccelVector);
}


void CExplosionNuke::Affect(CTuber *Victim)
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	CExplosion::Affect(Victim, (Damage * time));

	//CTuberPartFabric::Instance()->CreateTuberPart(Victim, true, ((rand() % 3) == 0) ? true : false);

	//---camera shaking when nuked---//
	//if (Victim == CMainGamePhase::Instance->InputPlayer)
	//{
	//	CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShaking;
	//	CMainGamePhase::Instance->Camera.StateDefaultingTime = 0.5f;
	//}
	//-------------------------------//
}
