/*
    CControllerSettings.cpp
	Autor: Tomislav Kukic
	Date: 17.3.2006.
	Version: 1.0    
*/


#include <dinput.h>
#include <CControllerSettings.h>
#include <CInputMessage.h>
#include <WeLoger.h>


CControllerSettings::CControllerSettings()
{
	Version = -5;
	ResetToDefaults();
}


void CControllerSettings::MakeInputMessage(CInputMessage *Msg)
{
	if (!Msg) return;
	if (!Msg->Key) return;

	if (MoveForwardOnMouse)
		Msg->MoveForward = (Msg->MouseButtons[MoveForward] != 0);
	else if (MoveForwardOnJoy)
		Msg->MoveForward = (Msg->JoyButtons[MoveForward] != 0);
	else
		Msg->MoveForward = ((Msg->Key[MoveForward] & 0x80) != 0);

	if (MoveBackwardOnMouse)
		Msg->MoveBackward = (Msg->MouseButtons[MoveBackward] != 0);
	else if (MoveBackwardOnJoy)
		Msg->MoveBackward = (Msg->JoyButtons[MoveBackward] != 0);
	else 
		Msg->MoveBackward = ((Msg->Key[MoveBackward] & 0x80) != 0);

	if (RotateLeftOnMouse)
		Msg->RotateLeft = (Msg->MouseButtons[RotateLeft] != 0);
	else if (RotateLeftOnJoy)
		Msg->RotateLeft = (Msg->JoyButtons[RotateLeft] != 0);
	else
		Msg->RotateLeft = ((Msg->Key[RotateLeft] & 0x80) != 0);

	if (RotateRightOnMouse)
		Msg->RotateRight = (Msg->MouseButtons[RotateRight] != 0);
	else if (RotateRightOnJoy)
		Msg->RotateRight = (Msg->JoyButtons[RotateRight] != 0);
	else
		Msg->RotateRight = ((Msg->Key[RotateRight] & 0x80) != 0);

	if (FireMineOnMouse)
		Msg->FireMine = (Msg->MouseButtons[FireMine] != 0);
	else if (FireMineOnJoy)
		Msg->FireMine = (Msg->JoyButtons[FireMine] != 0);
	else
		Msg->FireMine = ((Msg->Key[FireMine] & 0x80) != 0);

	if (FireWeaponOnMouse)
		Msg->FireWeapon = (Msg->MouseButtons[FireWeapon] != 0);
	else if (FireWeaponOnJoy)
		Msg->FireWeapon = (Msg->JoyButtons[FireWeapon] != 0);
	else
		Msg->FireWeapon = ((Msg->Key[FireWeapon] & 0x80) != 0);

	if (TurboOnMouse)
		Msg->Turbo = (Msg->MouseButtons[Turbo] != 0);
	else if (TurboOnJoy)
		Msg->Turbo = (Msg->JoyButtons[Turbo] != 0);
	else
		Msg->Turbo = ((Msg->Key[Turbo] & 0x80) != 0);

	if (LookLeftOnMouse)
		Msg->LookLeft = (Msg->MouseButtons[LookLeft] != 0);
	else if (LookLeftOnJoy)
		Msg->LookLeft = (Msg->JoyButtons[LookLeft] != 0);
	else
		Msg->LookLeft = ((Msg->Key[LookLeft] & 0x80) != 0);

	if (LookRightOnMouse)
		Msg->LookRight = (Msg->MouseButtons[LookRight] != 0);
	else if (LookRightOnJoy)
		Msg->LookRight = (Msg->JoyButtons[LookRight] != 0);
	else
		Msg->LookRight = ((Msg->Key[LookRight] & 0x80) != 0);

	if (LookUpOnMouse)
		Msg->LookUp = (Msg->MouseButtons[LookUp] != 0);
	else if (LookUpOnJoy)
		Msg->LookUp = (Msg->JoyButtons[LookUp] != 0);
	else
		Msg->LookUp = ((Msg->Key[LookUp] & 0x80) != 0);

	if (LookDownOnMouse)
		Msg->LookDown = (Msg->MouseButtons[LookDown] != 0);
	else if (LookDownOnJoy)
		Msg->LookDown = (Msg->JoyButtons[LookDown] != 0);
	else
		Msg->LookDown = ((Msg->Key[LookDown] & 0x80) != 0);

	if (ChangeCameraOnMouse)
		Msg->ChangeCamera = (Msg->MouseButtons[ChangeCamera] != 0);
	else if (ChangeCameraOnJoy)
		Msg->ChangeCamera = (Msg->JoyButtons[ChangeCamera] != 0);
	else
		Msg->ChangeCamera = ((Msg->Key[ChangeCamera] & 0x80) != 0);

	if (ReverseCameraOnMouse)
		Msg->ReverseCamera = (Msg->MouseButtons[ReverseCamera] != 0);
	else if (ReverseCameraOnJoy)
		Msg->ReverseCamera = (Msg->JoyButtons[ReverseCamera] != 0);
	else
		Msg->ReverseCamera = ((Msg->Key[ReverseCamera] & 0x80) != 0);
}

void CControllerSettings::Serialize(WeStream &FS)
{
	FS.WriteInt(Version);

	FS.WriteInt(MoveForward);
	FS.WriteBool(MoveForwardOnMouse);
	FS.WriteBool(MoveForwardOnJoy);
	FS.WriteInt(MoveBackward);
	FS.WriteBool(MoveBackwardOnMouse);
	FS.WriteBool(MoveBackwardOnJoy);
	FS.WriteInt(RotateLeft);
	FS.WriteBool(RotateLeftOnMouse);
	FS.WriteBool(RotateLeftOnJoy);
	FS.WriteInt(RotateRight);
	FS.WriteBool(RotateRightOnMouse);
	FS.WriteBool(RotateRightOnJoy);
	FS.WriteInt(FireWeapon);
	FS.WriteBool(FireWeaponOnMouse);
	FS.WriteBool(FireWeaponOnJoy);
	FS.WriteInt(FireMine);
	FS.WriteBool(FireMineOnMouse);
	FS.WriteBool(FireMineOnJoy);
	FS.WriteInt(Turbo);
	FS.WriteBool(TurboOnMouse);
	FS.WriteBool(TurboOnJoy);
	FS.WriteInt(GameMenu);
	FS.WriteBool(GameMenuOnMouse);
	FS.WriteBool(GameMenuOnJoy);
	FS.WriteBool(InverseMouse);

	FS.WriteInt(LookLeft);
	FS.WriteBool(LookLeftOnMouse);
	FS.WriteBool(LookLeftOnJoy);
	FS.WriteInt(LookRight);
	FS.WriteBool(LookRightOnMouse);
	FS.WriteBool(LookRightOnJoy);
	FS.WriteInt(LookUp);
	FS.WriteBool(LookUpOnMouse);
	FS.WriteBool(LookUpOnJoy);
	FS.WriteInt(LookDown);
	FS.WriteBool(LookDownOnMouse);
	FS.WriteBool(LookDownOnJoy);

	FS.WriteInt(ChangeCamera);
	FS.WriteBool(ChangeCameraOnMouse);
	FS.WriteBool(ChangeCameraOnJoy);

	FS.WriteInt(ReverseCamera);
	FS.WriteBool(ReverseCameraOnMouse);
	FS.WriteBool(ReverseCameraOnJoy);
}


void CControllerSettings::Deserialize(WeStream &FS)
{
	int Ver = FS.ReadInt();

	if ((Ver == 2) || (Ver < 0)) // versioned files!
	{
		switch (Ver)
		{
		case 2: //first versioned file
		case -2: //second versioned file
			MoveForward = FS.ReadInt();
			MoveForwardOnMouse = FS.ReadBool();
			MoveBackward = FS.ReadInt();
			MoveBackwardOnMouse = FS.ReadBool();
			RotateLeft = FS.ReadInt();
			RotateLeftOnMouse = FS.ReadBool();
			RotateRight = FS.ReadInt();
			RotateRightOnMouse = FS.ReadBool();
			FireWeapon = FS.ReadInt();
			FireWeaponOnMouse = FS.ReadBool();
			FireMine = FS.ReadInt();
			FireMineOnMouse = FS.ReadBool();
			Turbo = FS.ReadInt();
			TurboOnMouse = FS.ReadBool();
			GameMenu = FS.ReadInt();
			GameMenuOnMouse = FS.ReadBool();
			InverseMouse = FS.ReadBool();

			LookLeft = FS.ReadInt();
			LookLeftOnMouse = FS.ReadBool();
			LookRight = FS.ReadInt();
			LookRightOnMouse = FS.ReadBool();
			LookUp = FS.ReadInt();
			LookUpOnMouse = FS.ReadBool();
			LookDown = FS.ReadInt();
			LookDownOnMouse = FS.ReadBool();
			break;

		case -3:
			MoveForward = FS.ReadInt();
			MoveForwardOnMouse = FS.ReadBool();
			MoveForwardOnJoy = FS.ReadBool();
			MoveBackward = FS.ReadInt();
			MoveBackwardOnMouse = FS.ReadBool();
			MoveBackwardOnJoy = FS.ReadBool();
			RotateLeft = FS.ReadInt();
			RotateLeftOnMouse = FS.ReadBool();
			RotateLeftOnJoy = FS.ReadBool();
			RotateRight = FS.ReadInt();
			RotateRightOnMouse = FS.ReadBool();
			RotateRightOnJoy = FS.ReadBool();
			FireWeapon = FS.ReadInt();
			FireWeaponOnMouse = FS.ReadBool();
			FireWeaponOnJoy = FS.ReadBool();
			FireMine = FS.ReadInt();
			FireMineOnMouse = FS.ReadBool();
			FireMineOnJoy = FS.ReadBool();
			Turbo = FS.ReadInt();
			TurboOnMouse = FS.ReadBool();
			TurboOnJoy = FS.ReadBool();
			GameMenu = FS.ReadInt();
			GameMenuOnMouse = FS.ReadBool();
			GameMenuOnJoy = FS.ReadBool();
			InverseMouse = FS.ReadBool();

			LookLeft = FS.ReadInt();
			LookLeftOnMouse = FS.ReadBool();
			LookLeftOnJoy = FS.ReadBool();
			LookRight = FS.ReadInt();
			LookRightOnMouse = FS.ReadBool();
			LookRightOnJoy = FS.ReadBool();
			LookUp = FS.ReadInt();
			LookUpOnMouse = FS.ReadBool();
			LookUpOnJoy = FS.ReadBool();
			LookDown = FS.ReadInt();
			LookDownOnMouse = FS.ReadBool();
			LookDownOnJoy = FS.ReadBool();

		case -4:
			MoveForward = FS.ReadInt();
			MoveForwardOnMouse = FS.ReadBool();
			MoveForwardOnJoy = FS.ReadBool();
			MoveBackward = FS.ReadInt();
			MoveBackwardOnMouse = FS.ReadBool();
			MoveBackwardOnJoy = FS.ReadBool();
			RotateLeft = FS.ReadInt();
			RotateLeftOnMouse = FS.ReadBool();
			RotateLeftOnJoy = FS.ReadBool();
			RotateRight = FS.ReadInt();
			RotateRightOnMouse = FS.ReadBool();
			RotateRightOnJoy = FS.ReadBool();
			FireWeapon = FS.ReadInt();
			FireWeaponOnMouse = FS.ReadBool();
			FireWeaponOnJoy = FS.ReadBool();
			FireMine = FS.ReadInt();
			FireMineOnMouse = FS.ReadBool();
			FireMineOnJoy = FS.ReadBool();
			Turbo = FS.ReadInt();
			TurboOnMouse = FS.ReadBool();
			TurboOnJoy = FS.ReadBool();
			GameMenu = FS.ReadInt();
			GameMenuOnMouse = FS.ReadBool();
			GameMenuOnJoy = FS.ReadBool();
			InverseMouse = FS.ReadBool();

			LookLeft = FS.ReadInt();
			LookLeftOnMouse = FS.ReadBool();
			LookLeftOnJoy = FS.ReadBool();
			LookRight = FS.ReadInt();
			LookRightOnMouse = FS.ReadBool();
			LookRightOnJoy = FS.ReadBool();
			LookUp = FS.ReadInt();
			LookUpOnMouse = FS.ReadBool();
			LookUpOnJoy = FS.ReadBool();
			LookDown = FS.ReadInt();
			LookDownOnMouse = FS.ReadBool();
			LookDownOnJoy = FS.ReadBool();

			ChangeCamera = FS.ReadInt();
			ChangeCameraOnMouse = FS.ReadBool();
			ChangeCameraOnJoy = FS.ReadBool();

		case -5:
			MoveForward = FS.ReadInt();
			MoveForwardOnMouse = FS.ReadBool();
			MoveForwardOnJoy = FS.ReadBool();
			MoveBackward = FS.ReadInt();
			MoveBackwardOnMouse = FS.ReadBool();
			MoveBackwardOnJoy = FS.ReadBool();
			RotateLeft = FS.ReadInt();
			RotateLeftOnMouse = FS.ReadBool();
			RotateLeftOnJoy = FS.ReadBool();
			RotateRight = FS.ReadInt();
			RotateRightOnMouse = FS.ReadBool();
			RotateRightOnJoy = FS.ReadBool();
			FireWeapon = FS.ReadInt();
			FireWeaponOnMouse = FS.ReadBool();
			FireWeaponOnJoy = FS.ReadBool();
			FireMine = FS.ReadInt();
			FireMineOnMouse = FS.ReadBool();
			FireMineOnJoy = FS.ReadBool();
			Turbo = FS.ReadInt();
			TurboOnMouse = FS.ReadBool();
			TurboOnJoy = FS.ReadBool();
			GameMenu = FS.ReadInt();
			GameMenuOnMouse = FS.ReadBool();
			GameMenuOnJoy = FS.ReadBool();
			InverseMouse = FS.ReadBool();

			LookLeft = FS.ReadInt();
			LookLeftOnMouse = FS.ReadBool();
			LookLeftOnJoy = FS.ReadBool();
			LookRight = FS.ReadInt();
			LookRightOnMouse = FS.ReadBool();
			LookRightOnJoy = FS.ReadBool();
			LookUp = FS.ReadInt();
			LookUpOnMouse = FS.ReadBool();
			LookUpOnJoy = FS.ReadBool();
			LookDown = FS.ReadInt();
			LookDownOnMouse = FS.ReadBool();
			LookDownOnJoy = FS.ReadBool();

			ChangeCamera = FS.ReadInt();
			ChangeCameraOnMouse = FS.ReadBool();
			ChangeCameraOnJoy = FS.ReadBool();

			ReverseCamera = FS.ReadInt();
			ReverseCameraOnMouse = FS.ReadBool();
			ReverseCameraOnJoy = FS.ReadBool();
		}
	}
	else // unversioned file
	{
		MoveForward = Ver;
		MoveForwardOnMouse = FS.ReadBool();
		MoveBackward = FS.ReadInt();
		MoveBackwardOnMouse = FS.ReadBool();
		RotateLeft = FS.ReadInt();
		RotateLeftOnMouse = FS.ReadBool();
		RotateRight = FS.ReadInt();
		RotateRightOnMouse = FS.ReadBool();
		FireWeapon = FS.ReadInt();
		FireWeaponOnMouse = FS.ReadBool();
		FireMine = FS.ReadInt();
		FireMineOnMouse = FS.ReadBool();
		Turbo = FS.ReadInt();
		TurboOnMouse = FS.ReadBool();
		GameMenu = FS.ReadInt();
		GameMenuOnMouse = FS.ReadBool();
		InverseMouse = FS.ReadBool();
	}
}


void CControllerSettings::ResetToDefaults()
{
	MoveForward = DIK_W;
	MoveForwardOnMouse = false;
	MoveForwardOnJoy = false;
	MoveBackward = DIK_S;
	MoveBackwardOnMouse = false;
	MoveBackwardOnJoy = false;
	RotateLeft = DIK_A;
	RotateLeftOnMouse = false;
	RotateLeftOnJoy = false;
	RotateRight = DIK_D;
	RotateRightOnMouse = false;
	RotateRightOnJoy = false;
	FireWeapon = 0;
	FireWeaponOnMouse = true;
	FireWeaponOnJoy = false;
	FireMine = 1;
	FireMineOnMouse = true;
	FireMineOnJoy = false;
	Turbo = DIK_TAB;
	TurboOnMouse = false;
	TurboOnJoy = false;
	GameMenu = DIK_ESCAPE;
	GameMenuOnMouse = false;
	GameMenuOnJoy = false;
	LookLeft = DIK_LEFT;
	LookLeftOnMouse = false;
	LookLeftOnJoy = false;
	LookRight = DIK_RIGHT;
	LookRightOnMouse = false;
	LookRightOnJoy = false;
	LookUp = DIK_DOWN;
	LookUpOnMouse = false;
	LookUpOnJoy = false;
	LookDown = DIK_UP;
	LookDownOnMouse = false;
	LookDownOnJoy = false;
	InverseMouse = false;
	ChangeCamera = DIK_C;
	ChangeCameraOnMouse = false;
	ChangeCameraOnJoy = false;
	ReverseCamera = DIK_R;
	ReverseCameraOnMouse = false;
	ReverseCameraOnJoy = false;
}


CControllerSettings* CControllerSettings::Clone()
{
	CControllerSettings *NewCS = new CControllerSettings();

	NewCS->MoveForward = MoveForward;
	NewCS->MoveForwardOnMouse = MoveForwardOnMouse;
	NewCS->MoveForwardOnJoy = MoveForwardOnJoy;
	NewCS->MoveBackward = MoveBackward;
	NewCS->MoveBackwardOnMouse = MoveBackwardOnMouse;
	NewCS->MoveBackwardOnJoy = MoveBackwardOnJoy;
	NewCS->RotateLeft = RotateLeft;
	NewCS->RotateLeftOnMouse = RotateLeftOnMouse;
	NewCS->RotateLeftOnJoy = RotateLeftOnJoy;
	NewCS->RotateRight = RotateRight;
	NewCS->RotateRightOnMouse = RotateRightOnMouse;
	NewCS->RotateRightOnJoy = RotateRightOnJoy;
	NewCS->FireWeapon = FireWeapon;
	NewCS->FireWeaponOnMouse = FireWeaponOnMouse;
	NewCS->FireWeaponOnJoy = FireWeaponOnJoy;
	NewCS->FireMine = FireMine;
	NewCS->FireMineOnMouse = FireMineOnMouse;
	NewCS->FireMineOnJoy = FireMineOnJoy;
	NewCS->Turbo = Turbo;
	NewCS->TurboOnMouse = TurboOnMouse;
	NewCS->TurboOnJoy = TurboOnJoy;
	NewCS->GameMenu = GameMenu;
	NewCS->GameMenuOnMouse = GameMenuOnMouse;
	NewCS->GameMenuOnJoy = GameMenuOnJoy;
	NewCS->LookLeft = LookLeft;
	NewCS->LookLeftOnMouse = LookLeftOnMouse;
	NewCS->LookLeftOnJoy = LookLeftOnJoy;
	NewCS->LookRight = LookRight;
	NewCS->LookRightOnMouse = LookRightOnMouse;
	NewCS->LookRightOnJoy = LookRightOnJoy;
	NewCS->LookUp = LookUp;
	NewCS->LookUpOnMouse = LookUpOnMouse;
	NewCS->LookUpOnJoy = LookUpOnJoy;
	NewCS->LookDown = LookDown;
	NewCS->LookDownOnMouse = LookDownOnMouse;
	NewCS->LookDownOnJoy = LookDownOnJoy;
	NewCS->InverseMouse = InverseMouse;
	NewCS->ChangeCamera = ChangeCamera;
	NewCS->ChangeCameraOnMouse = ChangeCameraOnMouse;
	NewCS->ChangeCameraOnJoy = ChangeCameraOnJoy;
	NewCS->ReverseCamera = ReverseCamera;
	NewCS->ReverseCameraOnMouse = ReverseCameraOnMouse;
	NewCS->ReverseCameraOnJoy = ReverseCameraOnJoy;

	return NewCS;
}
