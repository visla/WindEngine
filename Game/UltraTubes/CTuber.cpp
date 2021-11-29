/*
		File: CTuber.cpp
		Author: Nebojsa Vislavski
		Date: 23.06.2006
		Version: 1.0
*/



#define _NEW_WEAPON				10
#define _DEAD					11
#define _TUBER_RESPAWN			12

#define MIN_LOCK_DOT			0.9f

#define QUICK_KILL_SPREAD		5.0f
#define HEADLIGHT_RANGE			60.0f

#define END_RACE_TIMEOUT		10.0f

#include <CInputMessage.h>
#include <CTuber.h>
#include <CPickupAction.h>
#include <Constants.h>
#include <WeTimer.h>
#include <CMainGamePhase.h>
#include <WePhysics.h>
#include <CWeaponDescriptionManager.h>
#include <CGame.h>
#include <WeServer.h>
#include <ActorTags.h>
#include <WeRenderDevice.h>
#include <CGameServer.h>
#include <WePathNode.h>
#include <CStartLocation.h>
#include <WeRadialTrail.h>
#include <WeMaterialManager.h>
#include <CPlayGamePhase.h>
#include <CollisionGroups.h>
#include <weclient.h>
#include <WeBox.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <dsound.h>
#include <COptionsSettings.h>
#include <WeListener.h>
#include <CExplosionNuke.h>
#include <CExplosionLocalized.h>
#include <CTuberPartFabric.h>
#include <WePostProcessEffect.h>
#include <WeMesh.h>
#include <weMaterial.h>
#include <WePostProcess.h>
#include <CUberText.h>
#include <CMainGameInterface.h>
#include <WeCanvas.h>
#include <WeLoger.h>
#include <CLocalizationSettings.h>
#include <CTuberSparks.h>
#include <WeModelManager.h>
#include <CConfig.h>
#include <CAIKernel.h>
#include <CAgent.h>
#include <CFinalizeGamePhase.h>
#include <WeParticleSystem.h>
#include <Helper.h>



WeParticleSystem* CTuber::StaticTrailPS = NULL;

CTuber::CTuber()
{
	OwnerProfile = NULL;
	Position = 0;
	LapCount = 0;
	ActualLapCount = 0;
	Type = ACTORTYPE_TUBER;
	NextPathNode = NULL;
	WrongWay = false;
	//PathNodeCounter = 0;
	MyStartLocation = NULL;
	//NumOfNodes = 0;
	CashCollected = 0;

	SoundEngine = NULL;
	SoundTurbo = NULL;
	SoundLocking = NULL;
	SoundQuadFire = NULL;
	SoundMultiKill = NULL;
	SoundLapCount = NULL;
	SoundTeleport = NULL;
	SoundScratch = NULL;
	SoundScratchLoop = NULL;

	Sparks = NULL;

	PPEffect = NULL;
	PPEffectTime = 0.0f;
	PPEffectTimer = 0.0f;
	ActivePPEffect = TuberPPNone;

	for (int i = 0; i < 10; ++i)
		Propulsion[i] = NULL;

	Score = 0;
	Kills = 0;
	Coins = 0;

	MaxRenderDistance = 400.0;
	Trail = NULL;
	CastShadowVolume = true;
	EnemyTarget = NULL;
	LastAttacker = NULL;
	Flares = NULL;
	Trails = NULL;
	Speedy = NULL;
	Light = NULL;
	LightProjector = NULL;
	Headlight = NULL;
	
	KillTime = -1.0f;
	QuickKillCounter = 0;
	QuickKillText = NULL;

	//AddonVelocity = WeVector(0.0f, 0.0f, 0.0f);
	//AddonVelocityResetCounter = 0;

	LapCount = 0;
	ActualLapCount = 0;
	//LastPathNode = NULL;
	//PathNodeCounter = 0;
	RefPathNode = NULL;
	RefDistance = 0.0;
	bInitialized = false;
	FSDamageNotifyTimer = FS_DAMAGE_NOTIFY_SPREAD;
	FSDamageNotify = false;
	FSDamageNotifyReset = false;

	LastEnemyHit = NULL;

	InfTurbo = false;

	PrevPosition = 0;

	FHDRIColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	FHDRIFactor = 0.0f;
	FHDRIFactorPrev = 0.0f;

	EndRaceTimer = 0.0f;
	
	PrevSpeed = 0.0f;
	MaxSpeedShakeTimer = 0.0f;

	CatchUpTimer = 0.0f;

	SpeedyTimer = 0.0f;
	SpeedyAlpha = 0.0f;

	TurboTimer = 0.0f;

	HealingInProgress = false;

	FirstPart = NULL;

	CatchUpSpeedFactor = 1.0f;

	TuberCollisionTimer = 1000.0f;

	MuteAudioTimer = 0.0f;
	MuteAudioEnable = false;
	MuteAudioSavedValue = WeAudioDevice::Instance()->GetSoundVolume();

	SoundScratchTimer = 0.0f;
	SoundScratchLoopTimer = 0.0f;

	TuberSparticles = NULL;
	_OldPathIndex = 0.0f;
}


CTuber::~CTuber()
{
	SAFE_DELETE(SoundEngine);
	SAFE_DELETE(SoundTurbo);
	SAFE_DELETE(SoundLocking);
	SAFE_DELETE(SoundQuadFire);
	SAFE_DELETE(SoundMultiKill);
	SAFE_DELETE(SoundLapCount);
	SAFE_DELETE(SoundTeleport);
	SAFE_DELETE(SoundScratch);
	SAFE_DELETE(SoundScratchLoop);

	if (!CMainGamePhase::Instance->Map.IsUnloading())
	{
		SAFE_DELETE(TurboModel);
		SAFE_DELETE(QuadDamage);
		SAFE_DELETE(InvulnerabilityModel);
		SAFE_DELETE(RefractionModel);
		SAFE_DELETE(HeadlightModel);
		SAFE_DELETE(Light);
		SAFE_DELETE(Headlight);
		SAFE_DELETE(LightProjector);
		SAFE_DELETE(Sparks);
		SAFE_DELETE(Trail);
	}

	SAFE_DELETE(SoundScratch);
	SAFE_DELETE(TuberSparticles);
}


void CTuber::Initialize()
{
	if (bInitialized) return;
	WeActor::Initialize();

	MaxTurboCharges = (1 + Description.TurboUpgradeLevel) * 100.0f;
	TurboCharges = MaxTurboCharges;

	PrimaryWeaponID = CWeaponDescriptionManager::Instance->GetWeaponID(Description.WeaponName);
	SecondaryWeaponID = CWeaponDescriptionManager::Instance->GetWeaponID(Description.MineName);
	CWeaponDescription* WeaponDesc;
	WeaponDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(PrimaryWeaponID);
	PrimaryWeaponAmmo = (WeaponDesc ? WeaponDesc->PickUpAmmo : 0);
	WeaponDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(SecondaryWeaponID);
	SecondaryWeaponAmmo = (WeaponDesc ? WeaponDesc->PickUpAmmo : 0);
	PrimaryWeaponAddonID = WEAPONTYPE_NONE;
	SecondaryWeaponAddonID = WEAPONTYPE_NONE;
	PrimaryWeaponAmmo2 = 0;
	SecondaryWeaponAmmo2 = 0;

	AfterTeleportedTimer = -1.0f;

	_OldLocation = Location;

	int Shape = 0;

	SAFE_DELETE(PhysicHandle);
	if (Shape == 0) // Box
	{
		PhysicHandle = WePhysics::Instance()->CreateBox(this, TM, WeVector(1.0f, 1.0f, 4.0f), COLLISION_GROUP_TUBER, 10000.0f, false);
		PhysicHandle->SetBoxCCDSkeleton(WeBox(WeVector(-0.5f, -0.5f, -3.5f), WeVector(0.5f, 0.5f, 3.5f)));
	}
	else if (Shape == 1) // Capsule
	{
		PhysicHandle = WePhysics::Instance()->CreateCapsule(this, Location, 4.0f, 1.0f, COLLISION_GROUP_TUBER, 10000.0f, false);
		PhysicHandle->SetBoxCCDSkeleton(WeBox(WeVector(-0.3f, -0.3f, -3.0f), WeVector(0.3f, 0.3f, 3.0f)));
	}
	else if (Shape == 10) // PhysicModel
	{
		//SAFE_DELETE(PhysicHandle);
		//PhysicHandle = WePhysics::Instance()->CreateFromPhysicModel(PhysicModelName, this, Location);
		//if (!PhysicHandle)
		//{
		//}
	}

	PhysicHandle->SetCCDThreshold(20);
	PhysicHandle->SetDisableGravity(true);
	//PhysicHandle->nxActor->setLinearDamping(7.0f);
	tb = true;


	TuberWorld = NULL;

	WeFileStream FS;
	char TuberMapName[1000];
	sprintf_s(TuberMapName, "%s.utt", TuberName);
	if (FS.Exists(TuberMapName))
	{
		TuberWorld = new CMap();
		while (!TuberWorld->IsLoaded())
			TuberWorld->LoadInTick(TuberMapName, false);
	}

	Trail = new WeRadialTrail();
	Trail->MaxSegments = 4;
	Trail->Location = Location;
	Trail->SegmentCreationTime = 0.05f;
	Trail->SegmentWidth = 0.15f;		
	SetTrailColor();
	Trail->FromRadius = 0.1f;
	Trail->Material = WeMaterialManager::Instance()->Load("Materials.Trails.Tuber");
	Trail->Initialize();
	CMainGamePhase::Instance->Map.AddActor(Trail);


	Flares = new WeActor;
	char FlareName[100];
	sprintf(FlareName, "Tubers.Models.%sFlare", TuberName);
	Flares->ModelName = FlareName;
	Flares->TM = TM;
	Flares->AutoUpdateTransformations = false;
	Flares->Initialize();
	CMainGamePhase::Instance->Map.AddActor(Flares);
	SAFE_DELETE(Flares->PhysicHandle);


	Trails = new WeActor;
	char TrailName[100];
	sprintf(TrailName, "Tubers.Models.%sTrail", TuberName);
	Trails->ModelName = TrailName;
	Trails->TM = TM;
	Trails->AutoUpdateTransformations = false;
	Trails->Initialize();
	//CMainGamePhase::Instance->Map.AddActor(Trails);
	SAFE_DELETE(Trails->PhysicHandle);


	Speedy = new WeActor;
	char SpeedyName[100];
	sprintf(SpeedyName, "Tubers.Models.%sSpeedScreen", TuberName);
	Speedy->ModelName = SpeedyName;
	Speedy->TM = TM;
	Speedy->AutoUpdateTransformations = false;
	Speedy->Initialize();
	CMainGamePhase::Instance->Map.AddActor(Speedy);
	SAFE_DELETE(Speedy->PhysicHandle);

	QuadDamage = new WeActor;
	char QuadDamageName[100];
	sprintf(QuadDamageName, "Tubers.Models.%sQuadDamage", TuberName);
	QuadDamage->ModelName = QuadDamageName;
	QuadDamage->TM = TM;
	QuadDamage->AutoUpdateTransformations = false;
	QuadDamage->Initialize();
	QuadDamage->Renderable = false;
	CMainGamePhase::Instance->Map.AddActor(QuadDamage);
	SAFE_DELETE(QuadDamage->PhysicHandle);

	TurboModel = new WeActor;
	char TurboModelName[100];
	sprintf(TurboModelName, "Tubers.Models.%sTurbo", TuberName);
	TurboModel->ModelName = TurboModelName;
	TurboModel->TM = TM;
	TurboModel->AutoUpdateTransformations = false;
	TurboModel->Initialize();
	TurboModel->Renderable = false;
	TurboModel->UseMaterialParams = true;
	CMainGamePhase::Instance->Map.AddActor(TurboModel);
	SAFE_DELETE(TurboModel->PhysicHandle);

	InvulnerabilityModel = new WeActor;
	char InvulnerabilityModelName[100];
	sprintf(InvulnerabilityModelName, "Tubers.Models.%sInvulnerability", TuberName);
	InvulnerabilityModel->ModelName = InvulnerabilityModelName;
	InvulnerabilityModel->TM = TM;
	InvulnerabilityModel->AutoUpdateTransformations = false;
	InvulnerabilityModel->Initialize();
	InvulnerabilityModel->Renderable = false;
	CMainGamePhase::Instance->Map.AddActor(InvulnerabilityModel);
	SAFE_DELETE(InvulnerabilityModel->PhysicHandle);

	RefractionModel = NULL;
	//RefractionModel = new WeActor;
	//char RefractionModelName[100];
	//sprintf(RefractionModelName, "Tubers.Models.%sRefraction", TuberName);
	//RefractionModel->ModelName = RefractionModelName;
	//RefractionModel->TM = TM;
	//RefractionModel->AutoUpdateTransformations = false;
	//RefractionModel->Initialize();
	//RefractionModel->Renderable = false;
	//CMainGamePhase::Instance->Map.AddActor(RefractionModel);
	//SAFE_DELETE(RefractionModel->PhysicHandle);

	InvisibilityModel = new WeActor;
	char InvisibilityModelName[100];
	sprintf(InvisibilityModelName, "Tubers.Models.%sInvisibility", TuberName);
	InvisibilityModel->ModelName = InvisibilityModelName;
	InvisibilityModel->TM = TM;
	InvisibilityModel->AutoUpdateTransformations = false;
	InvisibilityModel->Initialize();
	InvisibilityModel->Renderable = false;
	CMainGamePhase::Instance->Map.AddActor(InvisibilityModel);
	SAFE_DELETE(InvisibilityModel->PhysicHandle);

	HeadlightModel = new WeActor;
	char HeadlightModelName[100];
	sprintf(HeadlightModelName, "Tubers.Models.%sLights", TuberName);
	HeadlightModel->ModelName = HeadlightModelName;
	HeadlightModel->TM = TM;
	HeadlightModel->AutoUpdateTransformations = false;
	HeadlightModel->Initialize();
	HeadlightModel->Renderable = false;
	HeadlightModel->UseMaterialParams = true;
	CMainGamePhase::Instance->Map.AddActor(HeadlightModel);
	SAFE_DELETE(HeadlightModel->PhysicHandle);

	// sparticle trails
	InitializePropulsion();


	if (GetConfigInt("tuber.Projector.Enabled", 0) != 0)
	{
		LightProjector = new WeLight;	
		LightProjector->Type = LIGHT_PROJECTORONLY;	
		LightProjector->Projector = true;
		char tmp[100];
		sprintf(tmp, "Tubers.Projectors.%sProjector", TuberName);
		LightProjector->ProjectorMaterialName = tmp;
		LightProjector->Range = (float)GetConfigInt("ut.TuberProjector.Range", 100);
		LightProjector->TM = TM;		
		LightProjector->SpotAngle = D3DX_PI / 4.0f;
		LightProjector->Initialize();
		CMainGamePhase::Instance->Map.AddActor(LightProjector);
	}

	SAFE_DELETE(Light);
	if (GetConfigInt("tuber.Light.Enabled", 0) != 0)
	{
		Light = new WeLight;	
		Light->Type = LIGHT_POINT;
		Light->IsDynamic = true;
		Light->Range = (float)GetConfigInt("ut.TuberLight.Range", 100);
		Light->TM = TM;

		WeLightState LS;
		LS.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f); //Trail->SegmentColor;
		LS.Intensity = GetConfigInt("ut.TuberLight.Intensity", 100) / 100.0f;
		LS.SpecularIntensity = GetConfigInt("ut.TuberLight.SpecularIntensity", 0) / 100.0f;
		LS.Duration = 1.0f;

		Light->AddLightState(&LS);
		Light->Initialize();
		CMainGamePhase::Instance->Map.AddActor(Light);
	}

	SAFE_DELETE(Headlight);
	Headlight = new WeLight;	
	Headlight->Type = LIGHT_POINT;
	Headlight->IsDynamic = false;
	Headlight->Range = 0.0f;
	WeLightState LS;
	LS.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	LS.Intensity = 1.0f;
	LS.SpecularIntensity = 0.2f;
	LS.Duration = 1.0f;
	Headlight->AddLightState(&LS);
	Headlight->Initialize();
	CMainGamePhase::Instance->Map.AddActor(Headlight);

	SoundTurbo = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.Turbo", true);
	SoundTeleport = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.Teleport", true);

	bInitialized = true;
	CastShadowVolume = false;
}


void CTuber::TickTurbo()
{
	float DeltaF = WeTimer::Instance()->GetDeltaTimeF();

	TuberPartTimer += DeltaF;


	WeVector x,y,z;
	GetLocalAxis(x,y,z);
	float d;

	D3DXVec3Normalize(&z, &z);
	y = AngularVelocity;
	D3DXVec3Normalize(&y, &y);
	d = abs(D3DXVec3Dot(&y, &z));	

	if (OnTurbo)
	{
		TurboTimer += (DeltaF * 10.0f);
		if (TurboTimer > 1.0f) TurboTimer = 1.0f;

		if (SoundTurbo)
		{
			if (!SoundTurbo->IsPlaying())
				SoundTurbo->Play(true);
		}
		if (!InfTurbo)
		{
			TurboCharges -= DeltaF * TURBO_USAGE_PER_SEC; 
			if (TurboCharges < 0.0f) TurboCharges = 0.0f;
		}
	}
	else
	{
		TurboTimer -= (DeltaF * 3.5f);
		if (TurboTimer <= 0.0f) TurboTimer = 0.0f;

		if (SoundTurbo)
		{
			SoundTurbo->Stop();
		}
	}

	TurboModel->Renderable = (TurboTimer > 0.0f) && (!Invisible) && (!Dead) && (!RaceFinished);
	TurboModel->MaterialColor.a = TurboTimer;

	// make fov and blur according to maxturbo speed and current speed. 
	if (CMainGamePhase::Instance->InputPlayer == this)
	{
		CameraConfig *CC;
		CC = CMainGamePhase::Instance->Camera.CurrentCamera;

		/*
		float DeltaMaxSpeed, DeltaFOV, DeltaSpeed;

		DeltaMaxSpeed = TurboMaxSpeed - MaxSpeed;
		DeltaSpeed = CurrentSpeed - MaxSpeed;
		if (DeltaSpeed < 0)
			DeltaSpeed = 0;

		if (abs(DeltaMaxSpeed) < 0.001f)
			BlurFactor = 0.0f;
		else
			BlurFactor = DeltaSpeed * 0.92f / DeltaMaxSpeed;
		BlurFactor = ((BlurFactor + AddonBlurFactor) < 0.99f) ? (BlurFactor + AddonBlurFactor) : 0.99f;
		WeRenderDevice::Instance()->SetBlurFactor(BlurFactor);

		if (AddonBlurFactor > 0.0f)
		{
			AddonBlurFactor -= (0.4f * DeltaF);		//reduce addon blur 0.4f per sec
			if (AddonBlurFactor < 0.0f)
				AddonBlurFactor = 0.0f;
		}

		FOV = D3DX_PI / 2.0f;
		DeltaFOV = D3DX_PI / 2.7f - D3DX_PI / 4.0f;
		if (abs(DeltaMaxSpeed) >= 0.001f)
			FOV += DeltaSpeed * DeltaFOV / DeltaMaxSpeed;

		FOV = D3DX_PI / 2.0f + CurrentSpeed / TurboMaxSpeed * (D3DX_PI / 1.3f - D3DX_PI / 2.0f);
		*/

		if (CurrentSpeed >= TurboMaxSpeed)
		{
			FOV = CC->TurboFOV;
		}
		else if (CurrentSpeed >= MaxSpeed)
		{
			float d = TurboMaxSpeed - MaxSpeed;
			if (abs(d) < 0.001f)
				FOV = CC->MaxFOV;
			else
				FOV = CC->MaxFOV + (CurrentSpeed - MaxSpeed) / d * (CC->TurboFOV - CC->MaxFOV);
		}
		else if (CurrentSpeed <= 0.0f)
			FOV = CC->MinFOV;
		else
		{
			float k = CurrentSpeed / MaxSpeed;
			if (k <= CC->MinFOVSpeed)
				FOV = CC->MinFOV;
			else if (k >= CC->MaxFOVSpeed)
				FOV = CC->MaxFOV;
			else
				FOV = CC->MinFOV + (k - CC->MinFOVSpeed) / (CC->MaxFOVSpeed - CC->MinFOVSpeed) * (CC->MaxFOV - CC->MinFOV);
		}

		static float ActualFOV = -1.0f;

		if (ActualFOV < 0.0f)
			ActualFOV = FOV;
		else
		{
			float FOVSpeed = GetConfigFloat("cameras.General.FOVSpeed", 5.0f);
			float kSpeed = FOVSpeed * WeTimer::Instance()->GetDeltaTimeF();
			if (kSpeed > 1.0f)
				kSpeed = 1.0f;

			ActualFOV += (FOV - ActualFOV) * kSpeed;
		}

		CMainGamePhase::Instance->Camera.SetFov(ActualFOV);
		CMainGamePhase::Instance->Camera.UpdateProjection = true;
	}
}


void CTuber::SetTrailColor()
{
	WeColor Colors[16];
	int i = 0;
	Colors[i].r = 1.0f; Colors[i].g = 1.0f; Colors[i].b = 1.0f; i++;
	Colors[i].r = 0.0f; Colors[i].g = 0.0f; Colors[i].b = 1.0f; i++;
	Colors[i].r = 1.0f; Colors[i].g = 1.0f; Colors[i].b = 0.0f; i++;
	Colors[i].r = 1.0f; Colors[i].g = 0.0f; Colors[i].b = 1.0f; i++;
	Colors[i].r = 0.0f; Colors[i].g = 1.0f; Colors[i].b = 1.0f; i++;
	Colors[i].r = 1.0f; Colors[i].g = .5f; Colors[i].b = 0.0f; i++;
	Colors[i].r = .5f; Colors[i].g = 0; Colors[i].b = 1.0f; i++;
	Colors[i].r = 0; Colors[i].g = 1; Colors[i].b = .5f; i++;
	Colors[i].r = 0.5f; Colors[i].g = 1; Colors[i].b = 0; i++;
	Colors[i].r = 1.0f; Colors[i].g = 0; Colors[i].b = 0.5f; i++;
	Colors[i].r = 0.0f; Colors[i].g = 0.5f; Colors[i].b = 1.0f; i++;
	Colors[i].r = 1.0f; Colors[i].g = 1.0f; Colors[i].b = 1.0f; i++;
	Colors[i].r = .25f; Colors[i].g = 0; Colors[i].b = .5f; i++;
	Colors[i].r = 0; Colors[i].g = 0.5f; Colors[i].b = .25f; i++;
	Colors[i].r = 0.5f; Colors[i].g = 0.25f; Colors[i].b = .0f; i++;
	Colors[i].r = 0.25f; Colors[i].g = 0.25f; Colors[i].b = .5f; i++;

	//char tmp[100];
	//sprintf(tmp, "Team: %d", OwnerProfile->TeamNumber);
	//MessageBox(0, tmp, 0,0 );
	Trail->SegmentColor = CGameSettings::ClientInstance->TeamColors[OwnerProfile->TeamNumber-1];
	Trail->SegmentColor.a = 1.0f;
}


void CTuber::OnTeleported()
{
	_OldLocation = Location;

	WeVector V;
	PhysicHandle->GetLinearVelocity(V);
	D3DXVec3Normalize(&V, &V);
	V *= CurrentSpeed;
	PhysicHandle->SetLinearVelocity(V);

	if (IsInputPlayer)
		CMainGamePhase::Instance->bFixCameraDirection = true;

	if (SoundTeleport)
		SoundTeleport->Play(false);
}


void CTuber::Tick()
{
	float DeltaF = WeTimer::Instance()->GetDeltaTimeF();
	MaterialPreset = TuberMaterialPresetNormal;

	TickPathNodes();
	WeNetworkActor::Tick();

	TuberCollisionTimer += DeltaF;

 	if (RaceFinished /*&& CurrentSpeed == 0*/)
	{
		EndRaceTimer += DeltaF;

		CAgent *MyAgent = CPlayGamePhase::Instance->GetAIKernel()->GetAgent(this);
		if (!MyAgent)
		{
			for (int i = 0; i < MAX_PLAYERS; ++i)
				if (CGameSettings::ClientInstance->tmpPlayers[i].User == OwnerProfile)
				{
					MyAgent = new CAgent(&CGameSettings::ClientInstance->tmpPlayers[i]);
					CPlayGamePhase::Instance->GetAIKernel()->Agents.PushBack(MyAgent);
					break;
				}
		}
		MyAgent->GetAvatarConfig()->Status = PCFG_AIMEDIUM;
		MyAgent->ForceNodeMotorics(70.0f, 0.5f, 0.9f);
		MyAgent->SetArmoryEnabled(false);

		if ( PhysicHandle->GetCollision() && (abs(CurrentSpeed) > 10) )
		{
			MyAgent->ForceNodeMotorics(0.0f, 2.0f, 0.9f);
		}
		else
		{
			MyAgent->ForceNodeMotorics(70.0f, 0.5f, 0.9f);
			PhysicHandle->SetCollision(false);

			if (CMainGamePhase::Instance->InputPlayer != this)
			{
				MaterialPreset = TuberMaterialPresetFading;
				UseMaterialParams = true;
				MaterialColor.a -= DeltaF * 0.1f;
				MaterialParam1 -= DeltaF * 0.1f;
				MaterialParam2 -= DeltaF * 0.1f;
				if (MaterialColor.a <= 0.0f)
				{
					MaterialColor.a = 0.0f;
					MaterialParam1 = 0.0f;
					MaterialParam2 = 0.0f;
					Renderable = false;
				}
			}
		}

		if ( IsInputPlayer && (EndRaceTimer >= END_RACE_TIMEOUT) )
		{
			if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_SINGLERACE ||
				CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT ||
				CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_INSTANTRACE ||
				CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_DEATHMATCH ||
				CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE ||
				CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL)
			{
				 CMainGamePhase::Instance->SetPhase(CFinalizeGamePhase::Instance);
			}
		}
	}


	WeVector x,y,z;
	bool FPSCam = IsInputPlayer && CMainGamePhase::Instance->Camera.CurrentCamera && CMainGamePhase::Instance->Camera.CurrentCamera->FPS;

	TickLight();

	if (TuberWorld)
		TuberWorld->Tick();

	TickTubeLimit();
	TickTurbo();
	TickSound();
	TickPropulsion(FPSCam);
	TickMuteAudio();

	// fullscreen damage notifier timer
	FSDamageNotifyTimer += DeltaF;

	// weapon fire timers
	PrimaryWeaponFireTime += DeltaF;
	SecondaryWeaponFireTime += DeltaF;

	//AddonVelocityResetCounter++;
	//if (AddonVelocityResetCounter > 2)
	//{
	//	AddonVelocity = WeVector(0.0f, 0.0f, 0.0f);
	//	AddonVelocityResetCounter = 0;
	//}
	//if (IsPushable())
	//	Velocity += AddonVelocity;

	TickLevitate();

	//PhysicHandle->nxActor->addForce(NxVec3(Velocity.x, Velocity.y, Velocity.z), NX_SMOOTH_VELOCITY_CHANGE);
	//PhysicHandle->AddImpulse(Velocity);	

	TickCatchUp();

	PhysicHandle->SetLinearVelocity(Velocity * CatchUpSpeedFactor);
	PhysicHandle->SetAngularVelocity(AngularVelocity);

	TickAddons(FPSCam);

	TickPickups();

	// TODO: Ovo mozda prepraviti
	if (IsInputPlayer)
	{
		WeRenderDevice::Instance()->SetPixelMotionBlurFactor(CurrentSpeed / 200.0f);
	}

	// other game logic here...	
	if (Dead)
	{
		RespawnCounter += DeltaF;
		if (RespawnCounter >= RespawnTime)
		{
			Respawn();
		}
	}

	//if (Dead) Light->Projector = false;
	//else Light->Projector = true;


	if (!MyStartLocation)
	{
		for (CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
		{
			if (CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber == this)
				MyStartLocation = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem();
		}
	}

	if (IsInputPlayer && (CurrentSpeed > 1.0f) && (CurrentSpeed > PrevSpeed) && (CurrentSpeed < (MaxSpeed * 0.2f)))
	{
		CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShakingSofting;
		CMainGamePhase::Instance->Camera.StateDefaultingTime = 1.0f;
	}

	//if ( IsInputPlayer && (CurrentSpeed > 0.8f * MaxSpeed) && RefPathNode->Shaking )
	//{
	//	CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShakingSofting;
	//	CMainGamePhase::Instance->Camera.StateDefaultingTime = 2.0f;
	//}

	//if (CurrentSpeed >= MaxSpeed)
	//	MaxSpeedShakeTimer += DeltaF;
	//else
	//	MaxSpeedShakeTimer = 0.0f;
	//if ( IsInputPlayer && (MaxSpeedShakeTimer >= 5.0f) )
	//{
	//	CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShakingSofting;
	//	CMainGamePhase::Instance->Camera.StateDefaultingTime = 1.0f;
	//}


	TickHealing();
	TickLapLogic();
	TickScreenFlash();
	TickFHDRI™();
	TickPPEffect();
	TickTargetLock();
	TickScoreLogic();

	PrevSpeed = CurrentSpeed;


	if (AfterTeleportedTimer > -1.0f)
	{
		AfterTeleportedTimer += WeTimer::Instance()->GetDeltaTimeF();
		if (AfterTeleportedTimer >= 3.0f)
		{
			Trail->Renderable = true;
			AfterTeleportedTimer = -1.0f;
		} else Trail->Renderable = false;
	}


	Velocity = WeVector(0,0,0);
	AngularVelocity = WeVector(0,0,0);
}


void CTuber::TickPickups()
{
	if (Dead || RaceFinished) return;

	for (PickupActions.First(); !PickupActions.End(); )
	{
		PickupActions.GetItem()->Update(this);
		if (!PickupActions.GetItem()->Valid())
		{
			WeSound *EndPickupSound = NULL;
			switch (PickupActions.GetItem()->GetActionType())
			{
			case PickupActionQuadDamage:
				EndPickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.EndQuadDamage");
				break;
			case PickupActionInvisibility:
				EndPickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.EndInvisibility");
				break;
			case PickupActionInvulnerability:
				EndPickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.EndInvulnerability");
				break;
			}
			if (EndPickupSound)
			{
				EndPickupSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
				EndPickupSound->Play(false);
				EndPickupSound->DieOnStop();
			}
			
			delete PickupActions.GetItem();
			PickupActions.Delete();
		}
		else
		{
			PickupActions.Next();
		}
	}
}

void CTuber::TickScreenFlash()
{
	float DeltaF = WeTimer::Instance()->GetDeltaTimeF();
	//static float BaseBloomScale = WeRenderDevice::Instance()->GetBloomScale();
	static float BaseBloomScale = 1.0f;

	bool FlashStateElapsed = false;
	if (PreviousFlash != CurrentFlash)
	{
		FlashTimer = 0.0f;
		PreviousFlash = CurrentFlash;
	}
	FlashTimer += DeltaF;

	switch(CurrentFlash)
	{
	case TuberFlashNuke:
		WeRenderDevice::Instance()->SetBrightnessColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
		WeRenderDevice::Instance()->SetBrightness(sin((D3DX_PI / 2) * FlashTimer * 2.0f));
		if (FlashTimer >= 0.5f)
			CurrentFlash = TuberFlashNuke2;
		break;

	case TuberFlashNuke2:
		WeRenderDevice::Instance()->SetBrightness(sin((D3DX_PI / 2) + ((D3DX_PI / 2) * FlashTimer * 1.0f)));
		if (FlashTimer >= 1.0f)
			FlashStateElapsed = true;
		break;

	case TuberFlashTeleport:
		WeRenderDevice::Instance()->SetBrightnessColor(Trail->SegmentColor);
		WeRenderDevice::Instance()->SetBrightness(0.5f * (FlashTimer / 0.2f));
		if (FlashTimer >= 0.2f)
			CurrentFlash = TuberFlashTeleport2;
		break;

	case TuberFlashTeleport2:
		WeRenderDevice::Instance()->SetBrightness(0.5f - (0.5f * (FlashTimer / 0.2f)));
		if (FlashTimer >= 0.2f)
			FlashStateElapsed = true;
		break;

	case TuberFlashCoin:
		WeRenderDevice::Instance()->SetBrightnessColor(WeColor(1.0f, 1.0f, 0.0f, 1.0f));
		WeRenderDevice::Instance()->SetBrightness(0.1f * (FlashTimer / 0.05f));
		if (FlashTimer >= 0.05f)
			CurrentFlash = TuberFlashCoin2;
		break;

	case TuberFlashCoin2:
		WeRenderDevice::Instance()->SetBrightness(0.1f - (0.35f * (FlashTimer / 0.05f)));
		if (FlashTimer >= 0.05f)
			FlashStateElapsed = true;
		break;

	case TuberFlashTuberBoom:
		WeRenderDevice::Instance()->SetBrightnessColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
		WeRenderDevice::Instance()->SetBrightness((FlashTimer / 0.2f) * 0.6f);
		if (FlashTimer >= 0.1f)
			CurrentFlash = TuberFlashTuberBoom2;
		break;

	case TuberFlashTuberBoom2:
		WeRenderDevice::Instance()->SetBrightness((1.0f - ((FlashTimer - 0.2f) / 0.2f)) * 0.6f);
		if (FlashTimer >= 0.2f)
			FlashStateElapsed = true;
		break;

	case TuberFlashFHDRI:
		//WeRenderDevice::Instance()->SetBrightnessColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
		//WeRenderDevice::Instance()->SetBrightness((FlashTimer / 0.3f) * FlashFactor);
		//WeRenderDevice::Instance()->SetBloomScale(1.5f + (FlashTimer / 0.3f) * FlashFactor);
		//if (FlashTimer >= 0.3f)
			//CurrentFlash = TuberFlashFHDRI2;
		if (FlashFactor > CurrentFlashFactor)
		{
			CurrentFlashFactor += DeltaF / 0.3f;
			if (CurrentFlashFactor >= FlashFactor)
			{
				CurrentFlashFactor = FlashFactor;
				FlashFactor = 0.0f;
			}
		}
		else
		{
			CurrentFlashFactor -= DeltaF;
			if (CurrentFlashFactor <= FlashFactor)
			{
				CurrentFlashFactor = FlashFactor;
				FlashStateElapsed = true;
			}
		}
		WeRenderDevice::Instance()->SetBloomScale(CMainGamePhase::Instance->Map.MapHeader.FHDRI[1] + CurrentFlashFactor);
		//WeLoger::DebugLog("FHDRI %.3f\n", WeRenderDevice::Instance()->GetBloomScale());
		break;

	case TuberFlashFHDRI2:
		//WeRenderDevice::Instance()->SetBrightness((1.0f - ((FlashTimer - 0.3f) / 2.0f)) * FlashFactor);
		//WeRenderDevice::Instance()->SetBloomScale(1.5f + (1.0f - ((FlashTimer - 0.3f) / 1.0f)) * FlashFactor);
		//if (FlashTimer >= 1.0f)
			//FlashStateElapsed = true;
		if (FlashFactor < CurrentFlashFactor)
		{
			CurrentFlashFactor -= DeltaF / 0.2f;
			if (CurrentFlashFactor <= FlashFactor)
			{
				CurrentFlashFactor = FlashFactor;
				FlashFactor = 0.0f;
			}
		}
		else
		{
			CurrentFlashFactor += DeltaF * 5.0f;
			if (CurrentFlashFactor >= FlashFactor)
			{
				CurrentFlashFactor = FlashFactor;
				FlashStateElapsed = true;
			}
		}
		WeRenderDevice::Instance()->SetBloomScale(CMainGamePhase::Instance->Map.MapHeader.FHDRI[1] + CurrentFlashFactor);
		//WeLoger::DebugLog("FHDRI2 %.3f\n", WeRenderDevice::Instance()->GetBloomScale());
		break;
	}

	if (FlashStateElapsed)
	{
		CurrentFlash = TuberFlashNone;
		WeRenderDevice::Instance()->SetBrightness(0.0f);
		//WeRenderDevice::Instance()->SetBloomScale(BaseBloomScale);
		WeRenderDevice::Instance()->SetBloomScale(CMainGamePhase::Instance->Map.MapHeader.FHDRI[1]);
	}
}


void CTuber::TickSound()
{
	const float SOUND_SCRATCH_LIFETIME = 0.5f;

	float time = WeTimer::Instance()->GetDeltaTimeF();

	if (CMainGamePhase::Instance->InputPlayer == this)
	{
		WeListener *List = WeAudioDevice::Instance()->Listener;
		WeVector AX, AY, AZ;
		GetLocalAxis(AX, AY, AZ);
		D3DXVec3Normalize(&AZ, &AZ);
		D3DXVec3Normalize(&AY, &AY);
		if (List)
		{
			List->SetPosition(Location);
			List->SetOrientationUp(AY);
			List->SetOrientationFront(AZ);
		}
	}

	if(!SoundEngine)
	{
		SoundEngine = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.Engine", true);
		if (SoundEngine)
		{
			Frequency = SoundEngine->GetFrequency();
			SoundEngine->Play(true);
		}
	}
	else
	{
		if (SoundEngine)
		{
			if (!SoundEngine->IsPlaying())
				SoundEngine->Play(true);
			float SpeedFactor = abs(CurrentSpeed) / MaxSpeed;
			DWORD Freq = (Frequency) + (DWORD)(SpeedFactor * Frequency * 0.45f);
			SoundEngine->Update(Location - (2.0f * Direction), WeVector(0.0f, 0.0f, 0.0f));
			SoundEngine->SetFrequency(Freq);
		}
	}

	if (OnTurbo && SoundTurbo)
		SoundTurbo->Update(Location - (2.0f * Direction), WeVector(0.0f, 0.0f, 0.0f));

	if (SoundScratch && (SoundScratchTimer >= 0.0f))
	{
		SoundScratchTimer -= WeTimer::Instance()->GetDeltaTimeF();
		if (SoundScratchTimer <= 0.0f)
		{
			SoundScratch->Stop();
			SoundScratchTimer = -1.0f;
		}
		else
			SoundScratch->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
	}

	if (SoundTeleport)
		SoundTeleport->Update(Location, WeVector(0.0f, 0.0f, 0.0f));

	SoundScratchTimer += time;
	SoundScratchLoopTimer += time;

	if (SoundScratch && SoundScratch->IsPlaying())
	{
		SoundScratch->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		if (SoundScratchTimer > 0.2f)
			SoundScratch->Stop();
	}

	if (SoundScratchLoop && SoundScratchLoop->IsPlaying())
	{
		SoundScratchLoop->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		if (SoundScratchLoopTimer > 0.5f)
			SoundScratchLoop->Stop();
	}
}


void CTuber::TickPPEffect()
{
	if (PPEffectTime < 0.0f) return;

    float time = WeTimer::Instance()->GetDeltaTimeF();

    if (ActivePPEffect != TuberPPNone)
		PPEffectTimer += time;

	if (PPEffectTimer >= PPEffectTime)
	{
		switch (ActivePPEffect)
		{
		case TuberPPFadeToMonochrome:
			SetPPEffect(TuberPPMonochrome, -1.0f);
			break;

		default:
			WePostProcess::Instance()->RemoveEffect(PPEffect);
			SAFE_DELETE(PPEffect);
			ActivePPEffect = TuberPPNone;
			break;
		}
	}

	switch(ActivePPEffect)
	{
	case TuberPPGhost:
	case TuberPPFadeToColor:
		PPEffect->Param1 = 1.0f * (PPEffectTimer / PPEffectTime);
		break;

	case TuberPPFadeToMonochrome:
		PPEffect->Param1 = (1.0f - (1.0f * (PPEffectTimer / PPEffectTime)));
		break;

	default:
		return;
	}
}


void CTuber::TickLevitate()
{
	//if (!IsInputPlayer) return;
	if ( Dead || (abs(CurrentSpeed) > 0) ) return;

	WeVector AX, AY, AZ;
	GetLocalAxis(AX, AY, AZ);

	D3DXVec3Normalize(&AY, &AY);
	Velocity += 1.0f * sin(3.0f * WeTimer::Instance()->GetElapsedTimeF()) * AY;

	if ( (CPlayGamePhase::Instance->Status != PLAYGAME_RACE) || (AngularVelocity == WeVector(0.0f, 0.0f, 0.0f)) )
	{
		D3DXVec3Normalize(&AZ, &AZ);
		D3DXVec3Normalize(&AX, &AX);
		AngularVelocity += (0.04f * cos(3.0f * WeTimer::Instance()->GetElapsedTimeF())) * AZ;
		AngularVelocity += (0.02f * sin(3.0f * WeTimer::Instance()->GetElapsedTimeF())) * AX;
	}
}

void CTuber::TickScoreLogic()
{
	//if (Deathmatch) return; //uncomment this line ASAP
	
	//if (CPlayGamePhase::Instance->Status != PLAYGAME_RACE) return;

	//ScoreTimer += WeTimer::Instance()->GetDeltaTimeF();

	//if (ScoreTimer >= 0.2f)
	//{
	//	ScoreTimer = 0.0f;
	//	Score += (int)(20.0f * ((1.0f / (float)MAX_PLAYERS) * (float)((MAX_PLAYERS + 1) - Position)));
	//}
}

void CTuber::TickPathNodes()
{
	if (RefPathNode)
		_OldPathIndex = RefPathNode->PathIndex;

	RefPathNode = NULL;
	RefDistance = 0.0;

	if (World)
	{
		WePathNode *Node2;
		float d1, d2;
		WeVector RefPos;

		World->GetRefPathNodes(Location, RefPathNode, Node2, d1, d2, RefPos);

		//WeVector v;
		//WeVector XAxis, YAxis, ZAxis, tmpV;

		//RefPathNode->GetLocalAxis(XAxis, YAxis, ZAxis);
		//D3DXVec3Subtract(&v, &Location, &RefPathNode->Location);

		//RefPathNode->SortedIndex = RefPathNode->SortedIndex;
		//RefDistance = D3DXVec3Dot(&v, &ZAxis);
		static WePathNode *LastRefPathNode = NULL;

		//if (LastRefPathNode && (RefPathNode->SortedIndex < LastRefPathNode->SortedIndex))
			//WeLoger::DebugLog("RefPathNode(%s): %d %d %.2f\n", Name.GetString(), LapCount, RefPathNode->SortedIndex, RefDistance);

		LastRefPathNode = RefPathNode;

		RefDistance = d1;
		WrongWay = (D3DXVec3Dot(&Direction, &RefPathNode->Direction) < 0);
	}
}


void CTuber::TickHealing()
{
	float MinHealingSpeed = 0.0f, MaxHealingSpeed = 0.0f;
	WePathNode *Iterator;
	WeVector v;

	for(Iterator = World->PathNodes->NextPathNode; Iterator != World->PathNodes; Iterator = Iterator->NextPathNode)
	{
		D3DXVec3Subtract(&v, &Location, &Iterator->Location);
		if (D3DXVec3Length(&v) <= Iterator->HealingRange)
		{
			if (Iterator->HealingSpeed > MaxHealingSpeed)
				MaxHealingSpeed = Iterator->HealingSpeed;
			if (Iterator->HealingSpeed < MinHealingSpeed)
				MinHealingSpeed = Iterator->HealingSpeed;
		}
	}

	HealingInProgress = false;

	if (!Dead && !RaceFinished)
	{
		float HealAmount = MinHealingSpeed * WeTimer::Instance()->GetDeltaTimeF();

		HP += HealAmount;
		if (HP > MaxHP)
			HP = MaxHP;

		HealingInProgress = (HealAmount > 0.0f);

		if (MinHealingSpeed < 0.0f)
			TakeDamage(MinHealingSpeed);
	}
}


void CTuber::TickLapLogic()
{
	if (CPlayGamePhase::Instance->Status == PLAYGAME_RACE)
		LapTime += WeTimer::Instance()->GetDeltaTimeF();

	RaceTime = CPlayGamePhase::Instance->RaceTime;

	if (World)
	{
		if ((_OldPathIndex < 0.2f) && (RefPathNode->PathIndex > 0.8f))
		{
			ActualLapCount--;
		}
		else if ((_OldPathIndex > 0.8f) && (RefPathNode->PathIndex < 0.2f))
		{
			ActualLapCount++;
			if ((ActualLapCount > LapCount) && !RaceFinished)
			{
				LapFinished();

				LapCount++;
				LapTime = 0;

				LastLapTime = CPlayGamePhase::Instance->RaceTime - StartLapTime;
				if (LastLapTime < BestLapTime || BestLapTime == 0) BestLapTime = LastLapTime;
				StartLapTime = CPlayGamePhase::Instance->RaceTime;
				if(LastLapTime < CMainGamePhase::Instance->Map.FastesLap || 
					CMainGamePhase::Instance->Map.FastesLap == 0)
					CMainGamePhase::Instance->Map.FastesLap = LastLapTime;
				if (Position == 1)
					CMainGamePhase::Instance->Map.FastestTuberTime[LapCount] = CPlayGamePhase::Instance->RaceTime;

				DisplayLapText(LastLapTime);
			}
		}
	}

	_OldLocation = Location;

	if ( (SoundLapCount) && (!SoundLapCount->IsPlaying()) )
	{
		SAFE_DELETE(SoundLapCount);
		if (GetLapsRemaining() > 1)
			SoundLapCount = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.LapsRemaining", false);
		else
			SoundLapCount = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.LapRemaining", false);

		SoundLapCount->Play(false);
		SoundLapCount->DieOnStop();
		SoundLapCount = NULL;
	}

}


void CTuber::TickTargetLock()
{
	if (CPlayGamePhase::Instance->ShowInGameMenu) return;

	if (!CWeaponFabric::Instance->IsValid())
	{
		ClearEnemyTargetLock();
		return;
	}

	if (Invisible && HasTargetLock)
	{
		HasTargetLock = false;
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			if (!CGameSettings::ClientInstance->tmpPlayers[i].User) continue;
			CTuber *Dude = CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber;
			if (!Dude) continue;
			if (Dude->EnemyTarget == this)
			{
				Dude->ClearEnemyTargetLock();
			}
		}
	}

	CWeaponDescription *WeapDesc;
	if (PrimaryWeaponAddonID != WEAPONTYPE_NONE)
	{
		if (PrimaryWeaponAmmo2 <= 0) 
		{
			ClearEnemyTargetLock();
			return;
		}
		WeapDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(PrimaryWeaponAddonID);
	}
	else
	{
		if (PrimaryWeaponAmmo <= 0) 
		{
			ClearEnemyTargetLock();
			return;
		}
		WeapDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(PrimaryWeaponID);
	}
	if ( RaceFinished || Dead || (WeapDesc && (WeapDesc->HomingLockTime <= 0.0f)) )
	{
		ClearEnemyTargetLock();
		return;
	}

	if (!EnemyTarget)
	{
		EnemyTarget = GetBestEnemyTargetLock();
		if (!EnemyTarget) return;
		
		WEAPONTYPE CurrentWeaponID;
		if (PrimaryWeaponAddonID != WEAPONTYPE_NONE)
			CurrentWeaponID = PrimaryWeaponAddonID;
		else
			CurrentWeaponID = PrimaryWeaponID;
		EnemyLockWeaponID = CurrentWeaponID;
	}
	else
	{
		if (!CheckEnemyTargetLock())
		{
			ClearEnemyTargetLock();
			return;
		}

		EnemyLockTimer += WeTimer::Instance()->GetDeltaTimeF();
		EnemyLockFactor = (EnemyLockTimer / WeapDesc->HomingLockTime);
		if (EnemyLockFactor > 1.0f) EnemyLockFactor = 1.0f;
		if (EnemyLockFactor == 1.0f)
			EnemyTarget->HasTargetLock = true;

		if (this == CMainGamePhase::Instance->InputPlayer)
		{
			if ( (EnemyLockFactor < 1.0f) && (!SoundLocking) )
			{
				SoundLocking = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Locking", false);
				if (SoundLocking)
				{
					SoundLocking->Play(true);
				}
			}

			if ( (EnemyLockFactor == 1.0f) && (SoundLocking) )
			{
				SoundLocking->Stop();
				SAFE_DELETE(SoundLocking);
				WeSound *Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Locked", false);
				if (Sound)
				{
					Sound->Play(false);
					Sound->DieOnStop();
				}
			}
		}
	}
}

CTuber* CTuber::GetBestEnemyTargetLock()
{
	CGameSettings *Settings = NULL;
	Settings = CGameSettings::ClientInstance;

	if (!Settings->GetWeapons()) return NULL;

	CTuber*	BestTuber = NULL;
	float	BestValue = MIN_LOCK_DOT;
	int i = 0;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (!Settings->tmpPlayers[i].User) continue;
		if (!ValidateTarget(Settings->tmpPlayers[i].User->Tuber)) continue;

		WeVector VecDistance = (Settings->tmpPlayers[i].User->Tuber->Location - Location);
		if (D3DXVec3Length(&VecDistance) > MAX_LOCK_DISTANCE) continue;

		WeVector X, Y, Z;
		GetLocalAxis(X, Y, Z);
		D3DXVec3Normalize(&VecDistance, &VecDistance);
		D3DXVec3Normalize(&Z, &Z);
		float DotProduct = D3DXVec3Dot(&VecDistance, &Z);

		if (DotProduct > BestValue)
		{
			BestValue = DotProduct;
			BestTuber = Settings->tmpPlayers[i].User->Tuber;
		}
	}

	return BestTuber;
}


CTuber* CTuber::GetBestEnemyTargetLock(float Distance, float MinDot)
{
	CGameSettings *Settings = NULL;
	Settings = CGameSettings::ClientInstance;

	CTuber*	BestTuber = NULL;
	float	BestValue = Distance;
	int i = 0;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if (!Settings->tmpPlayers[i].User) continue;
		if (!ValidateTarget(Settings->tmpPlayers[i].User->Tuber)) continue;

		WeVector VecDistance = (Settings->tmpPlayers[i].User->Tuber->Location - Location);
		float NewDistance = D3DXVec3Length(&VecDistance);
		if (NewDistance > Distance) continue;

		WeVector X, Y, Z;
		GetLocalAxis(X, Y, Z);
		D3DXVec3Normalize(&VecDistance, &VecDistance);
		D3DXVec3Normalize(&Z, &Z);
		float DotProduct = D3DXVec3Dot(&VecDistance, &Z);
		if (DotProduct < MinDot) continue;

		if (NewDistance < BestValue)
		{
			BestValue = NewDistance;
			BestTuber = Settings->tmpPlayers[i].User->Tuber;
		}
	}

	return BestTuber;
}


bool CTuber::CheckEnemyTargetLock()
{
	if (!EnemyTarget) return false;
	if (EnemyTarget->Dead) return false;
	if (EnemyTarget->RaceFinished) return false;

	if ((RefPathNode->TubeID != 0) && (EnemyTarget->RefPathNode->TubeID != 0) &&
		(RefPathNode->TubeID != EnemyTarget->RefPathNode->TubeID))
		return false;

	WEAPONTYPE CurrentWeaponID;
	if (PrimaryWeaponAddonID != WEAPONTYPE_NONE)
		CurrentWeaponID = PrimaryWeaponAddonID;
	else
		CurrentWeaponID = PrimaryWeaponID;
	if (EnemyLockWeaponID != CurrentWeaponID)
		return false;

	WeVector VecDistance = (EnemyTarget->Location - Location);
	if (abs(D3DXVec3Length(&VecDistance)) > MAX_LOCK_DISTANCE) return false;

	WeVector X, Y, Z;
	GetLocalAxis(X, Y, Z);
	D3DXVec3Normalize(&VecDistance, &VecDistance);
	D3DXVec3Normalize(&Z, &Z);
	if (D3DXVec3Dot(&VecDistance, &Z) < MIN_LOCK_DOT) return false;

	return true;
}

void CTuber::ClearEnemyTargetLock()
{
	EnemyLockFactor = 0.0f;
	EnemyLockTimer = 0.0f;
	EnemyLockWeaponID = WEAPONTYPE_NONE;
	if (EnemyTarget)
		EnemyTarget->HasTargetLock = false;
	EnemyTarget = NULL;

	if (SoundLocking)
	{
		SoundLocking->Stop();
		SAFE_DELETE(SoundLocking);
	}
}


void CTuber::SetPPEffect(TuberPP Effect, float Lifetime)
{
	PPEffectTime = Lifetime;
	if (ActivePPEffect == Effect) return;
	ActivePPEffect = Effect;

	if (PPEffect)
	{
		WePostProcess::Instance()->RemoveEffect(PPEffect);
		SAFE_DELETE(PPEffect);
	}

	switch(Effect)
	{
	case TuberPPGhost:
		PPEffect = new WePostProcessEffect();
		PPEffect->Initialize("Effects.PostProcess.Ghost2", 1.0f, 1.0f, true);
		PPEffect->Param1 = 0.0f;
		break;

	case TuberPPFadeToMonochrome:
		PPEffect = new WePostProcessEffect();
		PPEffect->Initialize("Effects.PostProcess.Monochrome", 1.0f, 1.0f, true);
		PPEffect->Param1 = 1.0f;
		break;

	case TuberPPMonochrome:
		PPEffect = new WePostProcessEffect();
		PPEffect->Initialize("Effects.PostProcess.Monochrome", 1.0f, 1.0f, true);
		PPEffect->Param1 = 0.0f;
		break;

	case TuberPPFadeToColor:
		PPEffect = new WePostProcessEffect();
		PPEffect->Initialize("Effects.PostProcess.Monochrome", 1.0f, 1.0f, true);
		PPEffect->Param1 = 0.0f;
		break;
	}

	if (PPEffect)
	{
		WePostProcess::Instance()->AddEffect(PPEffect);
		PPEffectTimer = 0.0f;
	}
}


void CTuber::LapFinished()
{
	//PrimaryWeaponAmmo = Description.WeaponUpgradeLevel;
	//SecondaryWeaponAmmo = Description.MineUpgradeLevel;
	//TurboCharges = Description.TurboUpgradeLevel;
	CWeaponDescription* WeaponDesc;
	WeaponDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(PrimaryWeaponID);
	PrimaryWeaponAmmo = (WeaponDesc ? WeaponDesc->PickUpAmmo : 0);
	WeaponDesc = CWeaponDescriptionManager::Instance->GetWeaponDescription(SecondaryWeaponID);
	SecondaryWeaponAmmo = (WeaponDesc ? WeaponDesc->PickUpAmmo : 0);
	TurboCharges = MaxTurboCharges; // (1 + Description.TurboUpgradeLevel) * 100.0f;

	bool FinalLapFinished = !(LapCount < (CGameSettings::ClientInstance->LapCount - 1));

	switch (Position)
	{
	case 1:
		Score += FinalLapFinished ? (500 * CGameSettings::ClientInstance->LapCount) : 250;
		break;
	case 2:
		Score += FinalLapFinished ? (200 * CGameSettings::ClientInstance->LapCount) : 150;
		break;
	case 3:
		Score += FinalLapFinished ? (100 * CGameSettings::ClientInstance->LapCount) : 100;
		break;
	}

	if (FinalLapFinished)
	{
		int ExtraCoins = (int)((float)Score / 10.0f);
		Coins += ExtraCoins;

		if (IsInputPlayer && (ExtraCoins > 0) )
		{
			char zimbabwe[100];
			sprintf(zimbabwe, CLocalizationSettings::Instance()->GetLocalText("EXTRA_COINS"), ExtraCoins);
			CMainGameInterface::Instance->UberLines.PushBack( new CUberText(String(zimbabwe)) );
		}
	}
	else if (IsInputPlayer)
	{
		CMainGameInterface::Instance->UberLines.PushBack(
			new CUberText(CLocalizationSettings::Instance()->GetLocalText("RECHARGED_TURBO")));

		if (CGameSettings::ClientInstance->GetWeapons())
			CMainGameInterface::Instance->UberLines.PushBack(
				new CUberText(CLocalizationSettings::Instance()->GetLocalText("RECHARGED_WEAPS")));
	}
}


void CTuber::Respawn()
{
	HP = MaxHP;
	CurrentSpeed = 0;
	OnTurbo = false;
	Renderable = true;
	Dead = false;
	RespawnCounter = 0.0f;
	RespawnTime = TUBER_RESPAWN_TIME;

	YawManouver = Description.Manouverability;
	PitchManouver = Description.Manouverability * 1.4f;		// 40% bolje gore/dole
	TurboMaxSpeed = Description.MaxSpeed * 1.2f;			// 20% bolje u turbu
	TurboAcceleration = Description.MaxAcceleration * 2.0f;	// 100% bolje ubrzanje
	MaxSpeed  = Description.MaxSpeed;
	Acceleration = Description.MaxAcceleration;

	WePathNode *Node;
	Node = World->FindNextDirectionPathNode(Location);
	while ((Node->NodeType == NODETYPE_TELEPORTING) || (Node->NodeType == NODETYPE_TELEPORTED))
		Node = Node->NextNode;

	SetTransformation(Node->TM);
	_OldLocation = Location;

	PhysicHandle->SetCollision(true);
	if (IsInputPlayer)
		CMainGamePhase::Instance->bInitCamera = true;

	if (CMainGamePhase::Instance->InputPlayer == this) SetPPEffect(TuberPPFadeToColor, 1.0f);

	WeSound *Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.Spawn", true);
	if (Sound)
	{
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		Sound->Play(false);
		Sound->DieOnStop();
	}
}


void CTuber::ClearPickups()
{
	for (PickupActions.First(); !PickupActions.End(); )
	{
		delete PickupActions.GetItem();
		PickupActions.Delete();
	}
}



void CTuber::ResetValues()
{
	YawManouver = Description.Manouverability;
	PitchManouver = Description.Manouverability * 1.4f;				// 40% bolje gore/dole
	TurboMaxSpeed = Description.MaxSpeed * 1.2f;					// 20% bolje u turbu
	TurboAcceleration = Description.MaxAcceleration * 2.0f;			// 100% bolje ubrzanje 
	MaxSpeed  = Description.MaxSpeed;
	Acceleration = Description.MaxAcceleration;
	RearAcceleration = Acceleration * 2.0f;
	TuberName = Description.Name;

	PrimaryWeaponID = CWeaponDescriptionManager::Instance->GetWeaponID(Description.WeaponName);
	SecondaryWeaponID = CWeaponDescriptionManager::Instance->GetWeaponID(Description.MineName);

	PrimaryWeaponAddonID = WEAPONTYPE_NONE;
	SecondaryWeaponAddonID = WEAPONTYPE_NONE;
	PrimaryWeaponAmmo2 = 0;
	SecondaryWeaponAmmo2 = 0;
	ModelName = Description.Model;
	PhysicModelName = Description.PhysicModel;

	PrimaryWeaponFireTime = 0;
	SecondaryWeaponFireTime = 0;

	MaxHP = (float)Description.HP;
	HP = MaxHP;
	CurrentSpeed = 0;
	OnTurbo = false;
	Dead = false;
	RespawnCounter = 0.0f;
	RespawnTime = TUBER_RESPAWN_TIME;
	ClearPickups();

	WrongWay = false;
	RaceFinished = false;
	Win = false;
	//PathNodeCounter = 0;
	MyStartLocation = NULL;
	//NumOfNodes = 0;
	RefPathNode = NULL;
	//RefPathNode->SortedIndex = -1;
	RefDistance = 0.0;

	Score = 0;
	Kills = 0;
	Coins = 0;
	CashCollected = 0;

	BlurFactor = 0.0f;
	FOV = D3DX_PI / 4.0f;
	LastLapTime = 0.0f;
	StartLapTime = 0.0f;
	LapCount = 0;
	ActualLapCount = 0;
	LapTime = 0;
	RaceTime = 0;
	BestLapTime = 0;

	AddonBlurFactor = 0.0f;

	for (int i = 0; i < 2; i++)
        Muzzle[i] = NULL;

	PreviousFlash = TuberFlashNone;
	CurrentFlash = TuberFlashNone;
	FlashTimer = 0.0f;
	FlashFactor = 0.0f;
	CurrentFlashFactor = 0.0f;

	TuberPartTimer = 0.0f;
	HasWeaponLock = false;
	HasTargetLock = false;
	WeaponLockDistance = -1.0f;
	ClosestLockedWeapon = NULL;

	Invulnerable = false;
	Invisible = false;
	StrengthMultiplier = 1.0;

	ScoreTimer = 0.0f;

	KillTime = -1.0f;
	QuickKillCounter = 0;

	LastEnemyHit = NULL;

	ClearEnemyTargetLock();
}




float CTuber::TakeDamage(float damage)
{
	if (Invulnerable) return 0.0f;

	float ReturnValue = (HP < damage) ? HP : damage;

	HP -= damage;
	if(HP <= 0)
	{
		if(CGameServer::Instance->GetStatus() == GS_GAME_PLAYING)
		{
			WeNetworkMessage Msg;
			Msg.Init();
			Msg.WriteMessageType(_DEAD);
			WeServer::Instance()->SendGameCommand(&Msg,this);
			Die();
		}
	}

	return ReturnValue;
}


void CTuber::ProcessInput(CInputMessage* Msg)
{
	WeVector tmpV;
	WeVector  XAV, YAV, ZAV, XV, YV, ZV;
	float Speed;

	GetLocalAxis(XAV, YAV, ZAV);
	D3DXVec3Normalize(&XAV, &XAV);
	D3DXVec3Normalize(&YAV, &YAV);
	D3DXVec3Normalize(&ZAV, &ZAV);

	OnTurbo = false;

	if (Dead)
	{
		Velocity = WeVector(0,0,0);
		AngularVelocity = WeVector(0,0,0);
		Speed = 0.0f;
		return;
	}
	else
	{
		WeVector V;
		PhysicHandle->GetLinearVelocity(V);
		Speed = D3DXVec3Length(&V) * (D3DXVec3Dot(&ZAV, &V) >= 0.0f ? 1.0f : -1.0f);
		if (Speed < -20.0f)
			Speed = -20.0f;
	}

	CurrentSpeed = Speed / CatchUpSpeedFactor;
	Speed = CurrentSpeed;

	//if (RaceFinished)
	//{
	//	if (Speed < 0.0f) 
	//	{
	//		Speed += WeTimer::Instance()->GetDeltaTimeF()*160.0f;
	//		if (Speed > 0.0f) Speed = 0.0f;
	//	}
	//	if (Speed > 0.0f) 
	//	{
	//		Speed -= WeTimer::Instance()->GetDeltaTimeF()*160.0f;
	//		if (Speed < 0.0f) Speed = 0.0f;
	//	}	
	//	AngularVelocity = WeVector(0,0,0);		
	//}
	
	//if (!RaceFinished)
	{
		if (Msg->MoveForward) 
		{
			float ExtraAccel = 0.0f;
			if (Msg->RotateLeft || Msg->RotateRight)
				ExtraAccel = Acceleration * 0.5f;

			if (Msg->Turbo && (TurboCharges > 0 || InfTurbo))
			{
				Speed += (ExtraAccel + TurboAcceleration) * WeTimer::Instance()->GetDeltaTimeF() * 2.0f;
				if (Speed > TurboMaxSpeed)
				{
					Speed = TurboMaxSpeed;
				}
				OnTurbo = true;
			}
			else
			{
				if (Speed < MaxSpeed)
				{
					if (Speed >= (MaxSpeed * 0.9f))
						ExtraAccel = -(Acceleration * 0.95f);

					Speed += (ExtraAccel + Acceleration) * WeTimer::Instance()->GetDeltaTimeF();
					if (Speed >= MaxSpeed)
						Speed = MaxSpeed;
				}
			}
		}


		if (Msg->MoveBackward) 
		{
			if (Speed < 0)
				Speed -= RearAcceleration * WeTimer::Instance()->GetDeltaTimeF();		
			else
				Speed -= 2.0f * RearAcceleration * WeTimer::Instance()->GetDeltaTimeF();

			if (Speed < -MaxSpeed/3.0f)
				Speed = -MaxSpeed/3.0f;
		}


		if ((!Msg->MoveBackward && !Msg->MoveForward) || (Msg->MoveForward && Speed > MaxSpeed && (!Msg->Turbo || TurboCharges <= 0)))
		{
			if (Speed < 0.0f) 
			{
				Speed += Acceleration * WeTimer::Instance()->GetDeltaTimeF() * 1.0f; //3.0f;
				if (Speed > 0.0f)
					Speed = 0.0f;
			}
			if (Speed > 0.0f) 
			{
				Speed -= Acceleration * WeTimer::Instance()->GetDeltaTimeF() * 1.0f; //3.0f;
				if (Speed < 0.0f)
					Speed = 0.0f;
			}
		}
		

		//tmpV = WeVector(0,0,0);		
		//AngularVelocity = WeVector(0,0,0);	
		//if (Msg->RotateLeft) 
		//{
		//	AngularVelocity = -YAV*3.14f/8.0f;
		//}
		//if (Msg->RotateRight) 
		//{
		//	AngularVelocity = YAV*3.14f/8.0f;
		//}

		if (Msg->RotateLeft) 
		{
			AngularVelocity += ZAV * D3DX_PI / 2.0f;
		}

		if (Msg->RotateRight) 
		{
			AngularVelocity += -ZAV * D3DX_PI / 2.0f;
		}

		//if (Msg->RotateLeft) 
		//{
		//	AngularVelocity -= YAV*3.14f/16.0f;
		//}
		//if (Msg->RotateRight) 
		//{
		//	AngularVelocity += YAV*3.14f/16.0f;
		//}

		//static float rmx = 0.0f;
		//static float rmy = 0.0f;
		float ux, uy;
		//rmx += Msg->rx;
		//rmy += Msg->ry;

		//if (rmx > 300)	rmx = 300;
		//if (rmx < -300) rmx = -300;
		//if (rmy > 300) rmy = 300;
		//if (rmy < -300) rmy = -300;

		//if (rmx < 5 && rmx > -5)	rmx = 0;
		//if (rmy < 5 && rmy > -5)	rmy = 0;

		//ux = rmx / 300.0f;
		////ux *= ux;
		//uy = rmy / 300.0f;
		////uy *= uy;


		//if (rmx > 0)
		//{
		//	AngularVelocity += YAV*3.14f*YawManouver/3.0f*ux;
		//} else if( rmx < 0)
		//{
		//	AngularVelocity += YAV*3.14f*YawManouver/3.0f*ux;
		//}

		//if (rmy > 0)
		//{
		//	AngularVelocity += XAV*3.14f/3.0*PitchManouver*uy;
		//} else if( rmy < 0)
		//{
		//	AngularVelocity += XAV*3.14f/3.0*PitchManouver*uy;
		//}

		ux = Msg->rx / MAX_RX;
		uy = Msg->ry / MAX_RY;

		int XMotion = (int)(MAX_RX * COptionsSettings::Instance->KeyboardSensitivity);
		if (Msg->LookRight)
			ux += XMotion;
		else if (Msg->LookLeft)
			ux -= XMotion;

		int YMotion = (int)(MAX_RY * COptionsSettings::Instance->KeyboardSensitivity);
		if (Msg->LookDown)
			uy += YMotion;
		else if (Msg->LookUp)
			uy -= YMotion;

		if (ux > 1.0f) ux = 1.0f;
		if (ux < -1.0f) ux = -1.0f;
		if (uy > 1.0f) uy = 1.0f;
		if (uy < -1.0f) uy = -1.0f;

		float RotationToAngular = GetConfigFloat("tuber.Movement.RotationToAngular", 1.0f);
		AngularVelocity -= ZAV * ux * RotationToAngular;

		//ux = .1f;
		//uy = .1f;


	/*	WeRaycastHit HitLeft, HitRight, HitUp, HitDown;
		bool bHitLeft, bHitRight, bHitUp, bHitDown;
		float MaxHitDistance = 50.0f;
		bHitLeft = WePhysics::Instance()->RaycastClosest(Location, -XAV, WE_SHAPE_TYPE_STATIC, HitLeft, 7, MaxHitDistance);
		bHitRight = WePhysics::Instance()->RaycastClosest(Location, XAV, WE_SHAPE_TYPE_STATIC, HitRight, 7, MaxHitDistance);
		bHitUp = WePhysics::Instance()->RaycastClosest(Location, YAV, WE_SHAPE_TYPE_STATIC, HitUp, 7, MaxHitDistance);
		bHitDown = WePhysics::Instance()->RaycastClosest(Location, -YAV, WE_SHAPE_TYPE_STATIC, HitDown, 7, MaxHitDistance);

		float Factor = 1.0f;
		if (bHitLeft && bHitRight)
		{
			if (ux < 0.0f)
				Factor = HitLeft.Distance / ((HitLeft.Distance + HitRight.Distance) / 2.0f);
			else
				Factor = HitRight.Distance / ((HitLeft.Distance + HitRight.Distance) / 2.0f);

			if (Factor < 1.0f)
				ux *= Factor;
		}
		if (bHitDown && bHitUp)
		{
			if (uy < 0.0f)
				Factor = HitUp.Distance / ((HitDown.Distance + HitUp.Distance) / 2.0f);
			else
				Factor = HitDown.Distance / ((HitDown.Distance + HitUp.Distance) / 2.0f);

			if (Factor < 1.0f)
				uy *= Factor;
		}*/



		if (ux != 0) // Msg->rx > 0)
		{
			AngularVelocity += YAV * D3DX_PI * YawManouver * ux;
		}
		//else if ( Msg->rx < 0)
		//{
		//	AngularVelocity += YAV*3.14f*YawManouver*ux;
		//}

		if (uy != 0) // Msg->ry > 0)
		{
			AngularVelocity += XAV * D3DX_PI * PitchManouver * uy;
		}
		//else if( Msg->ry < 0)
		//{
		//	AngularVelocity += XAV*3.14f*PitchManouver*uy;
		//}
	}

	Velocity += ZAV * Speed; //Speed; // + tmpV;
	Direction = ZAV; //WeVector(0.0, -1.0f, 1.0f);// ZAV;

	//static bool prev = false;
	//if (AddonVelocity != WeVector(0.0f, 0.0f, 0.0f));
}



void CTuber::Server_MakeSpawnMessage(WeNetworkMessage *Msg)
{
	WeNetworkActor::Server_MakeSpawnMessage(Msg);
	Description.Serialize(*Msg);
	// Prolazimo kroz listu GamePlayerConfig-a i trazimo odgovarajuci ID koji zatim upisujemo u poruku
	// da bi smo na klijentskoj strani znali za kog CProfil-a treba da vezemo ovog tuber-a
	//MessageBox(0, "NOVI POZIV", 0, 0);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User)
		{
			if (CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber == this)
			{
				Msg->WriteInt(i);
				break;
			}
		}
		//char tmp[100];		
		//sprintf(tmp, "Server_MakeSpawnMessage %d", CGameSettings::ServerInstance->tmpPlayers[i].ID);
		//MessageBox(0, tmp, 0, 0);
		//if(CGameSettings::ServerInstance->tmpPlayers[i].User==OwnerProfile)
		//{
		//}
	}
}

void CTuber::Client_HandleSpawnMessage(WeNetworkMessage *Msg, bool IsInputPlayer)
{
	WeNetworkActor::Client_HandleSpawnMessage(Msg, IsInputPlayer);					
	Description.Deserialize(*Msg);

	// U ovom delu poruke upisan je ID profila sa kojim treba dapovezemo ovog tuber-a
	int id = Msg->ReadInt();

	CGameSettings::ClientInstance->tmpPlayers[id].User->Tuber= this;
	OwnerProfile = CGameSettings::ClientInstance->tmpPlayers[id].User;
	ResetValues();

	if (IsInputPlayer)
	{
		CMainGamePhase::Instance->InputPlayer = this;
		SetPPEffect(TuberPPFadeToColor, 1.0f);

		CMainGamePhase::Instance->bInitCamera = true;
		WeRenderDevice::Instance()->SetRendering(true);

		// no sound on first spawn
		//WeSound *Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.Spawn", true);
		//if (Sound)
		//{
		//	Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
		//	Sound->Play(false);
		//	Sound->DieOnStop();
		//}
	}

	Initialize();
	Velocity = WeVector(0,0,0);
	AngularVelocity = WeVector(0, 0, 0);

	for(CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
	{
		if(CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Location == Location)
			MyStartLocation = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem();
	}
}


void CTuber::Client_MakeDeltaCommand(WeNetworkMessage *Msg)
{
	//Msg->WriteBool(InputMessage.MoveForward);
	//Msg->WriteBool(InputMessage.MoveBackward);
	//Msg->WriteBool(InputMessage.RotateLeft);
	//Msg->WriteBool(InputMessage.RotateRight);	
	//Msg->WriteInt(InputMessage.rx);
	//Msg->WriteInt(InputMessage.ry);

	
	Msg->WriteVector(Velocity);
	Msg->WriteVector(AngularVelocity);
	if((InputMessage.FireMine || InputMessage.FireWeapon) && !Dead && !RaceFinished )
	{
		int FireMsg = 0;		
		if (InputMessage.FireWeapon) FireMsg += 1;
		if (InputMessage.FireMine) FireMsg += 2;
		Msg->WriteInt(FireMsg);

		//FireWeapon(FireMsg);

		//if (WeServer::Instance()->GetStatus() != WESERVER_HOSTING)
		//{
			//CWeaponFabric::Instance->Server_MakeWeapon(this, CGame::Instance->InputMessage.FireWeapon);
		//}
		//Msg->WriteMatrix( TM );
	}
	else Msg->WriteInt(0);
}


void CTuber::Server_HandleDeltaCommand(WeNetworkMessage *Msg)
{
	//InputMessage.MoveForward = Msg->ReadBool();
	//InputMessage.MoveBackward = Msg->ReadBool();
	//InputMessage.RotateLeft = Msg->ReadBool();
	//InputMessage.RotateRight = Msg->ReadBool();
	//InputMessage.rx = Msg->ReadInt();
	//InputMessage.ry = Msg->ReadInt();
	//InputMessage.FireWeapon = Msg->ReadBool();


	//CInputMessage msg;
	//msg.MoveForward = Msg->ReadBool();
	//msg.MoveBackward = Msg->ReadBool();
	//msg.rx = Msg->ReadChar();
	//msg.ry = Msg->ReadChar();


	//CInputMessage im;
	//im.MoveForward =  Msg->ReadBool();
	//im.MoveBackward = Msg->ReadBool();
	//im.RotateLeft =  Msg->ReadBool();
	//im.RotateRight =  Msg->ReadBool();
	//im.rx =  Msg->ReadChar();
	//im.ry =  Msg->ReadChar();
	//im.FireWeapon = Msg->ReadBool();

	//ProcessInput(&im);

	WeVector t1,t2;
	Msg->ReadVector(t1);
	//Velocity = t1;
	Msg->ReadVector(t2);
	//AngularVelocity = t2;

	int b;
	//if (InputMessage.FireWeapon)

	b = Msg->ReadInt(); 
	if(b != 0)
		FireWeapon(b);


	//Msg->WriteMessageType(1);
	//Msg->WriteBool(InputMessage.MoveForward);
	//Msg->WriteBool(InputMessage.MoveBackward);
	//Msg->WriteBool(InputMessage.RotateLeft);
	//Msg->WriteBool(InputMessage.RotateRight);	
	//Msg->WriteInt(InputMessage.rx);
	//Msg->WriteInt(InputMessage.ry);
	//Msg->WriteBool(InputMessage.FireMine);
	////Msg->WriteVector(t1);
	////Msg->WriteVector(t2);
	//WeServer::Instance()->SendGameCommand(Msg, this);
}

void CTuber::FireWeapon(int ID)
{
	//CWeaponFabric::Instance->Client_MakeWeapon(this,PrimaryWeaponID, TM);

	if (StrengthMultiplier == 4.0f)
	{
		if (!SoundQuadFire)
			SoundQuadFire = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.QuadDamageFire");
		if (SoundQuadFire)
		{
			SoundQuadFire->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
			SoundQuadFire->Play(false);
		}
	}

	WEAPONTYPE NewWeapon = WEAPONTYPE_NONE;
	if (ID < 3)
		NewWeapon = CWeaponFabric::Instance->Server_MakeWeapon(this, (ID == 1) );
	else
	{
		FireWeapon(1);
		FireWeapon(2);
		return;
	}

	if( NewWeapon != WEAPONTYPE_NONE )
	{
		WeNetworkMessage NewMsg;
		NewMsg.Init();
		NewMsg.WriteMessageType(_NEW_WEAPON);
		NewMsg.WriteInt((int)NewWeapon);
		NewMsg.WriteMatrix(TM);
		WeServer::Instance()->SendGameCommand(&NewMsg, this);
	}
}


void CTuber::Die()
{
	if (Dead) return;

	Dead = true;
	Renderable = false;
	Acceleration = 0;
	AngularVelocity = WeVector(0.0f, 0.0f, 0.0f);
	PhysicHandle->SetCollision(false);

	ClearPickups();

	CTuberPartFabric::Instance()->CreateTuberDestroyParts(this);
	//// must be after the parts creation
	CurrentSpeed = 0;

	new CExplosionLocalized(Location, this, 0.0f, this, LocalizedExplosionTuber, WEAPONTYPE_NONE);

	if (this == CMainGamePhase::Instance->InputPlayer)
	{
		CMainGamePhase::Instance->Camera.CurrentState = GameCameraStateShaking;
		CMainGamePhase::Instance->Camera.StateDefaultingTime = 1.0f;

		SetPPEffect(TuberPPFadeToMonochrome, (TUBER_RESPAWN_TIME * 0.7f));
	}

	if (!LastAttacker) return;	//for suicide only! there should always be last attacker!

	if ( (this != LastAttacker) &&
		((LastAttacker->KillTime < 0.0f) || (LastAttacker->RaceTime - LastAttacker->KillTime <= QUICK_KILL_SPREAD))
		)
		LastAttacker->QuickKillCounter++;
	else
	{
		LastAttacker->KillTime = -1.0f;
		LastAttacker->QuickKillCounter = 1;
	}
	LastAttacker->KillTime = LastAttacker->RaceTime;

	DeathTextOutput();
}


void CTuber::DeathTextOutput()
{
	char uber[100];
	CUberText *Writing;
	String GetText = "";

	switch (LastDamageWeaponID)
	{
	case WEAPONTYPE_STANDARDLASER:
		GetText = "BOLT";
		break;
	case WEAPONTYPE_DOUBLELASER:
		GetText = "DOUBLE_BOLT";
		break;
	case WEAPONTYPE_QUADLASER:
		GetText = "QUAD_BOLT";
		break;
	case WEAPONTYPE_QUANTUMLASER:
		GetText = "QUANTUM_BOLT";
		break;
	case WEAPONTYPE_STANDARDROCKET:
		GetText = "STANDARD_ROCKET";
		break;
	case WEAPONTYPE_MINIROCKETS:
		GetText = "MINI_ROCKET";
		break;
	case WEAPONTYPE_NUKEROCKET:
		GetText = "NUKE_ROCKET";
		break;
	case WEAPONTYPE_MINIGUN:
		GetText = "MINI_GUN";
		break;
	case WEAPONTYPE_SLOWINGROCKET:
		GetText = "SLOWING_ROCKET";
		break;
	case WEAPONTYPE_STANDARDMINE:
		GetText = "STANDARD_MINE";
		break;
	case WEAPONTYPE_ELECTRICMINE:
		GetText = "ELECTRIC_MINE";
		break;
	case WEAPONTYPE_HOMINGMINE:
		GetText = "HOMING_MINE";
		break;
	case WEAPONTYPE_SUPERNUKEMINE:
		GetText = "NUKE_MINE";
		break;
	case WEAPONTYPE_INVISIBLEMINE:
		GetText = "INVISIBLE_MINE";
		break;
	case WEAPONTYPE_MOVINGMINE:
		GetText = "MOVING_MINE";
		break;
	case WEAPONTYPE_PROXIMITYMINE:
		GetText = "PROXIMITY_MINE";
		break;
	}

	if (LastAttacker != this)
		sprintf(uber, "DEATH_%s_%d", GetText.GetString(), ((rand() % 10) + 1));
	else
		sprintf(uber, "SUICIDE_%s_%d", GetText.GetString(), ((rand() % 3) + 1));

	String UberOutput = CLocalizationSettings::Instance()->GetLocalText(String(uber));

	char type = UberOutput.GetString()[0];
	UberOutput.RemoveChar(0);

	switch (type)
	{
	case '1':
		sprintf(uber, UberOutput, LastAttacker->OwnerProfile->Name.GetString(), OwnerProfile->Name.GetString());
		break;
	case '2':
		sprintf(uber, UberOutput, OwnerProfile->Name.GetString(), LastAttacker->OwnerProfile->Name.GetString());
		break;
	case '3':
		sprintf(uber, UberOutput, OwnerProfile->Name.GetString());
		break;
	case '4':
		sprintf(uber, UberOutput, LastAttacker->OwnerProfile->Name.GetString());
		break;
	}

	Writing = new CUberText(String(uber));
	CMainGameInterface::Instance->UberLines.PushBack(Writing);

	if (LastAttacker == this) QuickKillCounter = 0;

	if ((LastAttacker->IsInputPlayer) &&
		(LastAttacker->QuickKillCounter > 1))
	{
		WeColor QuickKillColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
		int KillTextNo = LastAttacker->QuickKillCounter > 20 ? 20 : LastAttacker->QuickKillCounter;
		sprintf(uber, "QUICK_KILL_%d", KillTextNo);
		UberOutput = CLocalizationSettings::Instance()->GetLocalText(String(uber));

		if (KillTextNo < 10)
			QuickKillColor = WeColor(0.0f, 1.0f, 1.0f, 1.0f);
		else if (KillTextNo < 15)
			QuickKillColor = WeColor(1.0f, 1.0f, 0.0f, 1.0f);
		else
			QuickKillColor = WeColor(1.0f, 0.0f, 0.0f, 1.0f);

		static WeFont *QuickKillFont = WeCanvas::Instance()->MakeFont("Xirod", (int)(30.0f * (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f));
		Writing = new CUberText(UberOutput, 0.5f - CUberText::MeasureTextWidth(UberOutput, QuickKillFont) / 2.0f, 0.15f);
		Writing->SetFont(QuickKillFont);
		Writing->SetTextColor(QuickKillColor);

		for (CMainGameInterface::Instance->UberLines.First(); !CMainGameInterface::Instance->UberLines.End(); CMainGameInterface::Instance->UberLines.Next())
		{
 			if (CMainGameInterface::Instance->UberLines.GetItem() == LastAttacker->QuickKillText)
			{
				CMainGameInterface::Instance->UberLines.GetItem()->SetTimeShow(0.0f);
				CMainGameInterface::Instance->UberLines.GetItem()->SetTimeStay(0.0f);
				CMainGameInterface::Instance->UberLines.GetItem()->SetTimeHide(0.5f);
				break;
			}
		}

		LastAttacker->QuickKillText = Writing;
		CMainGameInterface::Instance->UberLines.PushBack(Writing);

		char Jean_Luc_Picard[100];
		sprintf(Jean_Luc_Picard, "Sounds.Voice.MultiKill%d", KillTextNo);
		SAFE_DELETE(LastAttacker->SoundMultiKill);
		LastAttacker->SoundMultiKill = WeAudioDevice::Instance()->LoadSoundAuto(String(Jean_Luc_Picard), false);
		if (LastAttacker->SoundMultiKill)
		{
			LastAttacker->SoundMultiKill->Play(false);
		}
	}
}


void CTuber::Client_HandleGameMessage(WeNetworkMessageType type, WeNetworkMessage *Msg)
{	
	WeVector t1,t2;
	WeMatrix M;

	switch(type)
	{
		case 1:
			InputMessage.MoveForward = Msg->ReadBool();
			InputMessage.MoveBackward = Msg->ReadBool();
			InputMessage.RotateLeft = Msg->ReadBool();
			InputMessage.RotateRight = Msg->ReadBool();
			InputMessage.rx = Msg->ReadInt();
			InputMessage.ry = Msg->ReadInt();
			InputMessage.FireWeapon = Msg->ReadBool();
			//Msg->ReadVector(t1);
			//Velocity += t1;
			//Msg->ReadVector(t2);
			//AngularVelocity += t2;
			break;

		case _NEW_WEAPON:
			//PrimaryWeaponAmmo = 100;
			PrimaryWeaponID = (WEAPONTYPE)Msg->ReadInt();
			Msg->ReadMatrix(M);
			CWeaponFabric::Instance->Client_MakeWeapon(this,PrimaryWeaponID,M);
			break;

		case _DEAD:
			Die();
			break;
	}
}


void CTuber::Server_Replicate(WeNetworkActor *Avatar, WeNetworkMessage *Msg)
{
	WeNetworkActor::Server_Replicate(Avatar, Msg);
	//Msg->WriteMatrix(TM);
	Msg->WriteInt(PrimaryWeaponID);
	Msg->WriteInt(SecondaryWeaponID);
	Msg->WriteInt(PrimaryWeaponAddonID);
	Msg->WriteInt(SecondaryWeaponAddonID);
	Msg->WriteInt(PrimaryWeaponAmmo);
	Msg->WriteInt(PrimaryWeaponAmmo2);
	Msg->WriteInt(SecondaryWeaponAmmo);
	Msg->WriteInt(SecondaryWeaponAmmo2);
	Msg->WriteFloat(LapTime);


	//Msg->WriteInt(PathNodeCounter);
	Msg->WriteInt(Position);
	Msg->WriteFloat(HP);
	//Msg->WriteInt(TurboCharges);
	Msg->WriteFloat(BestLapTime);
}


void CTuber::Client_HandleReplication(WeNetworkMessage *Msg)
{	
	WeNetworkActor::Client_HandleReplication(Msg);
	//TM = Msg->ReadMatrix(TM);
	PrimaryWeaponID = (WEAPONTYPE) Msg->ReadInt();
	SecondaryWeaponID = (WEAPONTYPE)Msg->ReadInt();
	PrimaryWeaponAddonID = (WEAPONTYPE)Msg->ReadInt();
	SecondaryWeaponAddonID = (WEAPONTYPE)Msg->ReadInt();
	PrimaryWeaponAmmo = Msg->ReadInt();
	PrimaryWeaponAmmo2 = Msg->ReadInt();
	SecondaryWeaponAmmo = Msg->ReadInt();
	SecondaryWeaponAmmo2 = Msg->ReadInt();
	LapTime = Msg->ReadFloat();

	//PathNodeCounter = Msg->ReadInt();
	Position = Msg->ReadInt();
	HP = Msg->ReadFloat();
	//TurboCharges = Msg->ReadInt();
	BestLapTime = Msg->ReadFloat();

}


bool CTuber::IsFriendWith(CTuber* Candidate)
{
	return (Candidate->OwnerProfile->TeamNumber == OwnerProfile->TeamNumber);
}

bool CTuber::ValidateTarget(CTuber* Candidate)
{
	if (!Candidate) return false;
	if (Candidate == this) return false;
	if (Candidate->Invisible) return false;
	if (Candidate->Dead) return false;
	if (Candidate->RaceFinished) return false;
	if (IsFriendWith(Candidate)) return false;

	return true;
}


bool CTuber::IsPushable()
{
	if (Dead) return false;
	if (RaceFinished) return false;
	if (Invulnerable) return false;

	return true;
}

void CTuber::DisplayLapText(float LastLapTime)
{
	char uber[100];
	CUberText *Writing = NULL;
	WeColor TextColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);

	int LapsLeft = GetLapsRemaining();
	bool LapNumbering = (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_SINGLERACE ||
						CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_SINGLERACE ||
						CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT ||
						CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_TOURNAMENT ||
						CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_INSTANTRACE);

	if (IsInputPlayer)
	{
		//=== lap time / spread time writing ===//
		static WeFont *WritingFontTime = WeCanvas::Instance()->MakeFont("Xirod", (int)(20.0f * (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f));
		String LapTimeString = CLocalizationSettings::Instance()->GetLocalText("LAP_TIME");
		if (Position == 1)
		{
			sprintf(uber, LapTimeString, CMainGameInterface::Instance->MakeTimeString(LastLapTime));
			TextColor = WeColor(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			sprintf(uber, LapTimeString, CMainGameInterface::Instance->MakeTimeString(CPlayGamePhase::Instance->RaceTime - CMainGamePhase::Instance->Map.FastestTuberTime[LapCount]));
			TextColor = WeColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
		Writing = new CUberText(
								String(uber), 
								0.5f - (CUberText::MeasureTextWidth(String(uber), WritingFontTime) / 2.0f), 0.25f, 
								UberTextTypeAnimFade, UberTextTypeAnimFade);
		Writing->SetFont(WritingFontTime);
		Writing->SetTimeShow(1.0f); Writing->SetTimeStay(1.0f); Writing->SetTimeHide(1.0f);
		Writing->SetTextColor(TextColor);
		CMainGameInterface::Instance->UberLines.PushBack(Writing);
		//=====================================//

		//=== lap number info ===//
		if (LapNumbering)
		{
			TextColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
			static WeFont *WritingFontStatus = WeCanvas::Instance()->MakeFont("Xirod", (int)(30.0f * (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f));
			if (LapsLeft > 1)
			{
				sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("LAPS_REMAINING"), LapsLeft);
				char catherine_janeway[100];
				sprintf(catherine_janeway, "Sounds.Voice.Laps%d", LapsLeft);
				SoundLapCount = WeAudioDevice::Instance()->LoadSoundAuto(String(catherine_janeway), false);
				SoundLapCount->Play(false);
			}
			else if (LapsLeft == 1)
			{
				sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("FINAL_LAP"), LapsLeft);
				TextColor = WeColor(1.0f, 0.0f, 0.0f, 1.0f);
				SoundLapCount = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.Lap1", false);
				SoundLapCount->Play(false);
			}
			else
				sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("RACE_FINISHED"), LapsLeft);
			Writing = new CUberText(
									String(uber), 
									0.5f - (CUberText::MeasureTextWidth(String(uber), WritingFontStatus) / 2.0f), 0.2f, 
									UberTextTypeAnimFade, UberTextTypeAnimFade);
			Writing->SetFont(WritingFontStatus);
			Writing->SetTimeShow(1.0f); Writing->SetTimeStay(1.0f); Writing->SetTimeHide(1.0f);
			CMainGameInterface::Instance->UberLines.PushBack(Writing);
		}
	}
	//=======================//

	//=== first player entered final lap ===//
	if ( LapNumbering && (Position == 1) && (LapsLeft == 1) )
	{
		sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("ENTERED_FINAL_LAP"), OwnerProfile->Name);
		Writing = new CUberText(String(uber));
		CMainGameInterface::Instance->UberLines.PushBack(Writing);
	}
	//======================================//
}

int CTuber::DistanceInPathNodes(CTuber* Target)
{
	return (RefPathNode->SortedIndex + World->TotalPathNodes - Target->RefPathNode->SortedIndex) % World->TotalPathNodes;
}


void CTuber::TickFHDRI™()
{
	static float LastFHDRI = 0.0f;

	if (!IsInputPlayer) return;
	if (CurrentFlash != TuberFlashNone) return;
	if (!RefPathNode) return;

	if (abs(RefPathNode->FHDRI - LastFHDRI) > 0.001f)
	{
		CMap *Map = &CMainGamePhase::Instance->Map;
		FlashFactor = RefPathNode->FHDRI - LastFHDRI;
		if (FlashFactor > 0.0f)
		{
			FlashFactor *= (Map->MapHeader.FHDRI[2] - Map->MapHeader.FHDRI[1]);
			CurrentFlash = TuberFlashFHDRI;
		}
		else
		{
			FlashFactor *= (Map->MapHeader.FHDRI[1] - Map->MapHeader.FHDRI[0]);
			CurrentFlash = TuberFlashFHDRI2;
		}
	}

	LastFHDRI = RefPathNode->FHDRI;

	//if ( ((RefPathNode->FHDRI > 0.0f) && (!WrongWay)) ||
	//	 ((RefPathNode->FHDRI < 0.0f) && (WrongWay)) )
	//{
	//	FlashFactor = abs(RefPathNode->FHDRI);
	//	CurrentFlash = TuberFlashFHDRI;
	//}
}

void CTuber::TickLight()
{
	WeVector x, y, z;
	WeVector AX, AY, AZ;

	GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AX, &AX);
	D3DXVec3Normalize(&AY, &AY);
	D3DXVec3Normalize(&AZ, &AZ);
	float HeadlightDistance = 40.0f;
	WeRaycastHit Hit;
	if (WePhysics::Instance()->RaycastClosest(Location, AZ, WE_SHAPE_TYPE_STATIC, Hit, 7, HeadlightDistance))
		HeadlightDistance = (Hit.Distance - 2.0f);
	Headlight->Location = Location + (AZ * HeadlightDistance);

	Headlight->IsDynamic = false;
	if (RefPathNode)
		Headlight->IsDynamic = (RefPathNode->HeadLight == HEADLINE_ON);
	Headlight->IsDynamic = Headlight->IsDynamic && (!Dead) && (!RaceFinished);

	bool HeadlightSound = false;

	if (Headlight->IsDynamic)
	{
		Light->IsDynamic = false;
		if (IsInputPlayer && (Headlight->Range == 0.0f) )
			CMainGameInterface::Instance->PlayHeadlightSound();
	
		if (Headlight->Range < HEADLIGHT_RANGE)
		{
			Headlight->Range += (HEADLIGHT_RANGE * WeTimer::Instance()->GetDeltaTimeF() * 0.5f);
			if (Headlight->Range > HEADLIGHT_RANGE)
				Headlight->Range = HEADLIGHT_RANGE;
		}

		return;
	}
	else
	{
		if (IsInputPlayer && (Headlight->Range == HEADLIGHT_RANGE) )
			CMainGameInterface::Instance->PlayHeadlightSound();

		if (Headlight->Range > 0.0f)
		{
			Headlight->Range -= (HEADLIGHT_RANGE * WeTimer::Instance()->GetDeltaTimeF());
			if (Headlight->Range < 0.0f)
				Headlight->Range = 0.0f;
		}
		if (Headlight->Range > 0.0f)
		{
			Headlight->IsDynamic = (!Dead) && (!RaceFinished);
			return;
		}
	}

	Light->IsDynamic = IsInputPlayer && (!Dead) && (!RaceFinished);
	if (Light->IsDynamic)
	{
		Light->Location = Location - (AZ * 10.0f) + (AY * 5.0f);
		//Light->GetLocalAxis(x,y,z);
		//Light->Location = Location - z*2;
		//Light->Location.y += 5;

		//Light->TM = TM;
		//Light->TM._21 = TM._31;
		//Light->TM._22 = TM._32;
		//Light->TM._23 = TM._33;
		//Light->TM._31 = -TM._21;
		//Light->TM._32 = -TM._22;
		//Light->TM._33 = -TM._23;
		//Light->TM._41 = Light->Location.x - Light->TM._31*3.0f;
		//Light->TM._42 = Light->Location.y - Light->TM._32*3.0f;
		//Light->TM._43 = Light->Location.z - Light->TM._33*3.0f;	
	}
}


void CTuber::TickCatchUp()
{
	const float CATCH_UP_START_TIME = 4.0f;
	const float CATCH_UP_MIN_DISTANCE = 20.0f;
	const float CATCH_UP_MAX_DISTANCE = 200.0f;

	CatchUpSpeedFactor = 1.0f;

	if (!CGameSettings::ClientInstance->GetCatchUp()) return;
	if (CGameSettings::ClientInstance->GetPlayersCount() == 1) return;

	if ( (Position > 3) || (CGameSettings::ClientInstance->GetPlayersCount() == Position) )
	{
		CatchUpTimer = 0.0f;
		return;
	}

	CatchUpTimer += WeTimer::Instance()->GetDeltaTimeF();

	if (CatchUpTimer >= CATCH_UP_START_TIME)
	{
		CTuber *SecondDude = NULL;
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			if (!CGameSettings::ClientInstance->tmpPlayers[i].User) continue;
			if (!CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber) continue;
			if (CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber->Position != (Position + 1)) continue;

			SecondDude = CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber;
			break;
		}

		if (!SecondDude) return;

		WeVector Distance = (SecondDude->Location - Location);
		float Dist = D3DXVec3Length(&Distance);
		if (Dist < CATCH_UP_MIN_DISTANCE) return;

		switch (Position)
		{
		case 1:
			CatchUpSpeedFactor = 0.95f;
			break;
		case 2:
			CatchUpSpeedFactor = 0.96f;
			break;
		case 3:
			CatchUpSpeedFactor = 0.97f;
			break;
		}
		//float DistFactor = (Dist / (CATCH_UP_MAX_DISTANCE - CATCH_UP_MIN_DISTANCE));
		//if (DistFactor > 1.0f) DistFactor = 1.0f;
		//Velocity *= (CatchUpSpeedFactor + ((1.0f - CatchUpSpeedFactor) * DistFactor));
	}
}


void CTuber::InitializePropulsion()
{
	int TotalEngines = 0;

	if (Description.Name == (String)"Mech")
	{
		TotalEngines = 4;
		RelativePropulsion[0] = WeVector(-1.4f, -0.1f, -1.5f);	// left
		RelativePropulsion[1] = WeVector(1.4f, -0.1f, -1.5f);	// right
		RelativePropulsion[2] = WeVector(-0.9f, 0.1f, -1.2f);	// inner-left
		RelativePropulsion[3] = WeVector(0.9f, 0.1f, -1.2f);	// inner-right
		//RelativePropulsion[4] = WeVector(0.0f, -0.3f, -0.6f);	// center
	}
	else if (Description.Name == (String)"Mosquito")
	{
		TotalEngines = 3;
		RelativePropulsion[0] = WeVector(-0.362f, 0.477f, -0.989f);
		RelativePropulsion[1] = WeVector(0.382f, 0.477f, -0.989f);
		RelativePropulsion[2] = WeVector(0.0f, 1.108f, -1.304f);
	}
	else if (Description.Name == (String)"UnderDog")
	{
		TotalEngines = 1;
		RelativePropulsion[0] = WeVector(0.0f, 0.087f, -2.283f);
	}
	else if (Description.Name == (String)"DarkLurker")
	{
		TotalEngines = 6;
		RelativePropulsion[0] = WeVector(-1.075f, 0.694f, -1.405f);
		RelativePropulsion[1] = WeVector(1.075f, 0.694f, -1.405f);
		RelativePropulsion[2] = WeVector(-1.773f, -0.136f, -0.577f);
		RelativePropulsion[3] = WeVector(1.773f, -0.136f, -0.577f);
		RelativePropulsion[4] = WeVector(-1.191f, -0.718f, -0.692f);
		RelativePropulsion[5] = WeVector(1.191f, -0.718f, -0.692f);
	}

	for (int i = 0; i < TotalEngines; ++i)
	{
		Propulsion[i] = new WeParticleSystem();
		StaticTrailPS->Clone(Propulsion[i]);
		Propulsion[i]->Initialize();
		//Propulsion[i]->Properties.ColorFrom = Trail->SegmentColor;
		//Propulsion[i]->Properties.ColorTo = Propulsion[i]->Properties.ColorFrom;
		World->AddActor(Propulsion[i]);
	}
}


void CTuber::TickPropulsion(bool FPSCam)
{
	WeMatrix MyMatrix, RelMatrix;

	for (int i = 0; i < 10; ++i)
	{
		if (!Propulsion[i]) continue;

		Propulsion[i]->Renderable = (IsInputPlayer) && (!Dead) && (!Invisible) && (!FPSCam) && (!RaceFinished);

		D3DXMatrixTranslation(&RelMatrix, RelativePropulsion[i].x, RelativePropulsion[i].y, RelativePropulsion[i].z);
		D3DXMatrixMultiply(&MyMatrix, &RelMatrix, &TM);
		Propulsion[i]->Location = WeVector(MyMatrix._41, MyMatrix._42, MyMatrix._43);

		float TuberSpeed = (CurrentSpeed < 0.0f) ? 0.0f : CurrentSpeed;

		float Factor = (TuberSpeed / (MaxSpeed * 0.1f));
		if (Factor > 1.0f) Factor = 1.0f;
		Propulsion[i]->Properties.EmitRate = (int)(10.0f + (40.0f * Factor));
		Propulsion[i]->Properties.Gravity = -Direction * 0.3f * Factor;
	}
}



void CTuber::TickAddons(bool FPSCam)
{
	if (HeadlightModel)
	{
		HeadlightModel->TM = TM;
		HeadlightModel->Location = Location;
		HeadlightModel->Renderable = (Headlight->IsDynamic) && (!FPSCam);
		HeadlightModel->MaterialColor.a = (Headlight->Range / HEADLIGHT_RANGE);
	}

	if (LightProjector)
	{
		WeVector x, y, z;

		//ReceiveProjectors = false;
		GetLocalAxis(x, y, z);
		LightProjector->Location = Location;				
		LightProjector->Location += y * 5.0f + z * 2.0f;

		bool LPState = (!Dead) && (!Invisible) && (!RaceFinished);
		LightProjector->IsDynamic = false;
		LightProjector->Type = LPState ? LIGHT_PROJECTORONLY : LIGHT_POINT;

		LightProjector->TM = TM;
		LightProjector->TM._21 = TM._31;
		LightProjector->TM._22 = TM._32;
		LightProjector->TM._23 = TM._33;
		LightProjector->TM._31 = -TM._21;
		LightProjector->TM._32 = -TM._22;
		LightProjector->TM._33 = -TM._23;
		LightProjector->TM._41 = LightProjector->Location.x - LightProjector->TM._31 * 3.0f;
		LightProjector->TM._42 = LightProjector->Location.y - LightProjector->TM._32 * 3.0f;
		LightProjector->TM._43 = LightProjector->Location.z - LightProjector->TM._33 * 3.0f;	
	}

	QuadDamage->Renderable = ( (StrengthMultiplier == 4.0f) && (!FPSCam) && (!RaceFinished) );
	if (QuadDamage->Renderable)
	{
		QuadDamage->TM = TM;
		QuadDamage->Location = Location;
	}

	InvulnerabilityModel->Renderable = ( (Invulnerable) && (!FPSCam) && (!RaceFinished) );
	if (InvulnerabilityModel->Renderable)
	{
		InvulnerabilityModel->TM = TM;
		InvulnerabilityModel->Location = Location;
	}

	Trail->Renderable = (!Dead) && (!Invisible) && (!RaceFinished) && (!FPSCam) && (CurrentSpeed > 0) &&
		(!IsInputPlayer || !CMainGamePhase::Instance->Camera.CurrentCamera->NoTrail );
	Trail->TM = TM;
	Trail->Location = Location;

	TurboModel->Renderable = (!Dead) && (!Invisible) && (!RaceFinished) && (!FPSCam);
	TurboModel->TM = TM;
	TurboModel->Location = Location;

	float SpeedFactor;
	SpeedFactor = CurrentSpeed / MaxSpeed;

	// set material alpha for flare 
	Flares->Renderable = (!Dead) && (!Invisible) && (!FPSCam) && (!RaceFinished);
	Flares->UseMaterialParams = true;
	Flares->MaterialColor = WeColor(1.0f, 1.0f, 1.0f, (SpeedFactor < 0.0f ? -SpeedFactor : (SpeedFactor > 1.0f ? 1.0f : SpeedFactor)));
	Flares->TM = TM;
	//WeLoger::DebugLog("speed %3.3f %3.3f\n", CurrentSpeed, abs(CurrentSpeed / MaxSpeed));

	Trails->Renderable = (!Dead) && (!Invisible) && (!FPSCam) && (!RaceFinished);
	Trails->UseMaterialParams = true;
	//Flares->MaterialParam1 = abs(CurrentSpeed / MaxSpeed); // + 0.1f;
	Trails->MaterialColor = Trail->SegmentColor;
	Trails->MaterialColor.a = (SpeedFactor < 0.0f ? 0.0f : (SpeedFactor > 1.0f ? 1.0f : SpeedFactor));
	Trails->TM = TM;

	if (RefractionModel)
	{
		RefractionModel->Renderable = (!Dead) && (!Invisible) && (!FPSCam) && (!RaceFinished);
		RefractionModel->UseMaterialParams = true;
		RefractionModel->MaterialColor.a = (SpeedFactor < 0.0f ? 0.0f : (SpeedFactor > 1.0f ? 1.0f : SpeedFactor));
		RefractionModel->TM = TM;
	}

	InvisibilityModel->Renderable = (!Dead) && (Invisible) && (!RaceFinished) && (!FPSCam);
	InvisibilityModel->TM = TM;
	InvisibilityModel->Location = Location;

	if (CurrentSpeed >= MaxSpeed)
	{
		SpeedyTimer += WeTimer::Instance()->GetDeltaTimeF();
		if (SpeedyTimer > 5.0f)
		{
			SpeedyAlpha += WeTimer::Instance()->GetDeltaTimeF();
			if (SpeedyAlpha > 1.0f)
				SpeedyAlpha = 1.0f;
		}
	}
	else
	{
		SpeedyTimer = 0.0f;
		SpeedyAlpha -= WeTimer::Instance()->GetDeltaTimeF();
		if (SpeedyAlpha < 0.0f)
			SpeedyAlpha = 0.0f;
	}

	Speedy->Renderable = (!RaceFinished);
	Speedy->UseMaterialParams = true;
	Speedy->MaterialColor = WeColor(1.0f, 1.0f, 1.0f, SpeedyAlpha);
	Speedy->TM = TM;
}


void CTuber::MultiplyVelocity(float Factor)
{
	WeVector Temper;
	PhysicHandle->GetLinearVelocity(Temper);
	PhysicHandle->SetLinearVelocity((Temper * Factor));
}

void CTuber::TickTubeLimit()
{
	WePathNode *Node1, *Node2;
	float d1, d2;
	float d12, d, r;
	WeVector RefPos;
	WeVector vDirNorm;
	WeVector tmpV, vDir;

	float MinHitDistance = GetConfigFloat("tuber.Movement.MinHitDistance", 0.0f);

	World->GetRefPathNodes(Location, Node1, Node2, d1, d2, RefPos);

	D3DXVec3Subtract(&vDir, &Location, &RefPos);
	D3DXVec3Normalize(&vDirNorm, &vDir);
	d = D3DXVec3Length(&vDir);

	WeRaycastHit Hit;
	if (WePhysics::Instance()->RaycastClosest(RefPos, vDirNorm, 
		WE_SHAPE_TYPE_STATIC, Hit, 7, d + MinHitDistance) &&
		!Hit.ActorHit->GhostObject)
	{
		float distance = Hit.Distance - d;
		if (distance < 0.0f)
			distance = 0.0f;

		//Velocity -= distance * vDirNorm;

		//Location = RefPos + vDirNorm * distance;

		//Direction = LookTarget->Location - Location;
		//D3DXVec3Normalize(&Direction, &Direction);
	}

	return;

	d12 = d1 - d2;
	if (abs(d12) < 0.001f)
		r = Node1->Radius;
	else
		r = (d1 * Node2->Radius - d2 * Node1->Radius) / d12;

#ifdef  _DEBUG
	char temp[1000];
	sprintf_s(temp, "D %6.3f  R %6.3f  D1 %6.3f  D2 %6.3f", d, r, d1, d2);
	CMainGamePhase::Instance->DebugStr1 = temp;
	CMainGamePhase::Instance->DebugStr1 = "";
#endif

	if (d > r)
	{
		float N;
		N = D3DXVec3Dot(&vDirNorm, &Velocity);
		if (N > 0)
		{
			WeVector tmpV;
			D3DXVec3Scale(&tmpV, &vDirNorm, N);
			//D3DXVec3Subtract(&Velocity, &Velocity, &tmpV);

			WeVector W;
			float V;
			D3DXVec3Subtract(&W, &Node2->Location, &Node1->Location);
			D3DXVec3Normalize(&W, &W);
			V = D3DXVec3Dot(&W, &Velocity);
			//D3DXVec3Scale(&Velocity, &Lz1, V);
		}
	}
}


void CTuber::TickMuteAudio()
{
	const float MUTE_AUDIO_REDUCE_TIME = 0.5f;
	const float MUTE_AUDIO_DURATION = 2.0f;
	const float MUTE_AUDIO_RESUME_TIME = 0.5f;

	if (!IsInputPlayer) return;
	if (!MuteAudioEnable) return;

	if (MuteAudioTimer == 0.0f)
	{
		MuteAudioSavedValue = WeAudioDevice::Instance()->GetSoundVolume();

		WeSound *BigBoom = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Deaf", false);
		BigBoom->Play(false);
		BigBoom->DieOnStop();
	}

	MuteAudioTimer += WeTimer::Instance()->GetDeltaTimeF();

	float NewVolume;
	if (MuteAudioTimer < MUTE_AUDIO_REDUCE_TIME)
		NewVolume = MuteAudioSavedValue - (MuteAudioSavedValue * (MuteAudioTimer / MUTE_AUDIO_REDUCE_TIME));
	else if (MuteAudioTimer < MUTE_AUDIO_DURATION)
		NewVolume = 0.0f;
	else if (MuteAudioTimer < MUTE_AUDIO_RESUME_TIME)
		NewVolume = MuteAudioSavedValue * ((MuteAudioTimer - MUTE_AUDIO_REDUCE_TIME - MUTE_AUDIO_DURATION) / MUTE_AUDIO_RESUME_TIME);
	else
	{
		NewVolume = MuteAudioSavedValue;
		MuteAudioEnable = false;
		MuteAudioTimer = 0.0f;
	}

	if (WeAudioDevice::Instance()->GetSoundVolume() != NewVolume)
		WeAudioDevice::Instance()->SetSoundVolume(NewVolume);
}


int CTuber::GetLapsRemaining()
{
	return (CGameSettings::ClientInstance->LapCount - LapCount);
}


void CTuber::OnWallScratch(WeVector &ScratchLocation)
{
	if (SoundScratch && SoundScratch->IsPlaying()) return;

	SAFE_DELETE(SoundScratch);

	char alphazulu[100];
	sprintf(alphazulu, "Sounds.Tuber.Scratch%d", Rnd(1, 7));
	SoundScratch = WeAudioDevice::Instance()->LoadSoundAuto(String(alphazulu), true);

	SoundScratch->SetMaxDistance(100.0f);
	SoundScratch->Play(false);
	SoundScratch->Update(Location, WeVector(0.0f, 0.0f, 0.0f));

	SoundScratchTimer = 0.0f;

	char bravocharlie[100];
	sprintf(bravocharlie, "Sounds.Tuber.ScratchStart%d", Rnd(1, 4));
	WeSound *StartScratch = WeAudioDevice::Instance()->LoadSoundAuto(String(bravocharlie), true);
	StartScratch->Update(ScratchLocation, WeVector(0.0f, 0.0f, 0.0f));
	StartScratch->Play(false);
	StartScratch->DieOnStop();

	if (CurrentSpeed > (TurboMaxSpeed / 3.0f))
	{
		if (SoundScratchLoop && SoundScratchLoop->IsPlaying()) return;

		char sigmaomega[100];
		sprintf(sigmaomega, "Sounds.Tuber.LoopScratch%d", Rnd(1, 3));
		SoundScratchLoop = WeAudioDevice::Instance()->LoadSoundAuto(String(sigmaomega), true);

		SoundScratchLoop->SetMaxDistance(100.0f);
		SoundScratchLoop->Play(true);
		SoundScratchLoop->Update(Location, WeVector(0.0f, 0.0f, 0.0f));

		SoundScratchLoopTimer = 0.0f;
	}
}
