/*
   CShipyardPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CShipyardPhase_H
#define _CShipyardPhase_H

#include <CAbstractPhase.h>
#include <WeCamera.h>
#include <WeActor.h>
#include <WeCEGUI.h>
#include <SmartList.h>
#include <CTuberdescription.h>
#include <WeLight.h>
#include <WeColor.h>
#include <CMap.h>

class CProgressBar;


class CShipyardPhase: public CAbstractPhase
{
	class CTuber 
	{
	protected:
	public:
		CTuber(): Ship(NULL), GroundPlate(NULL) {} 
		~CTuber() { SAFE_DELETE(Ship); SAFE_DELETE(GroundPlate); }

		void Tick();

		bool	CanBeBought;
		WeActor *Ship; 
		WeActor	*GroundPlate;
		CMap	*TuberWorld;
		CTuberDescription TuberDescription;
	};

protected:

	float				CameraDistance;
	float				CameraY;
	float				CameraSpeed;
	float				CameraYSpeed;
	float				CameraYDelta;
	float				CameraLookAtY;
	float				CameraFOV;

	float				ProgressBarsSpeed;
	float				CurrentProgress;

	WeColor				FogColor;
	float				FogStart;
	float				FogEnd;

	//float				PrevAngle;
	//float				CameraAngle;
	//float				DesiredAngle; // koji je ugao do kojeg se mrdamo.
	//float				AnglePerShip;

	int					CurrentShipIndex; // na kom smo trenutno brodu
	CShipyardPhase::CTuber *CurrentTuber; // -II-
	WeLight*			CameraLight;

	float				MaxSpeedProgress;
	float				MaxAccelrationProgress;
	float				MaxManouverabilityProgress;
	float				MaxHPProgress;

	float				CurrentSpeedProgress;
	float				CurrentAccelrationProgress;
	float				CurrentManouverabilityProgress;
	float				CurrentHPProgress;

	float				PrevSpeedProgress;
	float				PrevAccelrationProgress;
	float				PrevManouverabilityProgress;
	float				PrevHPProgress;

	CProgressBar		*LoadingProgressBar;

	bool				MovingToPrevShip;
	bool				MovingToNextShip;

	CEGUI::ProgressBar*	SpeedProgress;
	CEGUI::ProgressBar*	AccelerationProgress;
	CEGUI::ProgressBar*	ManouverabilityProgress;
	CEGUI::ProgressBar*	HPProgress;

	CEGUI::ProgressBar*	PSSpeedProgress;
	CEGUI::ProgressBar*	PSAccelerationProgress;
	CEGUI::ProgressBar*	PSManouverabilityProgress;
	CEGUI::ProgressBar*	PSHPProgress;

	CEGUI::Window*		DescText;
	CEGUI::Window*		ShipName;
	CEGUI::Window*		WeaponName;
	CEGUI::Window*		MineName;
	CEGUI::Window*		PriceText;

	CEGUI::Window*		PSDescText;
	CEGUI::Window*		PSShipName;
	CEGUI::Window*		PSWeaponName;
	CEGUI::Window*		PSMineName;
	CEGUI::Window*		PSPriceText;

	CEGUI::Window*		BuyButton;

    void				CheckBuyButton();

	bool				FirstTime;
	bool				FirstTimeRender;
	bool				Loaded;

	bool				LoadTuberInTick();

	void				CalculateRotatedVector(WeVector *V, float Angle, float Sx, float Sy, float Sz);

	CEGUI::Window*		BuyShipText;
	float				BuyShipTime;

	bool				ShipBought;

	static bool			OnBuyShip(const CEGUI::EventArgs& e);
	static bool			OnBack(const CEGUI::EventArgs& e);
	static bool			OnNextShip(const CEGUI::EventArgs& e);
	static bool			OnPrevShip(const CEGUI::EventArgs& e);

	static bool			OnUpgradeShip(const CEGUI::EventArgs& e);
	static bool			OnStartRace(const CEGUI::EventArgs& e);

	void				DisplayDescriptions();
	void				SetProgress();

	void				UpdateCamera();

public:
	CMap*				ShipyardWorld;
	CMap*				TuberWorld;
	WeCamera*			Camera;

	CSmartList <CShipyardPhase::CTuber *> Tubers;

	static CShipyardPhase*	Instance;

						CShipyardPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();
};


#endif
