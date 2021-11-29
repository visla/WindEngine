/*
     CDialogAIProfiles.h


	 Autor: Nebojsa Vislavski
	 Date: 21.3.2006.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogAIProfiles_H
#define _CDialogAIProfiles_H

#include <CDialog.h>
#include <CComponent.h>
#include <CMap.h>
#include <CProfile.h>

class CDialogAIProfiles: public CDialog
{
protected:
	static CDialogAIProfiles *_instance;

	void OnOKClick();
	void OnClick(DWORD ID);

	bool CanChange;


public:
	static CDialogAIProfiles *Instance();

	CButton  *OKButton;
	CButton  *CancelButton;


	CButton *AddAIProfile;
	CButton	*RemoveAIProfile;
	CButton	*EditAIProfile;

	CListBox	*AIList;

	
    bool Create(HWND hParent, bool Show = true);
	void FillAIList();
	void OnShow();	
};


#endif