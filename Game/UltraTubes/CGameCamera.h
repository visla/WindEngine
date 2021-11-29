/*
       File: CGameCamera.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#ifndef _CGameCamera_h_
#define _CGameCamera_h_


#include <WeCamera.h>
#include <CString.h>

class CTuber;

#define MAX_CAMERAS 10 

enum GameCameraStates
{
	GameCameraStateDefault,
	GameCameraStateShaking,
	GameCameraStateShakingSofting
};

struct CameraConfig
{
	float	BackDistance;
	float	UpDistance;
	float	Distance;
	float	AccelFactor;
	float	MinHitDistance;
	float	MinVisibleDistance;
	float	ViewDirection;
	float	Speed;
	float	AngularSpeed;
	bool	NoShaking;
	bool	FPS;
	bool	Centered;
	bool	Stopping;
	float	LookFactor;
	bool	NoTrail;
	float	MinDistance, MaxDistance;
	float	MinFOV, MaxFOV, TurboFOV                                                          ;
	float	MinFOVSpeed, MaxFOVSpeed; // u procentima maximalne brzine
	bool	NoHitCheck;
};

class CGameCamera: public WeCamera
{
protected:
	float				StateTimer;
	float				FinishedAngle;

	float				Distance;
	float				BackDistance;
	float				UpDistance;

	float				LastPlayerSpeed;

	void ReadCameraConfig(StringParam CameraName, CameraConfig *cc);
	void UpdateCameraLocation(WeVector &Location, WeVector &Direction, int i);

public:
	CGameCamera();
	~CGameCamera(){}

	CTuber*				Tuber;
	bool				FirstPerson;
	GameCameraStates	CurrentState;
	float				StateDefaultingTime;

	void				Init();
	void				Update();

	void				UpdateCurrentCamera();
	void				SetNextCamera();
	void				SetPrevCamera();
	void				SetCamera(int index);

	CameraConfig		CameraCfg[MAX_CAMERAS];
	int					CameraCount;
	int					CurrentCameraIndex;
	CameraConfig		*CurrentCamera;
	CameraConfig		FinishingCamera;
	CameraConfig		ReverseCamera;
};



#endif

