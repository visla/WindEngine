/*
      File: WeInput.cpp
	  Author: Tomislav Kukic
	  Date: 13.09.2005.
	  Version: 1.0
*/



#include <stdio.h>
#include <WeInput.h>


WeInput* WeInput::_instance = NULL;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeInput *WeInput::Instance()
{
	if (!_instance) 
		_instance = new WeInput;

	return _instance;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeInput::WeInput(void)
{
	bInitialized = false;
	OnMouseButton = NULL;
	OnMouseMove = NULL;
	OnKeydown = NULL;

	KeyboardLost = false;
	MouseLost = false;
	JoystickLost = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeInput::~WeInput(void)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::Initialize(HINSTANCE hInstance, HWND hWnd, bool InitJoystick)
{	
	if (bInitialized) return true;

	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&InputObject, NULL)))
	{
		return false;
	}
	
	// Create keyboard device and mouse device
    if (FAILED(InputObject->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL)))
	{
		SAFE_RELEASE(InputObject);
		return false;
	}

	if (FAILED(InputObject->CreateDevice(GUID_SysMouse, &Mouse, NULL)))
	{
		SAFE_RELEASE(Keyboard);
		SAFE_RELEASE(InputObject);
		return false;
	}

	// enum and create the first joystick in the system
	if (InitJoystick)
	{
		Joystick = NULL;
		InputObject->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (Joystick)
		{
			HRESULT hr = S_OK;
			hr |= Joystick->SetDataFormat( &c_dfDIJoystick2 );
			hr |= Joystick->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
			if (SUCCEEDED(hr))
				Joystick->Acquire();
			else
				SAFE_RELEASE(Joystick);
		}
	}

	Keyboard->SetDataFormat(&c_dfDIKeyboard);
	Mouse->SetDataFormat(&c_dfDIMouse2);

	Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 

	// ============ Toma added for testing ==========================
	DIPROPDWORD inputProp;
	// the header
    inputProp.diph.dwSize       = sizeof(DIPROPDWORD);
    inputProp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    inputProp.diph.dwObj        = 0;
    inputProp.diph.dwHow        = DIPH_DEVICE;
    inputProp.dwData            = 16;

	Keyboard->SetProperty(DIPROP_BUFFERSIZE, &inputProp.diph);
	// ==============================================================
                    


	Keyboard->Acquire();
	Mouse->Acquire();


	bInitialized = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeInput::ShutDown()
{
	if (!bInitialized) return;

	Keyboard->Unacquire();
	Mouse->Unacquire();
	if (Joystick) Joystick->Unacquire();

	SAFE_RELEASE(Keyboard);
	SAFE_RELEASE(Mouse);
	SAFE_RELEASE(Joystick);
	SAFE_RELEASE(InputObject);

	bInitialized = false;

	return;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeInput::MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
long WeInput::GetMouseX()
{
	long tmp = 0;
	return tmp;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
long WeInput::GetMouseY()
{
	long tmp = 0;
	return tmp;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
long WeInput::GetMouseZChange()
{
	long tmp = 0;
	return tmp;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::GetMouseButton(int Button)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::GetOldMouseButton(int Button)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::GetKeyState(int Key)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::GetOldKeyState(int Key)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeInput::LoadInputConfig(WeConfigFile* inputConfigFile)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeInput::SetSensitivity(float sensitivity)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeInput::GetSensitivity()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetMouseExlusive
//         Input Parametars :  bool e
//         Output Parametars:  N/A
//         Comments         :  Sets mouse to be exlusive
//
//////////////////////////////////////////////////////////////////////////
void WeInput::SetMouseExlusive(bool e)
{
	if (e)
		Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);	    
	else 
		Mouse->SetCooperativeLevel(hWnd,  DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void WeInput::SetKeyboardExlusive(bool e)
{
	if (e)
		Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);	    
	else 
		Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void WeInput::SetJoystickExclusive(bool exclusive)
{
	if (exclusive)
		Joystick->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);	    
	else 
		Joystick->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeInput::SetMouseAcquire(bool a)
{
	if (a)
		Mouse->Acquire();
	else 
		Mouse->Unacquire();
}

void WeInput::SetKeyboardAcquire(bool a)
{
	if (a)
		Keyboard->Acquire();
	else 
		Keyboard->Unacquire();
}

void WeInput::SetJoystickAcquire(bool acquire)
{
	if (acquire)
		Joystick->Acquire();
	else 
		Joystick->Unacquire();
}



bool WeInput::CheckDevices(HRESULT KeyBoard, HRESULT _Mouse, HRESULT hrJoystick)
{
	bool Res = false;

	if (KeyBoard == DIERR_INPUTLOST)
	{
		KeyboardLost = true;
		Keyboard->Unacquire();

		Res = true;
	}

	if (_Mouse == DIERR_INPUTLOST)
	{
		MouseLost = true;
		Mouse->Unacquire();

		Res = true;
	}

	if (Joystick && hrJoystick == DIERR_INPUTLOST)
	{
		JoystickLost = true;
		Joystick->Unacquire();

		Res = true;
	}

	if((KeyBoard != DIERR_INPUTLOST && KeyboardLost) || KeyBoard == DIERR_NOTACQUIRED)
	{
		if(DI_OK == Keyboard->Acquire())
            KeyboardLost = false;

		Res = true;
	}

	if((_Mouse != DIERR_INPUTLOST && MouseLost) || _Mouse == DIERR_NOTACQUIRED)
	{
		if(DI_OK  == Mouse->Acquire())
			MouseLost = false;

		Res = true;
	}

	if (Joystick)
	{
		if ((hrJoystick != DIERR_INPUTLOST && JoystickLost) || hrJoystick == DIERR_NOTACQUIRED )
		{
			if(DI_OK == Joystick->Acquire())
				JoystickLost = false;

			Res = true;
		}
	}

	if (Joystick)
		Res = ((_Mouse == 0) && (KeyBoard == 0) && (hrJoystick == 0));
	else
		Res = (_Mouse == 0) && (KeyBoard == 0);

	return Res;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Update
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Updates data from mouse and keyboard
//
//////////////////////////////////////////////////////////////////////////
void WeInput::Update()
{	
	if (!bInitialized) return;

	HRESULT hr, hr2;
	HRESULT hrJoy = -1;
	
	hr = Mouse->GetDeviceState(sizeof(MouseState), (void *)&MouseState);
	hr2 = Keyboard->GetDeviceState(sizeof(KeyboardState), (void *)&KeyboardState);
	if (Joystick)
		hrJoy = Joystick->GetDeviceState(sizeof(JoystickState), (void *)&JoystickState);

	if (CheckDevices(hr2, hr, hrJoy) == false) return;

	if(hr == 0)
	{
		if (MouseState.lX != 0 || MouseState.lY != 0 || MouseState.lZ != 0)
		{
			if (OnMouseMove) OnMouseMove(MouseState.lX, MouseState.lY, MouseState.lZ);
		}
//		if (MouseState.rgbButtons[0] != 0 || MouseState.rgbButtons[1] != 0 || MouseState.rgbButtons[2] != 0
//			|| MouseState.rgbButtons[3] != 0)
		{
			if (OnMouseButton) OnMouseButton(MouseState.rgbButtons);
		}
	}

	if(hr2 == 0)
	{
		if (OnKeydown) OnKeydown(KeyboardState);
	}

	if (SUCCEEDED(hrJoy))
	{
		if (OnJoystick)
			OnJoystick(JoystickState.lX, JoystickState.lY, JoystickState.lZ, JoystickState.rgbButtons);
	}
}



void WeInput::CheckKeyboard()
{
}



void WeInput::CheckMouse()
{
}



BOOL CALLBACK WeInput::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
	UNREFERENCED_PARAMETER( pContext );
	HRESULT hr;

	// Obtain an interface to the enumerated joystick.
	hr = WeInput::Instance()->InputObject->CreateDevice( pdidInstance->guidInstance, &WeInput::Instance()->Joystick, NULL );

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if( FAILED(hr) ) 
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}



void WeInput::ShutdownJoystick()
{
	SAFE_RELEASE(Joystick);
}
