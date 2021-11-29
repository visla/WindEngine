/*
CDialogPhysicProperties.h


Autor: Nebojsa Vislavski
Date: 04.11.2006.
Version: 1.0
Platform: Win32

*/

#ifndef _CDIALOGPHYSICPROPERTIES_H
#define _CDIALOGPHYSICPROPERTIES_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeActor.h>
#include <CMapEditorWindow.h>
#include <SmartList.h>

class CDialogPhysicProperties: public CDialog
{
protected:
	static CDialogPhysicProperties *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);


	
	void OnShow();
	

	bool CanChange;


public:
	static CDialogPhysicProperties *Instance();


	CComboBox *TypeCombo;
	CComboBox *TriggerTypeCombo;
	CEditBox  *Mass;
	CEditBox  *Group;
	CEditBox  *BoxDimensionX;
	CEditBox  *BoxDimensionY;
	CEditBox  *BoxDimensionZ;
	CEditBox  *SphereRadius;
	CEditBox  *CapsuleRadius;
	CEditBox  *CapsuleHeight;
	CCheckBox *SleepOnStart;
	CButton	  *GetFromObject;

	CButton *Cancel;
	CButton *OK;

	WeActor *Actor;
	CSmartList <WeActor *> ActorList;

	bool Create(HWND hParent, bool Show = true);
	void Show(WeActor *actor);
	void Show(CSmartList <WeActor *> &actorList);
	void Update(WeActor *actor);
	void UpdateFromList();
};


#endif