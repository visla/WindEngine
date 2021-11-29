/*
    CLoadMapPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
*/

#include <CLoadMapPhase.h>
#include <CGame.h>
#include <WeCanvas.h>
#include <AllPhases.h>
#include <CGameSettings.h>
#include <WeRenderDevice.h>
#include <CMenuPhase.h>
#include <CGameClient.h>
#include <CProgressBar.h>
#include <WeMaterialManager.h>
#include <WeAudioDevice.h>
#include <WeModelManager.h>
#include <WeSound.h>
#include <CPickUpFactory.h>
#include <WeFont.h>
#include <WeLoadCounter.h>
#include <CLocalizationSettings.h>
#include <CMainGameInterface.h>
#include <WeTimer.h>
#include <WeMesh.h>
#include <WeParticleSystem.h>
#include <CTuberDescriptionManager.h>



CLoadMapPhase *CLoadMapPhase::Instance = new CLoadMapPhase;


CLoadMapPhase::CLoadMapPhase()
{
	PreLoaded = false;
	Background = NULL;
	ProgressBar = NULL;
	InfoLoaded = false;
	ShitLoaded = 0;
	PartLoaded = 0;
}

void CLoadMapPhase::Start()
{
	MapLoaded = false;
	StoryText = " ";
	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
	{
		LoadingImage = CGameSettings::ClientInstance->MapSettings->LoadingImage;
		StoryText = CGameSettings::ClientInstance->MapSettings->LoadingText;
	}

	ProgressBar = new CProgressBar(ProgressBarVisualStyle1);
	ProgressBar->PosY = 0.85f;
	ProgressBar->ReplaceFinalText = true;
	ProgressBar->FinalText = CLocalizationSettings::Instance()->GetLocalText("PRESS_ANY_KEY");
	ProgressBar->Value = 0.0f;
	PreLoaded = false;
	InfoLoaded = false;
	ShitLoaded = 0;
	PartLoaded = 0;

	CPickUpFactory::Instance->SetWeaponPickupsActive(CGameSettings::ClientInstance->GetWeapons());
	
	//CMainGamePhase::Instance->PlaySong(SongNone);	//music off

	SavedSoundVolume = WeAudioDevice::Instance()->GetSoundVolume();
	WeAudioDevice::Instance()->SetSoundVolume(0.0f);
}


void CLoadMapPhase::OnLoadProgress(float Progress, bool &StopLoading)
{

}


void CLoadMapPhase::End()
{
	if (WeAudioDevice::Instance()->GetSoundVolume() != SavedSoundVolume)
		WeAudioDevice::Instance()->SetSoundVolume(SavedSoundVolume);

	WeTextureManager::Instance()->Free(Background);
	Background = NULL;
	CMenuPhase::Instance->SaveStates();
}



#define LoadMaterialsCount 2

void CLoadMapPhase::LoadMaterials(int i)
{
	static char *(MaterialNames[LoadMaterialsCount]) = {
		"Materials.Distortion.Distortion1",
		"Materials.Trails.Tuber",
		//"Weapons.Trails.HomingMine",
		//"Weapons.Trails.MiniRocket",
		//"Weapons.Trails.MovingMine",
		//"Weapons.Trails.SlowingRocket"
	};

	WeMaterialManager::Instance()->Load(MaterialNames[i]);
}



#define LoadTexturesCount 37

void CLoadMapPhase::LoadTextures(int i)
{
	static char *(TextureNames[LoadTexturesCount]) = {
		"Ingame.Countdown.One",
		"Ingame.Countdown.Two",
		"Ingame.Countdown.Three",
		"Ingame.Countdown.Go",
		"Ingame.Hud.BlueEmpty",
		"Ingame.Hud.BlueFull",
		"Ingame.Hud.GreenEmpty",
		"Ingame.Hud.GreenFull",
		"Ingame.Hud.LightBlueEmpty",
		"Ingame.Hud.LightBlueFull",
		"Ingame.Hud.RedEmpty",
		"Ingame.Hud.RedFull",
		"Ingame.Hud.Left",
		"Ingame.Hud.Right",
		"Ingame.Hud.Lines",
		"Ingame.Hud.LeftTop",
		"Ingame.Hud.LeftMiddle",
		"Ingame.Hud.LeftBottom",
		"Ingame.Hud.TopLeft",
		"Ingame.Hud.TopRight",
		"Ingame.Hud.TopPickups",
		"Textures.GameInsterfejs.LockL",
		"Textures.GameInsterfejs.LockR",
		"Textures.GameInsterfejs.LockM",
		"Ingame.Misc.BloodScreen",
		"Ingame.Misc.YouWin",
		"Ingame.Misc.1st place",
		"Ingame.Misc.2nd place",
		"Ingame.Misc.3rd place",
		"Ingame.Misc.YouLose",
		"Ingame.Misc.Invulnerability",
		"Ingame.Misc.SpeedScreen",
		"PickUps.Textures.StandardLaserAnim",
		"PickUps.Textures.StandardRocketAnim",
		"PickUps.Textures.QuantumLaserAnim",
		"PickUps.Textures.HomingMineAnim",
		"PickUps.Textures.BlurMineAnim"
	};

	WeTextureManager::Instance()->Load(TextureNames[i], true);
}



#define LoadSoundsCount 118

void CLoadMapPhase::LoadSounds(int i)
{
	static char *(SoundNames[LoadSoundsCount]) = {
		//special sound
		WeAudioDevice::Instance()->GetAutoLoadFromPackage() ? "Sounds.Tuber.Engine" : ".\\Sounds\\Tuber\\Engine.wav",
		//////////////////////////////////////////////////////////////////////////

		//interface 2D
		"Sounds.Interface.Locking",
		"Sounds.Interface.Locked",
		"Sounds.Interface.Lock",
		"Sounds.Interface.Warning",
		"Sounds.Interface.HeartBeat",
		"Sounds.Interface.Hit",
		"Sounds.Interface.ImmortalHit",
		"Sounds.Interface.TimeEndingText",
		"Sounds.Interface.Deaf",

		"Sounds.Explosions.DistantExplosion1",
		"Sounds.Explosions.DistantExplosion2",
		"Sounds.Explosions.DistantExplosion3",
		"Sounds.Tuber.HeadLight",

		//voice 2D
		"Sounds.Voice.MultiKill2",
		"Sounds.Voice.MultiKill3",
		"Sounds.Voice.MultiKill4",
		"Sounds.Voice.MultiKill10",
		"Sounds.Voice.MultiKill19",
		"Sounds.Voice.MultiKill20",

		"Sounds.Voice.CountOne",
		"Sounds.Voice.CountTwo",
		"Sounds.Voice.CountThree",
		"Sounds.Voice.CountGo",

		"Sounds.Voice.EndYouWin",
		"Sounds.Voice.EndYouLose",
		"Sounds.Voice.EndFirstPlace",
		"Sounds.Voice.EndSecondPlace",
		"Sounds.Voice.EndThirdPlace",

		"Sounds.Voice.PickupHomingMine",
		"Sounds.Voice.PickupInvisibility",
		"Sounds.Voice.PickupInvulnerability",
		"Sounds.Voice.PickupQuadDamage",
		"Sounds.Voice.PickupQuantumBolt",
		"Sounds.Voice.PickupSlowingRocket",
		"Sounds.Voice.PickupSmokeMine",
		"Sounds.Voice.PickupStandardBolt",
		"Sounds.Voice.PickupStandardRocket",
		"Sounds.Voice.PickupTranslocator",

		"Sounds.Voice.LapsRemaining",
		"Sounds.Voice.LapRemaining",
		"Sounds.Voice.Lap1",
		"Sounds.Voice.Laps2",
		"Sounds.Voice.Laps3",
		"Sounds.Voice.Laps4",
		"Sounds.Voice.Laps5",
		"Sounds.Voice.Laps6",
		"Sounds.Voice.Laps7",
		"Sounds.Voice.Laps8",
		"Sounds.Voice.Laps9",
		"Sounds.Voice.Laps10",
		"Sounds.Voice.Laps11",
		"Sounds.Voice.Laps12",
		"Sounds.Voice.Laps13",
		"Sounds.Voice.Laps14",
		//////////////////////////////////////////////////////////////////////////

		//tuber
		"Sounds.Tuber.Turbo",
		"Sounds.Tuber.Spawn",
		"Sounds.Tuber.Collision1",
		"Sounds.Tuber.Collision2",
		"Sounds.Tuber.Collision3",
		"Sounds.Tuber.Teleport",

		"Sounds.Tuber.LoopScratch1",
		"Sounds.Tuber.LoopScratch2",
		"Sounds.Tuber.LoopScratch3",
		"Sounds.Tuber.Scratch1",
		"Sounds.Tuber.Scratch2",
		"Sounds.Tuber.Scratch3",
		"Sounds.Tuber.Scratch4",
		"Sounds.Tuber.Scratch5",
		"Sounds.Tuber.Scratch6",
		"Sounds.Tuber.Scratch7",
		"Sounds.Tuber.ScratchStart1",
		"Sounds.Tuber.ScratchStart2",
		"Sounds.Tuber.ScratchStart3",
		"Sounds.Tuber.ScratchStart4",
		"Sounds.Tuber.Sparks1",
		"Sounds.Tuber.Sparks2",
		"Sounds.Tuber.Sparks3",
		"Sounds.Tuber.Sparks4",
		//-----

		//explosions
		"Sounds.Explosions.QuantumBolt1",
		"Sounds.Explosions.QuantumBolt2",
		"Sounds.Explosions.StandardRocket1",
		"Sounds.Explosions.StandardRocket2",
		"Sounds.Explosions.Translocator1",
		"Sounds.Explosions.Translocator2",
		"Sounds.Explosions.TuberExplosion1",
		"Sounds.Explosions.TuberExplosion2",
		"Sounds.Explosions.SlowingRocket1",
		"Sounds.Explosions.SlowingRocket2",
		"Sounds.Explosions.HomingMineExplosion1",
		"Sounds.Explosions.HomingMineExplosion2",
		"Sounds.Explosions.SmokeMineExplosion1",
		"Sounds.Explosions.SmokeMineExplosion2",
		//----------

		//primary weapons
		"Sounds.Weapons.StandardBolt1",
		"Sounds.Weapons.StandardBolt2",
		"Sounds.Weapons.StandardBolt3",
		"Sounds.Weapons.QuantumPrefire",
		"Sounds.Weapons.QuantumBolt",
		"Sounds.Weapons.StandardRocketLaunch",
		"Sounds.Weapons.StandardRocketEngine",
		"Sounds.Weapons.SlowingRocketLaunch",
		"Sounds.Weapons.SlowingRocketEngine",
		"Sounds.Weapons.TranslocatorLaunch",
		"Sounds.Weapons.TranslocatorEngine",
		//----------------

		//secondary weapons
		"Sounds.Mines.MineDrop",
		"Sounds.Mines.HomingMine",
		"Sounds.Mines.SmokeMine",
		//-----------------

		//pickup sounds
		"Sounds.Pickup.PrimaryWeapon",
		"Sounds.Pickup.SecondaryWeapon",
		"Sounds.Pickup.Coin",
		"Sounds.Pickup.QuadDamage",
		"Sounds.Pickup.Invisibility",
		"Sounds.Pickup.Invulnerability",
		"Sounds.Pickup.PickupRespawn",
		"Sounds.Pickup.QuadDamageFire",
		"Sounds.Pickup.EndQuadDamage",
		"Sounds.Pickup.EndInvulnerability",
		"Sounds.Pickup.EndInvisibility"
		//--------------
	};

	if (i < 1)
		WeAudioDevice::Instance()->LoadSound(SoundNames[i] , true, false, false, (DSBCAPS_LOCHARDWARE | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE) )->DieOnStop();
	else if (i < 55)
		WeAudioDevice::Instance()->LoadSoundAuto(SoundNames[i], false)->DieOnStop();
	else
		WeAudioDevice::Instance()->LoadSoundAuto(SoundNames[i], true)->DieOnStop();
}



#define LoadStaticModelsCount 58

void CLoadMapPhase::LoadStaticModels(int i)
{
	static char *(ModelNames[LoadStaticModelsCount]) = {
		//---explosions---//
		"Weapons.Explosions.StandardRocket1",
		"Weapons.Explosions.StandardRocket2",
		"Weapons.Explosions.StandardRocket3",
		"Weapons.Explosions.StandardRocket4",
		"Weapons.Explosions.BlurMine",
		"Weapons.Explosions.HomingMine",
		"Weapons.Explosions.TuberExplosion01",
		"Weapons.Explosions.TuberExplosion02",
		"Weapons.Explosions.TuberExplosion03",
		"Weapons.Explosions.SlowingRocket01",
		"Weapons.Explosions.Shockwave01",
		"Weapons.Explosions.ExplosionRefraction01",
		"Weapons.Explosions.StandardLaser1",
		"Weapons.Explosions.StandardLaser2",
		"Weapons.Explosions.StandardLaser3",
		"Weapons.Explosions.QuantumLaser1",
		//"Weapons.Explosion.ExplosionQuantumLaser2",
		//"Weapons.Explosion.ExplosionQuantumLaser3",
		//"Weapons.Explosion.ExplosionMiniRocket1",
		//"Weapons.Explosion.ExplosionMiniRocket2",
		//"Weapons.Explosion.ExplosionMiniRocket3",
		//"Weapons.Explosion.ExplosionBigAssRocket1",
		//"Weapons.Explosion.ExplosionBigAssRocket2",
		//"Weapons.Explosion.ExplosionBigAssRocket3",
		//"Weapons.Explosion.ExplosionMiniGun",
		//"Weapons.Explosion.ExplosionNuke",
		"Weapons.Explosion.ExplosionPlaneShockwave1",
		"Weapons.Explosion.ExplosionPlaneShockwave2",
		"Weapons.Models.Swirl",
		//----------------//

		//---primary weapons---//
		"Weapons.Models.StandardLaser",
		"Weapons.Models.StandardRocket",
		"Weapons.Models.QuantumPrefire",
		"Weapons.Models.QuantumBolt",
		//"Weapons.Models.MiniRocket",
		//"Weapons.Models.BigAssRocket",
		//"Weapons.Models.MiniGunAmmo",
		//"Weapons.Models.MiniGunMuzzle",
		//"Weapons.Models.MiniGunAmmoShell",
		"Weapons.Models.SlowingRocket",
		//"Weapons.Models.Translocator",
		//---------------------//

		//---secondary weapons---//
		//"Weapons.Models.StandardMine",
		"Weapons.Models.SmokeMine",
		//"Weapons.Models.BlurMine",
		//"Weapons.Models.ElectricityMine",
		//"Weapons.Models.GravityMine",
		"Weapons.Models.HomingMine",
		//"Weapons.Models.SlowingMine",
		//"Weapons.Models.SuperNukeMine",
		//"Weapons.Models.InvisibleMine",
		//"Weapons.Models.MovingMine",
		//"Weapons.Models.ProximityMine",
		//-----------------------//

		//---pickups---//
		"PickUps.Models.Mine",
		"PickUps.Models.Weapon4",
		"PickUps.Models.Weapon6",
		"PickUps.Models.Weapon7",
		"PickUps.Models.Weapon8",
		"PickUps.Models.Weapon10",
		"PickUps.Models.Weapon11",
		"PickUps.Models.Weapon13",
		//-------------//

		//---fragments---//
		"Fragments.Models.Fragment00",
		"Fragments.Models.Fragment01",
		"Fragments.Models.Fragment02",
		"Fragments.Models.Fragment03",
		"Fragments.Models.Fragment04",
		"Fragments.Models.Fragment05",
		"Fragments.Models.Fragment06",
		"Fragments.Models.Fragment07",
		"Fragments.Models.Fragment08",
		"Fragments.Models.Fragment09",
		//---------------//

		//---pickups---//
		"PickUps.Models.Invisibility",
		"PickUps.Models.QuadDamage",
		"PickUps.Models.Invulnerability",
		"PickUps.Models.QuantumLaser",
		"PickUps.Models.StandardBolt",
		"PickUps.Models.StandardRocket",
		"PickUps.Models.SlowingRocket",
		"PickUps.Models.Translocator",
		"Weapons.Models.HomingMine",
		"Weapons.Models.BlurMine",
		"PickUps.Models.CoinSmall",
		"PickUps.Models.CoinMedium",
		"PickUps.Models.CoinLarge",
		//-------------//

		//---------------//
		"Weapons.Models.Spark"
		//-----other-----//
	};

	WeStaticModel *Model;
	Model = WeModelManager::Instance()->Load(ModelNames[i]);
	if (Model)
	{
		for (int Preset = 0; Preset < MAX_MATERIAL_PRESETS2; ++Preset)
			Model->LoadMaterials(Preset);
	}
}



#define LoadTubersCount 4

void CLoadMapPhase::LoadTubers(int iPart)
{
	static char *(TuberModels[9]) = {
		"Tubers.Models.%s",
		"Tubers.Models.%sFlare",
		"Tubers.Models.%sTrail",
		"Tubers.Models.%sSpeedScreen",
		"Tubers.Models.%sQuadDamage",
		"Tubers.Models.%sLights",
		"Tubers.Models.%sTurbo",
		"Tubers.Models.%sRefraction",
		"Tubers.Models.%sInvulnerability"
	};

	CTuberDescription &TuberDesc = CTuberDescriptionManager::Instance()->Descriptions[iPart];

	// load all models for tuber
	for (int i = 0; i < 9; ++i)
	{
		char temp[100];
		sprintf_s(temp, TuberModels[i], TuberDesc.Name);

		WeStaticModel *Model;
		Model = WeModelManager::Instance()->Load(temp);
		if (Model)
		{
			for (int Preset = 0; Preset < MAX_MATERIAL_PRESETS2; ++Preset)
				Model->LoadMaterials(Preset);
		}
	}

	// load all destruction parts for tuber
	for (int i = 0; i < 10; ++i)
	{
		WeStaticModel *Model;
		Model = WeModelManager::Instance()->Load(TuberDesc.DestructionModel[i].Model);
		if (Model)
		{
			for (int Preset = 0; Preset < MAX_MATERIAL_PRESETS2; ++Preset)
				Model->LoadMaterials(Preset);
		}
	}

	// load projector material
	char MatName[100];
	sprintf(MatName, "Tubers.Projectors.%sProjector", TuberDesc.Name);
	WeMaterialManager::Instance()->Load(MatName);
}



void CLoadMapPhase::Tick()
{
	static float ZeroTime, LastEndingTime;
	float StartingTime, EndingTime;

	if (ShitLoaded == 0)
		ZeroTime = WeTimer::Instance()->GetAbsoluteTime();
	StartingTime = WeTimer::Instance()->GetAbsoluteTime() - ZeroTime;

	do {

#ifdef _DEBUG
		//WeLoger::DebugLog("--------------------------- LOADING %d @ %.6fs ---------------------------\n", ShitLoaded, StartingTime);
#endif

		switch (ShitLoaded)
		{
		case 0:
			StaticsPercentLoaded = 0.0f;
			CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			if (CMainGamePhase::Instance->Map.IsHeaderLoaded() && !CGameSettings::ClientInstance->MapSettings)
			{
				CMainGamePhase::Instance->Map.MapHeader.SetupRenderDevice();
				LoadingImage = CMainGamePhase::Instance->Map.MapHeader.LoadingImage;
				StoryText = " ";
				InfoLoaded = true;
				ShitLoaded++;
			}
			break;

		case 1:
			if (CMainGamePhase::Instance->Map.PercentLoaded < StaticsPercentLoaded)
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else if (PartLoaded < LoadMaterialsCount)
			{
				LoadMaterials(PartLoaded);
				PartLoaded++;
				StaticsPercentLoaded = 5.0f + 5.0f * PartLoaded / LoadMaterialsCount;
			}
			else
			{
				ShitLoaded++;
				PartLoaded = 0;
			}

			break;

		case 2:
			if (CMainGamePhase::Instance->Map.PercentLoaded < StaticsPercentLoaded)
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else if (PartLoaded < LoadSoundsCount)
			{
				LoadSounds(PartLoaded);
				PartLoaded++;
				StaticsPercentLoaded = 10.0f + 10.0f * PartLoaded / LoadSoundsCount;
			}
			else
			{
				ShitLoaded++;
				PartLoaded = 0;
			}

			break;

		case 3:
			if (CMainGamePhase::Instance->Map.PercentLoaded < StaticsPercentLoaded)
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else if (PartLoaded < LoadStaticModelsCount)
			{
				LoadStaticModels(PartLoaded);
				PartLoaded++;
				StaticsPercentLoaded = 20.0f + 30.0f * PartLoaded / LoadStaticModelsCount;
			}
			else
			{
				ShitLoaded++;
				PartLoaded = 0;
			}

			break;

		case 4:
			if (CMainGamePhase::Instance->Map.PercentLoaded < StaticsPercentLoaded)
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else if (PartLoaded < LoadTexturesCount)
			{
				LoadTextures(PartLoaded);
				PartLoaded++;
				StaticsPercentLoaded = 50.0f + 20.0f * PartLoaded / LoadTexturesCount;
			}
			else
			{
				ShitLoaded++;
				PartLoaded = 0;
			}

			break;

		case 5:
			if (CMainGamePhase::Instance->Map.PercentLoaded < StaticsPercentLoaded)
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else if (PartLoaded < LoadTubersCount)
			{
				LoadTubers(PartLoaded);
				PartLoaded++;
				StaticsPercentLoaded = 70.0f + 20.0f * PartLoaded / LoadTubersCount;
			}
			else
			{
				ShitLoaded++;
				PartLoaded = 0;
			}

			break;

		case 6:
			LoadPSs();
			ShitLoaded++;
			break;

		case 7:
			StaticsPercentLoaded = 100.0f;
			if (!CMainGamePhase::Instance->Map.IsLoaded())
				CMainGamePhase::Instance->Map.LoadInTick(CGameSettings::ClientInstance->MapName);
			else
			{
				CMainGameInterface::Instance->Initialize();
				ShitLoaded++;
			}

			break;

		case 8:
			MapLoaded = true;
			if (CGame::InputMessage.AnyKey)
			{
				CMainGamePhase::Instance->InputPlayer = NULL;
				CMainGamePhase::Instance->SetPhase(CPlayGamePhase::Instance);
			}

			break;
		}

		PercentLoaded = CMainGamePhase::Instance->Map.PercentLoaded;
		if (StaticsPercentLoaded < PercentLoaded)
			PercentLoaded = StaticsPercentLoaded;

		EndingTime = WeTimer::Instance()->GetAbsoluteTime() - ZeroTime;

#ifdef _DEBUG
		//WeLoger::DebugLog("--------------------------- LOADED %.3f%% (%.3f%% : %.3f%%) @ %.6fs : %.6fs ---------------------------\n\n",
			//PercentLoaded, CMainGamePhase::Instance->Map.PercentLoaded, StaticsPercentLoaded, EndingTime, EndingTime - StartingTime);
#endif

	} while (EndingTime - StartingTime < 0.02f);

	WeLoger::DebugLog("*************************** LOADED %.3f%% @ %.6fs : %.6fs ***************************\n\n\n",
		PercentLoaded, EndingTime - LastEndingTime, EndingTime);

	LastEndingTime = EndingTime;
}


void CLoadMapPhase::Render()
{
	if (CMainGamePhase::Instance->Map.IsHeaderLoaded())
	{
		// now we have desription and background image for displaying map load progress
		//static WeFont *Font = NULL;

		//if (!Font)
		//	Font = WeCanvas::Instance()->MakeFont("Xirod", 24);


		if (!Background && (LoadingImage.GetLen() != 0))
		{
			Background = WeTextureManager::Instance()->Load(LoadingImage, true);
		}

		if (Background)
			WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), Background);

		ProgressBar->Value = PercentLoaded;
		ProgressBar->Render();

		//static WeFont* Font = WeCanvas::Instance()->MakeFont("Arial", 16);
		//if (GetLoadingName().GetLen() != 0)
		//{
		//	WeCanvas::Instance()->Draw2DText(Font, GetLoadingName(), 0, WeRenderDevice::Instance()->GetResHeight() - 20, 300, 20, 0xFFFFFFFF);
		//	ResetLoadingName();
		//}

		//char tmp[100];
		//sprintf(tmp, "Progress: %4.0f\n", CMainGamePhase::Instance->Map.PercentLoaded);
		//int w,h;		
		//w = WeRenderDevice::Instance()->GetResWidth();
		//h = WeRenderDevice::Instance()->GetResHeight();

		//WeCanvas::Instance()->DrawRectangle(0,0,w,h, D3DCOLOR_XRGB(255,255,255), Background);
		//WeCanvas::Instance()->Draw2DText(Font, tmp, 10, 10, 400, 400, D3DCOLOR_XRGB(255,255,255));
		//WeCanvas::Instance()->Draw2DText(Font, StoryText, w/2 - w/3, h/2 - h/3, 700, 400, D3DCOLOR_XRGB(255,255,255));		
	}    
}


#include <CWeaponStandardRocket.h>
#include <CTuberSparks.h>
#include <CTuberPart.h>
#include <CWeaponTranslocator.h>
#include <CWeaponSmokeMine.h>
#include <CWeaponSlowingRocket.h>
#include <CWeaponNukeRocket.h>
#include <CExplosionLocalized.h>
#include <CWeaponQuantumLaser.h>
#include <CTuber.h>

void CLoadMapPhase::LoadPSs()
{
	if (!CWeaponStandardRocket::StaticPS)
	{
		CWeaponStandardRocket::StaticPS = new WeParticleSystem();
		CWeaponStandardRocket::StaticPS->LoadPreset("ParticleSystem.Weapons.StandardRocket");
	}
	CWeaponStandardRocket::StaticPS->Initialize();

	if (!CTuberSparks::StaticPS)
	{
		CTuberSparks::StaticPS = new WeParticleSystem();
		CTuberSparks::StaticPS->LoadPreset("ParticleSystem.Test.Spark2");
	}
	CTuberSparks::StaticPS->Initialize();

	if (!CTuberPart::StaticPSMini)
	{
		CTuberPart::StaticPSMini = new WeParticleSystem();
		CTuberPart::StaticPSMini->LoadPreset("ParticleSystem.Smokes.TuberPart");
	}
	CTuberPart::StaticPSMini->Initialize();

	if (!CTuberPart::StaticPSBurnBaby)
	{
		CTuberPart::StaticPSBurnBaby = new WeParticleSystem();
		CTuberPart::StaticPSBurnBaby->LoadPreset("ParticleSystem.Smokes.BurningTuberPart");
	}
	CTuberPart::StaticPSBurnBaby->Initialize();

	if (!CTuberPart::StaticPSBurnBabyBURNFire)
	{
		CTuberPart::StaticPSBurnBabyBURNFire = new WeParticleSystem();
		CTuberPart::StaticPSBurnBabyBURNFire->LoadPreset("ParticleSystem.Explosions.Fire");
	}
	CTuberPart::StaticPSBurnBabyBURNFire->Initialize();

	if (!CTuberPart::StaticPSBurnBabyBURNSmoke)
	{
		CTuberPart::StaticPSBurnBabyBURNSmoke = new WeParticleSystem();
		CTuberPart::StaticPSBurnBabyBURNSmoke->LoadPreset("ParticleSystem.Explosions.DustPuff");
	}
	CTuberPart::StaticPSBurnBabyBURNSmoke->Initialize();

	for (int i = 0; i < 3; ++i)
	{
		char zuuum[100];
		if (!CWeaponTranslocator::StaticPS[i])
		{
			sprintf(zuuum, "ParticleSystem.Weapons.TranslocatorAura%d", (i + 1));
			CWeaponTranslocator::StaticPS[i] = new WeParticleSystem();
			CWeaponTranslocator::StaticPS[i]->LoadPreset(String(zuuum));
		}
		CWeaponTranslocator::StaticPS[i]->Initialize();
	}

	if (!CWeaponSmokeMine::StaticPS)
	{
		CWeaponSmokeMine::StaticPS = new WeParticleSystem();
		CWeaponSmokeMine::StaticPS->LoadPreset("ParticleSystem.Smokes.SmokeMine");
	}
	CWeaponSmokeMine::StaticPS->Initialize();

	if (!CWeaponSlowingRocket::StaticPS)
	{
		CWeaponSlowingRocket::StaticPS = new WeParticleSystem();
		CWeaponSlowingRocket::StaticPS->LoadPreset("ParticleSystem.Weapons.SlowingRocket");
	}
	CWeaponSlowingRocket::StaticPS->Initialize();

	if (!CWeaponNukeRocket::StaticPS)
	{
		CWeaponNukeRocket::StaticPS = new WeParticleSystem();
		CWeaponNukeRocket::StaticPS->LoadPreset("ParticleSystem.Weapons.BigAssRocket");
	}
	CWeaponNukeRocket::StaticPS->Initialize();

	if (!CExplosionLocalized::PS)
	{
		CExplosionLocalized::PS = new WeParticleSystem();
		CExplosionLocalized::PS->LoadPreset("ParticleSystem.Explosions.Sparks01");
	}
	CExplosionLocalized::PS->Initialize();

	if (!CWeaponQuantumLaser::StaticPS)
	{
		CWeaponQuantumLaser::StaticPS = new WeParticleSystem();
		CWeaponQuantumLaser::StaticPS->LoadPreset("ParticleSystem.Weapons.QuantumBolt");
	}
	CWeaponQuantumLaser::StaticPS->Initialize();

	if (!CTuber::StaticTrailPS)
	{
		CTuber::StaticTrailPS = new WeParticleSystem();
		CTuber::StaticTrailPS->LoadPreset("ParticleSystem.Tuber.Trail");
	}
	CTuber::StaticTrailPS->Initialize();
}
