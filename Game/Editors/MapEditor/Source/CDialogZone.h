/*
     CDialogZone.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogZone_H
#define _CDialogZone_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeZone.h>
#include <CMapEditorWindow.h>


class CDialogZone: public CDialog
{
protected:
	static CDialogZone *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }


	bool CanChange;


public:
	static CDialogZone *Instance();

	CEditBox *Name;
	CEditBox *BoxSizeX;
	CEditBox *BoxSizeY;
	CEditBox *BoxSizeZ;
	CListBox *Portals;
	CButton *OKButton;
	WeZone *Zone;

    bool Create(HWND hParent, bool Show = true);
	void Show(WeZone *zone);
	void Update(WeZone *zone);
};


#endif