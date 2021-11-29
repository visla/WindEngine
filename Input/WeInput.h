/*
      File: WeInput.h
	  Author: Tomislav Kukic
	  Date: 13.09.2005.
	  Version: 1.0
	  Platform: Win32

	  This is input object used in application. It uses DirectInput.

*/




#ifndef _WEINPUT
#define _WEINPUT

#include <windows.h>
#include <dinput.h>
#include <WindEngine.h>


class WeConfigFile;


#define WEMOUSEBUTTON_LEFT 0
#define WEMOUSEBUTTON_RIGHT 1
#define WEMOUSEBUTTON_MIDDLE 2


typedef void (*MouseMove_Callback)(int Rx, int Ry, int Rz);
typedef void (*MouseButton_Callback)(BYTE *Buttons); // to test write if (Buttons[WEMOUSEBUTTON_LEFT]) 
typedef void (*Keyboard_Callback)(char *Key); // as in DirectInput if (Key[DIK_A] & 0x80)
typedef void (*Joystick_Callback)(long Rx, long Ry, long Rz, BYTE *Buttons);


class WINDENGINE_API WeInput
{
	friend class WeEngine;

protected:	
	LPDIRECTINPUTDEVICE8 Keyboard;
	LPDIRECTINPUTDEVICE8 Mouse;
	LPDIRECTINPUTDEVICE8 Joystick;
	HWND				hWnd;

	LPDIRECTINPUT8		 InputObject;

	float		MouseYaw;
	float		MousePitch;
	bool		InvertedPitch;
	bool		bInitialized;
	
	WeInput();
	virtual ~WeInput(void);

	bool  Initialize(HINSTANCE hInst,HWND hWnd, bool InitJoystick = true);
	void  ShutDown();
	void  MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


	long  GetMouseX();
	long  GetMouseY();
	long  GetMouseZChange();
	bool  GetMouseButton(int Button);
	bool  GetOldMouseButton(int Button);
	bool  GetKeyState(int Key);
	bool  GetOldKeyState(int Key);


	float Sensitivity;

	_DIMOUSESTATE2 MouseState;
	char		 KeyboardState[256];
	DIJOYSTATE2  JoystickState;
	

	bool KeyboardLost;
	bool MouseLost;
	bool JoystickLost;
	

	bool CheckDevices(HRESULT, HRESULT, HRESULT);
private:
	static WeInput* _instance;

	static BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );

public:
	static WeInput *Instance();

	virtual void SetMouseExlusive(bool exlusive);
	virtual void SetKeyboardExlusive(bool exlusive);
	virtual void SetMouseAcquire(bool acquire);
	virtual void SetKeyboardAcquire(bool acquire);

	virtual void SetJoystickAcquire(bool acquire);
	virtual void SetJoystickExclusive(bool exclusive);


	LPDIRECTINPUTDEVICE8 GetKeyboardDevice(){ return Keyboard; };
	LPDIRECTINPUTDEVICE8 GetMouseDevice(){ return Mouse; };



	void	LoadInputConfig(WeConfigFile* inputConfigFile);
	void	SaveInputConfig();

	void	SetSensitivity(float sensitivity);
	bool	GetSensitivity();

	void	ShutdownJoystick();

	void	Update();
	
	// Callbacks
	MouseMove_Callback	OnMouseMove;
	MouseButton_Callback OnMouseButton;
	Keyboard_Callback   OnKeydown;
	Joystick_Callback	OnJoystick;

	void CheckKeyboard();
	void CheckMouse();
};



#endif
