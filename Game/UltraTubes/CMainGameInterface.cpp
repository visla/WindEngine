/*
     File: CMainGameInterface.cpp
	 Author: Tomislav Kukic
	 Date: 27.03.2006
	 Version: 1.0
*/



#include <CMainGameInterface.h>
#include <WeCanvas.h>
#include <WeTexture.h>
#include <WeTextureManager.h>
#include <WeFont.h>
#include <CTuber.h>
#include <Constants.h>
#include <CPlayGamePhase.h>
#include <CMainGamePhase.h>
#include <CGameSettings.h>
#include <WeTimer.h>
#include <CUberText.h>
#include <CPickUpAnimation.h>
#include <WeSound.h>
#include <WeAudioDevice.h>
#include <WeAnimTexture.h>
#include <WePathNode.h>
#include <CLocalizationSettings.h>
#include <CGameSettings.h>
#include <Helper.h>
#include <CWeapon.h>
#include <CPickUp.h>
#include <CPickupAction.h>
#include <WePhysics.h>
#include <WeProfiler.h>


#define BLOOD_SPLAT_TIME	0.5f


CMainGameInterface* CMainGameInterface::Instance = new CMainGameInterface();

CMainGameInterface::CMainGameInterface()
{
	AnimatingWeapon = false;
	AnimatingMine = false;

	Renderable = true;
	Initialized = false;

	Weapon = NULL;
	Mine = NULL;
	Tuber = NULL;
	HeartBeatSound = NULL;
	EnemyHitSound = NULL;
	EnemyHitImmortalSound = NULL;
	TimeTextBeep = NULL;

	WeaponLockTimer = 0.0f;
	TargetLockTimer = 0.0f;
	WrongWayTimer = 0.0f;

	BloodTimer = 0.0f;

	AddDistantExplosion = false;

	Font12 = NULL;
	Font16 = NULL;
	Font20 = NULL;
	Font24 = NULL;
	Font32 = NULL;
	FontFixed = NULL;
}



CMainGameInterface::~CMainGameInterface()
{
	ShutDown();
}


void CMainGameInterface::Initialize()
{
	if(Initialized) return;

	Left = WeTextureManager::Instance()->Load("Ingame.Hud.Left", true);
	LeftFill = WeTextureManager::Instance()->Load("Ingame.Hud.LeftFill", true);
	Right = WeTextureManager::Instance()->Load("Ingame.Hud.Right", true);
	RightFill = WeTextureManager::Instance()->Load("Ingame.Hud.RightFill", true);
	TopLeft = WeTextureManager::Instance()->Load("Ingame.Hud.TopLeft", true);
	TopRight = WeTextureManager::Instance()->Load("Ingame.Hud.TopRight", true);
	LeftMiddle = WeTextureManager::Instance()->Load("Ingame.Hud.LeftMiddle", true);
	LeftTop = WeTextureManager::Instance()->Load("Ingame.Hud.LeftTop", true);
	LeftBottom = WeTextureManager::Instance()->Load("Ingame.Hud.LeftBottom", true);
	LeftLine = WeTextureManager::Instance()->Load("Ingame.Hud.Lines", true);
	TopPickups = WeTextureManager::Instance()->Load("Ingame.Hud.TopPickups", true);

	SpeedTex[0] = WeTextureManager::Instance()->Load("Ingame.Hud.GreenEmpty", true);
	SpeedTex[1] = WeTextureManager::Instance()->Load("Ingame.Hud.GreenFull", true);
	TurboSpeedTex[0] = WeTextureManager::Instance()->Load("Ingame.Hud.LightBlueEmpty", true);
	TurboSpeedTex[1] = WeTextureManager::Instance()->Load("Ingame.Hud.LightBlueFull", true);
	EnergyTex[0] = WeTextureManager::Instance()->Load("Ingame.Hud.RedEmpty", true);
	EnergyTex[1] = WeTextureManager::Instance()->Load("Ingame.Hud.RedFull", true);
	TurboTex[0] = WeTextureManager::Instance()->Load("Ingame.Hud.BlueEmpty", true);
	TurboTex[1] = WeTextureManager::Instance()->Load("Ingame.Hud.BlueFull", true);

	LockLeft = WeTextureManager::Instance()->Load("Textures.GameInsterfejs.LockL", true);
	LockRight = WeTextureManager::Instance()->Load("Textures.GameInsterfejs.LockR", true);
	LockMiddle = WeTextureManager::Instance()->Load("Textures.GameInsterfejs.LockM", true);

	Blood = WeTextureManager::Instance()->Load("Ingame.Misc.BloodScreen", true);

	LeftRed = WeTextureManager::Instance()->Load("Ingame.Misc.LeftRed", true);
	LeftGreen = WeTextureManager::Instance()->Load("Ingame.Misc.LeftGreen", true);
	RightRed = WeTextureManager::Instance()->Load("Ingame.Misc.RightRed", true);
	RightGreen = WeTextureManager::Instance()->Load("Ingame.Misc.RightGreen", true);

	BuffBackground = WeTextureManager::Instance()->Load("Ingame.Hud.BuffBackground", true);
	BuffInvulnerability = WeTextureManager::Instance()->Load("Ingame.Misc.InvulnerabilityBuff", true);
	BuffInvisibility = WeTextureManager::Instance()->Load("Ingame.Misc.InvisibilityBuff", true);
	BuffQuadDamage = WeTextureManager::Instance()->Load("Ingame.Misc.QuadDamageBuff", true);

	Weapon = NULL;
	Mine = NULL;

	ShakeOffsetX = 0.0f;
	ShakeOffsetY = 0.0f;

	WeaponPictureEX = AbsX(0.248f); //(int)(150*WOffset());
	WeaponPictureEY = AbsY(0.86f); //(int)(-10*HOffset());

	MinePictureEX = AbsX(0.612f); //(int)(725*WOffset());
	MinePictureEY = AbsY(0.86f); //(int)(-10*HOffset());

	PositionDrawTimer = 0.0f;

	DisplayCoins = 0;
	DisplayScore = 0;
	FinalDisplayCoins = 0;
	FinalDisplayScore = 0;
	DisplayCoinsSpeed = 0.0f;
	DisplayScoreSpeed = 0.0f;

	TimeTextBeep = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.TimeEndingText", false);
	TimeTextBeepFactor = 0.0f;
	TimeTextBeepSoundFactor = 0.0f;

	Initialized = true;
}



void CMainGameInterface::InitializeFonts()
{
	int Width = WeRenderDevice::Instance()->GetResWidth();
	int Height = WeRenderDevice::Instance()->GetResHeight();

	if(Width == 1024)
	{
		Font12 = WeCanvas::Instance()->MakeFont("Xirod",12);
		Font16 = WeCanvas::Instance()->MakeFont("Xirod",16);
		Font20 = WeCanvas::Instance()->MakeFont("Xirod",20);
		Font24 = WeCanvas::Instance()->MakeFont("Xirod",24);
		Font32 = WeCanvas::Instance()->MakeFont("Xirod",32);
		FontFixed = WeCanvas::Instance()->MakeFont("Courier New",16);
	}else if(Width == 1280)
	{
		Font12 = WeCanvas::Instance()->MakeFont("Xirod",14);
		Font16 = WeCanvas::Instance()->MakeFont("Xirod",18);
		Font20 = WeCanvas::Instance()->MakeFont("Xirod",22);
		Font24 = WeCanvas::Instance()->MakeFont("Xirod",26);
		Font32 = WeCanvas::Instance()->MakeFont("Xirod",34);
		FontFixed = WeCanvas::Instance()->MakeFont("Courier New",18);
	}else if(Width == 800)
	{
		Font12 = WeCanvas::Instance()->MakeFont("Xirod",10);
		Font16 = WeCanvas::Instance()->MakeFont("Xirod",14);
		Font20 = WeCanvas::Instance()->MakeFont("Xirod",18);
		Font24 = WeCanvas::Instance()->MakeFont("Xirod",22);
		Font32 = WeCanvas::Instance()->MakeFont("Xirod",30);
		FontFixed = WeCanvas::Instance()->MakeFont("Courier New",14);
	}else if(Width == 640)
	{
		Font12 = WeCanvas::Instance()->MakeFont("Xirod",8);
		Font16 = WeCanvas::Instance()->MakeFont("Xirod",10);
		Font20 = WeCanvas::Instance()->MakeFont("Xirod",14);
		Font24 = WeCanvas::Instance()->MakeFont("Xirod",18);
		Font32 = WeCanvas::Instance()->MakeFont("Xirod",24);
		FontFixed = WeCanvas::Instance()->MakeFont("Courier New",10);
	}
}



void CMainGameInterface::SetTuber(CTuber* _Tuber)
{
	Tuber = _Tuber;
}



void CMainGameInterface::ShutDown()
{
	if(!Initialized) return;

	Font12->ShutDown();
	Font16->ShutDown();
	Font20->ShutDown();
	Font24->ShutDown();
	Font32->ShutDown();

	if (HeartBeatSound)
	{
		WeAudioDevice::Instance()->FreeSound(HeartBeatSound);
		HeartBeatSound = NULL;
	}

	if (EnemyHitSound)
	{
		WeAudioDevice::Instance()->FreeSound(EnemyHitSound);
		EnemyHitSound = NULL;
	}

	if (EnemyHitImmortalSound)
	{
		WeAudioDevice::Instance()->FreeSound(EnemyHitImmortalSound);
		EnemyHitImmortalSound = NULL;
	}

	if (TimeTextBeep)
	{
		WeAudioDevice::Instance()->FreeSound(TimeTextBeep);
		TimeTextBeep = NULL;
	}

	for (UberLines.First(); !UberLines.End(); )
	{
		delete UberLines.GetItem();
		UberLines.Delete();
	}

	Initialized = false;
}



void CMainGameInterface::Tick()
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	for (UberLines.First(); !UberLines.End(); )
	{
		if (!UberLines.GetItem()->Dying)
		{
			UberLines.GetItem()->Tick();
			UberLines.Next();
		}
		else
		{
			delete UberLines.GetItem();
			UberLines.Delete();
		}
	}

	WeaponLockTimer += time;
	TargetLockTimer += time;
	WrongWayTimer += time;

	if(!Tuber) return;

	TickHealthSound();
	TickHitSound();
	TickDistantBoom();
	TickWeaponPics();
	TickDisplayCounters();
	TickTextBeepSound();

	float d,t;		
	d = Tuber->MaxSpeed*1.2f - Tuber->MaxSpeed*0.95f;		
	t = Tuber->CurrentSpeed - Tuber->MaxSpeed*0.95f;
	if (t < 0) t = 0;

	int r;
	r = (int)(t*10.0f/d);
	if (r == 0) r = 1;
	if (r > 10) r = 10;

	ShakeOffsetX = (float)((rand()%r) - r/2.0f);
	ShakeOffsetY = (float)((rand()%r) - r/2.0f);
}



void CMainGameInterface::Render()
{
/*
	int W = WeRenderDevice::Instance()->GetResWidth();
	int H = WeRenderDevice::Instance()->GetResHeight();

	int Speed;
	int Health;
	int Nitro;

	Speed = (int)(Tuber->CurrentSpeed / Tuber->MaxSpeed * 255);
	if (Speed < 0)
		Speed = 0;
	else if (Speed > 255)
		Speed = 255;

	Health = (int)Tuber->HP;
	Nitro = (int)Tuber->TurboCharges;
*/

	// this block renders things only for HUD ON

	if (Renderable)
	{
		RenderHud();
		RenderPositionTable();

		for(CPickUpAnimation::PickupAnimations.First(); !CPickUpAnimation::PickupAnimations.End(); CPickUpAnimation::PickupAnimations.Next())
			CPickUpAnimation::PickupAnimations.GetItem()->Render();

	}

	RenderFPS();
	RenderModelNames();

	RenderTuberNames();
	RenderUberText();
	RenderTargetLock();
	RenderWeaponLock();
	RenderBlood();
	if (CPlayGamePhase::Instance->Status == PLAYGAME_GAMEPAUSED) return;
	RenderPickupLock();
	RenderMineLock();
	RenderEnemyTargetLock();
	RenderWrongWay();
	RenderHealing();
}



String CMainGameInterface::MakeTimeString(float Time, String Separator)
{
	DWORD msec = (DWORD)(Time * 1000.0);
	int sec = (int)(msec / 1000.0);
	int min = sec / 60;
	sec = sec%60;
	msec = (msec%1000)/10;

	
	char tmp[100];
	if(min < 10)
		sprintf(tmp,"0%d",min);
	else
		sprintf(tmp,"%d",min);

	String Min = tmp;

	if(sec < 10)
		sprintf(tmp,"0%d",sec);
	else
		sprintf(tmp,"%d",sec);

	String Sec = tmp;

	if(msec < 10)
		sprintf(tmp,"0%d",msec);
	else
		sprintf(tmp,"%d",msec);

	String MSec = tmp;

	String Out = Min + Separator + Sec + Separator + MSec;

	return Out;
}



void CMainGameInterface::RenderUberText()
{
	for (UberLines.First(); !UberLines.End(); UberLines.Next())
	{
		UberLines.GetItem()->Render();
	}
}


void CMainGameInterface::RenderTargetLock()
{
	const float BLINK_TIME	= 0.5f;

	if (!Tuber->HasTargetLock) return;
	if (CPlayGamePhase::Instance->ShowInGameMenu) return;

	if (TargetLockTimer >= BLINK_TIME)
	{
		TargetLockTimer = 0.0f;
		CUberText *t = new CUberText(CLocalizationSettings::Instance()->GetLocalText("LOCK"), 0.7f, 0.8f, UberTextTypeAnimInstant, UberTextTypeAnimFade);
		t->SetTextColor(WeColor(1.0f, 1.0f, 0.0f, 1.0f));
		t->SetFont(Font24);
		t->SetTimeStay(0.0f);
		t->SetTimeHide(0.4f);
		UberLines.PushBack(t);

		WeSound *Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Lock", false);
		if (Sound)
		{
			Sound->Play(false);
			Sound->DieOnStop();
		}
	}
}


void CMainGameInterface::RenderWeaponLock()
{
	const float MAX_DISTANCE		= 100.0f;
	const float MAX_HIDE_TIME		= 1.0f;
	const float MAX_BLINK_FACTOR	= 1.0f;

	if (!Tuber->HasWeaponLock) return;
	if (CPlayGamePhase::Instance->ShowInGameMenu) return;

	float BlinkFactor = (Tuber->WeaponLockDistance / MAX_DISTANCE);
	if (BlinkFactor > MAX_BLINK_FACTOR) BlinkFactor = MAX_BLINK_FACTOR;

	if (WeaponLockTimer > (MAX_HIDE_TIME * BlinkFactor))
	{
		WeaponLockTimer = 0.0f;
		CUberText *t = new CUberText(CLocalizationSettings::Instance()->GetLocalText("WARNING"), 0.2f, 0.8f, UberTextTypeAnimInstant, UberTextTypeAnimFade);
		t->SetTextColor(WeColor(1.0f, 0.0f, 0.0f, 1.0f));
		t->SetFont(Font24);
		t->SetTimeStay(0.0f);
		t->SetTimeHide(0.1f);
		UberLines.PushBack(t);

		WeSound *Sound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Warning", false);
		if (Sound)
		{
			Sound->Play(false);
			Sound->DieOnStop();
		}
	}
}

void CMainGameInterface::RenderEnemyTargetLock()
{
	if (!Tuber->EnemyTarget) return;

	WeVector Loc;
	WeMatrix Mat;
	D3DXMatrixMultiply(&Mat, &CMainGamePhase::Instance->Camera.View, &CMainGamePhase::Instance->Camera.Projection);
	D3DXVECTOR4 Out;
	D3DXVec3Transform(&Out, &Tuber->EnemyTarget->Location, &Mat);

	Loc.x = ((Out.x / Out.w) + 1.0f) / 2.0f;
	Loc.y = (((Out.y * -1.0f) / Out.w) + 1.0f) / 2.0f;

	if(Out.z > 0.0f)
	{
		float X, Y;
		int WX, WY;
		float Alpha;
		float abstime = WeTimer::Instance()->GetElapsedTimeF();
		float RotationSpeed = 5.0f;

		if (Tuber->EnemyLockFactor == 1.0f)
			RotationSpeed = 1.0f;

		float FactorL;

		FactorL = Tuber->EnemyLockFactor * 1.5f;
		FactorL = FactorL > 1.0f ? 1.0f : FactorL;
		WX = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockLeft->Width));
		WY = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockLeft->Height));
		X = Loc.x - RelX(WX) / 2.0f;
		Y = Loc.y - RelY(WY) / 2.0f;
		Alpha = (FactorL * 0.8f);
		WeCanvas::Instance()->DrawRectangleRotated((float)AbsX(X), (float)AbsY(Y), (float)WX, (float)WY, RotationSpeed * abstime, WeColor(1.0f, 1.0f, 1.0f, Alpha), LockMiddle);

		if (Tuber->EnemyLockFactor >= (1.0f / 3.0f))
		{
			FactorL = (Tuber->EnemyLockFactor - (1.0f / 3.0f)) * 1.5f;
			FactorL = FactorL > 1.0f ? 1.0f : FactorL;
			WX = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockRight->Width));
			WY = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockRight->Height));
			X = Loc.x - RelX(WX) / 2.0f;
			Y = Loc.y - RelY(WY) / 2.0f;
			Alpha = (FactorL * 0.8f);
			WeCanvas::Instance()->DrawRectangleRotated((float)AbsX(X), (float)AbsY(Y), (float)WX, (float)WY, -RotationSpeed * abstime, WeColor(1.0f, 1.0f, 1.0f, Alpha), LockLeft);
		}

		if (Tuber->EnemyLockFactor >= (2.0f / 3.0f))
		{
			FactorL = (Tuber->EnemyLockFactor - (2.0f / 3.0f)) * 3.0f;
			FactorL = FactorL > 1.0f ? 1.0f : FactorL;
			WX = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockMiddle->Width));
			WY = (int)((2.33f + (1.0f - (3.0f * FactorL))) * (float)(LockMiddle->Height));
			X = Loc.x - RelX(WX) / 2.0f;
			Y = Loc.y - RelY(WY) / 2.0f;
			Alpha = (FactorL * 0.8f);
			WeCanvas::Instance()->DrawRectangleRotated((float)AbsX(X), (float)AbsY(Y), (float)WX, (float)WY, RotationSpeed * abstime, WeColor(1.0f, 1.0f, 1.0f, Alpha), LockRight);
		}
	}
}

void CMainGameInterface::RenderWrongWay()
{
	const float BLINK_TIME = 0.5f;

#ifdef _DEBUG
	if (!Tuber->WrongWay)
	{
		char temp[100];
		sprintf(temp, "[%d]", Tuber->RefPathNode->SequenceNumber);
		CUberText *t = new CUberText((String)temp, 0.35f, 0.4f, UberTextTypeAnimInstant, UberTextTypeAnimFade);
		t->SetTextColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
		t->SetFont(Font32);
		t->SetTimeStay(0.0f);
		t->SetTimeHide(0.1f);
		UberLines.PushBack(t);
	}
#endif

	if (!Tuber->WrongWay) return;
	if (Tuber->Dead) return;
	if (Tuber->RaceFinished) return;
	if (WrongWayTimer < BLINK_TIME) return;
	if (CPlayGamePhase::Instance->ShowInGameMenu) return;

	WrongWayTimer = 0.0f;
	char temp[100];
	sprintf(temp, "%s [%d]", CLocalizationSettings::Instance()->GetLocalText("WRONG_WAY").GetString(), Tuber->RefPathNode->SequenceNumber);
	CUberText *t = new CUberText((String)temp, 0.35f, 0.4f, UberTextTypeAnimInstant, UberTextTypeAnimFade);
	t->SetTextColor(WeColor(1.0f, 1.0f, 1.0f, 1.0f));
	t->SetFont(Font32);
	t->SetTimeStay(0.0f);
	t->SetTimeHide(0.4f);
	UberLines.PushBack(t);
}


void CMainGameInterface::TickHealthSound()
{
	if (!HeartBeatSound)
	{
		HeartBeatSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.HeartBeat", false);
	}

	if (HeartBeatSound)
	{
		if (!Tuber ||
			Tuber->Dead ||
			Tuber->RaceFinished ||
			Tuber->Invulnerable)
		{
			HeartBeatSound->Stop();
		}
		else
		{
			if ( ((float)Tuber->HP / (float)Tuber->MaxHP) < 0.2f )
			{
				if (!HeartBeatSound->IsPlaying())
					HeartBeatSound->Play(true, true);
			}
			else
			{
				if (HeartBeatSound->IsPlaying())
					HeartBeatSound->Stop();
			}
		}
	}
}


void CMainGameInterface::TickHitSound()
{
	if (!Tuber->LastEnemyHit) return;

	if (!Tuber->LastEnemyHit->Invulnerable)
	{
		if (!EnemyHitSound)
			EnemyHitSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.Hit", false);
		if (EnemyHitSound && (!EnemyHitSound->IsPlaying()))
			EnemyHitSound->Play(false);
	}
	else
	{
		if (!EnemyHitImmortalSound)
			EnemyHitImmortalSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Interface.ImmortalHit", false);
		if (EnemyHitImmortalSound && (!EnemyHitImmortalSound->IsPlaying()))
			EnemyHitImmortalSound->Play(false);
	}
	
	Tuber->LastEnemyHit = NULL;
}


void CMainGameInterface::RenderBlood()
{
	if (!Tuber->FSDamageNotify) return;

	BloodTimer += WeTimer::Instance()->GetDeltaTimeF();

	if (Tuber->FSDamageNotifyReset)
	{
		BloodTimer = 0.0f;
		Tuber->FSDamageNotifyReset = false;
	}

	if (BloodTimer >= BLOOD_SPLAT_TIME)
	{
		BloodTimer = 0.0f;
		Tuber->FSDamageNotify = false;
		return;
	}

	float Alpha = 0.7f - (BloodTimer / BLOOD_SPLAT_TIME);

	WeCanvas::Instance()->DrawRectangle(
		0, 0, 
		WeRenderDevice::Instance()->GetResWidth(), 
		WeRenderDevice::Instance()->GetResHeight(), 
		WeColor(1.0f, 1.0f, 1.0f, Alpha), 
		Blood);
}


void CMainGameInterface::TickWeaponPics()
{
	if (!CGameSettings::ClientInstance->GetWeapons())
	{
		Weapon = NULL;
		Mine = NULL;
		return;
	}

	if (!Weapon || Tuber->PrimaryWeaponAmmo2 <= 0)
		Weapon = CPickUpAnimation::GetAnimTexture(Tuber->PrimaryWeaponID);
	if (!Mine || Tuber->SecondaryWeaponAmmo2 <= 0)
		Mine = CPickUpAnimation::GetAnimTexture(Tuber->SecondaryWeaponID);
}


void CMainGameInterface::RenderTuberNames()
{
	CTuber *PlayerTuber = CMainGamePhase::Instance->InputPlayer;

	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		CGamePlayerConfig *PlayerConfig = &CGameSettings::ClientInstance->tmpPlayers[i];
		if (!PlayerConfig->User) continue;
		if (!PlayerConfig->User->Tuber) continue;

		CTuber *OtherTuber = PlayerConfig->User->Tuber;
		if (OtherTuber == PlayerTuber) continue;
		if (OtherTuber->Dead) continue;
		if (OtherTuber->RaceFinished) continue;
		if (OtherTuber->Invisible) continue;

		if ((PlayerTuber->RefPathNode->TubeID != 0) && (OtherTuber->RefPathNode->TubeID != 0) &&
			(PlayerTuber->RefPathNode->TubeID != OtherTuber->RefPathNode->TubeID))
			continue;

		//WeVector Distance = OtherTuber->Location - Tuber->Location;
		//float Dist = D3DXVec3Length(&Distance);
		//D3DXVec3Normalize(&Distance, &Distance);
		//WeRaycastHit Hit;
		//if (WePhysics::Instance()->RaycastClosest(Tuber->Location, Distance, WE_SHAPE_TYPE_STATIC, Hit, 7, Dist))
		//	continue;

		WeVector tmpV = OtherTuber->Location;

		D3DXVECTOR4 tmpOutV;
		D3DXVec3Transform(&tmpOutV, &tmpV, &CMainGamePhase::Instance->Camera.ViewProj);

		float x, y;
		x = ((tmpOutV.x / tmpOutV.w) + 1.0f) / 2.0f;
		y = ((-tmpOutV.y / tmpOutV.w) + 1.0f) / 2.0f;

		if(tmpOutV.z > 0.0f)
		{
			int DrawX, DrawY, DrawW, DrawH;
			WeTexture *TexFiller;

			DrawX = (int)(x * WeRenderDevice::Instance()->GetResWidth());
			DrawY = (int)(y * WeRenderDevice::Instance()->GetResHeight());
			DrawW = (int)(0.05f * WeRenderDevice::Instance()->GetResWidth());
			DrawH = (int)(0.012f * WeRenderDevice::Instance()->GetResHeight());
			TexFiller = WeTextureManager::Instance()->Load("MapEditor.Textures.DefaultWhite1x1", true);

			WeVector PlayerPos = PlayerTuber->Location;
			WeVector OtherPos = OtherTuber->Location;
			WeVector Diff = OtherPos - PlayerPos;
			float Length = D3DXVec3Length(&Diff);

			if (Length <= MAX_LOCK_DISTANCE)
			{
				char GreenGrassOfMyHome[100];
				sprintf(GreenGrassOfMyHome, "%s (%d)", PlayerConfig->User->Name.GetString(), (int)Length);
				String tmpS = String(GreenGrassOfMyHome);

				float AlphaFactor = 0.8f - (0.5f * (Length / MAX_LOCK_DISTANCE));

				WeColor NameColor = OtherTuber->Trail->SegmentColor;
				NameColor.a = AlphaFactor;

				//WeCanvas::Instance()->Draw2DText(Font16, tmpS, DrawX - DrawW / 2 + 2, DrawY - DrawH * 2 + 2, 200, 100, 0xFF000000);
				//WeCanvas::Instance()->Draw2DText(Font16, tmpS, DrawX - DrawW / 2, DrawY - DrawH * 2, 200, 100, 0xFFFFFFFF);
				DrawShadowedText(tmpS, Font12, x - 0.025f, y - 0.01f, NameColor);

				WeColor HealthColor;
				float OtherHealth = OtherTuber->HP / OtherTuber->MaxHP;

				HealthColor = WeColor(1.0f, 1.0f, 1.0f, AlphaFactor);
				WeCanvas::Instance()->DrawRectangle(DrawX - DrawW / 2, DrawY + 10 - DrawH / 2, DrawW, DrawH, HealthColor, TexFiller);
				HealthColor = WeColor(0.0f, 0.0f, 0.0f, AlphaFactor);
				WeCanvas::Instance()->DrawRectangle(DrawX - DrawW / 2 + 1, DrawY + 10 - DrawH / 2 + 1, DrawW - 2, DrawH - 2, HealthColor, TexFiller);

				HealthColor = WeColor(1.0f, 0.0f, 0.0f, AlphaFactor);
				if (OtherTuber->Invulnerable)
					HealthColor = WeColor(0.0f, 1.0f, 0.0f, AlphaFactor);
				WeCanvas::Instance()->DrawRectangle(DrawX - DrawW / 2 + 2, DrawY + 10 - DrawH / 2 + 2, (int)(OtherHealth * (DrawW - 4)), DrawH - 4, HealthColor, TexFiller);
			}
		}
	}
}


void CMainGameInterface::RenderHud()
{
	int RW = WeRenderDevice::Instance()->GetResWidth();
	int RH = WeRenderDevice::Instance()->GetResHeight();
	int X, Y, W, H, HH;
	float value;
	WeColor ColorF = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	WeColor ColorA = WeColor(1.0f, 1.0f, 1.0f, 0.7f);
	WeColor ColorLA = WeColor(1.0f, 1.0f, 1.0f, 0.4f);
	char Info[100], Info2[100];

	static float hudtimer = 10.0f;
	static float leftnumx, leftnumy, lefttexx, lefttexy;
	static float rightnumx, rightnumy, righttexx, righttexy;
	static float ammox, ammoy, primx, primy, secx, secy;
	static float lapx, lapy, clapx, clapy, tlapx, tlapy;
	static float killsx, killsy;
	static float coinsx, coinsy;
	static float scorex, scorey;
	static float lastlapx, lastlapy, lastlapnumx, lastlapnumy;
	static float fastestlapx, fastestlapy, fastestlapnumx, fastestlapnumy;
	static float racetimex, racetimey, racetimenumx, racetimenumy;
	static int SpeedRotX, SpeedRotY, SpeedRotRadius, SpeedRotCount;
	static float SpeedRotAngle, SpeedRotStartAngle;
	static int TurboSpeedRotX, TurboSpeedRotY, TurboSpeedRotRadius, TurboSpeedRotCount;
	static float TurboSpeedRotAngle, TurboSpeedRotStartAngle;
	static int TurboRotX, TurboRotY, TurboRotRadius, TurboRotCount;
	static float TurboRotAngle, TurboRotStartAngle;
	static int EnergyRotX, EnergyRotY, EnergyRotRadius, EnergyRotCount;
	static float EnergyRotAngle, EnergyRotStartAngle;
	static float weaponpickupx, weaponpickupy, minepickupx, minepickupy;
	static float buffbacky, buffbackspready;
	static float LeftFillPart, RightFillPart;


	hudtimer += WeTimer::Instance()->GetDeltaTimeF();
	if (hudtimer >= 1.0f)
	{
		hudtimer = 0.0f;
		FILE* f;
		f = fopen("hud.txt", "rt");
		fscanf(f, "%f %f %f %f\n", &leftnumx, &leftnumy, &lefttexx, &lefttexy);
		fscanf(f, "%f %f %f %f\n", &rightnumx, &rightnumy, &righttexx, &righttexy);
		fscanf(f, "%f %f %f %f %f %f\n", &ammox, &ammoy, &primx, &primy, &secx, &secy);
		fscanf(f, "%f %f %f %f %f %f\n", &lapx, &lapy, &clapx, &clapy, &tlapx, &tlapy);
		fscanf(f, "%f %f\n", &killsx, &killsy);
		fscanf(f, "%f %f\n", &coinsx, &coinsy);
		fscanf(f, "%f %f\n", &scorex, &scorey);
		fscanf(f, "%f %f %f %f\n", &lastlapx, &lastlapy, &lastlapnumx, &lastlapnumy);
		fscanf(f, "%f %f %f %f\n", &fastestlapx, &fastestlapy, &fastestlapnumx, &fastestlapnumy);
		fscanf(f, "%f %f %f %f\n", &racetimex, &racetimey, &racetimenumx, &racetimenumy);
		fscanf(f, "%d %d %d %f %f %d\n", &SpeedRotX, &SpeedRotY, &SpeedRotRadius, &SpeedRotAngle, &SpeedRotStartAngle, &SpeedRotCount);
		fscanf(f, "%d %d %d %f %f %d\n", &TurboSpeedRotX, &TurboSpeedRotY, &TurboSpeedRotRadius, &TurboSpeedRotAngle, &TurboSpeedRotStartAngle, &TurboSpeedRotCount);
		fscanf(f, "%d %d %d %f %f %d\n", &EnergyRotX, &EnergyRotY, &EnergyRotRadius, &EnergyRotAngle, &EnergyRotStartAngle, &EnergyRotCount);
		fscanf(f, "%d %d %d %f %f %d\n", &TurboRotX, &TurboRotY, &TurboRotRadius, &TurboRotAngle, &TurboRotStartAngle, &TurboRotCount);
		fscanf(f, "%f %f\n", &weaponpickupx, &minepickupx);
		fscanf(f, "%f %f\n", &buffbacky, &buffbackspready);
		fscanf(f, "%f %f\n", &LeftFillPart, &RightFillPart);
		fclose(f);
	}

	float TuberSpeed = Tuber->CurrentSpeed;
	if (Tuber->RaceFinished)
		TuberSpeed = 0.0f;

	if (Left)
	{
		W = (int)((float)Left->Width * WOffset());
		H = (int)((float)Left->Height * HOffset());
		X = 0;
		Y = RH - H;
		WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, Left);

		W = (int)((float)LeftFill->Width * WOffset());
		H = (int)((float)LeftFill->Height * HOffset() * LeftFillPart);
		X = 0;
		Y = RH - H;
		value = Tuber->HP / Tuber->MaxHP;
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		HH = (int)(H * value);
		WeCanvas::Instance()->DrawRectangle(X, Y + H - HH, W, HH, 0.0f, 1.0f - value * LeftFillPart, 1.0f, 1.0f, ColorF, LeftFill);

		value = abs(TuberSpeed) / Tuber->MaxSpeed;
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		value = value * SpeedRotCount;

		for (int i = 0; i < SpeedRotCount; ++i)
		{
			float angle = (SpeedRotStartAngle + i * SpeedRotAngle) / 180.0f * D3DX_PI;

			float alpha = value - i;
			if (alpha < 0.0f)
				alpha = 0.0f;
			else if (alpha > 1.0f)
				alpha = 1.0f;

			int dx = (int)(SpeedRotRadius * sin(-angle));
			int dy = (int)(SpeedRotRadius * cos(-angle));

			WeCanvas::Instance()->DrawRectangleRotated(
				(SpeedRotX + dx - SpeedTex[0]->Width / 2) * WOffset(),
				(SpeedRotY + dy - SpeedTex[0]->Height / 2) * HOffset(),
				SpeedTex[0]->Width * WOffset(),
				SpeedTex[0]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, 1.0f), SpeedTex[0]);

			WeCanvas::Instance()->DrawRectangleRotated(
				(SpeedRotX + dx - SpeedTex[1]->Width / 2) * WOffset(),
				(SpeedRotY + dy - SpeedTex[1]->Height / 2) * HOffset(),
				SpeedTex[1]->Width * WOffset(),
				SpeedTex[1]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, alpha), SpeedTex[1]);
		}

		value = 0.0f;
		if (Tuber->TurboMaxSpeed > Tuber->MaxSpeed)
			value = (abs(TuberSpeed) - Tuber->MaxSpeed) / (Tuber->TurboMaxSpeed - Tuber->MaxSpeed);
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		value = value * TurboSpeedRotCount;

		for (int i = 0; i < TurboSpeedRotCount; ++i)
		{
			float angle = (TurboSpeedRotStartAngle + i * TurboSpeedRotAngle) / 180.0f * D3DX_PI;

			float alpha = value - i;
			if (alpha < 0.0f)
				alpha = 0.0f;
			else if (alpha > 1.0f)
				alpha = 1.0f;

			int dx = (int)(TurboSpeedRotRadius * sin(-angle));
			int dy = (int)(TurboSpeedRotRadius * cos(-angle));

			WeCanvas::Instance()->DrawRectangleRotated(
				(TurboSpeedRotX + dx - TurboSpeedTex[0]->Width / 2) * WOffset(),
				(TurboSpeedRotY + dy - TurboSpeedTex[0]->Height / 2) * HOffset(),
				TurboSpeedTex[0]->Width * WOffset(),
				TurboSpeedTex[0]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, 1.0f), TurboSpeedTex[0]);

			WeCanvas::Instance()->DrawRectangleRotated(
				(TurboSpeedRotX + dx - TurboSpeedTex[1]->Width / 2) * WOffset(),
				(TurboSpeedRotY + dy - TurboSpeedTex[1]->Height / 2) * HOffset(),
				TurboSpeedTex[1]->Width * WOffset(),
				TurboSpeedTex[1]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, alpha), TurboSpeedTex[1]);
		}
	}

	if (Right)
	{
		W = (int)((float)Right->Width * WOffset());
		H = (int)((float)Right->Height * HOffset());
		X = RW - W;
		Y = RH - H;
		WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, Right);

		W = (int)((float)RightFill->Width * WOffset());
		H = (int)((float)RightFill->Height * HOffset() * RightFillPart);
		X = RW - W;
		Y = RH - H;
		value = Tuber->TurboCharges / Tuber->MaxTurboCharges;
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		HH = (int)(H * value);
		WeCanvas::Instance()->DrawRectangle(X, Y + H - HH, W, HH, 0.0f, 1.0f - value * RightFillPart, 1.0f, 1.0f, ColorF, RightFill);

/*
		value = Tuber->HP / Tuber->MaxHP;
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		value = value * SpeedRotCount;

		for (int i = 0; i < EnergyRotCount; ++i)
		{
			float angle = (EnergyRotStartAngle + i * EnergyRotAngle) / 180.0f * D3DX_PI;

			float alpha = value - i;
			if (alpha < 0.0f)
				alpha = 0.0f;
			else if (alpha > 1.0f)
				alpha = 1.0f;

			int dx = (int)(EnergyRotRadius * sin(-angle));
			int dy = (int)(EnergyRotRadius * cos(-angle));

			WeCanvas::Instance()->DrawRectangleRotated(
				(EnergyRotX + dx - EnergyTex[0]->Width / 2) * WOffset(),
				(EnergyRotY + dy - EnergyTex[0]->Height / 2) * HOffset(),
				EnergyTex[0]->Width * WOffset(),
				EnergyTex[0]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, 1.0f), EnergyTex[0]);

			WeCanvas::Instance()->DrawRectangleRotated(
				(EnergyRotX + dx - EnergyTex[1]->Width / 2) * WOffset(),
				(EnergyRotY + dy - EnergyTex[1]->Height / 2) * HOffset(),
				EnergyTex[1]->Width * WOffset(),
				EnergyTex[1]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, alpha), EnergyTex[1]);
		}

		value = Tuber->TurboCharges / Tuber->MaxTurboCharges;
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		value = value * TurboRotCount;

		for (int i = 0; i < TurboRotCount; ++i)
		{
			float angle = (TurboRotStartAngle + i * TurboRotAngle) / 180.0f * D3DX_PI;

			float alpha = value - i;
			if (alpha < 0.0f)
				alpha = 0.0f;
			else if (alpha > 1.0f)
				alpha = 1.0f;

			int dx = (int)(TurboRotRadius * sin(-angle));
			int dy = (int)(TurboRotRadius * cos(-angle));

			WeCanvas::Instance()->DrawRectangleRotated(
				(TurboRotX + dx - TurboTex[0]->Width / 2) * WOffset(),
				(TurboRotY + dy - TurboTex[0]->Height / 2) * HOffset(),
				TurboTex[0]->Width * WOffset(),
				TurboTex[0]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, 1.0f), TurboTex[0]);

			WeCanvas::Instance()->DrawRectangleRotated(
				(TurboRotX + dx - TurboTex[1]->Width / 2) * WOffset(),
				(TurboRotY + dy - TurboTex[1]->Height / 2) * HOffset(),
				TurboTex[1]->Width * WOffset(),
				TurboTex[1]->Height * HOffset(),
				angle, WeColor(1.0f, 1.0f, 1.0f, alpha), TurboTex[1]);
		}
*/
	}

	if (ShowStats)
	{
		if (TopLeft)
		{
			W = (int)((float)TopLeft->Width * WOffset());
			H = (int)((float)TopLeft->Height * HOffset());
			X = 0;
			Y = 0;
			WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, TopLeft);
		}

		if (TopRight)
		{
			W = (int)((float)TopRight->Width * WOffset());
			H = (int)((float)TopRight->Height * HOffset());
			X = RW - W;
			Y = 0;
			WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, TopRight);
		}
	}

	sprintf(Info, "%d", (int)abs(TuberSpeed * 3.6f));
	DrawShadowedText(String(Info), Font32, leftnumx, leftnumy, ColorA);
	DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("SPEED"), Font12, lefttexx, lefttexy, ColorA);

	sprintf(Info, "%d/%d", Tuber->Position, CGameSettings::ClientInstance->GetPlayersCount());
	DrawShadowedText(String(Info), Font32, rightnumx, rightnumy, ColorA);
	DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("POSITION"), Font12, righttexx, righttexy, ColorA);

	if (CWeaponFabric::Instance->IsValid())
	{
		int PriAmmoCount = (Tuber->PrimaryWeaponAmmo2 > 0) ? PriAmmoCount = Tuber->PrimaryWeaponAmmo2 : PriAmmoCount = Tuber->PrimaryWeaponAmmo;
		int SecAmmoCount = (Tuber->SecondaryWeaponAmmo2 > 0) ? SecAmmoCount = Tuber->SecondaryWeaponAmmo2 : SecAmmoCount = Tuber->SecondaryWeaponAmmo;
		sprintf(Info, "%d", PriAmmoCount);
		sprintf(Info2, "%d", SecAmmoCount);
	}
	else
	{
		sprintf(Info, "0");
		sprintf(Info2, "0");
	}
	DrawShadowedText(String(Info), Font16, primx, primy, ColorA);
	DrawShadowedText(String(Info2), Font16, secx, secy, ColorA);
	DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("AMMO"), Font12, ammox, ammoy, ColorA);

	int CurrentLap = Tuber->LapCount;
	if (CurrentLap + 1 > CGameSettings::ClientInstance->LapCount)
		CurrentLap--;
	sprintf(Info, "%d", (CurrentLap + 1));
	sprintf(Info2, "%d", CGameSettings::ClientInstance->LapCount);

	DrawShadowedText(String(Info), Font16, clapx, clapy, ColorA);
	if ( 
		(CGameSettings::ClientInstance->MapType != CGameSettings::ClientInstance->MAPTYPE_SP_LASTONE) &&
		(CGameSettings::ClientInstance->MapType != CGameSettings::ClientInstance->MAPTYPE_MP_LASTONE) )
	{
		DrawShadowedText(String(Info2), Font16, tlapx, tlapy, ColorA);
	}
	DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("LAP"), Font12, lapx, lapy, ColorA);

	if (ShowStats)
	{
		sprintf(Info, CLocalizationSettings::Instance()->GetLocalText("KILLS"), Tuber->Kills);
		DrawShadowedText(String(Info), Font12, killsx, killsy, ColorA);

		sprintf(Info, CLocalizationSettings::Instance()->GetLocalText("COINS"), DisplayCoins);
		DrawShadowedText(String(Info), Font12, coinsx, coinsy, ColorA);

		sprintf(Info, CLocalizationSettings::Instance()->GetLocalText("SCORE"), DisplayScore);
		DrawShadowedText(String(Info), Font12, scorex, scorey, ColorA);

		DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("LAST LAP TIME"), Font12, lastlapx, lastlapy, ColorLA);
		DrawShadowedText(MakeTimeString(CMainGamePhase::Instance->InputPlayer->LastLapTime), Font16, lastlapnumx, lastlapnumy, ColorA);

		DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("THE FASTEST LAP"), Font12, fastestlapx, fastestlapy, ColorLA);
		DrawShadowedText(MakeTimeString(CMainGamePhase::Instance->Map.FastesLap), Font16, fastestlapnumx, fastestlapnumy, ColorA);

		DrawShadowedText(CLocalizationSettings::Instance()->GetLocalText("RACE TIME"), Font12, racetimex, racetimey, ColorLA);
		DrawShadowedText(MakeTimeString(CPlayGamePhase::Instance->RaceTime), Font16, racetimenumx, racetimenumy, ColorA);
	}

	if (ShowWeapon)
	{
		// Weapons
		if (CGameSettings::ClientInstance->GetWeapons())
		{
			if (TopPickups)
			{
				W = (int)((float)TopPickups->Width * WOffset());
				H = (int)((float)TopPickups->Height * HOffset());
				X = AbsX(weaponpickupx);
				Y = AbsY(1.0f) - H;
				WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, TopPickups);

				X = AbsX(minepickupx);
				WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, TopPickups);
			}

			if(Weapon)
			{
				WeCanvas::Instance()->DrawRectangle(
					WeaponPictureEX,
					WeaponPictureEY,
					(int)((float)Weapon->Width * WOffset()),
					(int)((float)Weapon->Height * HOffset()),
					D3DCOLOR_XRGB(255,255,255),
					Weapon);
			}

			if(Mine)
			{
				WeCanvas::Instance()->DrawRectangle(
					MinePictureEX,
					MinePictureEY,
					(int)((float)Mine->Width * WOffset()),
					(int)((float)Mine->Height * HOffset()),
					D3DCOLOR_XRGB(255,255,255),
					Mine);
			}
		}
	}

	float BuffBackY = buffbacky;
	// Pickup Buffs
	for (Tuber->PickupActions.First(); !Tuber->PickupActions.End(); Tuber->PickupActions.Next())
	{
		WeTexture *BuffPic = NULL;
		CPickupAction *Action = Tuber->PickupActions.GetItem();

		switch (Action->GetActionType())
		{
		case PickupActionInvisibility:
			BuffPic = BuffInvisibility;
			break;
		case PickupActionQuadDamage:
			BuffPic = BuffQuadDamage;
			break;
		case PickupActionInvulnerability:
			BuffPic = BuffInvulnerability;
			break;
		}

		if (!BuffPic) continue;

		W = IntX(BuffBackground->Width);
		H = IntY(BuffBackground->Height);
		X = AbsX(1.0f) - W;
		Y = AbsY(BuffBackY);
		WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, BuffBackground);

		WeColor BuffColor = ColorA;
		if (Action->GetTimeRemaining() <= 3.0f)
		{
			BuffColor = ColorA;
			BuffColor.a *= ((1.0f + sin(10.0f * Action->GetTimeRemaining())) / 2.0f);
		}

		X = AbsX(1.0f) - IntX(BuffPic->Width);
		Y += (int)(((float)H / 2.0f) - ((float)IntY(BuffPic->Height) / 2.0f));
		WeCanvas::Instance()->DrawRectangle(X, Y, IntX(BuffPic->Width), IntY(BuffPic->Height), BuffColor, BuffPic);
		BuffBackY += RelY(H) + buffbackspready;
	}
}


void CMainGameInterface::DrawShadowedText(String Text, WeFont* Font, float x, float y, WeColor &Color)
{
	int X = AbsX(x);
	int Y = AbsY(y);
	int SX = X + (int)(3.0f * WOffset());
	int SY = Y + (int)(3.0f * HOffset());
	WeColor ShadowColor = Color;
	ShadowColor *= 0.25f;
	ShadowColor.a = Color.a;
	WeCanvas::Instance()->Draw2DText(Font, Text, SX, SY, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), ShadowColor);
	WeCanvas::Instance()->Draw2DText(Font, Text, X, Y, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), Color);
}


void CMainGameInterface::RenderPositionTable()
{
	const float StartY = 0.2f;

	int PCount = CGameSettings::ClientInstance->GetPlayersCount();
	if (PCount < 2) return;

	static bool PositionsChanged = false;

	if (!PositionsChanged)
	{
		for (int j = 0; j < MAX_PLAYERS; ++j)
		{
			if (!CGameSettings::ClientInstance->tmpPlayers[j].User) continue;

			CTuber *Dude = CGameSettings::ClientInstance->tmpPlayers[j].User->Tuber;
			if (!Dude) continue;
			if (Dude->PrevPosition != Dude->Position)
			{
				PositionDrawTimer = 0.0f;
				PositionsChanged = true;
			}
		}
	}
	else
		PositionDrawTimer += WeTimer::Instance()->GetDeltaTimeF();

	int RW = WeRenderDevice::Instance()->GetResWidth();
	int RH = WeRenderDevice::Instance()->GetResHeight();
	int X, Y, W, H;
	WeColor ColorF = WeColor(1.0f, 1.0f, 1.0f, 1.0f);

	float CurrentY = StartY;

	// top
	W = (int)((float)LeftTop->Width * WOffset());
	H = (int)((float)LeftTop->Height * HOffset());
	X = 0;
	Y = AbsY(CurrentY);
	WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, LeftTop);
	CurrentY += RelY(H);

	// middle
	if (PCount > 2)
	{
		W = (int)((float)LeftMiddle->Width * WOffset());
		H = AbsY(0.03f * (PCount - 2));
		X = 0;
		Y = AbsY(CurrentY);
		WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, LeftMiddle);
		CurrentY += RelY(H);
	}

	// bottom
	W = (int)((float)LeftBottom->Width * WOffset());
	H = (int)((float)LeftBottom->Height * HOffset());
	X = 0;
	Y = AbsY(CurrentY);
	WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, LeftBottom);

	// lines
	CurrentY = StartY + 0.01f;
	for (int i = 0; i < PCount; ++i)
	{
		W = (int)((float)LeftLine->Width * WOffset());
		H = (int)((float)LeftLine->Height * HOffset());
		X = 0;
		Y = AbsY(CurrentY);
		WeCanvas::Instance()->DrawRectangle(X, Y, W, H, ColorF, LeftLine);
		CurrentY += 0.03f;
	}

	// names
	CurrentY = StartY + 0.02f;
	char Name[100];
	for (int i = 0; i < PCount; ++i)
	{
		CTuber *Player = NULL;
		for (int j = 0; j < MAX_PLAYERS; ++j)
		{
			if (!CGameSettings::ClientInstance->tmpPlayers[j].User) continue;
			if (!CGameSettings::ClientInstance->tmpPlayers[j].User->Tuber) continue;
			if (CGameSettings::ClientInstance->tmpPlayers[j].User->Tuber->Position == (i + 1))
			{
				Player = CGameSettings::ClientInstance->tmpPlayers[j].User->Tuber;
				break;
			}
		}
		if (!Player) continue;

		WeColor TextColor = Player->Trail->SegmentColor;
		//if (Player->RaceFinished && Player->Win)
		//	TextColor = WeColor(1.0f, 1.0f, 0.0f, 0.7f);
		//else if (Player->RaceFinished && !Player->Win)
		//	TextColor = WeColor(1.0f, 0.0f, 0.0f, 0.7f);
		//else if (CMainGamePhase::Instance->InputPlayer == Player)
		//	TextColor = WeColor(0.0f, 1.0f, 0.0f, 0.7f);
		//else
		//	TextColor = WeColor(1.0f, 1.0f, 1.0f, 0.7f);

		float Factor = PositionDrawTimer / 0.5f;
		if (Factor > 1.0f) Factor = 1.0f;

		float DestY = CurrentY;
		float DestX = 0.01f;
		if ( PositionsChanged && (Player->PrevPosition != Player->Position) )
		{
			float OldPos = StartY + 0.02f + ((Player->PrevPosition - 1) * 0.03f);
			DestY = OldPos + (Factor * (CurrentY - OldPos));

			float Fact;
			float Sign;
			if (Factor < 0.5f)
				Fact = Factor;
			else
				Fact = (1.0f - Factor);
			Sign = (Player->PrevPosition < Player->Position) ? 1.0f : -1.0f;
			DestX = 0.01f + (Sign * Fact * 0.03f);

			if (Factor == 1.0f)
				Player->PrevPosition = Player->Position;
		}

		sprintf(Name, "%d. %s", (i + 1), Player->OwnerProfile->Name.GetString());
		String PlayerName = String(Name);
		if (PlayerName.GetLen() > 12)
		{
			PlayerName = "";
			for (int i = 0; i < 12; ++i)
				PlayerName.InsertChar(PlayerName.GetLen(), Name[i]);
		}
		DrawShadowedText(PlayerName, Font12, DestX, DestY, TextColor);
		CurrentY += 0.03f;
	}
	if (PositionDrawTimer > 0.6f)
		PositionsChanged = false;
}


void CMainGameInterface::RenderMineLock()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->DeployedMines.GetItemsCount() > 0
			)
		{
			CTuber *Gari = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;
			for (Gari->DeployedMines.First(); !Gari->DeployedMines.End(); Gari->DeployedMines.Next())
			{
				CWeapon *Mine = Gari->DeployedMines.GetItem();
				RenderItemLock(true, Mine->MineLockFactor, Mine->Location);
			}
		}
	}
}


void CMainGameInterface::RenderFPS()
{
	if (ShowFPS)
	{
		char fps[100];
		sprintf(fps, "FPS: %3.3f", WeTimer::Instance()->GetFPS());
		CMainGameInterface::DrawShadowedText(String(fps), CMainGameInterface::Instance->Font12, 0.005f, 0.15f, WeColor(1.0f, 1.0f, 1.0f, 0.7f));

#ifdef _DEBUG
		char temp[1000];

		sprintf(temp, "Polygons: %d", WeProfiler::Instance()->PolygonsRendered);
		CMainGameInterface::DrawShadowedText(String(temp), CMainGameInterface::Instance->Font12, 0.005f, 0.2f, WeColor(1.0f, 1.0f, 1.0f, 0.7f));

		temp[0] = (char)0;
		for (int i = 0; i <= 4; ++i)
			sprintf(&temp[strlen(temp)], " %d[%d]", i, (int)(WeProfiler::Instance()->GetBlockPercent(i) * 100.0f + 0.5f));
		//CMainGameInterface::DrawShadowedText(String(temp), CMainGameInterface::Instance->Font12, 0.005f, 0.25f, WeColor(1.0f, 1.0f, 1.0f, 0.7f));

		temp[0] = (char)0;
		for (int i = 10; i <= 16; ++i)
			sprintf(&temp[strlen(temp)], " %d[%d]", i, (int)(WeProfiler::Instance()->GetBlockPercent(i) * 100.0f + 0.5f));
		for (int i = 20; i <= 35; ++i)
			sprintf(&temp[strlen(temp)], " %d[%d]", i, (int)(WeProfiler::Instance()->GetBlockPercent(i) * 100.0f + 0.5f));
		//CMainGameInterface::DrawShadowedText(String(temp), CMainGameInterface::Instance->Font12, 0.005f, 0.3f, WeColor(1.0f, 1.0f, 1.0f, 0.7f));
#endif
	}
}



void CMainGameInterface::RenderPickupLock()
{
	for (CMainGamePhase::Instance->Map.Pickups.First(); !CMainGamePhase::Instance->Map.Pickups.End(); CMainGamePhase::Instance->Map.Pickups.Next())
	{
		CPickUp *Thingy = CMainGamePhase::Instance->Map.Pickups.GetItem();
		if (Thingy->Active)
		{
			switch (Thingy->Type)
			{
			case PICKUPTYPE_COINSMALL:
			case PICKUPTYPE_COINMEDIUM:
			case PICKUPTYPE_COINLARGE:
				continue;
			default:
				RenderItemLock(false, Thingy->LockFactor, Thingy->Location);
				break;
			}
		}
	}
}


void CMainGameInterface::RenderItemLock(bool Red, float &LockFactor, const WeVector &Location)
{
	WeVector Distance = Location - Tuber->Location;
	float Dist = D3DXVec3Length(&Distance);
	D3DXVec3Normalize(&Distance, &Distance);

	float Dot = D3DXVec3Dot(&Distance, &Tuber->Direction);
	if ( (Dot < 0.5f) || (Dist > 600.0f) )
	{
		LockFactor = 0.0f;
		return;
	}

	WeRaycastHit Hit;
	if (WePhysics::Instance()->RaycastClosest(Tuber->Location, Distance, WE_SHAPE_TYPE_STATIC, Hit, 7, Dist))
	{
		LockFactor = 0.0f;
		return;
	}

	WeMatrix Mat;
	D3DXMatrixMultiply(&Mat, &CMainGamePhase::Instance->Camera.View, &CMainGamePhase::Instance->Camera.Projection);

	WeVector Loc;
	D3DXVECTOR4 Out;
	D3DXVec3Transform(&Out, &Location, &Mat);
	Loc.x = ((Out.x / Out.w) + 1.0f) / 2.0f;
	Loc.y = (((Out.y * -1.0f) / Out.w) + 1.0f) / 2.0f;

	if (Out.z <= 0.0f)
	{
		LockFactor = 0.0f;
		return;
	}

	LockFactor += 2.0f * WeTimer::Instance()->GetDeltaTimeF();

	WeTexture *Left = LeftGreen;
	WeTexture *Right = RightGreen;
	if (Red)
	{
		Left = LeftRed;
		Right = RightRed;
	}

	float X, Y;
	int WX, WY;

	float Factor = LockFactor;
	if (Factor > 1.0f) Factor = 1.0f;
	float Alpha = (Factor * 0.8f);

	WX = IntX(Left->Width);
	WY = IntY(Left->Height);
	X = (Factor * Loc.x) - (RelX(WX) / 2.0f);
	X = X - 0.03f - (sin(4.0f * LockFactor) * 0.01f);
	Y = Loc.y - (RelY(WY) / 2.0f);
	WeCanvas::Instance()->DrawRectangle(AbsX(X), AbsY(Y), WX, WY, WeColor(1.0f, 1.0f, 1.0f, Factor), Left);

	WX = IntX(Left->Width);
	WY = IntY(Left->Height);
	X = (Loc.x + ((1.0f - Factor) * (1.0f - Loc.x))) - (RelX(WX) / 2.0f);
	X = X + 0.03f + (sin(4.0f * LockFactor) * 0.01f);
	Y = Loc.y - (RelY(WY) / 2.0f);
	Alpha = (Factor * 0.8f);
	WeCanvas::Instance()->DrawRectangle(AbsX(X), AbsY(Y), WX, WY, WeColor(1.0f, 1.0f, 1.0f, Factor), Right);
}


void CMainGameInterface::RenderHealing()
{
	if (!Tuber->HealingInProgress) return;

	String HealingText = CLocalizationSettings::Instance()->GetLocalText("HEALING");
	float x = CUberText::MeasureTextWidth(HealingText, CMainGameInterface::Instance->Font20);
	float RedFactor = 0.5f + ((1.0f + (sin(10.0f * WeTimer::Instance()->GetAbsoluteTime()))) / 4.0f);
	WeColor Color = WeColor(RedFactor, 0.0f, 0.0f, 0.7f);
	CMainGameInterface::DrawShadowedText(HealingText, CMainGameInterface::Instance->Font20, (0.5f - (x / 2.0f)), 0.85f, Color);
}


void CMainGameInterface::TickDisplayCounters()
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	const float MIN_SCORE_SPEED = 100.0f;
	const float MIN_COINS_SPEED = 10.0f;

	if (FinalDisplayScore != Tuber->Score)
	{
		FinalDisplayScore = Tuber->Score;
		DisplayScoreSpeed = (float)(FinalDisplayScore - DisplayScore) / 2.0f;
		if (abs(DisplayScoreSpeed) < MIN_SCORE_SPEED)
			DisplayScoreSpeed = (DisplayScoreSpeed < 0.0f) ? -MIN_SCORE_SPEED : MIN_SCORE_SPEED;
	}
	if (DisplayScore != FinalDisplayScore)
	{
		int Addon = (int)(DisplayScoreSpeed * time);
		if (abs(Addon) > abs(FinalDisplayScore - DisplayScore))
			Addon = FinalDisplayScore - DisplayScore;
		DisplayScore += Addon;
	}

	DisplayCoins = Tuber->Coins;
	//if (FinalDisplayCoins != Tuber->Coins)
	//{
	//	FinalDisplayCoins = Tuber->Coins;
	//	DisplayCoinsSpeed = (float)(FinalDisplayCoins - DisplayCoins) / 1.0f;
	//	if (abs(DisplayCoinsSpeed) < MIN_COINS_SPEED)
	//		DisplayCoinsSpeed = (DisplayCoinsSpeed < 0.0f) ? -MIN_COINS_SPEED : MIN_COINS_SPEED;
	//}
	//if (DisplayCoins != FinalDisplayCoins)
	//{
	//	int Addon = (int)(DisplayCoinsSpeed * time);
	//	if (abs(Addon) > abs(FinalDisplayCoins - DisplayCoins))
	//		Addon = FinalDisplayCoins - DisplayCoins;
	//	DisplayCoins += Addon;
	//}
}


void CMainGameInterface::TickDistantBoom()
{
	if (!AddDistantExplosion) return;

	char casablanca[100];
	sprintf(casablanca, "Sounds.Explosions.DistantExplosion%d", Rnd(1, 3));
	WeSound *Boom = WeAudioDevice::Instance()->LoadSoundAuto(String(casablanca), false);
	if (Boom)
	{
		Boom->Play(false);
		Boom->DieOnStop();
	}

	AddDistantExplosion = false;
}



void CMainGameInterface::RenderModelNames()
{
	if (!ShowModelNames) return;

	WeActor *FirstActor = CMainGamePhase::Instance->Map.Actors;
	for (WeActor *Iterator = FirstActor->NextActor; Iterator != FirstActor; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsParticleSystem) continue;
		if (Iterator->IsLight) continue;

		if (Iterator->IntersectFrustum(&CMainGamePhase::Instance->Camera.Frustum))
		{
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &Iterator->Location, &CMainGamePhase::Instance->Camera.ViewProj);

			if (v.z > 0.0f)
			{
				char temp[1000];
				sprintf(temp, "%s [%d : %.3f]", Iterator->ModelName.GetString(), Iterator->Type, v.z);
				DrawShadowedText((String)temp, Font12, (v.x / v.w + 1.0f) / 2.0f, (1.0f - v.y / v.w) / 2.0f, WeColor(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}


void CMainGameInterface::SetTimeTextBeepFactor(float Value)
{
	TimeTextBeepFactor = Value;
}

float CMainGameInterface::GetTimeTextBeepFactor()
{
	return TimeTextBeepFactor;
}

void CMainGameInterface::TickTextBeepSound()
{
	const float MIN_BEEP_TIME = 0.3f;

	if (TimeTextBeepFactor == 0.0f) return;

	TimeTextBeepSoundFactor += WeTimer::Instance()->GetDeltaTimeF();
	if ( (TimeTextBeepSoundFactor >= MIN_BEEP_TIME) && (TimeTextBeepSoundFactor >= (1.0f - TimeTextBeepFactor)) )
	{
		if (TimeTextBeep)
			TimeTextBeep->Play(false, true);
		TimeTextBeepSoundFactor = 0.0f;
	}
}


void CMainGameInterface::PlayHeadlightSound()
{
	WeSound *HLSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Tuber.HeadLight", false);
	HLSound->Play(false);
	HLSound->DieOnStop();
}
