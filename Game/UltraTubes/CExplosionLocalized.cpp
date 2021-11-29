/*
       File: CExplosionLocalized.cpp
	   Author: Goran Bogdanovic
	   Date: 13.04.2006.
	   Version: 1.0
*/


#include <CExplosionLocalized.h>
#include <CTuber.h>
#include <WeActor.h>
#include <WeModelManager.h>
#include <WeMesh.h>
#include <WeTimer.h>
#include <WeLight.h>
#include <CTuberPartFabric.h>
#include <ActorTags.h>
#include <CMainGamePhase.h>
#include <WePostProcessEffect.h>
#include <WePostProcess.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <CPlayGamePhase.h>
#include <Helper.h>
#include <WePhysics.h>
#include <WeParticleSystem.h>

#define LIGHT_RANGE		40.0f


WePostProcessEffect*	CExplosionLocalized::WaveFX					= NULL;
WeParticleSystem*		CExplosionLocalized::PS						= NULL;
bool					CExplosionLocalized::WaveFXActive			= false;


CExplosionLocalized::CExplosionLocalized(WeVector &MyLocation, CTuber *MyPyromaniac, float MyDamage, CTuber *MyVictim, LocalizedExplosionType MyExplosionType, WEAPONTYPE CreatorID)
{
	WaveFXOwner = false;
	Location = MyLocation;
	Pyromaniac = MyPyromaniac;
	Damage = MyDamage;
	Victim = MyVictim;
	ExplosionType = MyExplosionType;
	Creator = CreatorID;
	World = Pyromaniac->GetWorld();
	World->AddActor(this);
	WeSound *FireSound = NULL;
	LightRange = LIGHT_RANGE;
	Sparks = NULL;
	Pyromaniac->GetLocalAxis(PyroX, PyroY, PyroZ);
	D3DXVec3Normalize(&PyroX, &PyroX);
	D3DXVec3Normalize(&PyroY, &PyroY);
	D3DXVec3Normalize(&PyroZ, &PyroZ);

	bool	CreateParticles	= false;
	bool	ActivateLight	= false;
	WeColor	LightColor		= WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	ShockwaveSize = 0.0f;

	WeVector Distance;

	for (int i = 0; i < 6; ++i)
		BoomSplash[i] = NULL;

	switch(ExplosionType)
	{
	case LocalizedExplosionStandardLaser:
		BoomSplash[0] = new WeActor();
		BoomSplash[0]->Model = new WeStaticModel();
		BoomSplash[0]->Model->Clone(WeModelManager::Instance()->Load("Weapons.Explosions.StandardLaser1"), false);
		BoomSplash[0]->Model->Mesh->Materials[0].First();
		BoomSplash[0]->LifeTime = (BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		World->AddActor(BoomSplash[0]);

		Distance = (Location - MyPyromaniac->Location);
		D3DXVec3Normalize(&Distance, &Distance);
		BoomSplash[0]->Location = Location + (3.0f * Distance);

		LifeTime = 0.1f;
		//ActivateLight = true;
		//LightColor = WeColor(0.388f, 0.827f, 1.0f, 1.0f);
		break;

	case LocalizedExplosionStandardRocket:
		WeStaticModel *StaticStandardRocket[4];
		StaticStandardRocket[0] = WeModelManager::Instance()->Load("Weapons.Explosions.StandardRocket1");
		StaticStandardRocket[1] = WeModelManager::Instance()->Load("Weapons.Explosions.StandardRocket2");
		StaticStandardRocket[2] = WeModelManager::Instance()->Load("Weapons.Explosions.StandardRocket3");
		StaticStandardRocket[3] = WeModelManager::Instance()->Load("Weapons.Explosions.StandardRocket4");

		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.StandardRocket1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.StandardRocket2", true);

		for (int i = 0; i < 4; ++i)
		{
			BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			BoomSplash[i]->Model->Clone(StaticStandardRocket[i], false);
			if (i == 3)
			{
				Distance = (Location - MyPyromaniac->Location);
				D3DXVec3Normalize(&Distance, &Distance);
				BoomSplash[i]->Location = Location + (3.0f * Distance);
				BoomSplash[i]->SetScaling(WeVector(40.0f, 40.0f, 40.0f));
			}
			else
			{
				BoomSplash[i]->Location = (Location - WeVector(2.0f, 2.0f, 2.0f) + 
					WeVector(((float)Rnd(4000) / 1000.0f), ((float)Rnd(4000) / 1000.0f), ((float)Rnd(4000) / 1000.0f)));
				BoomSplash[i]->SetScaling(WeVector(20.0f, 20.0f, 20.0f));
			}

			BoomSplash[i]->Model->Mesh->Materials[0].First();
			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
			World->AddActor(BoomSplash[i]);
		}


		ActivateLight = true;
		LifeTime = 0.7f;
		CreateParticles = true;
		AddExplosiveRefraction(20.0f);
		ShockwaveSize = 50.0f;
		ForceAffectVictim();
		AddDistantExplosion();
		break;

	case LocalizedExplosionBlurMine:
		BoomSplash[0] = new WeActor();
		BoomSplash[0]->Model = new WeStaticModel();
		BoomSplash[0]->Model->Clone(WeModelManager::Instance()->Load("Weapons.Explosions.BlurMine"), false);
		BoomSplash[0]->Model->Mesh->Materials[0].First();
		BoomSplash[0]->LifeTime = (BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		BoomSplash[0]->Location = Location;
		BoomSplash[0]->SetScaling(WeVector(15.0f, 15.0f, 15.0f));
		World->AddActor(BoomSplash[0]);

		ActivateLight = true;
		LifeTime = 1.0f;
		AddExplosiveRefraction(15.0f);

		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.SmokeMineExplosion1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.SmokeMineExplosion2", true);

		AddDistantExplosion();
		break;

	case LocalizedExplosionQuantumLaser:
		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.QuantumBolt1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.QuantumBolt2", true);

		for (int i = 0; i < 1; ++i)
		{
			BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			char Tricycle[100];
			sprintf(Tricycle, "Weapons.Explosions.StandardLaser%d", (i + 1));
			BoomSplash[i]->Model->Clone(WeModelManager::Instance()->Load(String(Tricycle)), false);
			BoomSplash[i]->MaterialPreset = 1;
			BoomSplash[i]->Model->Mesh->Materials[1].First();
			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[1].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[1].GetItem()->TextureAnimParam4);
			World->AddActor(BoomSplash[i]);

			Distance = (Location - MyPyromaniac->Location);
			D3DXVec3Normalize(&Distance, &Distance);
			BoomSplash[i]->Location = Location + (3.0f * Distance);

			//if (i == 1)
			//{
			//	BoomSplash[i]->SetScaling(WeVector(0.0f, 0.0f, 0.0f));
			//	LifeTime = BoomSplash[i]->LifeTime;
			//}
		}

		LifeTime = 1.0f;
		ActivateLight = true;
		LightColor = WeColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;

	case LocalizedExplosionMiniRocket:
		WeStaticModel *StaticMiniRocket[3];
		StaticMiniRocket[0] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionMiniRocket1");
		StaticMiniRocket[1] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionMiniRocket2");
		StaticMiniRocket[2] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionMiniRocket3");
		FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.MiniRocket", true);

		for (int i = 0; i < 2; i++)
		{
			BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			BoomSplash[i]->Model->Clone(StaticMiniRocket[(rand() % 3)], false);
			BoomSplash[i]->Model->Mesh->Materials[0].First();
			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
			BoomSplash[i]->Location = (Location + WeVector((2.0f / ((rand() % 10) + 1)), (2.0f / ((rand() % 10) + 1)), (2.0f / ((rand() % 10) + 1))));
			World->AddActor(BoomSplash[i]);
		}

		if ((rand() % 6) == 1)
			CTuberPartFabric::Instance()->CreateTuberPart(this);
		ActivateLight = ((rand() % 10) == 1) ? true : false;
		LightRange /= 2.0f;

		ForceAffectVictim();
		break;

	case LocalizedExplosionStandardBigAss:
		WeStaticModel *StaticBigAss[3];
		StaticBigAss[0] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket1");
		StaticBigAss[1] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket2");
		StaticBigAss[2] = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionBigAssRocket3");

		if ((rand() % 2) == 0)
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.StandardSplash1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.StandardSplash2", true);

		for (int i = 0; i < 3; i++)
		{
            BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			BoomSplash[i]->Model->Clone(StaticBigAss[(rand() % 3)], false);
			BoomSplash[i]->Model->Mesh->Materials[0].First();
			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
			BoomSplash[i]->Location = (Location + WeVector((5.0f / ((rand() % 10) + 1)), (5.0f / ((rand() % 10) + 1)), (5.0f / ((rand() % 10) + 1))));
			World->AddActor(BoomSplash[i]);
			ActivateLight = true;
			CreateParticles = true;
		}

		ForceAffectVictim();
		AddDistantExplosion();
		break;

	case LocalizedExplosionMiniGun:
		Model = WeModelManager::Instance()->Load("Weapons.Explosion.ExplosionMiniGun");
		Model->Mesh->Materials[0].First();
		LifeTime = (Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		break;

	case LocalizedExplosionTranslocator:
		//BoomSplash[0] = new WeActor();
		//BoomSplash[0]->Model = WeModelManager::Instance()->Load("Weapons.Models.Swirl");
		//BoomSplash[0]->Model->Mesh->Materials[0].First();
		//BoomSplash[0]->LifeTime = (BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		//BoomSplash[0]->Location = Location;
		//World->AddActor(BoomSplash[0]);

		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.Translocator1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.Translocator2", true);

		//ActivateLight = true;
		//LightColor = WeColor(1.0f, 0.5f, 0.1f, 1.0f);
		LifeTime = 0.01f;

		if ( MyVictim && ((MyPyromaniac == CMainGamePhase::Instance->InputPlayer) || (MyVictim == CMainGamePhase::Instance->InputPlayer)) )
		{
			if (!WaveFX)
			{
				WaveFX = new WePostProcessEffect();
				WaveFX->Initialize("Effects.PostProcess.Wave", 1.0f, 1.0f, true);
			}
			if (!WaveFXActive)
			{
				WaveFXActive = true;
				WaveFXOwner = true;
				WePostProcess::Instance()->AddEffect(WaveFX);
			}
		}
		break;

	case LocalizedExplosionSlowingRocket:
		BoomSplash[0] = new WeActor();
		BoomSplash[0]->Model = WeModelManager::Instance()->Load("Weapons.Explosions.SlowingRocket01");
		BoomSplash[0]->Model->Mesh->Materials[0].First();
		BoomSplash[0]->LifeTime = (BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[0]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		BoomSplash[0]->Location = Location;
		BoomSplash[0]->SetScaling(WeVector(20.0f, 20.0f, 20.0f));
		World->AddActor(BoomSplash[0]);

		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.SlowingRocket1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.SlowingRocket2", true);

		ActivateLight = true;
		LightColor = WeColor(0.0f, 1.0f, 1.0f, 1.0f);
		LifeTime = 0.7f;
		AddExplosiveRefraction(15.0f);
		if (Victim) Victim->MuteAudioEnable = true;
		break;

	case LocalizedExplosionTuber:
		WeStaticModel *StaticStandardTuber[4];
		StaticStandardTuber[0] = WeModelManager::Instance()->Load("Weapons.Explosions.TuberExplosion01");
		StaticStandardTuber[1] = WeModelManager::Instance()->Load("Weapons.Explosions.TuberExplosion02");
		StaticStandardTuber[2] = WeModelManager::Instance()->Load("Weapons.Explosions.TuberExplosion03");

		if ( (rand() % 2) == 0 )
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.TuberExplosion1", true);
		else
			FireSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Explosions.TuberExplosion2", true);

		for (int i = 0; i < 3; ++i)
		{
			BoomSplash[i] = new WeActor();
			BoomSplash[i]->Model = new WeStaticModel();
			BoomSplash[i]->Model->Clone(StaticStandardTuber[i], false);
			BoomSplash[i]->Location = Location;
			if (i == 1)
			{
				BoomSplash[i]->SetScaling(WeVector(1.0f, 1.0f, 1.0f));
			}
			else
			{
				BoomSplash[i]->Location = (Location - WeVector(2.0f, 2.0f, 2.0f) + 
					WeVector(((float)Rnd(4000) / 1000.0f), ((float)Rnd(4000) / 1000.0f), ((float)Rnd(4000) / 1000.0f)));
				BoomSplash[i]->SetScaling(WeVector(40.0f, 40.0f, 40.0f));
				BoomSplash[i]->Model->Mesh->Materials[0].First();
				BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
			}

			World->AddActor(BoomSplash[i]);
		}
		ActivateLight = true;
		ShockwaveSize = 250.0f;
		BoomSplash[1]->Model->Mesh->Materials[0].First();
		LifeTime = (BoomSplash[1]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[1]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
		AddExplosiveRefraction(30.0f, LifeTime);

		Sparks = new WeParticleSystem();
		PS->Clone(Sparks);
		Sparks->Initialize();
		Sparks->Location = Location;
		World->AddActor(Sparks);

		// flash && shake
		Distance = (Location - CMainGamePhase::Instance->InputPlayer->Location);
		float DistanceLen = D3DXVec3Length(&Distance);
		D3DXVec3Normalize(&Distance, &Distance);
		WeRaycastHit Hit;
		WeVector AX, AY, AZ;
		CTuber *Gari = CMainGamePhase::Instance->InputPlayer;
		Gari->GetLocalAxis(AX, AY, AZ);
		if (D3DXVec3Dot(&Distance, &AZ) > 0.0f)
			if (!WePhysics::Instance()->RaycastClosest(Gari->Location, Distance, WE_SHAPE_TYPE_STATIC, Hit, 7, DistanceLen))
			{
				if (Gari->CurrentFlash == TuberFlashNone)
					Gari->CurrentFlash = TuberFlashTuberBoom;
				CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShaking;
				CMainGamePhase::Instance->Camera.StateDefaultingTime = 0.4f;
			}
		
		AddDistantExplosion();
		break;
	}

	if (ActivateLight)
	{
		Light = new WeLight();
		Light->IsDynamic = true;
		Light->Type = LIGHT_POINT;

		if (ExplosionType == LocalizedExplosionQuantumLaser)
		{
			//Light->Range = 30.0f;
			Light->TM = TM;

			Light->Corona = true;
			Light->CoronaTextureName = "Flare.Flare3Quantumlaser.Corona";
			Light->CoronaFixedSize = true;
			Light->CoronaRotation = true;
			Light->CoronaSizeX = 0.2f;
			Light->CoronaSizeY = 0.1f;

			Light->Flares = true;
			char gabl[100];
			for (int i = 0; i < 5; ++i)
			{
				sprintf(gabl, "Flare.Flare3Quantumlaser.Ring%d", (i + 1));
				Light->FlareTextureNames[i] = String(gabl);
			}
			Light->FlareSizeX = 0.3f;
			Light->FlareSizeY = 0.3f;

			Light->FadeInTime = 0.1f;
			Light->FadeOutTime = 0.1f;

			WeLightState LS;
			LS.Diffuse = WeColor(0.0f, 1.0f, 0.0f, 1.0f);
			LS.Intensity = 0.5f;
			LS.SpecularIntensity = 0.5f;
			LS.Duration = 1.0f;
			LS.CoronaAlpha = 1.0f;
			LS.FlareAlpha[0] = 0.1f;
			for (int i = 1; i < 5; ++i)
				LS.FlareAlpha[i] = 0.5f;
			Light->AddLightState(&LS);
		}
		else
		{
			WeLightState ls;
			ls.Diffuse = LightColor;
			ls.Intensity = 0.5f;
			ls.SpecularIntensity = 0.5f;
			ls.Duration = 1.0f;
			Light->AddLightState(&ls);
		}
		Light->Location = Location;
		Light->Initialize();
		World->AddActor(Light);

		if (CreateParticles)
			for (int i = 0; i < 2; i++)
				CTuberPartFabric::Instance()->CreateTuberPart(this, true, ((rand() % 8) == 0) ? true : false);
	}
	else
	{
		Light = NULL;
		LifeTime = 0.1f;
	}

	if (ShockwaveSize > 0.0f)
	{
		Shockwave = new WeActor();
		Shockwave->Model = WeModelManager::Instance()->Load("Weapons.Explosions.Shockwave01");
		Shockwave->SetScaling(WeVector(0.0f, 0.0f, 0.0f));
		Shockwave->Location = Location;
		Shockwave->UseMaterialParams = true;
		World->AddActor(Shockwave);
	}
	else
	{
		Shockwave = NULL;
	}

	if (FireSound)
	{
		FireSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		FireSound->Play(false);
		FireSound->DieOnStop();
	}

	if (Victim)
		CExplosion::Affect(Victim);
}


CExplosionLocalized::~CExplosionLocalized()
{
	if (Light)
		Light->LifeTime = 0.0001f;

	if (WaveFXActive && WaveFXOwner)
	{
		WaveFXActive = false;
		WaveFXOwner = false;
		WePostProcess::Instance()->RemoveEffect(WaveFX);
	}

	if (Shockwave)
		Shockwave->LifeTime = 0.0001f;

	if (ExplosionType == LocalizedExplosionTuber)
		BoomSplash[1]->LifeTime = 0.0001f;

	if (Sparks)
	{
		Sparks->Properties.EmitRate = 0;
		Sparks->LifeTime = PS->Properties.LifeTime;
	}
}


void CExplosionLocalized::Tick()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();

	if (Light)
	{
		float DecStep = ((LightRange / GetFinalAge()) * Time);

		if (Light->Range - DecStep >= 0.0f)
			Light->Range -= DecStep;

		if (ExplosionType == LocalizedExplosionQuantumLaser)
		{
			static float FIRST_PART_LIFE = 0.1f;

			if (ObjectAge <= FIRST_PART_LIFE)
			{
				Light->LightStates.First();
				Light->LightStates.GetItem().CoronaAlpha = ObjectAge / FIRST_PART_LIFE;
				for (int i = 0; i < 5; ++i)
					Light->LightStates.GetItem().FlareAlpha[i] = Light->LightStates.GetItem().CoronaAlpha;
				Light->CoronaSizeX = (0.7f * (ObjectAge / FIRST_PART_LIFE));
				Light->CoronaSizeY = (0.6f * (ObjectAge / FIRST_PART_LIFE));
			}
			else
			{
				float Factor = ((ObjectAge - FIRST_PART_LIFE) / (GetFinalAge() - FIRST_PART_LIFE));
				Light->LightStates.First();
				Light->LightStates.GetItem().CoronaAlpha = 1.0f - Factor;
				for (int i = 0; i < 5; ++i)
					Light->LightStates.GetItem().FlareAlpha[i] = Light->LightStates.GetItem().CoronaAlpha;
				Light->CoronaSizeX = 0.7f + (0.8f * Factor);
				Light->CoronaSizeY = 0.6f - (0.6f * Factor);
			}
		}
	}

	if (ExplosionType == LocalizedExplosionTuber)
	{
		float Move = (150.0f / GetFinalAge()) * Time;
		BoomSplash[1]->Scale = WeVector(BoomSplash[1]->Scale.x + Move, BoomSplash[1]->Scale.y + Move, BoomSplash[1]->Scale.z + Move);

		if (Sparks)
			Sparks->Location = Location;

		//if (Sparks && (ObjectAge >= LifeTime / 2.0f))
		//{
		//	Sparks->Properties.EmitRate = 0;
		//	Sparks->LifeTime = PS->Properties.LifeTime;
		//	Sparks = NULL;
		//}
	}
	else if (ExplosionType == LocalizedExplosionStandardLaser)
	{
		float RightSplashAge = 0.0f;
		for (int i = 1; i < 3; ++i)
		{
			RightSplashAge += 0.175f;

			if (ObjectAge >= RightSplashAge && !BoomSplash[i])
			{
				BoomSplash[i] = new WeActor();
				BoomSplash[i]->Model = new WeStaticModel();
				char Bycicle[100];
				sprintf(Bycicle, "Weapons.Explosions.StandardLaser%d", i);
				BoomSplash[i]->Model->Clone(WeModelManager::Instance()->Load(String(Bycicle)), false);
				BoomSplash[i]->Model->Mesh->Materials[0].First();
				BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
				World->AddActor(BoomSplash[i]);
				BoomSplash[i]->Location = Location;
				BoomSplash[i]->Location += (PyroX * ((float)Rnd(5000) / 1000.0f)) - (PyroX * 2.5f);
				BoomSplash[i]->Location += (PyroY * ((float)Rnd(5000) / 1000.0f)) - (PyroY * 2.5f);
			}
		}
	}
	//else if (ExplosionType == LocalizedExplosionQuantumLaser)
	//{
	//	float RightSplashAge = 0.0f;
	//	for (int i = 2; i < 3; ++i)
	//	{
	//		RightSplashAge += 0.175f;

	//		if (ObjectAge >= RightSplashAge && !BoomSplash[i])
	//		{
	//			BoomSplash[i] = new WeActor();
	//			BoomSplash[i]->Model = new WeStaticModel();
	//			char Bycicle[100];
	//			sprintf(Bycicle, "Weapons.Explosion.ExplosionQuantumLaser%d", i);
	//			BoomSplash[i]->Model->Clone(WeModelManager::Instance()->Load(String(Bycicle)), false);
	//			BoomSplash[i]->Model->Mesh->Materials[0].First();
	//			BoomSplash[i]->LifeTime = (BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / BoomSplash[i]->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
	//			World->AddActor(BoomSplash[i]);
	//			BoomSplash[i]->Location = Location;
	//			BoomSplash[i]->Location += (PyroX * ((float)Rnd(5000) / 1000.0f)) - (PyroX * 2.5f);
	//			BoomSplash[i]->Location += (PyroY * ((float)Rnd(5000) / 1000.0f)) - (PyroY * 2.5f);
	//		}
	//	}

	//	float Move = (1.3f / LifeTime) * Time;
	//	BoomSplash[1]->Scale = WeVector(BoomSplash[1]->Scale.x + Move, BoomSplash[1]->Scale.y + Move, BoomSplash[1]->Scale.z + Move);
	//}

	if (Shockwave)
	{
		float FinalLifetime = GetFinalAge() * 0.5f;
		float Age = ObjectAge;

		Shockwave->Scale = WeVector(ShockwaveSize, ShockwaveSize, ShockwaveSize) * (Age / FinalLifetime);
		Shockwave->MaterialColor.a = (1.0f - (Age / FinalLifetime));
	}

	CExplosion::Tick();
}

void CExplosionLocalized::ForceAffectVictim()
{
	if (!Victim) return;

	float Time = WeTimer::Instance()->GetDeltaTimeF();

    CTuber *PlayerVictim = NULL;
	if (ACTORTYPE_TUBER == Victim->Type)
		PlayerVictim = (CTuber*)Victim;

	WeVector DistanceVector, AccelVector;

	DistanceVector = (Victim->Location - Location);
	D3DXVec3Normalize(&AccelVector, &DistanceVector);
	AccelVector *= (1000.0f * Time);

	if (PlayerVictim && PlayerVictim->IsPushable())
	{
		if (PlayerVictim == CMainGamePhase::Instance->InputPlayer)
		{
			CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShaking;
			CMainGamePhase::Instance->Camera.StateDefaultingTime = 0.2f;
		}
		PlayerVictim->Velocity += AccelVector;
		return;
	}

	if (Victim->PhysicHandle && CPlayGamePhase::ActorPushable(Victim))
		Victim->PhysicHandle->AddVelocity(AccelVector);
}
