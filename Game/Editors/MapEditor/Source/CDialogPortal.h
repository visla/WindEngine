/*
     CDialogPortal.h


	 Autor: Nebojsa Vislavski
	 Date: 5.7.2006.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogPortal_H
#define _CDialogPortal_H

#include <CDialog.h>
#include <CComponent.h>
#include <WePortal.h>
#include <CMapEditorWindow.h>


class CDialogPortal: public CDialog
{
protected:
	static CDialogPortal *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }


	bool CanChange;


public:
	static CDialogPortal *Instance();

	CEditBox *Name;
	CComboBox *Zone1Name;
	CComboBox *Zone2Name;
	CEditBox *SizeX;
	CEditBox *SizeY;
	CCheckBox *TwoWayPortal;
	CButton *OKButton;
	WePortal *Portal;

    bool Create(HWND hParent, bool Show = true);
	void Show(WePortal *portal);
	void Update(WePortal *portal);
};


#endif