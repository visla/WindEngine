/*
      File: CGame.cpp
	  Author: Tomislav Kukic
	  Date: 19.01.2006
	  Version: 1.0
*/


#include <Windows.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeVector.h>
#include <WeInput.h>
#include <CGame.h>
#include <WeCEGUI.h>
#include <CUTApplication.h>
#include <CUTWindow.h>
#include <CIntroPhase.h>
#include <WeRenderDevice.h>
#include <d3d9renderer.h>
#include <COptionsVideoPhase.h>
#include <COptionsSettings.h>
#include <CControllerSettings.h>
#include <Weengine.h>
#include <CLoadMenuPhase.h>
#include <CGameServer.h>
#include <CGameClient.h>
#include <CLoadMenuPhase.h>
#include <CPlayGamePhase.h>
#include <CMainGameInterface.h>
#include <CMenuPhase.h>
#include <CGUIHelper.h>
#include <WeTimer.h>
#include <CMainGamePhase.h>
#include <CTuber.h>
#include <WeProfiler.h>
#include <WeCanvas.h>


CGame* CGame::Instance = new CGame;

CInputMessage CGame::InputMessage;


void LostDeviceCall()
{
	/*CEGUI::DirectX9Renderer* Rend = (CEGUI::DirectX9Renderer*)CEGUI::System::getSingleton().getRenderer();
	Rend->preD3DReset();*/
	if (CLoadMenuPhase::Instance->Initialized)
		CLoadMenuPhase::Instance->Renderer->preD3DReset();

	SAFE_RELEASE(CMenuPhase::Instance->ppSB);
}


void ResetDeviceCall()
{
	/*CEGUI::DirectX9Renderer* Rend = (CEGUI::DirectX9Renderer*)CEGUI::System::getSingleton().getRenderer();
	Rend->postD3DReset();*/
	if (CLoadMenuPhase::Instance->Initialized)
		CLoadMenuPhase::Instance->Renderer->postD3DReset();

	if (WeRenderDevice::Instance()->IsFullScreen())
	{
		RECT MouseClip;
		MouseClip.left = 0;
		MouseClip.top = 0;
		MouseClip.right = WeRenderDevice::Instance()->GetResWidth();
		MouseClip.bottom = WeRenderDevice::Instance()->GetResHeight();
		ClipCursor(&MouseClip);
	}

	WeAudioDevice::Instance()->SetMusicVolume(COptionsSettings::Instance->MusicVolume);
	WeAudioDevice::Instance()->SetSoundVolume(COptionsSettings::Instance->SoundVolume);
}



CGame::CGame()
{
	Camera = NULL;
	bDemo = true;
	WeRenderDevice::Instance()->SetLostDeviceCallback(LostDeviceCall);
	WeRenderDevice::Instance()->SetResetDeviceCallback(ResetDeviceCall);
}


void CGame::Start()
{
	SetPhase(CIntroPhase::Instance);
}


void CGame::End()
{
	CAbstractPhase::End();
	//if (CurrentPhase) CurrentPhase->End();
}


void CGame::UpdateKeys()
{
	static bool KeyState = false;
	static bool prevKeyState = false;
	if(InputMessage.Key)
	{
		for(int i=0; i<256; i++)
		{
			if(InputMessage.Key[i]&0x80)
			{
				if(!COptionsSettings::Instance->ControllerSettings.MoveForwardOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.MoveForward)
						InputMessage.MoveForward = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.MoveBackwardOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.MoveBackward)
						InputMessage.MoveBackward = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.RotateLeftOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.RotateLeft)
						InputMessage.RotateLeft = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.RotateRightOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.RotateRight)
						InputMessage.RotateRight = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.FireWeaponOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.FireWeapon)
						InputMessage.FireWeapon = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.FireMineOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.FireMine)
						InputMessage.FireMine = true;
				}
				if(!COptionsSettings::Instance->ControllerSettings.TurboOnMouse)
				{
					if(i == COptionsSettings::Instance->ControllerSettings.Turbo)
						InputMessage.Turbo = true;
				}

				if(i == 0x01)
					KeyState = true;
				else
					InputMessage.AnyKey = true;
			}else{
				if(i == 0x01)
					KeyState = false;
			}
		}
	}
	if(!KeyState && prevKeyState)
	{
		InputMessage.Exit = true;
		InputMessage.AnyKey = true;
	}

	prevKeyState = KeyState;
}



void CGame::UpdateMouseStates()
{
	for (int i = 0; i < 8; i++)
	{
		if (!InputMessage.MouseButtons) continue;
		if (InputMessage.MouseButtons[i] == 0) continue;

		InputMessage.AnyKey = true;

		if (COptionsSettings::Instance->ControllerSettings.MoveForwardOnMouse)
			if (i == COptionsSettings::Instance->ControllerSettings.MoveForward)
			{
				InputMessage.MoveForward = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.MoveBackwardOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.MoveBackward)
			{
				InputMessage.MoveBackward = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.RotateLeftOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.RotateLeft)
			{
				InputMessage.RotateLeft = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.RotateRightOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.RotateRight)
			{
				InputMessage.RotateRight = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.FireWeaponOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.FireWeapon)
			{
				InputMessage.FireWeapon = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.FireMineOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.FireMine)
			{
				InputMessage.FireMine = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.TurboOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.Turbo)
			{
				InputMessage.Turbo = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.LookLeftOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.LookLeft)
			{
				InputMessage.LookLeft = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.LookRightOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.LookRight)
			{
				InputMessage.LookRight = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.LookUpOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.LookUp)
			{
				InputMessage.LookUp = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.LookDownOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.LookDown)
			{
				InputMessage.LookDown = true;
			}
		if(COptionsSettings::Instance->ControllerSettings.ChangeCameraOnMouse)
			if(i == COptionsSettings::Instance->ControllerSettings.ChangeCamera)
			{
				InputMessage.ChangeCamera = true;
			}
			if(COptionsSettings::Instance->ControllerSettings.ReverseCameraOnMouse)
				if(i == COptionsSettings::Instance->ControllerSettings.ReverseCamera)
				{
					InputMessage.ReverseCamera = true;
				}
	}
}


void CGame::UpdateJoy()
{
	for (int i = 0; i < 128; ++i)
	{
		if (!InputMessage.JoyButtons) continue;
		if (InputMessage.JoyButtons[i] == 0) continue;

		InputMessage.AnyKey = true;

		if (COptionsSettings::Instance->ControllerSettings.MoveForwardOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.MoveForward)
				InputMessage.MoveForward = true;

		if(COptionsSettings::Instance->ControllerSettings.MoveBackwardOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.MoveBackward)
				InputMessage.MoveBackward = true;

		if(COptionsSettings::Instance->ControllerSettings.RotateLeftOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.RotateLeft)
				InputMessage.RotateLeft = true;

		if(COptionsSettings::Instance->ControllerSettings.RotateRightOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.RotateRight)
				InputMessage.RotateRight = true;

		if(COptionsSettings::Instance->ControllerSettings.FireWeaponOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.FireWeapon)
				InputMessage.FireWeapon = true;

		if(COptionsSettings::Instance->ControllerSettings.FireMineOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.FireMine)
				InputMessage.FireMine = true;

		if(COptionsSettings::Instance->ControllerSettings.TurboOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.Turbo)
				InputMessage.Turbo = true;

		if(COptionsSettings::Instance->ControllerSettings.LookLeftOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.LookLeft)
				InputMessage.LookLeft = true;

		if(COptionsSettings::Instance->ControllerSettings.LookRightOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.LookRight)
				InputMessage.LookRight = true;

		if(COptionsSettings::Instance->ControllerSettings.LookUpOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.LookUp)
				InputMessage.LookUp = true;

		if(COptionsSettings::Instance->ControllerSettings.LookDownOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.LookDown)
				InputMessage.LookDown = true;

		if(COptionsSettings::Instance->ControllerSettings.ChangeCameraOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.ChangeCamera)
				InputMessage.ChangeCamera = true;
		if(COptionsSettings::Instance->ControllerSettings.ReverseCameraOnJoy)
			if (i == COptionsSettings::Instance->ControllerSettings.ReverseCamera)
				InputMessage.ReverseCamera = true;
	}
}




void CGame::Tick()
{
	UpdateKeys();
	UpdateMouseStates();
	UpdateJoy();
	TickScreenCap();

#ifdef _DEBUG
	static float ReloadTimer = 2.0f;
	ReloadTimer = ReloadTimer + WeTimer::Instance()->GetDeltaTimeF();
	if (ReloadTimer > 1.0f)
	{
		CGUIHelper::ReloadAllWindowsConfig();
		ReloadTimer = 0.0f;
	}
#endif

	CAbstractPhase::Tick();

	InputMessage.Clear();
}



void CGame::OnRender()
{
	if (CGame::Instance->CurrentPhase)
	{
		if (CLoadMenuPhase::Instance->Initialized)
		{
			CGUIHelper::Render(); 
		}
		CGame::Instance->CurrentPhase->Render();
	}

#ifdef _DEBUG
	char ttt[100];
	sprintf(ttt, "0:%3d%% 1:%3d%% 2:%3d%% 3:%3d%% 4:%3d%%",
		(int)(100.0f * WeProfiler::Instance()->GetBlockPercent(0)),
		(int)(100.0f * WeProfiler::Instance()->GetBlockPercent(1)),
		(int)(100.0f * WeProfiler::Instance()->GetBlockPercent(2)),
		(int)(100.0f * WeProfiler::Instance()->GetBlockPercent(3)),
		(int)(100.0f * WeProfiler::Instance()->GetBlockPercent(4)));
	WeCanvas::Instance()->Draw2DTextOutlined(CMainGameInterface::Instance->FontFixed, (String)ttt, 0, 0, 500, 100, WeColor(1.0f, 1.0f, 0.0f, 1.0f), WeColor(0.0f, 0.0f, 0.0f, 1.0f));
#endif
}


void CGame::OnKeyDown(char* Key)
{

	static int p = 0;

	if (p == 4)
	{
		for (int i = 0; i < 256; i++)
		{
			if (Key[i] & 0x80 )
			{
				if (CLoadMenuPhase::Instance->Initialized)
					CEGUI::System::getSingleton().injectKeyDown(i);
			}
		}
		p = 0;
	}
	p++;



	if (Key[DIK_ESCAPE] & 0x80)
	{
		//CGameServer::Instance->StopHosting();
		//CGameClient::Instance->Disconnect();
		//CUTWindow::Instance->Terminate();
		//CPlayGamePhase::Instance->ShowInGameMenu = true;
				
	}

	InputMessage.Key = Key;
}


void CGame::OnMouseMove(int Rx, int Ry, int Rz)
{
	int XA = (int)((float)Rx * COptionsSettings::Instance->MouseSensitivity);
	int YA = (int)((float)Ry * COptionsSettings::Instance->MouseSensitivity);
	InputMessage.rx = XA;
	InputMessage.ry = COptionsSettings::Instance->ControllerSettings.InverseMouse ? -YA : YA;

    if(CLoadMenuPhase::Instance->Initialized)
		CEGUI::System::getSingleton().injectMouseWheelChange((float)Rz);

	if(CEditorWindow::Playing)
		CEGUI::System::getSingleton().injectMouseMove((float)Rx,(float)Ry);
}


void CGame::OnMouseButton(BYTE* Buttons)
{
	static int PrevState[8];

	InputMessage.MouseButtons = Buttons;

	for(int i = 0; i < 8; ++i)
	{
		if((Buttons[i] != PrevState[i]) && (PrevState[i] == 0))
		{
			if(i == 0)
			{
				InputMessage.LButton = true;
				if(CLoadMenuPhase::Instance->Initialized)
					CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
			}
			if(i == 1)
			{
				InputMessage.RButton = true;
				//if (!CMainGamePhase::Instance->InputPlayer || CMainGamePhase::Instance->InputPlayer->RaceFinished)
				//	InputMessage.Exit = true;
			}
			if(i == 2)
				InputMessage.MButton = true;
		} 

		if((Buttons[i] != PrevState[i]) && (PrevState[i] != 0))
		{
			if(i == 0)
			{
				InputMessage.LButton = false;
				if(CLoadMenuPhase::Instance->Initialized)
					CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
			}
			if(i == 1)
			{
				InputMessage.RButton = false;
				//if (!CMainGamePhase::Instance->InputPlayer || CMainGamePhase::Instance->InputPlayer->RaceFinished)
				//	InputMessage.Exit = false;
			}
			if(i == 2)
				InputMessage.MButton = false;
		}
		PrevState[i] = Buttons[i];
	}

	if(CPlayGamePhase::Instance->Status == PLAYGAME_RACE)
	{
		InputMessage.LButton = (Buttons[0] != 0);
		InputMessage.RButton = (Buttons[1] != 0);
		InputMessage.MButton = (Buttons[2] != 0);
	}
}



void CGame::OnDblClick()
{
	InputMessage.DoubleClick = true;
}



void CGame::TickScreenCap()
{
	DWORD time = WeTimer::Instance()->GetSystemTime();
	while (true)
	{
		char SSFName[100];
		sprintf(SSFName, "%s.bmp", CMainGameInterface::Instance->MakeTimeString(time / 1000.0f, "-"));

		FILE *f = fopen(SSFName, "rb");
		if (!f)
		{
			if (InputMessage.Key && (InputMessage.Key[DIK_F9] & 0x80))
				WeRenderDevice::Instance()->SaveThumbnail(String(SSFName));
			return;
		}

		time++;
	}
}

void CGame::OnJoystick(long Rx, long Ry, long Rz, BYTE *Buttons)
{
	const long MAX_JOY_RX = 0x7FFF;
	const long MAX_JOY_RY = 0x7FFF;

	static int PrevState[128];

	InputMessage.JoyButtons = Buttons;

	for (int i = 0; i < 3; ++i)
	{
		if ( (Buttons[i] != PrevState[i]) && (PrevState[i] == 0) )
		{
			switch (i)
			{
			case 0:
				InputMessage.LButton = true;
				if(CLoadMenuPhase::Instance->Initialized)
					CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
				break;

			case 1:
				InputMessage.RButton = true;
				if (!CMainGamePhase::Instance->InputPlayer || CMainGamePhase::Instance->InputPlayer->RaceFinished)
					InputMessage.Exit = true;
				break;

			case 2:
				InputMessage.MButton = true;
				break;
			}
		}

		if ( (Buttons[i] != PrevState[i]) && (PrevState[i] != 0) )
		{
			switch (i)
			{
			case 0:
				InputMessage.LButton = false;
				if(CLoadMenuPhase::Instance->Initialized)
					CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
				break;

			case 1:
				InputMessage.RButton = false;
				if (!CMainGamePhase::Instance->InputPlayer || CMainGamePhase::Instance->InputPlayer->RaceFinished)
					InputMessage.Exit = false;
				break;

			case 2:
				InputMessage.MButton = false;
				break;
			}
		}

		PrevState[i] = Buttons[i];
	}

	int FinalRX = 0;
	int FinalRY = 0;

	int DeadZoneX = (int)((float)MAX_JOY_RX * COptionsSettings::Instance->JoystickDeadzone);
	int DeadZoneY = (int)((float)MAX_JOY_RY * COptionsSettings::Instance->JoystickDeadzone);

	int SignedDeadZone;
	SignedDeadZone = (Rx < MAX_JOY_RX) ? DeadZoneX : -DeadZoneX;
	if ( abs(Rx - MAX_JOY_RX) > DeadZoneX )
		FinalRX = (int)( ((float)MAX_RX / (float)(MAX_JOY_RX - DeadZoneX)) * (float)(Rx - MAX_JOY_RX + SignedDeadZone) );

	SignedDeadZone = (Ry < MAX_JOY_RY) ? DeadZoneY : -DeadZoneY;
	if ( abs(Ry - MAX_JOY_RY) > DeadZoneY )
		FinalRY = (int)( ((float)MAX_RY / (float)(MAX_JOY_RY - DeadZoneY)) * (float)(Ry - MAX_JOY_RY + SignedDeadZone) );

	//char bzz[100];
	////sprintf(bzz, "RX: %d  RY: %d  RZ: %d  B1: %d  B2: %d  B3: %d  B4: %d", Rx, Ry, Rz, Buttons[0], Buttons[1], Buttons[2], Buttons[3]);
	//sprintf(bzz, "%d\n%d", FinalRX, FinalRY);
	//CMainGameInterface::Instance->Blabla = String(bzz);

	FinalRX = (int)((float)FinalRX * COptionsSettings::Instance->JoystickSensitivity);
	FinalRY = (int)((float)FinalRY * COptionsSettings::Instance->JoystickSensitivity);

	InputMessage.rx += FinalRX;
	InputMessage.ry += COptionsSettings::Instance->ControllerSettings.InverseMouse ? -FinalRY : FinalRY;

	if (CLoadMenuPhase::Instance->Initialized || CEditorWindow::Playing)
		CEGUI::System::getSingleton().injectMouseMove((float)((FinalRX) * 0.5f), (float)((FinalRY) * 0.5f));
}
