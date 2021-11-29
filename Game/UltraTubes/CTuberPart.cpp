/*
       File: CTuberPart.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#include <CTuberPart.h>
#include <WeTimer.h>
#include <WeParticleSystem.h>
#include <WeWorld.h>
#include <ActorTags.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeTimer.h>
#include <CExplosionLocalized.h>
#include <CMainGamePhase.h>


#define DEFAULT_LIFETIME	4.0f


WeParticleSystem *CTuberPart::StaticPSMini = NULL;
WeParticleSystem *CTuberPart::StaticPSBurnBaby = NULL;
WeParticleSystem *CTuberPart::StaticPSBurnBabyBURNFire = NULL;
WeParticleSystem *CTuberPart::StaticPSBurnBabyBURNSmoke = NULL;


CTuberPart::CTuberPart(TuberPartPSType PSType)
{
	Type = ACTORTYPE_FRAGMENT;
	LifeTime = DEFAULT_LIFETIME;
	PS = PSType;
	Particle = NULL;
	ParticleAlt = NULL;
	SoundPlayTimer = 0.0f;
	SoundPlayTime = 0.0f;
	Tuber = NULL;

	if (PS == TuberPartPSTypeBurnBabyBURN)
		LifeTime *= 0.7f;
}


CTuberPart::~CTuberPart()
{
	if ( (PS == TuberPartPSTypeBurnBabyBURN) && (!CMainGamePhase::Instance->Map.IsUnloading()) )
		new CExplosionLocalized(Location, CMainGamePhase::Instance->InputPlayer, 0.0f, NULL, LocalizedExplosionTuber, WEAPONTYPE_NONE);

	if (Particle)
	{
		Particle->Properties.EmitRate = 0;
		Particle->LifeTime = Particle->Properties.LifeTime;
	}

	if (ParticleAlt)
	{
		ParticleAlt->Properties.EmitRate = 0;
		ParticleAlt->LifeTime = ParticleAlt->Properties.LifeTime;
	}

	if ((!CMainGamePhase::Instance->Map.IsUnloading()) && (Tuber != NULL) && (Tuber->FirstPart == this))
		Tuber->FirstPart = NULL;
}



void CTuberPart::Tick()
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	if ( (PS != TuberPartPSTypeNone) && (!Particle) )
	{
		WeParticleSystem *BasePS = NULL;

		switch (PS)
		{
		case TuberPartPSTypeMini:
			BasePS = StaticPSMini;
			break;
		case TuberPartPSTypeBurnBaby:
			BasePS = StaticPSBurnBaby;
			break;
		case TuberPartPSTypeBurnBabyBURN:
			BasePS = StaticPSBurnBabyBURNFire;
			break;
		}

		Particle = new WeParticleSystem();
		BasePS->Clone(Particle);
		if (PS == TuberPartPSTypeBurnBabyBURN)
			Particle->Properties.BlendType = WEPARTICLE_BLEND_ADDITIVE;
		Particle->Initialize();
		Particle->Location = Location;
		//Particle->Properties.ParticleDirection = WeVector(0.0f, 0.0f, 0.0f);
		GetWorld()->AddActor(Particle);

		if (PS == TuberPartPSTypeBurnBabyBURN)
		{
			ParticleAlt = new WeParticleSystem();
			StaticPSBurnBabyBURNSmoke->Clone(ParticleAlt);
			ParticleAlt->Initialize();
			ParticleAlt->Location = Location;
			//ParticleAlt->Properties.ParticleDirection = WeVector(0.0f, 0.0f, 0.0f);
			GetWorld()->AddActor(ParticleAlt);
		}
	}

	if (Particle)
		Particle->Location = Location;
	if (ParticleAlt)
		ParticleAlt->Location = Location;

	SoundPlayTimer += time;
	if (SoundPlayTime == 0.0f)
		SoundPlayTime = ( 1.0f / ((rand() % 10) + 1) );

	WeActor::Tick();
}


void CTuberPart::OnCollision(WeActor *Actor)
{
	return;	//sound off

	if (!Particle) return;
	if (SoundPlayTimer < SoundPlayTime) return;

	WeVector MyVel, AcVel;
	PhysicHandle->GetLinearVelocity(MyVel);
	Actor->PhysicHandle->GetLinearVelocity(AcVel);
	if (D3DXVec3LengthSq(&(MyVel - AcVel)) < 70.0f) return;
	SoundPlayTimer = 0.0f;
	SoundPlayTime = 0.0f;

	WeSound *Sound = NULL;
	if ((rand() % 2) == 0)
		Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Generic.ParticleRock", true);
	else
		Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Generic.ParticleRock2", true);

	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(false);
		Sound->DieOnStop();
	}
}
