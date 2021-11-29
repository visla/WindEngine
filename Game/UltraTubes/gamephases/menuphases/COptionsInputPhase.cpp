/*
    COptionsInputPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
*/

#include <COptionsInputPhase.h>
#include <CMenuPhase.h>
#include <CInputMessage.h>
#include <CGame.h>
#include <COptionsSettings.h>
#include <CControllerSettings.h>
#include <CString.h>
#include <dinput.h>
#include <COptionsPhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLocalizationSettings.h>
#include <CProfileManager.h>
#include <CMainMenuPhase.h>
#include <CEGUIPropertyHelper.h>
#include <CPlayGamePhase.h>



COptionsInputPhase *COptionsInputPhase::Instance = new COptionsInputPhase;


COptionsInputPhase::COptionsInputPhase()
{
	MouseClicked = false;
	EnterKey = false;
	SelectedItem = NULL;
}



void COptionsInputPhase::Start()
{
	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Scrollbar *scrollbar;


	//CGUIHelper::CreateText("InputOptions_PressControlKey", "Press key or mouse button", 0.0f, 0.9f, 1.0f, 0.05f, "", "", "", "");
	//winMgr.getWindow("InputOptions_PressControlKey")->setAlpha(0.0f);

	//CGUIHelper::CreateText("InputOptions_MouseClick", "Mouse clicked", 0.0f, 0.95f, 1.0f, 0.05f, "", "", "", "");
	//winMgr.getWindow("InputOptions_MouseClick")->setAlpha(0.0f);

	CGUIHelper::CreateWnd("InputOptions_Window",CLocalizationSettings::Instance()->GetLocalText("CONTROLS"),
		0.0f,0.5f,0.13f,0.5f,0.5f,"RootWindow");
	CEGUI::Window* InputOptionsWnd = winMgr.getWindow("InputOptions_Window");


	CSmartList<String> ColumnNames;
	ColumnNames.PushBack("ACTION");
	ColumnNames.PushBack("KEY");

	CEGUI::MultiColumnList* KeyList;
	KeyList = CGUIHelper::CreateMultiList("InputOptions_KeyList", 0.1f, 0.1f, 0.8f, 0.65f, ColumnNames, false, false, "InputOptions_Window");
	//KeyList->setAlwaysOnTop(true);
	KeyList->subscribeEvent(CEGUI::MultiColumnList::EventMouseClick, CEGUI::Event::Subscriber(&OnKeyChange));
	
	KeyList->setNominatedSelectionColumn(1);
	for (int i = 0; i < 13; i++)
		KeyList->addRow();

	String tmpName;
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("MOVE FORWARD"), ""),0,0);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.MoveForward, COptionsSettings::Instance->ControllerSettings.MoveForwardOnMouse, COptionsSettings::Instance->ControllerSettings.MoveForwardOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,0);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("MOVE BACKWARD"), ""),0,1);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.MoveBackward, COptionsSettings::Instance->ControllerSettings.MoveBackwardOnMouse, COptionsSettings::Instance->ControllerSettings.MoveBackwardOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,1);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("ROTATE LEFT"), ""),0,2);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.RotateLeft, COptionsSettings::Instance->ControllerSettings.RotateLeftOnMouse, COptionsSettings::Instance->ControllerSettings.RotateLeftOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,2);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("ROTATE RIGHT"), ""),0,3);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.RotateRight, COptionsSettings::Instance->ControllerSettings.RotateRightOnMouse, COptionsSettings::Instance->ControllerSettings.RotateRightOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,3);

	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("NITRO"), ""),0,4);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.Turbo, COptionsSettings::Instance->ControllerSettings.TurboOnMouse, COptionsSettings::Instance->ControllerSettings.TurboOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,4);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("FIRE WEAPON"), ""),0,5);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.FireWeapon, COptionsSettings::Instance->ControllerSettings.FireWeaponOnMouse, COptionsSettings::Instance->ControllerSettings.FireWeaponOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,5);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("FIRE MINE"), ""),0,6);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.FireMine, COptionsSettings::Instance->ControllerSettings.FireMineOnMouse, COptionsSettings::Instance->ControllerSettings.FireMineOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,6);

	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("LOOK LEFT"), ""),0,7);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.LookLeft, COptionsSettings::Instance->ControllerSettings.LookLeftOnMouse, COptionsSettings::Instance->ControllerSettings.LookLeftOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,7);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("LOOK RIGHT"), ""),0,8);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.LookRight, COptionsSettings::Instance->ControllerSettings.LookRightOnMouse, COptionsSettings::Instance->ControllerSettings.LookRightOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,8);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("LOOK UP"), ""),0,9);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.LookUp, COptionsSettings::Instance->ControllerSettings.LookUpOnMouse, COptionsSettings::Instance->ControllerSettings.LookUpOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,9);
	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("LOOK DOWN"), ""),0,10);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.LookDown, COptionsSettings::Instance->ControllerSettings.LookDownOnMouse, COptionsSettings::Instance->ControllerSettings.LookDownOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,10);

	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("CHANGE CAMERA"), ""),0,11);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.ChangeCamera, COptionsSettings::Instance->ControllerSettings.ChangeCameraOnMouse, COptionsSettings::Instance->ControllerSettings.ChangeCameraOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,11);

	KeyList->setItem(new MyListItem(CLocalizationSettings::Instance()->GetLocalText("REVERSE CAMERA"), ""),0,12);
	tmpName = GetKeyID(COptionsSettings::Instance->ControllerSettings.ReverseCamera, COptionsSettings::Instance->ControllerSettings.ReverseCameraOnMouse, COptionsSettings::Instance->ControllerSettings.ReverseCameraOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,12);


	CGUIHelper::CreateText("InvertMouseCheckTitle", CLocalizationSettings::Instance()->GetLocalText("INVERT MOUSE"),
		0.1f, 0.8f, 0.4f, 0.05f, "", "", "", "InputOptions_Window");

	CEGUI::Checkbox* InvertMouseCheck = CGUIHelper::CreateCheckbox("InvertMouseCheck", 0.6f, 0.8f, 1.0f, 0.05f, "InputOptions_Window", &OnInvertMouseClick);
	InvertMouseCheck->setSelected(COptionsSettings::Instance->ControllerSettings.InverseMouse);


	CGUIHelper::CreateText("InputOptions_KeyboardSensitivityTitle", CLocalizationSettings::Instance()->GetLocalText("KEYBOARD SENSITIVITY"),
		0.1f, 0.82f, 0.4f, 0.05f, "", "", "", "InputOptions_Window");

	scrollbar = CGUIHelper::CreateHorzScrollbar("InputOptions_KeyboardSensitivityScrollbar",
		0.6f, 0.82f, 0.3f, 0.02f, 0.1f, 1.1f, "InputOptions_Window", &OnKeyboardSensitivityChanged);

	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeKeyboardSensitivity());

	CGUIHelper::CreateText("InputOptions_MouseSensitivityTitle", CLocalizationSettings::Instance()->GetLocalText("MOUSE SENSITIVITY"),
		0.1f, 0.84f, 0.4f, 0.05f, "", "", "", "InputOptions_Window");

	scrollbar = CGUIHelper::CreateHorzScrollbar("InputOptions_MouseSensitivityScrollbar",
		0.6f, 0.84f, 0.3f, 0.02f, 0.1f, 1.1f, "InputOptions_Window", &OnMouseSensitivityChanged);

	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeMouseSensitivity());


	CGUIHelper::CreateText("InputOptions_JoystickSensitivityTitle", CLocalizationSettings::Instance()->GetLocalText("JOYSTICK SENSITIVITY"),
		0.1f, 0.86f, 0.4f, 0.05f, "", "", "", "InputOptions_Window");

	scrollbar = CGUIHelper::CreateHorzScrollbar("InputOptions_JoystickSensitivityScrollbar",
		0.6f, 0.86f, 0.3f, 0.02f, 0.1f, 1.1f, "InputOptions_Window", &OnJoystickSensitivityChanged);

	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeJoystickSensitivity());


	CGUIHelper::CreateText("InputOptions_JoystickDeadzoneTitle", CLocalizationSettings::Instance()->GetLocalText("JOYSTICK DEADZONE"),
		0.1f, 0.88f, 0.4f, 0.05f, "", "", "", "InputOptions_Window");

	scrollbar = CGUIHelper::CreateHorzScrollbar("InputOptions_JoystickDeadzoneScrollbar",
		0.6f, 0.88f, 0.3f, 0.02f, 0.1f, 1.1f, "InputOptions_Window", &OnJoystickDeadzoneChanged);

	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeJoystickDeadzone());


	CGUIHelper::CreateButton("OPTIONS_DEFAULT_BUTTON",CLocalizationSettings::Instance()->GetLocalText("DEFAULT"),0.1f,0.9f,0.6f,0.05f,"InputOptions_Window", &COptionsInputPhase::OnResetToDefaultsClick);


	CGUIHelper::CreateButton("InputOptions_BackButton", CLocalizationSettings::Instance()->GetLocalText("BACK"),
		0.05f, 0.8f, 1.0f, 0.12f, "InputOptions_Window", &COptionsInputPhase::OnBack);



	winMgr.getWindow("InputOptions_Window")->setProperty("Visible","true");
}



void COptionsInputPhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	winMgr.getWindow("InputOptions_Window")->destroy();
	//winMgr.getWindow("InputOptions_PressControlKey")->destroy();
	//winMgr.getWindow("InputOptions_MouseClick")->destroy();

	COptionsSettings::Instance->Save();

	COptionsPhase::Instance->End();
}



void COptionsInputPhase::SetKeyID(int RowID, int Value, bool OnMouse, bool OnJoy)
{
	switch(RowID)
	{
	case 0:
		COptionsSettings::Instance->ControllerSettings.MoveForward = Value;
		COptionsSettings::Instance->ControllerSettings.MoveForwardOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.MoveForwardOnJoy = OnJoy;
		break;
	case 1:
		COptionsSettings::Instance->ControllerSettings.MoveBackward = Value;
		COptionsSettings::Instance->ControllerSettings.MoveBackwardOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.MoveBackwardOnJoy = OnJoy;
		break;
	case 2:
		COptionsSettings::Instance->ControllerSettings.RotateLeft = Value;
		COptionsSettings::Instance->ControllerSettings.RotateLeftOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.RotateLeftOnJoy = OnJoy;
		break;
	case 3:
		COptionsSettings::Instance->ControllerSettings.RotateRight = Value;
		COptionsSettings::Instance->ControllerSettings.RotateRightOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.RotateRightOnJoy = OnJoy;
		break;
	case 4:
		COptionsSettings::Instance->ControllerSettings.Turbo = Value;
		COptionsSettings::Instance->ControllerSettings.TurboOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.TurboOnJoy = OnJoy;
		break;
	case 5:
		COptionsSettings::Instance->ControllerSettings.FireWeapon = Value;
		COptionsSettings::Instance->ControllerSettings.FireWeaponOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.FireWeaponOnJoy = OnJoy;
		break;
	case 6:
		COptionsSettings::Instance->ControllerSettings.FireMine = Value;
		COptionsSettings::Instance->ControllerSettings.FireMineOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.FireMineOnJoy = OnJoy;
		break;
	case 7:
		COptionsSettings::Instance->ControllerSettings.LookLeft = Value;
		COptionsSettings::Instance->ControllerSettings.LookLeftOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.LookLeftOnJoy = OnJoy;
		break;
	case 8:
		COptionsSettings::Instance->ControllerSettings.LookRight = Value;
		COptionsSettings::Instance->ControllerSettings.LookRightOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.LookRightOnJoy = OnJoy;
		break;
	case 9:
		COptionsSettings::Instance->ControllerSettings.LookUp = Value;
		COptionsSettings::Instance->ControllerSettings.LookUpOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.LookUpOnJoy = OnJoy;
		break;
	case 10:
		COptionsSettings::Instance->ControllerSettings.LookDown = Value;
		COptionsSettings::Instance->ControllerSettings.LookDownOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.LookDownOnJoy = OnJoy;
		break;
	case 11:
		COptionsSettings::Instance->ControllerSettings.ChangeCamera = Value;
		COptionsSettings::Instance->ControllerSettings.ChangeCameraOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.ChangeCameraOnJoy = OnJoy;
		break;
	case 12:
		COptionsSettings::Instance->ControllerSettings.ReverseCamera = Value;
		COptionsSettings::Instance->ControllerSettings.ReverseCameraOnMouse = OnMouse;
		COptionsSettings::Instance->ControllerSettings.ReverseCameraOnJoy = OnJoy;
		break;
	}
}


String COptionsInputPhase::GetKeyID(int Value, bool OnMouse, bool OnJoy)
{
	char RetVal[100];
	if (OnMouse)
		sprintf(RetVal, "MOUSE %d", (Value + 1));
	else if (OnJoy)
		sprintf(RetVal, "JOY %d", (Value + 1));
	else
		return GetControlString(Value);

	return String(RetVal);
}


void COptionsInputPhase::Tick()
{
	static bool EscapePressed = false;

	COptionsPhase::Instance->Tick();

	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("InputOptions_Window", WNDAlpha);

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	//winMgr.getWindow("InputOptions_PressControlKey")->setAlpha(EnterKey ? 1.0f : 0.0f);
	//winMgr.getWindow("InputOptions_MouseClick")->setAlpha(MouseClicked ? 1.0f : 0.0f);

	if(EnterKey)
	{
		CEGUI::MultiColumnList* MCL = static_cast<CEGUI::MultiColumnList*>(winMgr.getWindow("InputOptions_KeyList"));

		for (int i = 0; i < 8; ++i)
		{
			if (CGame::InputMessage.MouseButtons[i] != 0)
			{
				char Thingy[100];
				sprintf(Thingy, "MOUSE %d", (i + 1));
				String Thingamabob = String(Thingy);
				ValidateInput(Thingamabob);
				SelectedItem->setText(Thingamabob.GetString());
				MCL->setText(Thingamabob.GetString());
				EnterKey = false;
				if (i == 0) MouseClicked = false;

				SetKeyID(ClickedIndex, i, true, false);
				SelectedItem->SetSelectedColorConfigName("cegui.List.SelectedColor");
				MCL->requestRedraw();
			}
		}

		if (EnterKey && CGame::InputMessage.JoyButtons)
		{
			for (int i = 0; i < 128; ++i)
			{
				if (CGame::InputMessage.JoyButtons[i] != 0)
				{
					char Thingy[100];
					sprintf(Thingy, "JOY %d", (i + 1));
					String Thingamabob = String(Thingy);
					ValidateInput(Thingamabob);
					SelectedItem->setText(Thingamabob.GetString());
					MCL->setText(Thingamabob.GetString());
					EnterKey = false;

					SetKeyID(ClickedIndex, i, false, true);
					SelectedItem->SetSelectedColorConfigName("cegui.List.SelectedColor");
					MCL->requestRedraw();
				}
			}
		}

		if (EnterKey)
		{
			for (int i = 0; i < 256; ++i)
			{
				if (!(CGame::InputMessage.Key[i] & 0x80)) continue;
				if (i == DIK_ESCAPE)
				{
					EnterKey = false;

					SelectedItem->SetSelectedColorConfigName("cegui.List.SelectedColor");
					MCL->requestRedraw();

					EscapePressed = true;
					break;
				}

				if (i == DIK_GRAVE) continue;
				if ((i >= DIK_F1) && (i <= DIK_F10)) continue;
				if ((i == DIK_F11) || (i == DIK_F12) || (i == DIK_F13) || (i == DIK_F14) || (i == DIK_F15)) continue;
				if ((i >= DIK_1) && (i <= DIK_0)) continue;

				{
					String res = GetControlString(i);
					ValidateInput(res);
					SelectedItem->setText(res.GetString());
					MCL->setText(res.GetString());
					EnterKey = false;

					SetKeyID(ClickedIndex, i, false, false);
					SelectedItem->SetSelectedColorConfigName("cegui.List.SelectedColor");
					MCL->requestRedraw();

					break;
				}
			}
		}

		if (!EnterKey)
		{
			SaveSettings();
		}
	}
	else 
	{
		if (CGame::Instance->InputMessage.Exit)
		{
			if (EscapePressed)
				EscapePressed = false;
			else
			{
				CEGUI::EventArgs e;
				OnBack(e);
			}
		}
	}
}


bool COptionsInputPhase::ValidateInput(String KeyName)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiColumnList* MCL = static_cast<CEGUI::MultiColumnList*>(winMgr.getWindow("InputOptions_KeyList"));

	String KeyID;
	CEGUI::ListboxItem* tmpItem;

	for(int i = 0; i < (int)MCL->getRowCount(); i++)
	{
		tmpItem = MCL->getItemAtGridReference(CEGUI::MCLGridRef(i, 1));
		KeyID = tmpItem->getText().c_str();
		if ((KeyID == KeyName) && (i != ClickedIndex))
		{
			SetKeyID(i, 0, false, false);
			MCL->setItem(new MyListItem("", "InputOptions_KeyList"), 1, i);
		}
	}

	return true;
}


void COptionsInputPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}


bool COptionsInputPhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	if (COptionsPhase::Instance->FromInGame)
		COptionsInputPhase::Instance->End();
	else
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	return true;
}



bool COptionsInputPhase::OnKeyChange(const CEGUI::EventArgs& e)
{
	if (COptionsInputPhase::Instance->EnterKey) return false;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiColumnList* MCL = static_cast<CEGUI::MultiColumnList*>(winMgr.getWindow("InputOptions_KeyList"));
	CEGUI::ListboxItem* Item = MCL->getFirstSelectedItem();

	if (Item)
	{
		if (MCL->getItemColumnIndex(Item) != 1)
		{
			return true;
		}

		if (!COptionsInputPhase::Instance->MouseClicked)
		{
			COptionsInputPhase::Instance->MouseClicked = true;
			COptionsInputPhase::Instance->ClickedIndex = MCL->getItemRowIndex(Item);
			//((MyListItem*)Item)->SetSelectionColourIndex(5);
			//MCL->requestRedraw();

			return true;
		}
		else
		{
			int tmpIndex = MCL->getItemRowIndex(Item);

			if (tmpIndex == COptionsInputPhase::Instance->ClickedIndex)
			{
				COptionsInputPhase::Instance->EnterKey = true;
				COptionsInputPhase::Instance->SelectedItem = (MyListItem*)Item;
				COptionsInputPhase::Instance->SelectedItem->SetSelectedColorConfigName("cegui.List.DoubleSelectedColor");
				MCL->requestRedraw();

				return true;
			}
			else
			{
				COptionsInputPhase::Instance->ClickedIndex = MCL->getItemRowIndex(Item);
				return true;
			}
		}
	}
	else
	{
		COptionsInputPhase::Instance->MouseClicked = false;
	}

	return true;
}



bool COptionsInputPhase::OnInvertMouseClick(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	COptionsSettings::Instance->ControllerSettings.InverseMouse = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("InvertMouseCheck"))->isSelected();

	COptionsInputPhase::Instance->SaveSettings();
	return true;
}



bool COptionsInputPhase::OnResetToDefaultsClick(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	COptionsSettings::Instance->ControllerSettings.ResetToDefaults();


	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiColumnList* KeyList = static_cast<CEGUI::MultiColumnList*>(winMgr.getWindow("InputOptions_KeyList"));

	String tmpName;
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.MoveForward, COptionsSettings::Instance->ControllerSettings.MoveForwardOnMouse, COptionsSettings::Instance->ControllerSettings.MoveForwardOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,0);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.MoveBackward, COptionsSettings::Instance->ControllerSettings.MoveBackwardOnMouse, COptionsSettings::Instance->ControllerSettings.MoveBackwardOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,1);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.RotateLeft, COptionsSettings::Instance->ControllerSettings.RotateLeftOnMouse, COptionsSettings::Instance->ControllerSettings.RotateLeftOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,2);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.RotateRight, COptionsSettings::Instance->ControllerSettings.RotateRightOnMouse, COptionsSettings::Instance->ControllerSettings.RotateRightOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,3);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.Turbo, COptionsSettings::Instance->ControllerSettings.TurboOnMouse, COptionsSettings::Instance->ControllerSettings.TurboOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,4);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.FireWeapon, COptionsSettings::Instance->ControllerSettings.FireWeaponOnMouse, COptionsSettings::Instance->ControllerSettings.FireWeaponOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,5);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.FireMine, COptionsSettings::Instance->ControllerSettings.FireMineOnMouse, COptionsSettings::Instance->ControllerSettings.FireMineOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,6);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.LookLeft, COptionsSettings::Instance->ControllerSettings.LookLeftOnMouse, COptionsSettings::Instance->ControllerSettings.LookLeftOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,7);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.LookRight, COptionsSettings::Instance->ControllerSettings.LookRightOnMouse, COptionsSettings::Instance->ControllerSettings.LookRightOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,8);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.LookUp, COptionsSettings::Instance->ControllerSettings.LookUpOnMouse, COptionsSettings::Instance->ControllerSettings.LookUpOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,9);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.LookDown, COptionsSettings::Instance->ControllerSettings.LookDownOnMouse, COptionsSettings::Instance->ControllerSettings.LookDownOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,10);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.ChangeCamera, COptionsSettings::Instance->ControllerSettings.ChangeCameraOnMouse, COptionsSettings::Instance->ControllerSettings.ChangeCameraOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,11);
	tmpName = COptionsInputPhase::Instance->GetKeyID(COptionsSettings::Instance->ControllerSettings.ReverseCamera, COptionsSettings::Instance->ControllerSettings.ReverseCameraOnMouse, COptionsSettings::Instance->ControllerSettings.ReverseCameraOnJoy);
	KeyList->setItem(new MyListItem(tmpName, "InputOptions_KeyList"),1,12);
	
	static_cast<CEGUI::Checkbox*>(winMgr.getWindow("InvertMouseCheck"))->setSelected(COptionsSettings::Instance->ControllerSettings.InverseMouse);

	COptionsInputPhase::Instance->SaveSettings();


	COptionsSettings::Instance->SetDefaultSensitivity();
	CEGUI::Scrollbar *scrollbar = NULL;

	scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_KeyboardSensitivityScrollbar");
	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeKeyboardSensitivity());
	
	scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_MouseSensitivityScrollbar");
	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeMouseSensitivity());
	
	scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_JoystickSensitivityScrollbar");
	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeJoystickSensitivity());
	
	scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_JoystickDeadzoneScrollbar");
	scrollbar->setScrollPosition(COptionsSettings::Instance->GetRelativeJoystickDeadzone());

	COptionsSettings::Instance->Save();

	return true;
}




String COptionsInputPhase::GetControlString(DWORD i)
{
	if(i == DIK_UP) return ("UP");
	else if(i == DIK_DOWN) return ("DOWN");
	else if(i == DIK_LEFT) return ("LEFT");
	else if(i == DIK_RIGHT) return ("RIGHT");
	else if(i == DIK_LCONTROL) return ("L CTRL");
	else if(i == DIK_RCONTROL) return ("R CTRL");
	else if(i == DIK_LSHIFT) return ("L SHIFT");
	else if(i == DIK_RSHIFT) return ("R SHIFT");
	else if(i == DIK_LALT) return ("L ALT");
	else if(i == DIK_RALT) return ("R ALT");
	else if(i == DIK_RETURN) return ("ENTER");
	else if(i == DIK_NUMPADENTER) return ("NUM ENTER");
	else if(i == DIK_INSERT) return ("INSERT");
	else if(i == DIK_DELETE) return ("DELETE");
	else if(i == DIK_HOME) return ("HOME");
	else if(i == DIK_END) return ("END");
	else if(i == DIK_PGUP) return ("PG UP");
	else if(i == DIK_PGDN) return ("PG DOWN");
	else if(i == DIK_NUMPADSLASH) return ("NUM /");

	DWORD j=0;
	unsigned char i_mask;
	DWORD res_mask;
    for(i_mask=0x01, res_mask=0x00010000; i_mask; i_mask<<=1,res_mask<<=1)
	{
	    if(i&i_mask) j|=res_mask;
		else j&=(~res_mask);
	}

	
	char buf[21];
	GetKeyNameText(j,buf,20);
	String Res(buf);

    /*j|=0x01000000;
    GetKeyNameText(j,buf,20);
	String Res1(buf);*/

    //FILE *f = fopen("operations.txt", "a+");
	//fprintf(f," %x %x %s\n", j, i, buf);
	//fclose(f);

	//if( Res != Res1 )
	//{
	    /*if(Res.GetLen() > Res1.GetLen())*/ //return Res;
		//else return Res1;
	//}

	return Res;
}


void COptionsInputPhase::SaveSettings()
{
	delete CProfileManager::Instance()->GetCurrentProfile()->ControllerSettings;
	CProfileManager::Instance()->GetCurrentProfile()->ControllerSettings = COptionsSettings::Instance->ControllerSettings.Clone();
	CProfileManager::Instance()->SaveAllProfiles();
}



bool COptionsInputPhase::OnKeyboardSensitivityChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_KeyboardSensitivityScrollbar");
	COptionsSettings::Instance->SetRelativeKeyboardSensitivity(Scrollbar->getScrollPosition());

	return true;
}



bool COptionsInputPhase::OnMouseSensitivityChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_MouseSensitivityScrollbar");
	COptionsSettings::Instance->SetRelativeMouseSensitivity(Scrollbar->getScrollPosition());

	return true;
}



bool COptionsInputPhase::OnJoystickSensitivityChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_JoystickSensitivityScrollbar");
	COptionsSettings::Instance->SetRelativeJoystickSensitivity(Scrollbar->getScrollPosition());

	return true;
}



bool COptionsInputPhase::OnJoystickDeadzoneChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("InputOptions_JoystickDeadzoneScrollbar");
	COptionsSettings::Instance->SetRelativeJoystickDeadzone(Scrollbar->getScrollPosition());

	return true;
}

