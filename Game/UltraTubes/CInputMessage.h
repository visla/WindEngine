/*
   CInputMessage.h
   Autor: Tomislav Kukic
   Date: 17.3.2006.
   Version: 1.0

*/



#ifndef _CInputMessage_h_
#define _CInputMessage_h_


class CInputMessage
{
public:
	CInputMessage();
	~CInputMessage(){}

	bool MoveForward;
	bool MoveBackward;
	bool RotateLeft;
	bool RotateRight;
	bool FireWeapon;
	bool FireMine;
	bool Turbo;
	bool Exit;
	bool AnyKey;

	bool LookLeft;
	bool LookRight;
	bool LookUp;
	bool LookDown;
	bool ChangeCamera;
	bool ReverseCamera;

	bool LButton;
	bool RButton;
	bool MButton;
	bool DoubleClick;
	
	unsigned char* MouseButtons;
	unsigned char* JoyButtons;
	char* Key;

	int rx;
	int ry;

	void Clear();
};




#endif
