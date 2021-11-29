/*
   CInputMessage.cpp
   Autor: Tomislav Kukic
   Date: 17.3.2006.
   Version: 1.0

*/


#include <Windows.h>
#include <CInputMessage.h>


CInputMessage::CInputMessage()
{
	 Clear();
}



void CInputMessage::Clear()
{
	Key = NULL;

	MoveForward = false;
	MoveBackward = false;
	RotateLeft = false;
	RotateRight = false;
	FireWeapon = false;
	FireMine = false;
	Turbo = false;
	Exit = false;
	AnyKey = false;
	LookLeft = false;
	LookRight = false;
	LookUp = false;
	LookDown = false;
	ChangeCamera = false;
	ReverseCamera = false;

	LButton = false;
	RButton = false;
	MButton = false;
	DoubleClick = false;

	rx = 0;
	ry = 0;
}

