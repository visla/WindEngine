/*
		File: CTuber.h
		Author: Nebojsa Vislavski
		Date: 23.06.2006
		Version: 1.0
*/




#ifndef _CTuber_h_
#define _CTuber_h_

#define TURBO_USAGE_PER_SEC	50.0f

#define FS_DAMAGE_NOTIFY_SPREAD	0.0f

#define MAX_LOCK_DISTANCE		500.0f


#include <SmartList.h>
#include <CWeaponFabric.h>
#include <WeNetworkActor.h>
#include <CTuberDescription.h>
#include <CProfile.h>
#include <WeRadialTrail.h>
#include <WeLight.h>
#include <CTuberMuzzle.h>
#include <CInputMessage.h>
#include <WePathNode.h>
#include <CSparticles.h>


class CInputMessage;
class CPickUp;
class CStartLocation;
class WeSound;
class CTuberSparks;
class WePostProcessEffect;
class CWeapon;
class CPickupAction;
class CUberText;
class CTuberPart;
class WeParticleSystem;


enum TuberFlash
{
	TuberFlashNone,
	TuberFlashNuke,
	TuberFlashNuke2,
	TuberFlashTeleport,
	TuberFlashTeleport2,
	TuberFlashCoin,
	TuberFlashCoin2,
	TuberFlashFHDRI,
	TuberFlashFHDRI2,
	TuberFlashTuberBoom,
	TuberFlashTuberBoom2
};

enum TuberPP
{
	TuberPPNone,
	TuberPPGhost,
	TuberPPFadeToMonochrome,
	TuberPPMonochrome,
	TuberPPFadeToColor
};

enum TuberMaterialPreset
{
	TuberMaterialPresetNormal = 0,
	TuberMaterialPresetFading = 1,
	TuberMaterialPresetInvisibility = 2
};


class CTuber:public WeNetworkActor
{
protected:
	WePathNode* NextPathNode;
	CStartLocation* MyStartLocation;
	WeVector _OldLocation;
	float _OldPathIndex; // RefPathNode->PathIndex


	void				SetTrailColor();
	DWORD				Frequency;

	// postproces distorsions
	TuberPP				ActivePPEffect;
	WePostProcessEffect	*PPEffect;
	float				PPEffectTime;
	float				PPEffectTimer;

	WeActor*			Flares;
	WeActor*			Trails;
	WeActor*			Speedy;
	WeActor*			QuadDamage;
	WeActor*			HeadlightModel;
	WeActor*			InvulnerabilityModel;
	WeActor*			RefractionModel;
	WeActor*			TurboModel;
	WeActor*			InvisibilityModel;

	CTuber*				GetBestEnemyTargetLock();
	bool				CheckEnemyTargetLock();
	void				ClearEnemyTargetLock();
	bool				bInitialized;
	void				InitializePropulsion();


public:
	static WeParticleSystem	*StaticTrailPS;

						CTuber();
						~CTuber();


	String				TuberName;
	CProfile			*OwnerProfile;   // ko je vlasnik ovo tuber-a
	CTuberDescription	Description; // maxspeed, maxacceleration, manouverability, ...
	CInputMessage		InputMessage;
	float				MaxSpeed;
	float				Acceleration;	// trenutno ubrzanje. Ne menja se
	float				CurrentSpeed;
	float				RearAcceleration;
	float				YawManouver;  // Levo / Desno
	float				PitchManouver; // Gore / Dole  za 40% vece od YawChangeSpeed
	float				TurboMaxSpeed;
	float				TurboAcceleration; // 
	float				PrimaryWeaponFireTime;
	float				SecondaryWeaponFireTime;

	float				AfterTeleportedTimer; // timer after teleportation. If negative no teleporting has occured

	float				LastLapTime;
	float				StartLapTime;
	float				LapTime;			// koliko se dugo vozi ovaj krug	
	float				RaceTime;
	float				BestLapTime;

	WeWorld*			TuberWorld;

	WEAPONTYPE			PrimaryWeaponID;
	WEAPONTYPE			SecondaryWeaponID;
	WEAPONTYPE			PrimaryWeaponAddonID; // ID dodatnog oruzija
	WEAPONTYPE			SecondaryWeaponAddonID;

	WeSound*			SoundEngine;
	WeSound*			SoundTurbo;
	WeSound*			SoundLocking;
	WeSound*			SoundQuadFire;
	WeSound*			SoundMultiKill;
	WeSound*			SoundLapCount;
	WeSound*			SoundTeleport;
	WeSound*			SoundScratch;
	WeSound*			SoundScratchLoop;

	WeParticleSystem*	Propulsion[10];
	WeVector			RelativePropulsion[10];

	int					PrimaryWeaponAmmo;
	int					SecondaryWeaponAmmo;	
	int					PrimaryWeaponAmmo2;
	int					SecondaryWeaponAmmo2;
	float				TurboCharges;
	float				MaxTurboCharges;
	float				HP;
	float				MaxHP;

	int					Score;
	int					Kills;
	int					Coins;
	int					CashCollected;

	// for external velocity manipulation
	//WeVector			AddonVelocity;
	//int					AddonVelocityResetCounter;
	// for turbo
	bool				OnTurbo;   // da li je turbo ukljucen
	float				FOV;
	float				BlurFactor;
	// other blurring
	float				AddonBlurFactor;
	// for trail
	WeRadialTrail*		Trail;
	WeLight*			Light;
	WeLight*			Headlight;
	WeLight*			LightProjector;

	// weapon muzzles
	CTuberMuzzle		*Muzzle[2];
	// for brightness flashes
	TuberFlash			PreviousFlash;
	TuberFlash			CurrentFlash;
	float				FlashTimer;
	float				FlashFactor;
	float				CurrentFlashFactor;
	// collision sparkies
	CTuberSparks*		Sparks;
	// collision parts
	float				TuberPartTimer;
	// enemy target locking
	float				EnemyLockTimer;
	float				EnemyLockFactor;
	CTuber*				EnemyTarget;
	WEAPONTYPE			EnemyLockWeaponID;
	// target lock notifier
	bool				HasTargetLock;
	// weapon lock notifier
	bool				HasWeaponLock;
	float				WeaponLockDistance;
	CWeapon*			ClosestLockedWeapon;
	// score logic stuff
	float				ScoreTimer;
	// required damage / death info
	CTuber*				LastAttacker;
	WEAPONTYPE			LastDamageWeaponID;
	float				KillTime;
	int					QuickKillCounter;
	CUberText*			QuickKillText;
	// fullscreen damage notifier info
	float				FSDamageNotifyTimer;
	bool				FSDamageNotify;
	bool				FSDamageNotifyReset;
	// enemy hit sound
	CTuber*				LastEnemyHit;
	// deployed mine collection
	CSmartList<CWeapon*>	DeployedMines;
	// for position calcs
	int					PrevPosition;
	// for FHDRI (Fake HDRI :P)
	WeColor				FHDRIColor;
	float				FHDRIFactor;
	float				FHDRIFactorPrev;
	// end race stuff
	float				EndRaceTimer;
	// camera shaking stuff
	float				PrevSpeed;
	float				MaxSpeedShakeTimer;
	// catch up
	float				CatchUpTimer;
	float				CatchUpSpeedFactor;
	// speedy
	float				SpeedyTimer;
	float				SpeedyAlpha;
	// turbo model stuff
	float				TurboTimer;
	// healing
	bool				HealingInProgress;
	// tuber-tuber collision
	float				TuberCollisionTimer;
	// explosion audio mute
	float				MuteAudioSavedValue;
	float				MuteAudioTimer;
	bool				MuteAudioEnable;

	// pickup stuff
	bool				Invisible;
	bool				Invulnerable;
	float				StrengthMultiplier;

	// tweak stuff
	bool				InfTurbo;


	// TEMP
	bool				tb;
	bool				WrongWay;

	bool				RaceFinished;
	bool				Win;

	// za trku
	int					LapCount;			// koliko je krugova prosao
	int					ActualLapCount;		// smanjuje se kada se unazad prodje kroz startnu lokaciju
	//WePathNode*			LastPathNode;		// poslednja noda koju je prosao
	//int					PathNodeCounter;	// broj noda koje je prosao
	WePathNode*			RefPathNode;		// najbliza noda iza igraca, NULL ako je na pocetku kruga
	//int				RefPathNode->SortedIndex;	// indeks najblize node iza igraca, -1 na pocetku kruga
	float				RefDistance;		// daljina od RefPathNode, odnosno od StartPosition na pocetku kruga
	int					Position;			// na kojoj je poziciji

	CSmartList<CPickupAction*> PickupActions;
	bool				Dead;
	float				RespawnCounter;
	float				RespawnTime;

	// first part for camera following
	CTuberPart			*FirstPart;

	float				SoundScratchTimer;
	float				SoundScratchLoopTimer;

	CSparticles			*TuberSparticles;

	virtual void		OnTeleported();
	void				Tick();
	void				TickTubeLimit();
	void				TickTurbo();
	void				TickScreenFlash();
	void				TickSound();
	void				TickPPEffect();
	void				TickLevitate();
	void				TickTargetLock();
	void				TickScoreLogic();
	void				TickPickups();
	void				TickLapLogic();
	void				TickPathNodes();
	void				TickHealing();
	void				TickFHDRI™();
	void				TickLight();
	void				TickCatchUp();
	void				TickPropulsion(bool FPSCam);
	void				TickAddons(bool FPSCam);
	void				TickMuteAudio();
	void				ProcessInput(CInputMessage* Msg);
	void			    LapFinished();   // - ovo se poziva kada se predje krug
	void				Respawn();  
	void				ResetValues(); // this will reset values to defaults
	void				Initialize();
	void				Die();
	void				SetPPEffect(TuberPP Effect, float Lifetime);
	void				DeathTextOutput();
	void				ClearPickups();
	void				FireWeapon(int ID);
	void				DisplayLapText(float LastLapTime);
	float				TakeDamage(float damage);	// returns how much damage is really taken

	bool				IsPushable();
	bool				IsFriendWith(CTuber* Candidate);
	bool				ValidateTarget(CTuber* Candidate);
	CTuber*				GetBestEnemyTargetLock(float Distance, float MinDot);

	int					GetLapsRemaining();

	int					DistanceInPathNodes(CTuber* Target);

	void				MultiplyVelocity(float Factor);

	void				OnWallScratch(WeVector &ScratchLocation);


	void				OnRender() { }
	// Network methods
	void				Client_MakeDeltaCommand(WeNetworkMessage *Msg);
	void				Server_HandleDeltaCommand(WeNetworkMessage *Msg);	
	void				Client_HandleSpawnMessage(WeNetworkMessage *Msg, bool IsInputPlayer);	
	void				Server_MakeSpawnMessage(WeNetworkMessage *Msg);
	void				Client_HandleReplication(WeNetworkMessage *Msg);
	void				Server_Replicate(WeNetworkActor *Avatar, WeNetworkMessage *Msg);
	void				Client_HandleGameMessage(WeNetworkMessageType Type, WeNetworkMessage *Msg);
};




#endif
