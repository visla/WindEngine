/*
     File: CMainGameInterface.h
	 Author: Tomislav Kukic
	 Date: 27.03.2006
	 Version: 1.0
*/




#ifndef _CMainGameInterface_h_
#define _CMainGameInterface_h_


#include <Windows.h>
#include <CString.h>
#include <WeRenderDevice.h>
#include <Constants.h>

#define DEFAULT_TIME_SEPARATOR	":"

class CTuber;
class WeTexture;
class WeFont;
class CUberText;
class WeAnimTexture;
class WeSound;

class CMainGameInterface
{
protected:
	bool Initialized;

	WeTexture*		Left;
	WeTexture*		LeftFill;
	WeTexture*		Right;
	WeTexture*		RightFill;
	WeTexture*		TopLeft;
	WeTexture*		TopRight;
	WeTexture*		LeftMiddle;
	WeTexture*		LeftTop;
	WeTexture*		LeftBottom;
	WeTexture*		LeftLine;

	WeTexture*		SpeedTex[2];
	WeTexture*		TurboSpeedTex[2];
	WeTexture*		EnergyTex[2];
	WeTexture*		TurboTex[2];

	WeTexture*		LockLeft;
	WeTexture*		LockRight;
	WeTexture*		LockMiddle;

	WeTexture*		TopPickups;

	WeTexture*		Blood;

	WeTexture*		LeftRed;
	WeTexture*		RightRed;
	WeTexture*		LeftGreen;
	WeTexture*		RightGreen;

	WeTexture*		BuffBackground;
	WeTexture*		BuffInvulnerability;
	WeTexture*		BuffInvisibility;
	WeTexture*		BuffQuadDamage;

	void			RenderUberText();
	void			RenderTargetLock();
	void			RenderWeaponLock();
	void			RenderEnemyTargetLock();
	void			RenderWrongWay();
	void			RenderBlood();
	void			RenderTuberNames();
	void			RenderHud();
	void			RenderPositionTable();
	void			RenderFPS();
	void			RenderHealing();
	void			RenderModelNames();
	
	void			RenderMineLock();
	void			RenderPickupLock();
	void			RenderItemLock(bool Red, float &LockFactor, const WeVector &Location);

	float			WeaponLockTimer;
	float			TargetLockTimer;
	float			WrongWayTimer;

	float			BloodTimer;

	float			PositionDrawTimer;

	float			TimeTextBeepFactor;
	float			TimeTextBeepSoundFactor;

	int				DisplayCoins;
	int				DisplayScore;
	int				FinalDisplayCoins;
	int				FinalDisplayScore;
	float			DisplayCoinsSpeed;
	float			DisplayScoreSpeed;

	WeSound*		HeartBeatSound;
	WeSound*		EnemyHitSound;
	WeSound*		EnemyHitImmortalSound;
	WeSound*		TimeTextBeep;

	void			TickHealthSound();
	void			TickHitSound();
	void			TickWeaponPics();
	void			TickDisplayCounters();
	void			TickDistantBoom();
	void			TickTextBeepSound();


public:
	WeFont* Font12;
	WeFont* Font16;
	WeFont* Font20;
	WeFont* Font24;
	WeFont* Font32;
	WeFont* FontFixed;

	static CMainGameInterface* Instance;

	CMainGameInterface();
	~CMainGameInterface();

	int WeaponPictureEX;
	int WeaponPictureEY;

	int MinePictureEX;
	int MinePictureEY;

	bool AnimatingWeapon;
	bool AnimatingMine;

	CTuber* Tuber;
	bool Renderable;
	bool ShowFPS;
	bool ShowShaderInfo;
	bool ShowWeapon;
	bool ShowStats;
	bool ShowModelNames;

	bool AddDistantExplosion;


	WeTexture* Weapon;
	WeTexture* Mine;

	float ShakeOffsetX;
	float ShakeOffsetY;

	CSmartList<CUberText*> UberLines;

	String MakeTimeString(float Time, String Separator = DEFAULT_TIME_SEPARATOR);	//Time is in seconds

	void SetTuber(CTuber* _Tuber = NULL);
	void Tick();
	void Render();
	void Initialize();
	void InitializeFonts();
	void ShutDown();

	void PlayHeadlightSound();

	void	SetTimeTextBeepFactor(float Value);
	float	GetTimeTextBeepFactor();

	static void				DrawShadowedText(String Text, WeFont* Font, float x, float y, WeColor &Color = WeColor(1.0f, 1.0f, 1.0f, 1.0f));

};






#endif
