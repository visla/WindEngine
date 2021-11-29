/*
     CDialogAIProfile.h


	 Autor: Nebojsa Vislavski
	 Date: 21.3.2006.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogAIProfile_H
#define _CDialogAIProfile_H

#include <CDialog.h>
#include <CComponent.h>
#include <CMap.h>
#include <CProfile.h>

class CDialogAIProfile: public CDialog
{
protected:
	static CDialogAIProfile *_instance;

	void OnOKClick();
	void OnClick(DWORD ID);

	bool CanChange;


public:
	static CDialogAIProfile *Instance();

	CButton  *OKButton;
	CButton  *CancelButton;

	CEditBox*	NameEdit;
	CComboBox*	TeamNumber;
	CEditBox*	CashCategory;
	CComboBox*	TuberName;
	CEditBox*	ModelName;
	CButton*	ModelBrowse;
	CEditBox*	Speed;
	CEditBox*	Acceleration;
	CEditBox*	Manouverability;
	CComboBox*	WeaponName;
	CComboBox*	MineName;
	CEditBox*	HP;
	CEditBox*	WeaponUpgradeLevel;
	CEditBox*	MineUpgradeLevel;
	CEditBox*	TurboUpgradeLevel;
	
	CComboBox*	PartNum;
	CEditBox*	PartModelName;
	CButton*	PartModelBrowse;
	CEditBox*	PartCount;

	CProfile *Profile;

	void UpdateTuberComponents();
	void FillWeaponCombo();
    bool Create(HWND hParent, bool Show = true);
	void Update(CProfile *Profile);	
	void OnShow();	
};


#endif