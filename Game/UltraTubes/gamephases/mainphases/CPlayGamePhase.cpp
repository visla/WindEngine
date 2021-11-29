/*
    CPlayGamePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
*/

#include <CPlayGamePhase.h>
#include <CEditorWindow.h>
#include <CGame.h>
#include <WeCanvas.h>
#include <CMainGameInterface.h>
// ====== for testing ====
#include <CTuber.h>
#include <CMainGamePhase.h>
#include <AllPhases.h>
#include <CFinalizeGamePhase.h>
#include <CMenuPhase.h>
#include <DInput.h>
#include <WeCEGUI.h>
#include <d3d9renderer.h>
#include <CGameClient.h>
#include <WeTimer.h>
#include <CLanServerBrowsePhase.h>
#include <CGameServer.h>
#include <COptionsSettings.h>
#include <WePhysics.h>
#include <WeActor.h>
#include <ActorTags.h>
#include <CollisionGroups.h>
#include <CExplosion.h>
#include <CWeapon.h>
#include <CMenuPhase.h>
#include <WePostProcess.h>
#include <WeProfiler.h>
#include <CGUIHelper.h>
#include <WeAudioDevice.h>
#include <WeModelManager.h>
#include <WePhysicModelManager.h>
#include <WePhysicModel.h>
#include <CMainMenuPhase.h>
#include <CExitMenuPhase.h>
#include <CGameClient.h>
#include <CPickup.h>
#include <CPickupMiniRockets.h>
#include <CPickupHealth.h>
#include <CPickUpNukeRocket.h>
#include <CPickupBlurMine.h>
#include <CPickupAnimationBlurMine.h>
#include <CPickupElectricMine.h>
#include <CPickupAnimationElectricMine.h>
#include <CPickupForceFieldMine.h>
#include <CPickupAnimationForceFieldMine.h>
#include <CPickupFrontFieldGenerator.h>
#include <CPickupInvisibleMine.h>
#include <CPickupAnimationInvisibleMine.h>
#include <CPickupMinigun.h>
#include <CPickupMovingMine.h>
#include <CPickupAnimationMovingMine.h>
#include <CPickupNukeRocket.h>
#include <CPickupQuantumLaser.h>
#include <CPickupRandomWeapon.h>
#include <CPickupSlowingRocket.h>
#include <CPickupSuperNukeMine.h>
#include <CPickupAnimationSuperNukeMine.h>
#include <CPickupTranslocator.h>
#include <CPickupProximityMine.h>
#include <CPickupAnimationProximityMine.h>
#include <CPickupSlowingMine.h>
#include <CPickupAnimationSlowingMine.h>
#include <CPickupHomingMine.h>
#include <CPickupAnimationHomingMine.h>
#include <CPickupStandardMine.h>
#include <CPickupAnimationStandardMine.h>
#include <CPickupSmokeMine.h>
#include <CPickupStandardLaser.h>
#include <CPickupDoubleLaser.h>
#include <CPickupQuadLaser.h>
#include <CPickUpAnimationStandardLaser.h>
#include <CPickUpAnimationDoubleLaser.h>
#include <CPickUpAnimationQuadLaser.h>
#include <CPickupAnimationSmokeMine.h>
#include <CPickupAnimationTranslocator.h>
#include <CPickupAnimationSlowingRocket.h>
#include <CPickupAnimationQuantumLaser.h>
#include <CPickupAnimationNukeRocket.h>
#include <CPickupAnimationMiniRockets.h>
#include <CPickupAnimationMinigun.h>
#include <CPickupStandardRocket.h>
#include <CPickupAnimationStandardRocket.h>
#include <CPickupCoin.h>
#include <CPickupInvisibility.h>
#include <CPickupInvulnerability.h>
#include <CPickupDoubleDamage.h>
#include <CPickupQuadDamage.h>
#include <CPickupAnimationCoin.h>
#include <CPickupAnimationInvisibility.h>
#include <CPickupAnimationInvulnerability.h>
#include <CPickupAnimationDoubleDamage.h>
#include <CPickupAnimationQuadDamage.h>
#include <CTrigger.h>
#include <CTuberPartFabric.h>
#include <WeTextureManager.h>
#include <CTrigger.h>
#include <CPickupGravityMine.h>
#include <CPickupAnimationGravityMine.h>
#include <CStatistics.h>
#include <CTuberSparks.h>
#include <CPickUpAnimation.h>
#include <WeSound.h>
#include <CTuberPart.h>
#include <CAIKernel.h>
#include <CAgent.h>
#include <CLocalizationSettings.h>
#include <CWeaponFabric.h>
#include <WeSoundAttenuator.h>
#include <WeEngine.h>
#include <CUberText.h>
#include <CWeaponDescriptionManager.h>
#include <Helper.h>
#include <COptionsPhase.h>
#include <CConfig.h>
#include <WeLoger.h>
#include <CPickupAction.h>
#include <CPickupActionInvisibility.h>
#include <CPickupActionInvulnerability.h>
#include <CPickupActionQuadDamage.h>
#include <WeProfiler.h>
#include <COptionsVideoPhase.h>
#include <COptionsAudioPhase.h>
#include <COptionsInputPhase.h>
#include <WeRenderTree.h>
#include <CSparticles.h>
#include <WeFlareRenderer.h>



#define SAFETY_LOCK_TIME	5.0f


CPlayGamePhase *CPlayGamePhase::Instance = new CPlayGamePhase;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
CPlayGamePhase::CPlayGamePhase()
{
	//Anim321 = NULL;
	//AnimGO = NULL;
	//FinishTextWin = NULL;
	//FinishTextLose = NULL;
	LocalHumanPlayer = NULL;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::Start()
{
	Status = PLAYGAME_WAITINGFORPLAYERS;
	ShowInGameMenu = false;
	GameFinishedTimer = 0.0f;
	OkForExit = false;

	EndAnimPlayed = false;
	//WinningText = NULL;

	LocalHumanPlayer = NULL;

	TimeTrialLap = -10;
	TimeTrialLapTimeLimit = 0.0f;
	TimeTrialLapTime = 0.0f;

	CWeaponFabric::Instance->SetActive(CGameSettings::ClientInstance->GetWeapons());
	SafetyLockTimer = 0.0f;

	// Kazi da smo spremni za trku i trazi spawn
	CGameClient::Instance->ReadyForRace();

	//CMainGamePhase::Instance->Map.SetZonedRendering(true);

	//---prepare collisions---//
	WePhysics::Instance()->SetCollisionCallback(OnCollision);
	WePhysics::Instance()->SetTriggerCallback(OnTrigger);
	DisableAllCollisions();
	EnablePredefCollisions();
	//------------------------//

	//---teleport callback---//
	WePhysics::Instance()->SetTeleportReport(OnTeleport);
	//-----------------------//


	CEGUI::Window* wnd = CGUIHelper::CreateWnd("IN_GAME_MENU", "GAME PAUSED",0.0f,0.3f,0.2f,0.5f,0.5f,"RootWindow", "false");

	CGUIHelper::CreateButton("PLAYGAME_RESUME_BUTTON", CLocalizationSettings::Instance()->GetLocalText("RESUME"), 0.05f, 0.2f, 0.8f, 0.2f, "IN_GAME_MENU", &CPlayGamePhase::OnBackToGame);

	CGUIHelper::CreateButton("PLAYGAME_BACK_BUTTON", CLocalizationSettings::Instance()->GetLocalText("BACK TO MENU"), 0.05f, 0.4f, 0.8f, 0.2f, "IN_GAME_MENU", &CPlayGamePhase::OnBackToMenu);

	CGUIHelper::CreateButton("PLAYGAME_OPTIONS_BUTTON", CLocalizationSettings::Instance()->GetLocalText("OPTIONS"), 0.05f, 0.6f, 0.8f, 0.2f, "IN_GAME_MENU", &CPlayGamePhase::OnOptionsMenu);

	CGUIHelper::CreateButton("PLAYGAME_EXIT_BUTTON", CLocalizationSettings::Instance()->GetLocalText("EXIT GAME"), 0.05f, 0.8f, 0.8f, 0.2f, "IN_GAME_MENU", &CPlayGamePhase::OnExitGame);

	WeFlareRenderer::Instance()->RemoveAllLights();

	WePostProcess::Instance()->UpdateEffects();

	WeRenderDevice::Instance()->SetPixelMotionBlurFactor(0.7f);
	WeRenderDevice::Instance()->SetBloomScale(CMainGamePhase::Instance->Map.MapHeader.FHDRI[1]);

	CMainGameInterface::Instance->Initialize();

	CMainGamePhase::Instance->PlaySong(SongRace);

	CStatistics::Instance->Init(*CGameSettings::ClientInstance);
	RaceTime = 0.0f;
	CEditorWindow::Playing = true;   // oznaceno za mrezu. Prilikom diskonektovanja

	PrevMaxLap = 0;

	//init AI
	AIKernel = new CAIKernel();
	AIKernel->Initialize();

	if (!CMainGamePhase::Instance->InputPlayer)
	{
		WeRenderDevice::Instance()->SetRendering(false);
	}

	CMainGamePhase::Instance->Camera.SetFarClip(CMainGamePhase::Instance->Map.MapHeader.ClippingDistance);
	CMainGamePhase::Instance->Camera.CurrentCameraIndex = 0;

	CMainGameInterface::Instance->ShowFPS = false;
	CMainGameInterface::Instance->ShowShaderInfo = false;
	CMainGameInterface::Instance->ShowWeapon = true;
	CMainGameInterface::Instance->ShowStats = true;
	CMainGameInterface::Instance->ShowModelNames = false;

	// temp objects... can be removed if looks bad
	//WeActor *tmpA;
	//tmpA = new WeActor();
	//tmpA->ModelName = "MapEditor.Geometry.Box";
	//tmpA->Type = 100;
	//tmpA->Location = WeVector(0,0,0);
	//tmpA->Initialize();
	//SAFE_DELETE(tmpA->PhysicHandle);
	//WeVector tmpV = WeVector(5,5,5);
	//tmpA->PhysicHandle = WePhysics::Instance()->CreateSphere(tmpA, tmpA->Location, 100.0f, COLLISION_GROUP_NOTIFY_TRIGGER, 10.0f, false, WETRIGGER_ON_ENABLE);
	//tmpA->PhysicHandle->SetDisableGravity(true);
	//tmpA->Renderable = false;
	//CMainGamePhase::Instance->Map.AddActor(tmpA);

	//WeActor *tmpA2;
	//tmpA2 = new WeActor();
	//tmpA2->Type = 101;
	//tmpA2->Location = WeVector(0,-2.0f,0);
	//tmpA2->ModelName = "MapEditor.Geometry.Box";	
	//tmpA2->Initialize();
	//SAFE_DELETE(tmpA2->PhysicHandle);
	//tmpA2->PhysicHandle = WePhysics::Instance()->CreateSphere(tmpA2, tmpA2->Location, 2.0f, 0, 10.0f, false);
	//CMainGamePhase::Instance->Map.AddActor(tmpA2);
	//tmpA2->PhysicHandle->SetDisableGravity(true);

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DisableAllCollisions()
{
	for (int i = 0; i < 32; i++)
		for (int j = i; j < 32; j++)
		{
			WePhysics::Instance()->SetGroupCollisionInteract(i, j, false);
			WePhysics::Instance()->SetGroupCollision(i, j, WECOLLISION_DISABLE);
		}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::EnablePredefCollisions()
{
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_PREFAB, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_TUBER, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_WEAPON, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_FRAGMENT, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_SHELL, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_MINE, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_TRANSMINE, true);
	SetCollisionPair(COLLISION_GROUP_PREFAB, COLLISION_GROUP_NOTIFY_TRIGGER, true);

	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_TUBER, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_WEAPON, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_PICKUP, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_EXPLOSION, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_MINE, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_FRAGMENT, true);
	SetCollisionPair(COLLISION_GROUP_TUBER, COLLISION_GROUP_NOTIFY_TRIGGER, true);

	SetCollisionPair(COLLISION_GROUP_WEAPON, COLLISION_GROUP_MINE, true);
	SetCollisionPair(COLLISION_GROUP_WEAPON, COLLISION_GROUP_TRANSMINE, true);

	SetCollisionPair(COLLISION_GROUP_NOTIFY_TRIGGER, COLLISION_GROUP_MINE, true);
	SetCollisionPair(COLLISION_GROUP_NOTIFY_TRIGGER, COLLISION_GROUP_TRANSMINE, true);
	SetCollisionPair(COLLISION_GROUP_NOTIFY_TRIGGER, COLLISION_GROUP_FRAGMENT, true);
	SetCollisionPair(COLLISION_GROUP_NOTIFY_TRIGGER, COLLISION_GROUP_SHELL, true);

	SetCollisionPair(COLLISION_GROUP_EXPLOSION, COLLISION_GROUP_MINE, true);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::SetCollisionPair(DWORD GroupOne, DWORD GroupTwo, bool Enable)
{
	int WindCollision;
	if (Enable)
		WindCollision = WECOLLISION_ON_ALL;
	else
		WindCollision = WECOLLISION_DISABLE;

	WePhysics::Instance()->SetGroupCollision((WORD)GroupOne, (WORD)GroupTwo, WindCollision);
	WePhysics::Instance()->SetGroupCollisionInteract((WORD)GroupOne, (WORD)GroupTwo, Enable);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::End()
{
	if (CMainGamePhase::Instance->InputPlayer->MuteAudioEnable)
		WeAudioDevice::Instance()->SetSoundVolume(CMainGamePhase::Instance->InputPlayer->MuteAudioSavedValue);

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CMainGamePhase::Instance->Map.Unload();

	if (winMgr.isWindowPresent("Options_VideoWindow"))
		COptionsVideoPhase::Instance->End();
	if (winMgr.isWindowPresent("Options_AudioWindow"))
		COptionsAudioPhase::Instance->End();
	if (winMgr.isWindowPresent("InputOptions_Window"))
		COptionsInputPhase::Instance->End();

	winMgr.getWindow("IN_GAME_MENU")->destroy();

	CMainGameInterface::Instance->ShutDown();
	CEditorWindow::Playing = false;

	SAFE_DELETE(AIKernel);

	//delete tmp;
}
//	CMainGameInterface::Instance->ShutDown();


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::OnCollision(WeActor *a1, WeActor *a2, WeVector &Normal, WeVector &Tangent, DWORD Flag, WeVector &FirstTouchPoint)
{

	/*char tmp[100];
	sprintf(tmp, "Typ1: %d, type2: %d", a1->Type, a2->Type);
	MessageBox(0, tmp, 0, 0);*/

	if (ACTORTYPE_FRAGMENT == a1->Type)
	{
		if (CollideFragment((CTuberPart*)a1, a2)) return;
	}
	else if (ACTORTYPE_FRAGMENT == a2->Type)
	{
		if (CollideFragment((CTuberPart*)a2, a1)) return;
	}

	if (ACTORTYPE_WEAPON == a1->Type)
		if (CollideWeapon((CWeapon*)a1, a2)) return;
	if (ACTORTYPE_WEAPON == a2->Type)
		if (CollideWeapon((CWeapon*)a2, a1)) return;

	if (ACTORTYPE_MINE == a1->Type)
		if (CollideMine((CWeapon*)a1, a2)) return;
	if (ACTORTYPE_MINE == a2->Type)
		if (CollideMine((CWeapon*)a2, a1)) return;

	if (a1->Type == ACTORTYPE_TUBER && a2->Type == ACTORTYPE_TUBER)
	{
		CollideTubers((CTuber*)a1, (CTuber*)a2, FirstTouchPoint);
		return;
	}

	if (((a1->Type == WETYPE_ACTOR) && a1->IsPrefab) || ((a2->Type == WETYPE_ACTOR) && a2->IsPrefab))
	{
		CTuber *TempTuber = NULL;
		if (a1->Type == ACTORTYPE_TUBER) TempTuber = (CTuber *)a1;
		if (a2->Type == ACTORTYPE_TUBER) TempTuber = (CTuber *)a2;

		if (TempTuber)
		{
			//---create tuber parts---//
			//if ( (TempTuber->TuberPartTimer >= 0.2f) && (TempTuber->CurrentSpeed > (TempTuber->MaxSpeed * 0.8f)) )
			//{
			//	TempTuber->TuberPartTimer = 0.0f;
			//	static int MaxParts = 3;
			//	int n = (int)((TempTuber->CurrentSpeed / TempTuber->MaxSpeed) * MaxParts);
			//	if (n > MaxParts) n = MaxParts;
			//	for (int i = 0; i < n; i++)
			//	{
			//		CTuberPartFabric::Instance()->CreateTuberPart(TempTuber, false, ((rand() % 3 == 0) ? true : false));
			//	}
			//}
			//------------------------//

			TempTuber->OnWallScratch(FirstTouchPoint);

			//---create tuber sparks---//
			if (TempTuber->CurrentSpeed > (TempTuber->TurboMaxSpeed / 3.0f))
			{
				if (Flag != WECOLLISION_ON_END_TOUCH)
				{
					//WeVector SpawnVec = (FirstTouchPoint - TempTuber->Location);
					//WeMatrix SpawnMat;
					//D3DXMatrixTranslation(&SpawnMat, SpawnVec.x, SpawnVec.y, SpawnVec.z);

					//if (!TempTuber->Sparks)
					//{
					//	TempTuber->Sparks = new CTuberSparks(TempTuber, SpawnMat, Normal);
					//}
					//else
					//{
					//	TempTuber->Sparks->Update(SpawnMat, Normal);
					//	TempTuber->Sparks->SetActive(true);
					//}

					if (D3DXVec3LengthSq(&Normal) > 0.1f)
					{
						if (!TempTuber->TuberSparticles)
							TempTuber->TuberSparticles = new CSparticles(TempTuber->GetWorld(), "Weapons.Models.Spark", "tuber.Sparticles", 32, 100);
						TempTuber->TuberSparticles->Init(); // da se ponovo ucita konfiguracija, izbaci u release verziji igre
						TempTuber->TuberSparticles->Emit(TempTuber, FirstTouchPoint, Normal);
					}
				}
			}
			//-------------------------//

			//---tuber speed manipulation---//
			//float d;
			//WeVector v2;
			//D3DXVec3Normalize(&Tangent, &Tangent);
			//v2 = TempTuber->Direction;
			//D3DXVec3Normalize(&v2, &v2);
			//d = abs(D3DXVec3Dot(&v2, &Tangent));

			// version 1
			//float NormalCoef = -20.0f;
			//float TangentCoef = 0;
			//float SpeedPercent = 0.98f;

			//float SpeedCoef = 0.0f;
			//if (TempTuber->TurboMaxSpeed > 0.0f)
			//	SpeedCoef = TempTuber->CurrentSpeed / TempTuber->TurboMaxSpeed;

			//D3DXVec3Normalize(&Normal, &Normal);
			//TempTuber->Velocity += (Normal * SpeedCoef * NormalCoef);
			//TempTuber->Velocity += (Tangent * SpeedCoef * TangentCoef);

			//TempTuber->CurrentSpeed *= SpeedPercent;

			// version 2
			float VelocityCoef;
			WeVector Lx, Ly, Lz;

			TempTuber->RefPathNode->GetLocalAxis(Lx, Ly, Lz);

			//VelocityCoef = D3DXVec3Dot(&TempTuber->Velocity, &Lz);
			VelocityCoef = D3DXVec3Length(&TempTuber->Velocity);
			D3DXVec3Scale(&TempTuber->Velocity, &Lz, VelocityCoef);

			WeVector Tx, Ty, Tz;
			TempTuber->GetLocalAxis(Tx, Ty, Tz);

			float Dx, Dy, Dz;
			Dx = D3DXVec3Dot(&Tx, &Lz);
			Dy = D3DXVec3Dot(&Tx, &Ly);
			Dz = D3DXVec3Dot(&Tx, &Lz);

			//TempTuber->AngularVelocity.x = atan2(Dz, Dy);
			//TempTuber->AngularVelocity.y = atan2(Dx, Dz);
			//------------------------------//
		}
	}
}



bool CPlayGamePhase::PrevRand = false;
int CPlayGamePhase::RndID = 0;

bool CPlayGamePhase::CreatePickUpByType(int _Type, CPickUp* Pickup, WeActor* Actor, bool Animate)
{
	bool Res = false;
	WeSound *PickupSound = NULL;
	bool PrimaryPickup = false;
	bool SecondaryPickup = false;

	switch(_Type)
	{	
	case PICKUPTYPE_MINIROCKETS:
		{
			if(!PrevRand)
				((CPickupMiniRockets*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationMiniRockets();
		}
		break;

	case PICKUPTYPE_HEALTH:
		((CPickupHealth*)Pickup)->OnTrigger(Actor);
		break;

	case PICKUPTYPE_NUKEROCKET:
		{
			if(!PrevRand)
				((CPickupNukeRocket*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationNukeRocket();
		}
		break;

	case PICKUPTYPE_QUANTUMLASER:
		{
			if(!PrevRand)
			 ((CPickupQuantumLaser*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationQuantumLaser();
		}
		break;

	case PICKUPTYPE_BLURMINE:
		{
			if(!PrevRand)
				((CPickupBlurMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationBlurMine();
		}
		break;

	case PICKUPTYPE_ELECTRICMINE:
		{
			if(!PrevRand)
				((CPickupElectricMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationElectricMine();
		}
		break;

	case PICKUPTYPE_FORCEFIELDMINE:
		{
			if(!PrevRand)
				((CPickupForceFieldMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationForceFieldMine();
		}
		break;

	case PICKUPTYPE_FRONTFIELDGENERATOR:
		((CPickupFrontFieldGenerator*)Pickup)->OnTrigger(Actor);
		CMainGameInterface::Instance->AnimatingWeapon = Animate;
		break;

	case PICKUPTYPE_INVISIBLEMINE:
		{
			if(!PrevRand)
				((CPickupInvisibleMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationInvisibleMine();
		}
		break;

	case PICKUPTYPE_MINIGUN:
		{
			if(!PrevRand)
				((CPickupMinigun*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationMinigun();
		}
		break;

	case PICKUPTYPE_MOVINGMINE:
		{
			if(!PrevRand)
				((CPickupMovingMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationMovingMine();
		}
		break;

	case PICKUPTYPE_RANDOMWEAPON:
		((CPickupRandomWeapon*)Pickup)->OnTrigger(Actor);
		RndID = ((CPickupRandomWeapon*)Pickup)->RandomID;
		Res = true;
		PrevRand = true;
		break;

	case PICKUPTYPE_PROXIMITYMINE:
		{
			if(!PrevRand)
				((CPickupProximityMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationProximityMine();
		}
		break;

	case PICKUPTYPE_SLOWINGROCKET:
		{
			if(!PrevRand)
				((CPickupSlowingRocket*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationSlowingRocket();
		}
		break;		

	case PICKUPTYPE_SUPERNUKEMINE:
		{
			if(!PrevRand)
				((CPickupSuperNukeMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationSuperNukeMine();
		}
		break;

	case PICKUPTYPE_TRANSLOCATOR:
		{
			if(!PrevRand)
				((CPickupTranslocator*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationTranslocator();
		}
		break;
	
	case PICKUPTYPE_GRAVITYMINE:
		{
			if(!PrevRand)
				((CPickupGravityMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationGravityMine();
		}
		break;

	case PICKUPTYPE_STANDARDMINE:
		{
			if(!PrevRand)
				((CPickupStandardMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationStandardMine();
		}
		break;

	case PICKUPTYPE_SMOKEMINE:
		{
			if(!PrevRand)
				((CPickupSmokeMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationSmokeMine();
		}
		break;

	case PICKUPTYPE_HOMINGMINE:
		{
			if(!PrevRand)
				((CPickupHomingMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationHomingMine();
		}
		break;

	case PICKUPTYPE_SLOWINGMINE:
		{
			if(!PrevRand)
				((CPickupSlowingMine*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingMine = Animate;
			SecondaryPickup = Animate;
			if (Animate) new CPickupAnimationSlowingMine();
		}
		break;

	case PICKUPTYPE_STANDARDROCKET:
		{
			if(!PrevRand)
			((CPickupStandardRocket*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationStandardRocket();
		}
		break;

	case PICKUPTYPE_STANDARDLASER:
		{
			if(!PrevRand)
				((CPickupStandardLaser*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationStandardLaser();
		}
		break;

	case PICKUPTYPE_DOUBLELASER:
		{
			if(!PrevRand)
				((CPickupDoubleLaser*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationDoubleLaser();
		}
		break;

	case PICKUPTYPE_QUADLASER:
		{
			if(!PrevRand)
				((CPickupQuadLaser*)Pickup)->OnTrigger(Actor);
			CMainGameInterface::Instance->AnimatingWeapon = Animate;
			PrimaryPickup = Animate;
			if (Animate) new CPickupAnimationQuadLaser();
		}
		break;


	case PICKUPTYPE_COINSMALL:
	case PICKUPTYPE_COINMEDIUM:
	case PICKUPTYPE_COINLARGE:
		((CPickupCoin*)Pickup)->OnTrigger(Actor);
		if (Animate)
		{
			PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.Coin", true);
			new CPickupAnimationCoin(_Type, (CTuber*)Actor);
		}
		break;

	case PICKUPTYPE_INVISIBILITY:
		((CPickupInvisibility*)Pickup)->OnTrigger(Actor);
		if (Animate)
		{
			PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.Invisibility", true);
			new CPickupAnimationInvisibility((CTuber*)Actor);
		}
		break;

	case PICKUPTYPE_INVULNERABILITY:
		((CPickupInvulnerability*)Pickup)->OnTrigger(Actor);
		if (Animate)
		{
			PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.Invulnerability", true);
			new CPickupAnimationInvulnerability((CTuber*)Actor);
		}
		break;

	case PICKUPTYPE_DOUBLEDAMAGE:
		((CPickupDoubleDamage*)Pickup)->OnTrigger(Actor);
		if (Animate)
		{
			PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.DoubleDamage", true);
			new CPickupAnimationDoubleDamage((CTuber*)Actor);
		}
		break;

	case PICKUPTYPE_QUADDAMAGE:
		((CPickupQuadDamage*)Pickup)->OnTrigger(Actor);
		if (Animate)
		{
			PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.QuadDamage", true);
			new CPickupAnimationQuadDamage((CTuber*)Actor);
		}
		break;
	}

	if (PrimaryPickup)
	{
		PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.PrimaryWeapon", true);
	}
	else if (SecondaryPickup)
	{
		PickupSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.SecondaryWeapon", true);
	}

	if (PickupSound)
	{
		PickupSound->Update(Actor->Location, WeVector(0.0f, 0.0f, 0.0f));
		PickupSound->Play(false);
		PickupSound->DieOnStop();
	}

	return Res;
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DoPickUpLogiks(WeActor* a1, WeActor* a2)
{
	if ( (a1->Type < 10000) && (a2->Type < 10000) ) return;

	CPickUp* Pickup = NULL;
	WeActor* tmpA = NULL;

	if(a2->Type == ACTORTYPE_TUBER)
	{
		Pickup = (CPickUp*)a1;
		tmpA = a2;
	}else if(a1->Type == ACTORTYPE_TUBER)
	{
		Pickup = (CPickUp*)a2;
		tmpA = a1;
	}

	if(!Pickup && !tmpA) return;
	if(!Pickup->Active) return;

	bool Animate = false;
	bool Rnd = false;
	Animate = (tmpA == CMainGamePhase::Instance->InputPlayer);
	if(CreatePickUpByType(Pickup->Type, Pickup, tmpA, Animate))
	{
		CreatePickUpByType(RndID, Pickup, tmpA, Animate);
	}
	PrevRand = false;
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::OnTrigger(WeActor *a1, WeActor *a2, DWORD Flag)
{
	//WeVector tmpV;
	//float r;

	//if (a1->Type == ACTORTYPE_TUBER) 
	//{
	//	
	//	tmpV = a2->Location - a1->Location;
	//	r = D3DXVec3Length(&tmpV);
	//	D3DXVec3Normalize(&tmpV, &tmpV);
	//	if (r < 1.0) r = 200.0f;
	//	tmpV = tmpV * 200.0f/r;
	//	//tmpV = WeVector(0, 1.01f, 0);
	//	((CTuber*)a1)->Velocity += tmpV;
	//	//a1->PhysicHandle->AddVelocity(tmpV);
	//}
	//if (a2->Type == ACTORTYPE_TUBER) 
	//{		
	//	tmpV = a1->Location - a2->Location;
	//	r = D3DXVec3Length(&tmpV);
	//	D3DXVec3Normalize(&tmpV, &tmpV);
	//	if (r < 1.0) r = 200.0f;
	//	tmpV = tmpV * 200.0f/r;
	//	//tmpV = WeVector(0, 1.01f, 0);
	//	((CTuber*)a2)->Velocity += tmpV;
	//	//a2->PhysicHandle->AddVelocity(tmpV);
	//}

	DoPickUpLogiks(a1,a2);

	if (ACTORTYPE_EXPLOSION == a1->Type)
		if (CollideExplosion((CExplosion*)a1, a2)) return;
	if (ACTORTYPE_EXPLOSION == a2->Type)
		if (CollideExplosion((CExplosion*)a2, a1)) return;

	if (ACTORTYPE_TRIGGER == a1->Type)
		if (CollideTrigger((CTrigger*)a1, a2)) return;
	if (ACTORTYPE_TRIGGER == a2->Type)
		if (CollideTrigger((CTrigger*)a2, a1)) return;

}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool CPlayGamePhase::CollideWeapon(CWeapon *Weapon, WeActor *Actor)
{
	if (Actor == Weapon->WeaponOwner) return false;

	switch(Actor->Type)
	{
	case ACTORTYPE_ACTOR:
	case ACTORTYPE_TUBER:
		Weapon->OnHit(Actor);
		return true;

	case ACTORTYPE_MINE:
		Weapon->OnHit(Actor);
		return false;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool CPlayGamePhase::CollideMine(CWeapon *Mine, WeActor *Actor)
{
	switch(Actor->Type)
	{
	case ACTORTYPE_ACTOR:
	case ACTORTYPE_TUBER:
	case ACTORTYPE_EXPLOSION:
	case ACTORTYPE_WEAPON:
		Mine->OnHit(Actor);
		return true;
	}

	return false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool CPlayGamePhase::CollideTrigger(CTrigger *Trigger, WeActor *Actor)
{
	if (Actor->Type == ACTORTYPE_TUBER)
		if (Trigger->Parent->Type == ACTORTYPE_MINE)
		{
			((CWeapon*)(Trigger->Parent))->OnNotify(Actor);
			return true;
		}

	if (Trigger->Parent->Type == ACTORTYPE_EXPLOSION)
	{
		((CExplosion*)(Trigger->Parent))->OnNotify(Actor);
		return true;
	}

	return false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool CPlayGamePhase::CollideExplosion(CExplosion *Explosion, WeActor *Actor)
{
	switch(Actor->Type)
	{
	case ACTORTYPE_TUBER:
		Explosion->Affect((CTuber*)Actor);
		return true;
	case ACTORTYPE_MINE:
		((CWeapon*)Actor)->OnHit(Explosion);
		return true;
	}

	return false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool CPlayGamePhase::CollideFragment(CTuberPart *Part, WeActor *Actor)
{
	Part->OnCollision(Actor);
	
	return false;
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DoSingleRaceAndInstantRaceLogiks()
{
	int WinCounter = 0;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!CGameSettings::ServerInstance->tmpPlayers[i].User) continue;
		if (!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber) continue;

		if (CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->LapCount == CGameSettings::ServerInstance->LapCount)
		{
			if (!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->RaceFinished)
			{
				// posalji da smo zavrsili trku				
				CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->RaceFinished = true;
				if (CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Position <= 3)
					CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Win = true;
				else
					CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Win = false;

				WeNetworkMessage Msg;
				Msg.Init();
				Msg.WriteMessageType(CLIENT_RACEFINISHED);
				Msg.WriteInt(i); // index in game settings
				Msg.WriteBool(CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Win);
				WeServer::Instance()->Send(&Msg);							
			}
			else
			{
				// vec smo zavrsili trku sa ovim i izbroj ga
				WinCounter++;
				if (WinCounter >= 3 || WinCounter == CGameSettings::ServerInstance->GetPlayersCount())
				{
					for (int j = 0; j < MAX_PLAYERS; j++)
					{
						if (!CGameSettings::ServerInstance->tmpPlayers[j].User) continue;
						if (!CGameSettings::ServerInstance->tmpPlayers[j].User->Tuber) continue;

						if (!CGameSettings::ServerInstance->tmpPlayers[j].User->Tuber->RaceFinished)
						{
							CGameSettings::ServerInstance->tmpPlayers[j].User->Tuber->Win = false;
							WeNetworkMessage MsgLose;
							MsgLose.Init();
							MsgLose.WriteMessageType(CLIENT_RACEFINISHED);
							MsgLose.WriteInt(j);
							MsgLose.WriteBool(false);
							WeServer::Instance()->Send(&MsgLose);
						}
					}

					WeNetworkMessage Msg;
					Msg.Init();
					Msg.WriteMessageType(GAMEFINISHED);
					WeServer::Instance()->Send(&Msg);
				}
			}
		}
	}
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DoSingleRaceTimeTrialLogiks()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!CGameSettings::ServerInstance->tmpPlayers[i].User) continue;
		if (!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber) continue;

		CTuber* Player = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;

		if ( (Player->LapCount != TimeTrialLap) && (Player->LapCount != CGameSettings::ServerInstance->LapCount) )
		{
			TimeTrialLap = Player->LapCount;
			TimeTrialLapTimeLimit = CMainGamePhase::Instance->Map.MapHeader.RaceTime;
			for (int i = 0; i < TimeTrialLap; ++i)
			{
				TimeTrialLapTimeLimit *= 0.98f;
			}
			Player->MaxTurboCharges = ((1 + Player->Description.TurboUpgradeLevel) * 100.0f);
			Player->MaxTurboCharges += Player->MaxTurboCharges * TimeTrialLap * 0.12f;
			Player->TurboCharges = Player->MaxTurboCharges;
		}

		if (Player->LapTime > TimeTrialLapTimeLimit)
		{
			TimeTrialLapTime = TimeTrialLapTimeLimit;
			Player->RaceFinished = true;
			Player->Win = false;

			WeNetworkMessage Msg;
			Msg.Init();
			Msg.WriteMessageType(CLIENT_RACEFINISHED);
			Msg.WriteInt(i);
			Msg.WriteBool(false);
			WeServer::Instance()->Send(&Msg);

			Msg.Init();
			Msg.WriteMessageType(GAMEFINISHED);
			WeServer::Instance()->Send(&Msg);
		}
		else if (Player->LapCount == CGameSettings::ServerInstance->LapCount)
		{
			Player->RaceFinished = true;
			Player->Win = true;			

			WeNetworkMessage Msg;
			Msg.Init();
			Msg.WriteMessageType(CLIENT_RACEFINISHED);
			Msg.WriteInt(i);
			Msg.WriteBool(true);			
			WeServer::Instance()->Send(&Msg);

			Msg.Init();
			Msg.WriteMessageType(GAMEFINISHED);
			WeServer::Instance()->Send(&Msg);
		}
		else
		{
			TimeTrialLapTime = Player->LapTime;
		}
	}
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DoSPLastOneAndMPLastOneLogiks()
{
	int WinCounter = 0;
	int LastID, ID = -10000;
	CTuber* TuberForOut = NULL;
	int MinLap = 10000;
	int OutCounter = 0;
	int LastOne = -1;
	int MaxLap = -1;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!CGameSettings::ServerInstance->tmpPlayers[i].User) continue;

		if (!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->RaceFinished)
		{
			LastID = i;
			WinCounter++;

			if (CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Position > LastOne)
			{
				LastOne = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Position;
				TuberForOut = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;
				ID = i;
			}

			if (CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->LapCount > MaxLap)
			{
				MaxLap = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->LapCount;
			}
		}
	}

	// imamo samo jednog koji nije zavrsio trku on je pobednik
	if (WinCounter == 1)
	{
		CGameSettings::ServerInstance->tmpPlayers[LastID].User->Tuber->RaceFinished = true;
		CGameSettings::ServerInstance->tmpPlayers[LastID].User->Tuber->Win = true;

		WeNetworkMessage Msg;
		Msg.Init();
		Msg.WriteMessageType(CLIENT_RACEFINISHED);
		Msg.WriteInt(LastID);
		Msg.WriteBool(true);  // winner nije 
		WeServer::Instance()->Send(&Msg);

		Msg.Init();
		Msg.WriteMessageType(GAMEFINISHED);
		WeServer::Instance()->Send(&Msg);
		return;
	}

	if (PrevMaxLap >= MaxLap)
		return;
	else
		PrevMaxLap = MaxLap;

	// tuber ejaculation :P
	TuberForOut->RaceFinished = true;
	TuberForOut->Win = false;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(CLIENT_RACEFINISHED);
	Msg.WriteInt(ID);
	Msg.WriteBool(false);  // winner nije 
	WeServer::Instance()->Send(&Msg);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
// TODO: Test and Make Tournament "Logiks" :P
void CPlayGamePhase::DoSPAndMPTournamentLogiks()
{
	int winCounter = 0;
	int ID = -10000;
	CTuber* TuberForOut = NULL;
	int BefoureCount = 0;
	bool Res = false;

	if(CGameSettings::Instance->MapSettings->RaceType == RACETYPE_STANDARD)
	{
		for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
		{
			if(CGameSettings::Instance->Players.GetItem().User->Tuber)
			if(CGameSettings::Instance->Players.GetItem().User->Tuber->LapCount == CGameSettings::Instance->MapSettings->LapCount)
			{
				if(!CGameSettings::Instance->Players.GetItem().User->Tuber->RaceFinished)
				{
					CGameSettings::Instance->Players.GetItem().User->Tuber->Position--;
					WeNetworkMessage tmpMsg;
					tmpMsg.Init();
					tmpMsg.WriteMessageType(CLIENT_RACEFINISHED);
					tmpMsg.WriteInt(CGameSettings::Instance->Players.GetItem().ID);

					switch(CGameSettings::Instance->MapSettings->EndCondition)
					{
					case RACEENDCONDITION_1:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 1)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);
						}
						break;
					case RACEENDCONDITION_12:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 1 ||
							   CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 2)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);
						}
						break;
					case RACEENDCONDITION_123:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 1 || 
							   CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 2 ||
							   CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 3)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);

						}
						break;
					case RACEENDCONDITION_2:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 2)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);
						}
						break;
					case RACEENDCONDITION_23:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 2 ||
							   CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 3)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);
						}
						break;
					case RACEENDCONDITION_3:
						{
							if(CGameSettings::Instance->Players.GetItem().User->Tuber->Position == 3)
								tmpMsg.WriteBool(true);
							else
								tmpMsg.WriteBool(false);
						}
						break;
					}
					
					WeServer::Instance()->Send(&tmpMsg);
					break;
				}else
				{
					winCounter++;
					if(winCounter == 1 && 
						(CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_1 ||
						 CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_2 ||
						 CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_3))
					{
						Res = true;
					}

					if(winCounter == 2 && 
						(CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_12 ||
						 CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_23 ))
					{
						Res = true;
					}

					if(winCounter == 3 && CGameSettings::Instance->MapSettings->EndCondition == RACEENDCONDITION_123)
					{
						Res = true;
					}
					
					if(Res)
					{
						WeNetworkMessage tmpMsg;
						tmpMsg.Init();
						tmpMsg.WriteMessageType(GAMEFINISHED);
						WeServer::Instance()->Send(&tmpMsg);
					}
				}
			}
		}
	}
	
	if(CGameSettings::Instance->MapSettings->RaceType == RACETYPE_LASTOUT)
	{
	}
	
	if(CGameSettings::Instance->MapSettings->RaceType == RACETYPE_TIMETRIAL)
	{
		for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
		{
			if((CGameSettings::Instance->MapSettings->TimeForRace - RaceTime) <= 0.0f && !CMainGamePhase::Instance->InputPlayer->RaceFinished)
			{
				CMainGamePhase::Instance->InputPlayer->LapCount--;
				WeNetworkMessage tmpMsg;
				tmpMsg.Init();
				tmpMsg.WriteMessageType(CLIENT_RACEFINISHED);
				tmpMsg.WriteInt(CGameSettings::Instance->Players.GetItem().ID);
				tmpMsg.WriteBool(false);			
				WeServer::Instance()->Send(&tmpMsg);

				WeNetworkMessage tmpMsg2;
				tmpMsg2.Init();
				tmpMsg2.WriteMessageType(GAMEFINISHED);
				WeServer::Instance()->Send(&tmpMsg2);
			}
			
			if(CMainGamePhase::Instance->InputPlayer->LapCount == CGameSettings::Instance->MapSettings->LapCount  && !CMainGamePhase::Instance->InputPlayer->RaceFinished)
			{
				CMainGamePhase::Instance->InputPlayer->LapCount--;
				WeNetworkMessage tmpMsg;
				tmpMsg.Init();
				tmpMsg.WriteMessageType(CLIENT_RACEFINISHED);
				tmpMsg.WriteInt(CGameSettings::Instance->Players.GetItem().ID);
				tmpMsg.WriteBool(true);			
				WeServer::Instance()->Send(&tmpMsg);

				WeNetworkMessage tmpMsg2;
				tmpMsg2.Init();
				tmpMsg2.WriteMessageType(GAMEFINISHED);
				WeServer::Instance()->Send(&tmpMsg2);
			}
		}
	}

}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::Server_CheckGameFinished()
{
	if(CGameServer::Instance->GetStatus() != GS_GAME_PLAYING) return;

	switch(CGameSettings::ServerInstance->MapType)
	{
	case CGameSettings::MAPTYPE_MP_SINGLERACE:
	case CGameSettings::MAPTYPE_SP_INSTANTRACE:
	case CGameSettings::MAPTYPE_SP_SINGLERACE:
		{
			DoSingleRaceAndInstantRaceLogiks();
		}
		break;
	case CGameSettings::MAPTYPE_SP_TOURNAMENT:
	case CGameSettings::MAPTYPE_MP_TOURNAMENT:
		{
			DoSPAndMPTournamentLogiks();
		}
		break;
	case CGameSettings::MAPTYPE_SP_TIMETRIAL:
		{
			DoSingleRaceTimeTrialLogiks();
		}
		break;
	case CGameSettings::MAPTYPE_SP_LASTONE:
	case CGameSettings::MAPTYPE_MP_LASTONE:
		{
			DoSPLastOneAndMPLastOneLogiks();
		}
		break;
	}
}






/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::MakeStatus()
{
	if (CGameClient::Instance->GetStatus() == GC_IDLE && Status != PLAYGAME_HOSTLEFT)	
	{
		Status = PLAYGAME_HOSTLEFT;
		// napravi ispis da se izaslo iz igre
		static WeFont *WinFont = WeCanvas::Instance()->MakeFont("Xirod", (int)(50.0f * (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f));
		CUberText *Writing = NULL;

		String s;
		s = CLocalizationSettings::Instance()->GetLocalText("HOSTLEFT");
		Writing = new CUberText(s, 0.5f - CUberText::MeasureTextWidth(s, WinFont) / 2.0f, 0.40f, UberTextTypeAnimFade, UberTextTypeAnimFade);
		Writing->SetTimeStay(3.0f);
		Writing->SetTimeShow(1.0f);
		Writing->SetTimeHide(1.0f);
		Writing->SetTextColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
		Writing->SetFont(WinFont);				
		CMainGameInterface::Instance->UberLines.PushBack(Writing);
	}

	if (CGameClient::Instance->GetStatus() == GC_WAITING_FOR_PLAYERS)	Status = PLAYGAME_WAITINGFORPLAYERS;
	if (CGameClient::Instance->GetStatus() == GC_PLAYING)
	{
		if (Status == PLAYGAME_WAITINGFORPLAYERS)
		{
			Status = PLAYGAME_PREPARECOUNTDOWN;
		}
	}
	if (CGameClient::Instance->GameFinished) Status = PLAYGAME_GAMEFINISHED;
}








/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::CheckReadyForRace()
{
	// Ako nemamo input player-a salji informaciju da smo spremni za trku svake sekunde
	if (!CMainGamePhase::Instance->InputPlayer) 
	{		
		static float tmpTimer = 0.0f;
		tmpTimer += WeTimer::Instance()->GetDeltaTimeF();
		if (tmpTimer > 1.0f)
		{
			CGameClient::Instance->ReadyForRace();			
			tmpTimer = 0.0f;
		}		
	}
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::PrepareAnimTextures(int i)
{
	switch (i)
	{
	case 0:
		//Anim321 = WeTextureManager::Instance()->LoadAnimTexture("Animacije.Proba_321.1");
		break;

	case 1:
		//AnimGO = WeTextureManager::Instance()->LoadAnimTexture("Animacije.Proba_321.2");
		break;

	case 2:
		//FinishTextWin = WeTextureManager::Instance()->LoadAnimTexture("Animacije.WinningText.Win");
		break;

	case 3:
		//FinishTextLose = WeTextureManager::Instance()->LoadAnimTexture("Animacije.WinningText.Lose");
		break;
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::TickPrepareCount()
{
	static float tmpCount = 0.0f;

	tmpCount += WeTimer::Instance()->GetDeltaTimeF();
	if (tmpCount >= 4.0f)
	{
		CountdownCounter = 0;
		CountdownTimer = 0.0f;
		Status = PLAYGAME_COUNTDOWN;
		tmpCount = 0.0f;
	}	
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::RenderCountDown()
{
	WeTexture *CDTex = NULL;
	CUberText *CDUber = NULL;
	WeSound *CountSound = NULL;

	CountdownTimer += WeTimer::Instance()->GetDeltaTimeF();

	bool CreateNew = false;

	switch (CountdownCounter)
	{
	case 0:
		//if (CountdownTimer < 0.6f)
		{
			CreateNew = true;
			CDTex = WeTextureManager::Instance()->Load("Ingame.Countdown.Three", true);
			CountSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.CountThree", false);
			CountdownCounter++;
		}
		break;
	case 1:
		//if ( (CountdownTimer >= 0.6f) && (CountdownTimer < 1.2f) )
		if (CountdownTimer >= 0.6f)
		{
			CreateNew = true;
			CDTex = WeTextureManager::Instance()->Load("Ingame.Countdown.Two", true);
			CountSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.CountTwo", false);
			CountdownCounter++;
		}
		break;
	case 2:
		//if ( (CountdownTimer >= 1.2f) && (CountdownTimer < 1.8f) )
		if (CountdownTimer >= 1.2f)
		{
			CreateNew = true;
			CDTex = WeTextureManager::Instance()->Load("Ingame.Countdown.One", true);
			CountSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.CountOne", false);
			CountdownCounter++;
		}
		break;
	case 3:
		//if ( (CountdownTimer >= 1.8f) && (CountdownTimer < 2.4f) )
		if (CountdownTimer >= 1.8f)
		{
			CreateNew = true;
			CDTex = WeTextureManager::Instance()->Load("Ingame.Countdown.Go", true);
			CountSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.CountGo", false);
			CountdownCounter++;
			Status = PLAYGAME_RACE;
		}
		break;
	}

	if (CreateNew && CDTex)
	{
		float WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
		float HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;
		float Width = RelX((int)((float)CDTex->Width * WOffset)) * 1.2f;
		float Height = RelY((int)((float)CDTex->Height * HOffset)) * 1.2f;

		CDUber = new CUberText(CDTex, 0.5f - (Width / 2.0f), 0.5f - (Height / 2.0f), Width, Height, UberTextTypeAnimFadeFrontTO);
		CDUber->SetTimeShow(0.7f);
		CDUber->SetTimeStay(0.0f);
		CDUber->SetTimeHide(0.0f);

		CMainGameInterface::Instance->UberLines.PushBack(CDUber);

		if (CountSound)
		{
			CountSound->Play(false);
			CountSound->DieOnStop();
		}
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::DoStatus()
{
	CStatistics::Instance->Update(*CGameSettings::ClientInstance);

	if (Status == PLAYGAME_WAITINGFORPLAYERS)
	{
		//if (CGame::Instance->InputMessage.LButton)
		//{
		//	CGame::Instance->SetPhase(CMenuPhase::Instance);
		//	CGameClient::Instance->Disconnect();
		//	CGameClient::Instance->ShutDown();
		//	CGameServer::Instance->StopHosting();
		//	CGameServer::Instance->ShutDown();

		//	if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_SINGLERACE || 
		//		CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL ||
		//		CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE)			
		//		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

		//	if (CGame::Instance->ServerInfo.LAN)
		//		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
		//}
	}


	if (Status == PLAYGAME_HOSTLEFT)
	{
		//if (CGame::Instance->InputMessage.LButton)
		//{
		static float Timer = 0.0f;
		Timer += WeTimer::Instance()->GetDeltaTimeF();
		if(Timer >= 5.0f)
		{		
			Timer = 0.0f;
			CGameClient::Instance->Disconnect();
			CGameClient::Instance->ShutDown();
			CGameServer::Instance->StopHosting();
			CGameServer::Instance->ShutDown();
			CMainGamePhase::Instance->SetPhase(CFinalizeGamePhase::Instance);
		}
	}

	if (Status == PLAYGAME_PREPARECOUNTDOWN)
	{
		TickPrepareCount();
	}

	if (Status == PLAYGAME_RACE)
	{
		if (CMainGamePhase::Instance->InputPlayer)
		{
			RaceTime += WeTimer::Instance()->GetDeltaTimeF();
            CMainGamePhase::Instance->InputPlayer->InputMessage = CGame::InputMessage;
			if (!AIKernel->HasAnAgent(CMainGamePhase::Instance->InputPlayer))
			{
				CMainGamePhase::Instance->InputPlayer->ProcessInput(&CGame::InputMessage); // make input message
				//CMainGamePhase::Instance->InputPlayer->ProcessInput(&CMainGamePhase::Instance->InputPlayer->InputMessage); // make input message
				WeClient::Instance()->SendDeltaCommand(CMainGamePhase::Instance->InputPlayer);
			}
		}		
		Server_CheckGameFinished(); // proveri jer kraj trke
		Server_SetPositions();		 // odredi pozicije brodova		

		// Get Physic Stats
		//WePhysics::Instance()->SaveSceneStats("physicstats.txt");
	}


	if (Status == PLAYGAME_GAMEFINISHED)
	{
		if (!AIKernel->HasAnAgent(CMainGamePhase::Instance->InputPlayer))
		{
			CMainGamePhase::Instance->InputPlayer->ProcessInput(&CGame::InputMessage); // make input message
			WeClient::Instance()->SendDeltaCommand(CMainGamePhase::Instance->InputPlayer);
		}

		if(GameFinishedTimer >= 7.0f)
		{		
			CMainGamePhase::Instance->SetPhase(CFinalizeGamePhase::Instance);
		}else
			GameFinishedTimer += WeTimer::Instance()->GetDeltaTimeF();
	}

}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::Tick()
{
	CMainGamePhase::Instance->DebugStr = "";
	CMainGamePhase::Instance->DebugStr1 = "";
	CMainGamePhase::Instance->DebugStr2 = "";
	CMainGamePhase::Instance->DebugStr3 = "";

	COptionsSettings::Instance->ControllerSettings.MakeInputMessage(&CGame::Instance->InputMessage);

	CheckReadyForRace();
	MakeStatus();
	DoStatus();


	float dtmp;
	dtmp = WeTimer::Instance()->GetDeltaTimeF();
	if (Status != PLAYGAME_GAMEPAUSED)
	{
		WeEngine::Instance()->Unpause();
		//AI tick
		if (AIKernel) AIKernel->Tick();
		CMainGamePhase::Instance->Map.Tick();
		CMainGamePhase::Instance->Camera.Update();
	}
	else
	{
		if (CGameClient::Instance->GetSinglePlayer())		
			WeEngine::Instance()->Pause();
	}

	CMainGameInterface::Instance->SetTuber(CMainGamePhase::Instance->InputPlayer);
	CMainGameInterface::Instance->Tick();


	if (ShowInGameMenu)
	{
		CMenuPhase::Instance->Tick();

		if (CGame::Instance->InputMessage.Exit)
		{
			ShowInGameMenu = false;
			Status = StatusSaved;
			CEGUI::WindowManager::getSingleton().getWindow("IN_GAME_MENU")->setVisible(false);
			//WeRenderDevice::Instance()->SetPixelMotionBlur(COptionsSettings::Instance->MotionBlur);
		}
	}
	else
	{
		if (CGame::Instance->InputMessage.Exit &&
			(Status == PLAYGAME_GAMEFINISHED ||
			 Status == PLAYGAME_HOSTLEFT ||
			 Status == PLAYGAME_RACE ||
			 Status == PLAYGAME_WAITINGFORPLAYERS)
			)
		{
			if (CMainGamePhase::Instance->InputPlayer->MuteAudioEnable)
				WeAudioDevice::Instance()->SetSoundVolume(CMainGamePhase::Instance->InputPlayer->MuteAudioSavedValue);

			ShowInGameMenu = true;
			StatusSaved = Status;
			Status = PLAYGAME_GAMEPAUSED;
			CMenuPhase::Instance->CurrentPhase = NULL;
			CMenuPhase::Instance->NextPhase = NULL;
			CEGUI::WindowManager::getSingleton().getWindow("IN_GAME_MENU")->setVisible(true);
			CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(WeRenderDevice::Instance()->GetResWidth()/2.0f,WeRenderDevice::Instance()->GetResHeight()/2.0f));
			//WeRenderDevice::Instance()->SetPixelMotionBlur(false);
		}
	}


	bool Ctrl = false;
	bool Alt = false;
	bool Shift = false;

	if (CGame::Instance->InputMessage.Key)
	{
		Ctrl =
			(CGame::Instance->InputMessage.Key[DIK_LCONTROL] & 0x80) ||
			(CGame::Instance->InputMessage.Key[DIK_RCONTROL] & 0x80);
		Shift =
			(CGame::Instance->InputMessage.Key[DIK_LSHIFT] & 0x80) ||
			(CGame::Instance->InputMessage.Key[DIK_RSHIFT] & 0x80);
		Alt =
			(CGame::Instance->InputMessage.Key[DIK_LALT] & 0x80) ||
			(CGame::Instance->InputMessage.Key[DIK_RALT] & 0x80);
	}


#ifdef  _DEBUG
	//AI observation TESTING AREA
	if (CGame::Instance->InputMessage.Key && !Ctrl && !Shift && !Alt)
	{
		if (!LocalHumanPlayer && CMainGamePhase::Instance->InputPlayer)
			LocalHumanPlayer = CMainGamePhase::Instance->InputPlayer;

		int i = -1;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD0] & 0x80) i = 0;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD1] & 0x80) i = 1;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD2] & 0x80) i = 2;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD3] & 0x80) i = 3;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD4] & 0x80) i = 4;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD5] & 0x80) i = 5;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD6] & 0x80) i = 6;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD7] & 0x80) i = 7;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD8] & 0x80) i = 8;
		if (CGame::Instance->InputMessage.Key[DIK_NUMPAD9] & 0x80) i = 9;

		if (i == 0)
			CMainGamePhase::Instance->InputPlayer = LocalHumanPlayer;
		else if (i > 0)
		{
			AIKernel->Agents.First();
			for (int z = 1; (!AIKernel->Agents.End() && (z < i)); z++, AIKernel->Agents.Next())
			{
				//NOP
			}
			if (!AIKernel->Agents.End() &&
				AIKernel->Agents.GetItem() &&
				AIKernel->Agents.GetItem()->GetAvatarConfig() &&
				AIKernel->Agents.GetItem()->GetAvatarConfig()->User &&
				AIKernel->Agents.GetItem()->GetAvatarConfig()->User->Tuber)
				CMainGamePhase::Instance->InputPlayer = AIKernel->Agents.GetItem()->GetAvatarConfig()->User->Tuber;
		}
	}
	//----------------------------

	// Sound Volume Balance TESTING AREA
	if (CGame::Instance->InputMessage.Key && CGame::Instance->InputMessage.Key[DIK_F8] & 0x80)
	{
		WeSoundAttenuator::Instance->Refresh();
	}
	// ---------------------------------
#endif

	// Change Camera
	if ((CMainGamePhase::Instance->InputPlayer) && (CGame::Instance->InputMessage.Key) && !Alt && !Shift && !Ctrl)
	{
		static bool CameraChanged = false;

		if (COptionsSettings::Instance->ControllerSettings.ChangeCamera)
		{
			if (CGame::Instance->InputMessage.Key[COptionsSettings::Instance->ControllerSettings.ChangeCamera] & 0x80)
			{
				if (!CameraChanged)
				{
					CMainGamePhase::Instance->Camera.SetNextCamera();
					CameraChanged = true;
				}
			}
			else
			{
				CameraChanged = false;
			}
		}
	}

	// Direct Camera
	if (!Ctrl && !Shift && !Alt && (CMainGamePhase::Instance->InputPlayer) && (CGame::Instance->InputMessage.Key))
	{
		static bool CameraSet[10] = {false, false, false, false, false, false, false, false, false, false};

		for (int i = 0; i < 10; ++i)
		{
			if (CGame::Instance->InputMessage.Key[DIK_1 + i] & 0x80)
			{
				if (!CameraSet[i])
				{
					CMainGamePhase::Instance->Camera.SetCamera(i);
					CameraSet[i] = true;
				}
			}
			else
				CameraSet[i] = false;
		}
	}

	// Reverse Camera
	if ((CMainGamePhase::Instance->InputPlayer) && (CGame::Instance->InputMessage.Key))
	{
		if (COptionsSettings::Instance->ControllerSettings.ReverseCamera)
		{
			CMainGamePhase::Instance->UseReverseCamera = ((CGame::Instance->InputMessage.Key[COptionsSettings::Instance->ControllerSettings.ReverseCamera] & 0x80) != 0);
			CMainGamePhase::Instance->Camera.UpdateCurrentCamera();
		}
	}

#ifdef _DEBUG

	if ( (CMainGamePhase::Instance->InputPlayer) && (CGame::Instance->InputMessage.Key) )
	{
		// Wireframe
		static bool WireClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F5] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!WireClicked)
			{
				WeRenderDevice::Instance()->SetWireFrame(!WeRenderDevice::Instance()->GetWireFrame());
				WireClicked = true;
			}
		}
		else
		{
			WireClicked = false;
		}

		// ShaderInfo
		static float ShowShaderInfoClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F4] & 0x80) && Shift && !Ctrl && !Alt)
		{
			if (!ShowShaderInfoClicked)
			{
				CMainGameInterface::Instance->ShowShaderInfo = !CMainGameInterface::Instance->ShowShaderInfo;
				ShowShaderInfoClicked = true;
			}
		}
		else
			ShowShaderInfoClicked = false;

		// ModelNames
		static float ShowModelNamesClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F6] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!ShowModelNamesClicked)
			{
				CMainGameInterface::Instance->ShowModelNames = !CMainGameInterface::Instance->ShowModelNames;
				ShowModelNamesClicked = true;
			}
		}
		else
			ShowModelNamesClicked = false;

		// ActorTypes
		if (Ctrl && Shift && !Alt)
		{
			static bool NumPadPressed[10] = {false, false, false, false, false, false, false, false, false, false};
			int NumPadKey[10] = {DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4, DIK_NUMPAD5, DIK_NUMPAD6, DIK_NUMPAD7, DIK_NUMPAD8, DIK_NUMPAD9, DIK_NUMPAD0};

			for (int iKey = 0; iKey < 10; ++iKey)
			{
				if (CGame::Instance->InputMessage.Key[NumPadKey[iKey]] & 0x80)
				{
					if (!NumPadPressed[iKey])
					{
						CMainGamePhase::Instance->Map.ShowActorTypes[iKey] = !CMainGamePhase::Instance->Map.ShowActorTypes[iKey];
						NumPadPressed[iKey] = true;
					}
					return;
				}
				else
					NumPadPressed[iKey] = false;
			}
		}

	}

	if (Ctrl && Shift && !Alt)
	{
		static bool PhaseKeyPressed[RENDERPHASE_COUNT] = {false, false, false, false, false, false, false, false, false, false};

		for (int iPhase = 0; iPhase < RENDERPHASE_COUNT; ++iPhase)
		{
			if (CGame::Instance->InputMessage.Key[DIK_1 + iPhase] & 0x80)
			{
				if (!PhaseKeyPressed[iPhase])
				{
					CMainGamePhase::Instance->Map.EnableRenderPhase(iPhase, !CMainGamePhase::Instance->Map.RenderPhaseEnabled(iPhase));
					PhaseKeyPressed[iPhase] = true;
				}
				return;
			}
			else
				PhaseKeyPressed[iPhase] = false;
		}
	}

	if (Ctrl && Shift && !Alt)
	{
		static bool PhaseKeyPressed[RENDERPHASE_COUNT] = {false, false, false, false, false, false, false, false, false, false};

		for (int iPhase = 0; iPhase < RENDERPHASE_COUNT; ++iPhase)
		{
			if (CGame::Instance->InputMessage.Key[DIK_1 + iPhase] & 0x80)
			{
				if (!PhaseKeyPressed[iPhase])
				{
					CMainGamePhase::Instance->Map.EnableRenderPhase(iPhase, !CMainGamePhase::Instance->Map.RenderPhaseEnabled(iPhase));
					PhaseKeyPressed[iPhase] = true;
				}
				return;
			}
			else
				PhaseKeyPressed[iPhase] = false;
		}
	}

#endif

	// HUD
	if ( (CMainGamePhase::Instance->InputPlayer) && (CGame::Instance->InputMessage.Key) )
	{
		static bool RenderableClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F1] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!RenderableClicked)
			{
				CMainGameInterface::Instance->Renderable = !CMainGameInterface::Instance->Renderable;
				RenderableClicked = true;
			}
		}
		else
		{
			RenderableClicked = false;
		}

		static float ShowWeaponClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F2] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!ShowWeaponClicked)
			{
				CMainGameInterface::Instance->ShowWeapon = !CMainGameInterface::Instance->ShowWeapon;
				ShowWeaponClicked = true;
			}
		}
		else
			ShowWeaponClicked = false;

		static float ShowStatsClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F3] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!ShowStatsClicked)
			{
				CMainGameInterface::Instance->ShowStats = !CMainGameInterface::Instance->ShowStats;
				ShowStatsClicked = true;
			}
		}
		else
			ShowStatsClicked = false;

		static float ShowFPSClicked = false;

		if ((CGame::Instance->InputMessage.Key[DIK_F4] & 0x80) && !Shift && !Ctrl && !Alt)
		{
			if (!ShowFPSClicked)
			{
				CMainGameInterface::Instance->ShowFPS = !CMainGameInterface::Instance->ShowFPS;
				ShowFPSClicked = true;
			}
		}
		else
			ShowFPSClicked = false;
	}

	// ==============================================================================

	TickPickupAnimation();
	TickCheats();
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::Render()
{
	if (!CMainGameInterface::Instance->Tuber) return;

	float WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	float HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;

	CMenuPhase::Instance->SaveStates();


	WePostProcess::Instance()->BeginScreenCapture();

	CMainGamePhase::Instance->Map.Render(&CMainGamePhase::Instance->Camera);

	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		CGamePlayerConfig *PlayerConfig = &CGameSettings::Instance->ClientInstance->tmpPlayers[i];
		if (!PlayerConfig->User) continue;
		if (!PlayerConfig->User->Tuber) continue;

		CTuber *OtherTuber = PlayerConfig->User->Tuber;
		if (OtherTuber->Dead) continue;

		if (OtherTuber->TuberWorld)
		{
			for (WeActor *Iterator = OtherTuber->TuberWorld->Actors->NextActor;
				Iterator != OtherTuber->TuberWorld->Actors;
				Iterator = Iterator->NextActor)
			{
				WeMatrix M;
				Iterator->PrevTM = Iterator->TM;
				D3DXMatrixMultiply(&M, &Iterator->TM, &OtherTuber->TM);
				Iterator->SetTransformation(M);
			}

			OtherTuber->TuberWorld->Render(&CMainGamePhase::Instance->Camera, false);

			for (WeActor *Iterator = OtherTuber->TuberWorld->Actors->NextActor;
				Iterator != OtherTuber->TuberWorld->Actors;
				Iterator = Iterator->NextActor)
			{
				Iterator->TM = Iterator->PrevTM;
			}
		}
	}

	WePostProcess::Instance()->EndScreenCapture();
	WePostProcess::Instance()->PostRender();


	CMainGameInterface::Instance->Render();

	CMenuPhase::Instance->SaveStates(false);


	if (ShowInGameMenu)
	{
		WeTexture *TexFiller = WeTextureManager::Instance()->Load("MapEditor.Textures.DefaultWhite1x1", true);
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), 0x80000000, TexFiller);
		CEGUI::System::getSingleton().renderGUI();
	}

#ifdef _DEBUG
	char ss[1000];	
	String stmp;
	for (WeProfiler::Instance()->ShadersActive.First(); !WeProfiler::Instance()->ShadersActive.End(); WeProfiler::Instance()->ShadersActive.Next())
	{
		sprintf(ss,"%s\n", WeProfiler::Instance()->ShadersActive.GetItem());
		stmp = stmp + ss;
	}

	static bool b = false;
	if (!b)
	{
		CMainGamePhase::Instance->Map.RenderTree->SaveToXMLFile("world.xml");
		b = true;
	}

	sprintf(ss, "Nodes visible: %d, NodesCount: %d, ProcessedNodes: %d\n", 
		WeProfiler::Instance()->RenderTreeNodesVisible, CMainGamePhase::Instance->Map.RenderTree->GetNodesCount(), CMainGamePhase::Instance->Map.RenderTree->GetProcessedNodes());
	stmp = stmp + ss;

	sprintf(ss, "\nNum Polygons: %d, Objects Visible: %d", WeProfiler::Instance()->PolygonsRendered, WeProfiler::Instance()->ObjectsRendered);
	stmp = stmp + ss;

	//sprintf(ss,"RenderTree Nodes Visible: %d\nNodes Count: %d, MaxDepth: %d, Contains: %d\
	//		   \nPolygons rendererd: %d, Shader begin: %d",
	//	WeProfiler::Instance()->RenderTreeNodesVisible, EditorWindow->MapEditorWorld->RenderTree->GetNodesCount(), EditorWindow->MapEditorWorld->RenderTree->GetProcessedDepth(), WeProfiler::Instance()->tmp,
	//	WeProfiler::Instance()->PolygonsRendered, WeProfiler::Instance()->ShaderBegins);
	//sprintf(ss,"Visible Zones: %d:",
	//	WeProfiler::Instance()->ZonesVisible);
	//for (WeProfiler::Instance()->Zones.First(); !WeProfiler::Instance()->Zones.End(); WeProfiler::Instance()->Zones.Next())
	//	sprintf(ss, "%s %s", ss, WeProfiler::Instance()->Zones.GetItem().GetString());

	if (CMainGameInterface::Instance->ShowShaderInfo)
		WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font16,stmp,50,150,1300,400,D3DCOLOR_XRGB(255,255,255));


	WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->FontFixed, CMainGamePhase::Instance->DebugStr.GetString(), 0, 340, 800, 200, 0xffffffff);
	WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font12, CMainGamePhase::Instance->DebugStr1.GetString(), 0, 360, 800, 200, 0xffffffff);
	WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font12, CMainGamePhase::Instance->DebugStr2.GetString(), 0, 380, 800, 200, 0xffffffff);
	WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font12, CMainGamePhase::Instance->DebugStr3.GetString(), 0, 400, 800, 200, 0xffffffff);
#endif


	if (CGameClient::Instance->LastPlayerKicked.GetLen() != 0)
	{
		String s;
		char uber[100];
		s = CLocalizationSettings::Instance()->GetLocalText("LEFTGAME");
		sprintf(uber, s, CGameClient::Instance->LastPlayerKicked);
		CUberText *Writing;
		Writing = new CUberText(String(uber));
		CMainGameInterface::Instance->UberLines.PushBack(Writing);

		CGameClient::Instance->LastPlayerKicked = "";
	}



	if (Status == PLAYGAME_WAITINGFORPLAYERS)
	{
		CMainGameInterface::DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("WAITING_FOR_PLAYERS"), CMainGameInterface::Instance->Font32, 0.25f, 0.47f);
	}

	//// =========================== 
	if (Status == PLAYGAME_COUNTDOWN)
	{
		RenderCountDown();
	}

	if (Status == PLAYGAME_RACE)
	{
		//if(AnimGO && AnimGO->GetIsPlayin())
		//{
		//	float tmpX = (float)WeRenderDevice::Instance()->GetResWidth() / 2.0f - (float)AnimGO->GetWidth() * WOffset;
		//	float tmpY = (float)WeRenderDevice::Instance()->GetResHeight() / 2.0f - (float)AnimGO->GetHeight() * HOffset;

		//	WeCanvas::Instance()->DrawRectangle((int)tmpX, (int)tmpY, (int)((float)AnimGO->GetWidth() * WOffset * 2), (int)((float)AnimGO->GetHeight() * HOffset * 2), D3DCOLOR_XRGB(255,255,255), AnimGO->GetCurrentTexture());
		//}else{
		//	if(AnimGO)
		//		WeTextureManager::Instance()->FreeAnimTexture(AnimGO);
		//	if(Anim321)
		//		WeTextureManager::Instance()->FreeAnimTexture(Anim321);
		//	AnimGO = NULL;
		//}
	}
	RenderSafetyLock();

	if(CMainGamePhase::Instance->InputPlayer && CMainGamePhase::Instance->InputPlayer->RaceFinished)
		RenderFinishAnim();

	//if (Status == PLAYGAME_RACE)
	//{
	//	char tmp[1000];
	//	String s;
	//	for (WeNetworkActor::NetworkActors.First(); !WeNetworkActor::NetworkActors.End(); WeNetworkActor::NetworkActors.Next())
	//	{
	//		WeQuaternion quat;
	//		WeNetworkActor::NetworkActors.GetItem()->PhysicHandle->GetOrientationQuat(quat);
	//		//sprintf(tmp, "%3.3f, %3.3f, %3.3f", CMainGamePhase::Instance->InputPlayer->Location.x, CMainGamePhase::Instance->InputPlayer->Location.y, CMainGamePhase::Instance->InputPlayer->Location.z);
	//		if (WeNetworkActor::NetworkActors.GetItem() == CMainGamePhase::Instance->InputPlayer)
	//			sprintf(tmp, "ID: %d [ %3.3f, %3.3f, %3.3f ] AngVelocity [ %3.3f, %3.3f, %3.3f, %3.3f] %s *\n", WeNetworkActor::NetworkActors.GetItem()->GetID(), WeNetworkActor::NetworkActors.GetItem()->Location.x, WeNetworkActor::NetworkActors.GetItem()->Location.y, WeNetworkActor::NetworkActors.GetItem()->Location.z, 
	//			quat.x, quat.y, quat.z, quat.w, 
	//				((CTuber *)WeNetworkActor::NetworkActors.GetItem())->OwnerProfile->Name);
	//		else
	//		sprintf(tmp, "ID: %d [ %3.3f, %3.3f, %3.3f ] AVel [ %3.3f, %3.3f, %3.3f, %3.3f ] %s \n", WeNetworkActor::NetworkActors.GetItem()->GetID(), WeNetworkActor::NetworkActors.GetItem()->Location.x, WeNetworkActor::NetworkActors.GetItem()->Location.y, WeNetworkActor::NetworkActors.GetItem()->Location.z, 
	//			quat.x, quat.y, quat.z, quat.w,
	//			((CTuber *)WeNetworkActor::NetworkActors.GetItem())->OwnerProfile->Name);
	//		s = s + tmp;
	//	}
	//

	//	//for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
	//	//{
	//	//	char tmpLCount[100];
	//	//	sprintf(tmpLCount,"Player ID: %d, Lap: %d, Nodes: %d\n",CGameSettings::Instance->Players.GetItem().ID, CGameSettings::Instance->Players.GetItem().User->Tuber->LapCount, CGameSettings::Instance->Players.GetItem().User->Tuber->PathNodeCounter);
	//	//	s = s + tmpLCount;
	//	//}

	String s;
	char tmp[100];

	//sprintf(tmp, "Camera: %3.3f, %3.3f, %3.3f,\n", CMainGamePhase::Instance->Camera.Location.x, CMainGamePhase::Instance->Camera.Location.y, CMainGamePhase::Instance->Camera.Location.z);
	//s = s + tmp;

	if (CGameSettings::ClientInstance->MapType == CGameSettings::ClientInstance->MAPTYPE_SP_TIMETRIAL)
	{
		if (CMainGamePhase::Instance->InputPlayer)
		{
			float tempF = TimeTrialLapTimeLimit - TimeTrialLapTime;
			if (tempF < 0.0f)
			{
				tempF = 0.0f;
			}
			sprintf(tmp, CLocalizationSettings::Instance()->GetLocalText("TIME_TRIAL_SECONDS_LEFT"), tempF);
			s = String(tmp);
			WeColor Color;
			if ( (Status == PLAYGAME_GAMEFINISHED) && (CMainGamePhase::Instance->InputPlayer->Win) )
				Color = WeColor(0.0f, 1.0f, 0.0f, 0.7f);
			else if (tempF < TimeTrialLapTimeLimit * 0.3f)
			{
				Color = WeColor(1.0f, 0.0f, 0.0f, 0.7f);
				if (Status != PLAYGAME_GAMEFINISHED)
					CMainGameInterface::Instance->SetTimeTextBeepFactor(1.0f - (tempF / (TimeTrialLapTimeLimit * 0.3f)));
				else
					CMainGameInterface::Instance->SetTimeTextBeepFactor(0.0f);
			}
			else
				Color = WeColor(1.0f, 1.0f, 1.0f, 0.7f);

			CMainGameInterface::DrawShadowedText(s, CMainGameInterface::Instance->Font20, 0.35f, 0.9f, Color);
		}
	//}else if(CGameSettings::ClientInstance->MapSettings->RaceType == RACETYPE_TIMETRIAL)
	//{
	//	float tempF = CGameSettings::ClientInstance->MapSettings->TimeForRace - CPlayGamePhase::Instance->RaceTime;
	//	if(tempF < 0.0f) tempF = 0.0f;
	//	sprintf(tmp,"TIME LEFT: %.3f\n", tempF);
	//	s = s + tmp;		
	}

	//WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font24, s, 302, 102, 500, 600, D3DCOLOR_XRGB(155,155,155));
	//WeCanvas::Instance()->Draw2DText(CMainGameInterface::Instance->Font24, s, 300, 100, 500, 600, D3DCOLOR_XRGB(255,255,255));

	//	for (int i = 0; i < MAX_PLAYERS; i++)
	//	{
	//		if (CGameSettings::ClientInstance->tmpPlayers[i].User)
	//		{
	//			if (CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber)
	//			{
	//				sprintf(tmp, "Index: %d Lap Time: %3.3f\n", i, CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber->LapTime);
	//				s = s + tmp;
	//			}
	//		}
	//	}

	//	sprintf(tmp, "Client Ping: %d\n", WeClient::Instance()->GetPing());
	//	s = s + tmp;
	//	//for (WeProfiler::Instance()->ShadersActive.First(); !WeProfiler::Instance()->ShadersActive.End(); WeProfiler::Instance()->ShadersActive.Next())
	//	//{
	//	//	sprintf(tmp, "Shader Begin: %s\n", WeProfiler::Instance()->ShadersActive.GetItem());
	//	//	s = s + tmp;
	//	//}

	//	/*sprintf(tmp, "ClientSend: %d bytes, ClientReceive %d\n", WeProfiler::Instance()->ClientSendBytesPerSec, WeProfiler::Instance()->ClientRecvBytesPerSec);
	//	s = s + tmp;
	//	sprintf(tmp, "ServerSend: %d bytes, ServerReceive %d\n", WeProfiler::Instance()->ServerSendBytesPerSec, WeProfiler::Instance()->ClientSendBytesPerSec);
	//	s = s + tmp;
	//	sprintf(tmp, "\n\nCurrent Speed: %3.3f  Max Speed: %3.3f\n", CMainGamePhase::Instance->InputPlayer->CurrentSpeed, CMainGamePhase::Instance->InputPlayer->MaxSpeed);
	//	s = s + tmp;
	//	sprintf(tmp, "\n\nOOu the HP: %d\n", CMainGamePhase::Instance->InputPlayer->HP);
	//	s = s + tmp;
	//	sprintf(tmp, "\n\nPath node counter: %d\n", CMainGamePhase::Instance->InputPlayer->PathNodeCounter);
	//	s = s + tmp;*/
	//	
	//	WeCanvas::Instance()->Draw2DText(Font16, s, 10, 100, 500, 600, D3DCOLOR_XRGB(255,255,255));
	//}
		//WeCanvas::Instance()->Draw2DText(Font16, s, 10, 100, 500, 600, D3DCOLOR_XRGB(255,255,255));
	//}
}




struct Info
{
	CTuber* Tuber;
};


int CompareTuberPositions(CTuber* Tuber1, CTuber* Tuber2)
{
	int result;
	
	result = Tuber1->ActualLapCount - Tuber2->ActualLapCount;
	if (result != 0)
		return result;

	result = Tuber1->RefPathNode->SortedIndex - Tuber2->RefPathNode->SortedIndex;
	if (result != 0)
		return result;

	float diff = (Tuber1->RefDistance - Tuber2->RefDistance);
	if (diff < 0)
		return -1;
	else if (diff > 0)
		return +1;
	else
		return 0;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Set positions
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::Server_SetPositions()
{

	if (CGameServer::Instance->GetStatus() != GS_GAME_PLAYING) return;

	CGamePlayerConfig *pIter = NULL;
	Info InfoTable[MAX_PLAYERS];
	int InfoCount = 0;
	int Position = 1;


	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User)
		{				
			CTuber* Tuber = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;
			if (!Tuber->RaceFinished)
			{
				InfoTable[InfoCount].Tuber = Tuber;
				++InfoCount;
			}
			else {
				if ((CGameSettings::ServerInstance->MapType != CGameSettings::MAPTYPE_MP_LASTONE) &&
					(CGameSettings::ServerInstance->MapType != CGameSettings::MAPTYPE_SP_LASTONE))
				{
					++Position;
				}
			}
		}
	}

	// sortiraj node po <Lap, Node, Distance>
	// najveca pozicija je na kraju
	Info tmp;
	int max;
	for (int j = InfoCount - 1; j >= 1; j--)
	{
		max = j;

		for (int i = j - 1; i >= 0; i--)
		{
			if (CompareTuberPositions(InfoTable[i].Tuber, InfoTable[max].Tuber) > 0)
				max = i;
		}

		if (max != j)
		{
			tmp = InfoTable[j];
			InfoTable[j] = InfoTable[max];
			InfoTable[max] = tmp;
		}
	}


	for (int i = InfoCount - 1; i >= 0; i--)
	{
		InfoTable[i].Tuber->Position = Position;
		Position++;
	}


/*
	// odredi pos da krene od tubera koji nisu zavrsili trku
	int pos = 1;
	if ((CGameSettings::ServerInstance->MapType != CGameSettings::MAPTYPE_MP_LASTONE) &&
		(CGameSettings::ServerInstance->MapType != CGameSettings::MAPTYPE_SP_LASTONE))
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			CProfile* User = CGameSettings::ServerInstance->tmpPlayers[i].User;
			if (User)
			{
				if (User->Tuber->RaceFinished)
					pos++;
			}
		}
	}
*/


	//for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
	//{
	//	Info In;
	//	pIter = &CGameSettings::Instance->Players.GetItem();
	//	if(pIter->User->Tuber) In.Node = pIter->User->Tuber->PathNodeCounter;
	//	else In.Node = 0;
	//	In.ID = pIter->ID;

	//	Players.First();
	//	bool toEnd = true;
	//	while(!Players.End())
	//	{
	//		if(Players.GetItem().Node < In.Node)
	//		{
	//			toEnd=false;
	//			break;
	//		}
	//		Players.Next();
	//	}
	//	if(toEnd) Players.PushBack(In);
	//	else Players.InsertBefore(In);
	//}

	//for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
	//{
	//	pIter = &CGameSettings::Instance->Players.GetItem();
	//	int i;
	//	for(i=1, Players.First(); !Players.End(); i++, Players.Next())
	//	{
	//		if(pIter->ID == Players.GetItem().ID)
	//		{
	//			if(pIter->User->Tuber) pIter->User->Tuber->Position = i;
	//			break;
	//		}
	//	}
	//}

	//FILE *f = fopen("PosSort.txt", "a+");
	//for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
	//{
	//	fprintf(f,"\t\tNode: %d\n\t\tPos: %d\n\t\tID: %d\n",CGameSettings::Instance->Players.GetItem().User->Tuber->PathNodeCounter,
	//														CGameSettings::Instance->Players.GetItem().User->Tuber->Position,
	//														CGameSettings::Instance->Players.GetItem().ID );
	//}
	//fclose(f);
}




bool CPlayGamePhase::OnBackToMenu(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CPlayGamePhase::Instance->Status = PLAYGAME_GAMEFINISHED;
	CMainGamePhase::Instance->SetPhase(CFinalizeGamePhase::Instance);
	return true;
}




bool CPlayGamePhase::OnBackToGame(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CPlayGamePhase::Instance->ShowInGameMenu = false;
	CPlayGamePhase::Instance->Status = PLAYGAME_RACE;
	//WeRenderDevice::Instance()->SetPixelMotionBlur(COptionsSettings::Instance->MotionBlur);
	return true;
}




bool CPlayGamePhase::OnOptionsMenu(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	winMgr.getWindow("IN_GAME_MENU")->setVisible(false);

	CMenuPhase::Instance->SetPhase(COptionsPhase::Instance);
	COptionsPhase::Instance->FromInGame = true;

	return true;
}




bool CPlayGamePhase::OnExitGame(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CGameClient::Instance->ShutDown();
	CGameServer::Instance->ShutDown();

	CGame::Instance->SetPhase(CMenuPhase::Instance);	
	CMenuPhase::Instance->SetPhase(CExitMenuPhase::Instance);
	return true;
}



void CPlayGamePhase::OnTeleport(WeActor *Actor, WeActor *Desc)
{
	if (Actor->Type == ACTORTYPE_TUBER)
	{		
		((CTuber *)Actor)->Trail->Renderable = false;
		((CTuber *)Actor)->AfterTeleportedTimer = 0.0f;
	}

	if (CMainGamePhase::Instance->InputPlayer == Actor)
		CMainGamePhase::Instance->InputPlayer->CurrentFlash = TuberFlashTeleport;
}



bool CPlayGamePhase::ActorPushable(WeActor *Who)
{
	if (!Who) return false;

	switch (Who->Type)
	{
	case ACTORTYPE_CHECKPOINT:
	case ACTORTYPE_EXPLOSION:
	case ACTORTYPE_PATHNODE:
	case ACTORTYPE_PHYSICMODEL:
	case ACTORTYPE_STARTINGLOCATION:
	case ACTORTYPE_TRIGGER:
	case ACTORTYPE_TUBEPART:
	case ACTORTYPE_WEAPON:
		return false;
	}
	if (Who->Type > 10000)
		return false;

	return true;
}

void CPlayGamePhase::RenderFinishAnim()
{
	float WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	float HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;

	if (!EndAnimPlayed)
	{
		EndAnimPlayed = true;

		WeTexture *CDTex = NULL;
		CUberText *CDUber = NULL;
		WeSound *EndSound = NULL;

		if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_SINGLERACE ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_SINGLERACE ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_INSTANTRACE ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_TOURNAMENT ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
		{
			switch (CMainGamePhase::Instance->InputPlayer->Position)
			{
			case 1:
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.1st place", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndFirstPlace", false);
				break;
			case 2:
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.2nd place", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndSecondPlace", false);
				break;
			case 3:
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.3rd place", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndThirdPlace", false);
				break;
			default:
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.YouLose", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndYouLose", false);
				break;
			}
		}
		else //if	(CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL ||
			//	 CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE ||
			//	 CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_LASTONE)
		{
			if (CMainGamePhase::Instance->InputPlayer->Win)
			{
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.YouWin", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndYouWin", false);
			}
			else
			{
				CDTex = WeTextureManager::Instance()->Load("Ingame.Misc.YouLose", true);
				EndSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.EndYouLose", false);
			}
		}

		if (CDTex)
		{
			float WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
			float HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;
			float Width = RelX((int)((float)CDTex->Width * WOffset)) * 1.0f;
			float Height = RelY((int)((float)CDTex->Height * HOffset)) * 1.0f;

			CDUber = new CUberText(CDTex, 0.5f - (Width / 2.0f), 0.5f - (Height / 2.0f), Width, Height, UberTextTypeAnimFadeBackTO, UberTextTypeAnimFade);
			CDUber->SetTimeShow(1.0f);
			CDUber->SetTimeStay(6.0f);
			CDUber->SetTimeHide(0.0f);

			CMainGameInterface::Instance->UberLines.PushBack(CDUber);

			if (EndSound)
			{
				EndSound->Play(false);
				EndSound->DieOnStop();
			}
		}
	}


	//if (!WinningText && !EndAnimPlayed)
	//{
	//	if(CMainGamePhase::Instance->InputPlayer->Win)
	//	{
	//		WinningText = FinishTextWin;
	//		WeTextureManager::Instance()->FreeAnimTexture(FinishTextLose);
	//	}
	//	else
	//	{
	//		WinningText = FinishTextLose;
	//		WeTextureManager::Instance()->FreeAnimTexture(FinishTextWin);
	//	}
	//	FinishTextWin = NULL;
	//	FinishTextLose = NULL;

	//	WinningText->Play();
	//	WinningText->SetLooped(false);
	//	EndAnimPlayed = true;
	//}

	//if (WinningText && WinningText->GetIsPlayin())
	//{
	//	float tmpX = (float)WeRenderDevice::Instance()->GetResWidth() / 2.0f - (float)WinningText->GetWidth() * WOffset;
	//	float tmpY = (float)WeRenderDevice::Instance()->GetResHeight() / 2.0f - (float)WinningText->GetHeight() * HOffset;

	//	WeCanvas::Instance()->DrawRectangle((int)tmpX, (int)tmpY, (int)((float)WinningText->GetWidth() * WOffset * 2), (int)((float)WinningText->GetHeight() * HOffset * 2), D3DCOLOR_XRGB(255,255,255), WinningText->GetCurrentTexture());
	//}
	//else if (WinningText && !WinningText->GetIsPlayin() && EndAnimPlayed)
	//{
	//	WeTextureManager::Instance()->FreeAnimTexture(WinningText);
	//	WinningText = NULL;
	//}
}


void CPlayGamePhase::TickPickupAnimation()
{
	bool bMine = false;
	bool bWeapon = false;
	for(CPickUpAnimation::PickupAnimations.First(); !CPickUpAnimation::PickupAnimations.End(); CPickUpAnimation::PickupAnimations.Next())
	{
		if(CPickUpAnimation::PickupAnimations.GetItem()->AnimatingMine) bMine = true; else bMine = false;
		if(CPickUpAnimation::PickupAnimations.GetItem()->AnimatingWeapon) bWeapon = true; else bWeapon = false;

		CPickUpAnimation::PickupAnimations.GetItem()->Tick();
		if(!CPickUpAnimation::PickupAnimations.GetItem()->AnimatingMine && bMine)
		{
			CMainGameInterface::Instance->Mine = CPickUpAnimation::PickupAnimations.GetItem()->Mine;
			delete CPickUpAnimation::PickupAnimations.GetItem();
			CPickUpAnimation::PickupAnimations.Delete();
			continue;
		}
		if(!CPickUpAnimation::PickupAnimations.GetItem()->AnimatingWeapon && bWeapon)
		{
			CMainGameInterface::Instance->Weapon = CPickUpAnimation::PickupAnimations.GetItem()->Weapon;
			delete CPickUpAnimation::PickupAnimations.GetItem();
			CPickUpAnimation::PickupAnimations.Delete();
			continue;
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// Cheat Legend:

// Alt + Shift +
// I = Invulnerability
// A = Primary & Secondary, Basic & Addon Ammo
// S = Strip Primary and Secondary Addon Weapons
// K = Suicide
// T = Infinite Turbo
// D = Normal/Quad Damage
// W = AI Weapons
// M = AI Movement
// H = Reload HP & Turbo
// U = Unlimited Speed
// V = Invisibility
// 1,2,3,4,5,6,7,8,9,0,-,= = Primary Addon Weapons

// Alt + Shift + Ctrl +
// 1,2,3,4,5,6,7,8,9,0,-,= = Secondary Addon Weapons
//////////////////////////////////////////////////////////////////////////
void CPlayGamePhase::TickCheats()
{
	if (!CGame::Instance->InputMessage.Key) return;
	if (!CMainGamePhase::Instance->InputPlayer) return;

	bool Ctrl = 
		(CGame::Instance->InputMessage.Key[DIK_LCONTROL] & 0x80) ||
		(CGame::Instance->InputMessage.Key[DIK_RCONTROL] & 0x80);
	bool Shift =
		(CGame::Instance->InputMessage.Key[DIK_LSHIFT] & 0x80) ||
		(CGame::Instance->InputMessage.Key[DIK_RSHIFT] & 0x80);
	bool Alt =
		(CGame::Instance->InputMessage.Key[DIK_LALT] & 0x80) ||
		(CGame::Instance->InputMessage.Key[DIK_RALT] & 0x80);

	int iNum = 0;
	if (CGame::Instance->InputMessage.Key[DIK_1] & 0x80) iNum = 1;
	if (CGame::Instance->InputMessage.Key[DIK_2] & 0x80) iNum = 2;
	if (CGame::Instance->InputMessage.Key[DIK_3] & 0x80) iNum = 3;
	if (CGame::Instance->InputMessage.Key[DIK_4] & 0x80) iNum = 4;
	if (CGame::Instance->InputMessage.Key[DIK_5] & 0x80) iNum = 5;
	if (CGame::Instance->InputMessage.Key[DIK_6] & 0x80) iNum = 6;
	if (CGame::Instance->InputMessage.Key[DIK_7] & 0x80) iNum = 7;
	if (CGame::Instance->InputMessage.Key[DIK_8] & 0x80) iNum = 8;
	if (CGame::Instance->InputMessage.Key[DIK_9] & 0x80) iNum = 9;
	if (CGame::Instance->InputMessage.Key[DIK_0] & 0x80) iNum = 10;
	if (CGame::Instance->InputMessage.Key[DIK_MINUS] & 0x80) iNum = 11;
	if (CGame::Instance->InputMessage.Key[DIK_EQUALS] & 0x80) iNum = 12;

	static bool KeyDown = false;

	if (!Alt || !Shift) return;

	bool CheatEntered = false;
	bool CheatStatus;
	String CheatInfo;

	if (CGame::Instance->InputMessage.Key[DIK_I] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "Invulnerability";

			bool Found = false;
			CTuber *Tuber = CMainGamePhase::Instance->InputPlayer;
			for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
				if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionInvulnerability)
				{
					delete Tuber->PickupActions.GetItem();
					Tuber->PickupActions.Delete();
					Found = true;
					break;
				}
				if (!Found)
				{
					CPickupActionInvulnerability* PickupAction = new CPickupActionInvulnerability();
					PickupAction->SetValidityTime(3600.0f);
					Tuber->PickupActions.PushBack(PickupAction);
				}
				CheatStatus = !Found;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_A] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatStatus = true;
			CheatInfo = "Ammo";

			CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAmmo = 999;
			CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAmmo = 999;
			if (CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAddonID != WEAPONTYPE_NONE)
				CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAmmo2 = 999;
			if (CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAddonID != WEAPONTYPE_NONE)
				CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAmmo2 = 999;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_S] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatStatus = true;
			CheatInfo = "Strip Addon Weapons";
			CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAddonID = WEAPONTYPE_NONE;
			CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAmmo2 = 0;
			CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAddonID = WEAPONTYPE_NONE;
			CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAmmo2 = 0;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_K] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatStatus = true;
			CheatInfo = "Suicide";
			CMainGamePhase::Instance->InputPlayer->Die();
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_T] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "Infinite Turbo";
			CMainGamePhase::Instance->InputPlayer->InfTurbo = !CMainGamePhase::Instance->InputPlayer->InfTurbo;
			CheatStatus = CMainGamePhase::Instance->InputPlayer->InfTurbo;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_D] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "Quad Damage";

			bool Found = false;
			CTuber *Tuber = CMainGamePhase::Instance->InputPlayer;
			for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
				if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionQuadDamage)
				{
					delete Tuber->PickupActions.GetItem();
					Tuber->PickupActions.Delete();
					Found = true;
					break;
				}
				if (!Found)
				{
					CPickupActionQuadDamage* PickupAction = new CPickupActionQuadDamage();
					PickupAction->SetValidityTime(3600.0f);
					Tuber->PickupActions.PushBack(PickupAction);
				}
				CheatStatus = !Found;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_W] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "AI Weapons";
			AIKernel->SetArmoryStatus(!AIKernel->GetArmoryStatus());
			CheatStatus = AIKernel->GetArmoryStatus();
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_M] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "AI Movement";
			AIKernel->SetMotoricsStatus(!AIKernel->GetMotoricsStatus());
			CheatStatus = AIKernel->GetMotoricsStatus();
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_H] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatStatus = true;
			CheatInfo = "Recharge Health and Turbo";
			CMainGamePhase::Instance->InputPlayer->HP = CMainGamePhase::Instance->InputPlayer->MaxHP;
			CMainGamePhase::Instance->InputPlayer->TurboCharges = CMainGamePhase::Instance->InputPlayer->MaxTurboCharges;
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_U] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "Unlimited Speed";
			if (CMainGamePhase::Instance->InputPlayer->MaxSpeed == 10000.0f)
			{
				CheatStatus = false;
				CMainGamePhase::Instance->InputPlayer->MaxSpeed = CMainGamePhase::Instance->InputPlayer->Description.MaxSpeed;
				CMainGamePhase::Instance->InputPlayer->TurboMaxSpeed = CMainGamePhase::Instance->InputPlayer->Description.MaxSpeed * 1.2f;
			}
			else
			{
				CheatStatus = true;
				CMainGamePhase::Instance->InputPlayer->MaxSpeed = 10000.0f;
				CMainGamePhase::Instance->InputPlayer->TurboMaxSpeed = 12000.0f;
			}
		}
	}
	else if (CGame::Instance->InputMessage.Key[DIK_V] & 0x80)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatInfo = "Invisibility";

			bool Found = false;
			CTuber *Tuber = CMainGamePhase::Instance->InputPlayer;
			for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
				if (Tuber->PickupActions.GetItem()->GetActionType() == PickupActionInvisibility)
				{
					delete Tuber->PickupActions.GetItem();
					Tuber->PickupActions.Delete();
					Found = true;
					break;
				}
			if (!Found)
			{
				CPickupActionInvisibility* PickupAction = new CPickupActionInvisibility();
				PickupAction->SetValidityTime(3600.0f);
				Tuber->PickupActions.PushBack(PickupAction);
			}
			CheatStatus = !Found;
		}
	}
	else if (iNum > 0)
	{
		if (!KeyDown)
		{
			CheatEntered = true;
			CheatStatus = true;
			
			//////////////////////////////////////////////////////////////////////////
			// for demo only
			Ctrl = false;
			//////////////////////////////////////////////////////////////////////////

			WEAPONTYPE MyWeapon = WEAPONTYPE_NONE;
			switch(iNum)
			{
				//////////////////////////////////////////////////////////////////////////
				// for demo only
			case 1:
				MyWeapon = WEAPONTYPE_STANDARDLASER;
				break;
			case 2:
				MyWeapon = WEAPONTYPE_QUANTUMLASER;
				break;
			case 3:
				MyWeapon = WEAPONTYPE_STANDARDROCKET;
				break;
			case 4:
				MyWeapon = WEAPONTYPE_SLOWINGROCKET;
				break;
			case 5:
				MyWeapon = WEAPONTYPE_TRANSLOCATOR;
				break;
			case 6:
				MyWeapon = WEAPONTYPE_HOMINGMINE;
				Ctrl = true;
				break;
			case 7:
				MyWeapon = WEAPONTYPE_SMOKEMINE;
				Ctrl = true;
				break;
				//////////////////////////////////////////////////////////////////////////

			//case 1:
			//	MyWeapon = Ctrl ? WEAPONTYPE_STANDARDMINE : WEAPONTYPE_STANDARDLASER;
			//	break;
			//case 2:
			//	MyWeapon = Ctrl ? WEAPONTYPE_BLURMINE : WEAPONTYPE_DOUBLELASER;
			//	break;
			//case 3:
			//	MyWeapon = Ctrl ? WEAPONTYPE_ELECTRICMINE : WEAPONTYPE_QUADLASER;
			//	break;
			//case 4:
			//	MyWeapon = Ctrl ? WEAPONTYPE_GRAVITYMINE : WEAPONTYPE_STANDARDROCKET;
			//	break;
			//case 5:
			//	MyWeapon = Ctrl ? WEAPONTYPE_HOMINGMINE : WEAPONTYPE_QUANTUMLASER;
			//	break;
			//case 6:
			//	MyWeapon = Ctrl ? WEAPONTYPE_SLOWINGMINE : WEAPONTYPE_MINIROCKETS;
			//	break;
			//case 7:
			//	MyWeapon = Ctrl ? WEAPONTYPE_SUPERNUKEMINE : WEAPONTYPE_NUKEROCKET;
			//	break;
			//case 8:
			//	MyWeapon = Ctrl ? WEAPONTYPE_INVISIBLEMINE : WEAPONTYPE_MINIGUN;
			//	break;
			//case 9:
			//	MyWeapon = Ctrl ? WEAPONTYPE_MOVINGMINE : WEAPONTYPE_SLOWINGROCKET;
			//	break;
			//case 10:
			//	MyWeapon = Ctrl ? WEAPONTYPE_PROXIMITYMINE : WEAPONTYPE_TRANSLOCATOR;
			//	break;
			//case 11:
			//	MyWeapon = WEAPONTYPE_SMOKEMINE;
			//	break;
			//case 12:
			//	MyWeapon = Ctrl ? WEAPONTYPE_FORCEFIELDMINE : WEAPONTYPE_FRONTFIELDGENERATOR;
			//	break;
			}

			if (MyWeapon != WEAPONTYPE_NONE)
			{
				if (Ctrl)
				{
					CheatInfo = "Change Addon Secondary Weapon";
					CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAddonID = MyWeapon;
					CMainGamePhase::Instance->InputPlayer->SecondaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(MyWeapon)->PickUpAmmo;
					CMainGameInterface::Instance->Mine = CPickUpAnimation::GetAnimTexture(MyWeapon);
				}
				else
				{
					CheatInfo = "Change Addon Primary Weapon";
					CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAddonID = MyWeapon;
					CMainGamePhase::Instance->InputPlayer->PrimaryWeaponAmmo2 = CWeaponDescriptionManager::Instance->GetWeaponDescription(MyWeapon)->PickUpAmmo;
					CMainGameInterface::Instance->Weapon = CPickUpAnimation::GetAnimTexture(MyWeapon);
				}
			}
			else
				KeyDown = true;
		}
	}
	else
	{
		KeyDown = false;
		CheatEntered = false;
	}

	if (CheatEntered && !KeyDown)
	{
		char FullCheatInfo[200];
		String CheatStatusText;
		if (CheatStatus)
			CheatStatusText = "ON";
		else
			CheatStatusText = "OFF";

		sprintf(FullCheatInfo, "Cheat Mode %s %s", CheatInfo.GetString(), CheatStatusText.GetString());
		CUberText *CheatText = new CUberText(String(FullCheatInfo));
		CMainGameInterface::Instance->UberLines.PushBack(CheatText);

		KeyDown = true;
	}
}


void CPlayGamePhase::RenderSafetyLock()
{
	if (CGameSettings::ClientInstance->GetPlayersCount() < 2) return;
	if (!CGameSettings::ClientInstance->GetWeapons()) return;
	if ( (CWeaponFabric::Instance->IsValid()) && (SafetyLockTimer >= SAFETY_LOCK_TIME) ) return;

	if (Status == PLAYGAME_RACE)
		SafetyLockTimer += WeTimer::Instance()->GetDeltaTimeF();

	if (SafetyLockTimer >= SAFETY_LOCK_TIME)
	{
		CWeaponFabric::Instance->SetActive(CGameSettings::ClientInstance->GetWeapons());
		CMainGameInterface::Instance->SetTimeTextBeepFactor(0.0f);
		return;
	}

	CWeaponFabric::Instance->SetActive(false);

	char text[100];
	float TimeLeft = (SAFETY_LOCK_TIME - SafetyLockTimer);
	if (TimeLeft < 0.0f) TimeLeft = 0.0f;
	sprintf(text, CLocalizationSettings::Instance()->GetLocalText("SAFETY_LOCK"), TimeLeft);
	String SafetyText = String(text);
	float W = CUberText::MeasureTextWidth(SafetyText, CMainGameInterface::Instance->Font20);
	WeColor TextColor = WeColor(0.0f, 1.0f, 0.0f, 0.8f);
	if (SafetyLockTimer > (SAFETY_LOCK_TIME * 0.7f))
	{
		TextColor = WeColor(1.0f, 0.0f, 0.0f, 0.8f * (1.0f + (sin(15.0f * SafetyLockTimer) / 2.0f)));
		CMainGameInterface::Instance->SetTimeTextBeepFactor(SafetyLockTimer / SAFETY_LOCK_TIME);
	}
	CMainGameInterface::DrawShadowedText(SafetyText, CMainGameInterface::Instance->Font20, (0.5f - (W / 2.0f)), 0.85f, TextColor);
}


void CPlayGamePhase::CollideTubers(CTuber* DudeOne, CTuber* DudeTwo, WeVector &TouchPoint)
{
	const float SOUND_SPREAD = 0.5f;
	const float MIN_SPEED_DIFF = 20.0f;

	if ( (DudeOne->TuberCollisionTimer < SOUND_SPREAD) || (DudeTwo->TuberCollisionTimer < SOUND_SPREAD) )
		return;

	if (abs(DudeOne->CurrentSpeed - DudeTwo->CurrentSpeed) < MIN_SPEED_DIFF)
		return;

	DudeOne->TuberCollisionTimer = 0.0f;
	DudeTwo->TuberCollisionTimer = 0.0f;

	char phobos[100];
	sprintf(phobos, "Sounds.Tuber.Collision%d", Rnd(1, 3));
	WeSound *Plonk = WeAudioDevice::Instance()->LoadSoundAuto(String(phobos), true);

	Plonk->Play(false);
	Plonk->Update(TouchPoint, WeVector(0.0f, 0.0f, 0.0f));
	Plonk->DieOnStop();
}
