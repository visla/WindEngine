/*
    CControllerSettings.h
	Autor: Tomislav Kukic
	Date: 17.3.2006.
	Version: 1.0    
*/



#ifndef _CControllerSettings_h_
#define _CControllerSettings_h_



#include <WeStream.h>
class CInputMessage;


class CControllerSettings
{
public:
	CControllerSettings();
	~CControllerSettings(){}

	//static CControllerSettings* Instance;

	int		Version;

	int		MoveForward;
	bool	MoveForwardOnMouse;
	bool	MoveForwardOnJoy;

	int		MoveBackward;
	bool	MoveBackwardOnMouse;
	bool	MoveBackwardOnJoy;

	int		RotateLeft;
	bool	RotateLeftOnMouse;
	bool	RotateLeftOnJoy;

	int		RotateRight;
	bool	RotateRightOnMouse;
	bool	RotateRightOnJoy;

	int		FireWeapon;
	bool	FireWeaponOnMouse;
	bool	FireWeaponOnJoy;

	int		FireMine;
	bool	FireMineOnMouse;
	bool	FireMineOnJoy;

	int		Turbo;
	bool	TurboOnMouse;
	bool	TurboOnJoy;

	int		GameMenu;
	bool	GameMenuOnMouse;
	bool	GameMenuOnJoy;

	int		LookLeft;
	bool	LookLeftOnMouse;
	bool	LookLeftOnJoy;

	int		LookRight;
	bool	LookRightOnMouse;
	bool	LookRightOnJoy;

	int		LookUp;
	bool	LookUpOnMouse;
	bool	LookUpOnJoy;

	int		LookDown;
	bool	LookDownOnMouse;
	bool	LookDownOnJoy;

	int		ChangeCamera;
	bool	ChangeCameraOnMouse;
	bool	ChangeCameraOnJoy;

	int		ReverseCamera;
	bool	ReverseCameraOnMouse;
	bool	ReverseCameraOnJoy;

	bool	InverseMouse;

	void	ResetToDefaults();
	void	MakeInputMessage(CInputMessage *Msg);

	void	Serialize(WeStream &FS);
	void	Deserialize(WeStream &FS);

	CControllerSettings*		Clone();
};




#endif
