/*
   CPlayGamePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CPlayGamePhase_H
#define _CPlayGamePhase_H

#include <CAbstractPhase.h>
#include <CGamePlayerConfig.h>
#include <WeVector.h>
#include <WeCEGUI.h>
#include <CString.h>
#include <WeRenderDevice.h>


class CInputMessage;
class WeActor;
class CExplosion;
class CWeapon;
class CTrigger;
class WeAnimTexture;
class CPickUp;
class CTuberPart;
class CAIKernel;
class CTuber;


enum PLAYGAMESTATUS
{
	PLAYGAME_WAITINGFORPLAYERS = 0,
	PLAYGAME_HOSTLEFT = 1,
	PLAYGAME_COUNTDOWN = 2,
	PLAYGAME_PREPARECOUNTDOWN = 3,
	PLAYGAME_RACE = 4,
	PLAYGAME_GAMEFINISHED = 5,
	PLAYGAME_GAMEPAUSED = 6
};

class CPlayGamePhase: public CAbstractPhase
{
public:
	static CPlayGamePhase*	Instance;

							CPlayGamePhase();

	bool					ShowInGameMenu;
	float					GameFinishedTimer;
	float					RaceTime;
	void					Tick();
	void					Start();
	void					End();
	void					Render();
	void					Server_CheckGameFinished();
	void					PrepareAnimTextures(int i);

	float					SafetyLockTimer;

	//WeAnimTexture*		Anim321;
	//WeAnimTexture*		AnimGO;
	//WeAnimTexture*		FinishTextWin;
	//WeAnimTexture*		FinishTextLose;
	//WeAnimTexture*		WinningText;

	static void				OnCollision(WeActor *a1, WeActor *a2, WeVector &Normal, WeVector &Tangent, DWORD Flag);
	static void				OnCollision(WeActor *a1, WeActor *a2, WeVector &Normal, WeVector &Tangent, DWORD Flag, WeVector &FirstTouchPoint);
	static void				OnTrigger(WeActor *a1, WeActor *a2, DWORD Flag);
	static void				OnTeleport(WeActor *Actor, WeActor *Dest);

	PLAYGAMESTATUS			Status;
	PLAYGAMESTATUS			StatusSaved;

	static bool				ActorPushable(WeActor *Who);

	CAIKernel				*GetAIKernel() { return AIKernel; }


protected:
	void					TickPickupAnimation();
	void					TickCheats();

	int						TimeTrialLap;
	float					TimeTrialLapTimeLimit;
	float					TimeTrialLapTime;

	int						CountdownCounter;
	float					CountdownTimer;

	static int				RndID;
	static bool				PrevRand;
	void					Server_SetPositions();
	void					DoSingleRaceAndInstantRaceLogiks();
	void					DoSingleRaceTimeTrialLogiks();
	void					DoSPLastOneAndMPLastOneLogiks();
	void					DoSPAndMPTournamentLogiks();	
	static void				DoPickUpLogiks(WeActor* a1, WeActor* a2);
	static bool				CreatePickUpByType(int _Type, CPickUp* Pickup, WeActor* Actor, bool Animate);

    void					MakeStatus();
	void					CheckReadyForRace();
	void					DoStatus();

	void					RenderCountDown();
	void					RenderSafetyLock();
	void					TickPrepareCount();
	bool					OkForExit;

	void					RenderFinishAnim();

	// lastone logic
	int						PrevMaxLap;

	// AI stuff
	CAIKernel				*AIKernel;

	// animation stuff
	bool					EndAnimPlayed;

	// input player saving
	CTuber					*LocalHumanPlayer;

	static void				DisableAllCollisions();
	static void				EnablePredefCollisions();
	static void				SetCollisionPair(DWORD GroupOne, DWORD GroupTwo, bool Enable);

	static bool				CollideWeapon(CWeapon*, WeActor*);
	static bool				CollideMine(CWeapon*, WeActor*);
	static bool				CollideExplosion(CExplosion*, WeActor*);
	static bool				CollideTrigger(CTrigger*, WeActor*);
	static bool				CollideFragment(CTuberPart*, WeActor*);
	static void				CollideTubers(CTuber*, CTuber*, WeVector &TouchPoint);

	static bool				OnBackToMenu(const CEGUI::EventArgs& e);
	static bool				OnBackToGame(const CEGUI::EventArgs& e);
	static bool				OnExitGame(const CEGUI::EventArgs& e);
	static bool				OnOptionsMenu(const CEGUI::EventArgs& e);

	float					RelX(int x)		{ return ((float)x / (float)WeRenderDevice::Instance()->GetResWidth()); }
	float					RelY(int y)		{ return ((float)y / (float)WeRenderDevice::Instance()->GetResHeight()); }
};



#endif
