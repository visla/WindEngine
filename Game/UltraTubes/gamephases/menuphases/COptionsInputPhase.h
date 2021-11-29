/*
   COptionsInputPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _COptionsInputPhase_H
#define _COptionsInputPhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>
#include <CGUIHelper.h>



class COptionsInputPhase: public CAbstractPhase
{
protected:

	bool WNDFadeIn;
	float WNDAlpha;

	bool MouseClicked;
	bool EnterKey;
	int  ClickedIndex;
	MyListItem* SelectedItem;

	String GetControlString(DWORD i);
	void SetKeyID(int RowID, int Value, bool OnMouse, bool OnJoy);
	String GetKeyID(int Value, bool OnMouse, bool OnJoy); 
	bool ValidateInput(String KeyName);

public:
	static COptionsInputPhase*	Instance;

						COptionsInputPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool OnBack(const CEGUI::EventArgs& e);
	static bool OnKeyChange(const CEGUI::EventArgs& e);
	static bool OnInvertMouseClick(const CEGUI::EventArgs& e);
	static bool OnResetToDefaultsClick(const CEGUI::EventArgs& e);
	static bool OnKeyboardSensitivityChanged(const CEGUI::EventArgs& e);
	static bool OnMouseSensitivityChanged(const CEGUI::EventArgs& e);
	static bool OnJoystickSensitivityChanged(const CEGUI::EventArgs& e);
	static bool OnJoystickDeadzoneChanged(const CEGUI::EventArgs& e);

	void				SaveSettings();
};


#endif