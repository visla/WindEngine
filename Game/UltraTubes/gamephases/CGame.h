/*
      File: CGame.h
	  Author: Tomislav Kukic
	  Date: 19.01.2006
	  Version: 1.0
*/



#ifndef _CGAME_H
#define _CGAME_H

#include <CString.h>
#include <CAbstractPhase.h>
#include <WeCamera.h>
#include <CInputMessage.h>
#include <CServerInfo.h>

#define GAME_VERSION	"ver 0.2b"
#define MAX_RX				70.0f
#define MAX_RY				70.0f


class CGame: public CAbstractPhase
{
protected:
	void UpdateKeys();
	void UpdateMouseStates();
	void UpdateJoy();
	void TickScreenCap();

public:
	WeCamera*			Camera;
	bool				bDemo;

	CServerInfo			ServerInfo;

	static CInputMessage InputMessage;

	static CGame*		Instance;

						CGame();
	void				Start();
	void				End();
	void				Tick();
	void				RenderStdScreen();
	void				LoadMap(StringParam MapName);

	
	static void			OnKeyDown(char* Key);
	static void			OnMouseMove(int Rx, int Ry, int Rz);
	static void			OnMouseButton(BYTE *Buttons);
	static void			OnJoystick(long Rx, long Ry, long Rz, BYTE *Buttons);
	static void			OnDblClick();
	static void			OnRender();
};


#endif 