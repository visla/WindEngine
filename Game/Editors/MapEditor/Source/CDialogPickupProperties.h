/*
     CDialogPickupProperties.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogPickupProperties_H
#define _CDialogPickupProperties_H

#include <CDialog.h>
#include <CComponent.h>
#include <CMap.h>

class CPickUp;

class CDialogPickupProperties: public CDialog
{
protected:
	static CDialogPickupProperties* _instance;
	void OnClick(DWORD ID);

    CPickUp* PickUp;

public:
	static CDialogPickupProperties *Instance();

	CButton*	OKButton;
	CCheckBox*	Active;
	CCheckBox*	ShouldTerminate;
	CEditBox*	RespawnTime;


    bool Create(HWND hParent, bool Show = true);
	void Update();	
	void Show(CPickUp*);	
};


#endif