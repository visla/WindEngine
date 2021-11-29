/*
		File: CDialogStartLoacationProperties.h
		Author: Tomislav Kukic
		Date: 05.04.2006
		Version: 1.0
*/




#ifndef _CDialogStartLoacationProperties_h_
#define _CDialogStartLoacationProperties_h_


#include <CDialog.h>
#include <CComponent.h>
#include <CMapEditorWindow.h>


class CStartLocation;



class CDialogStartLoacationProperties:public CDialog
{
protected:
	static CDialogStartLoacationProperties* _instance;

	void OnClick(DWORD ID);
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }

	CStartLocation* StartLocation;

	CComboBox* TeamNum;

public:
	CDialogStartLoacationProperties();
	~CDialogStartLoacationProperties();

	static CDialogStartLoacationProperties* Instance();

	bool Create(HWND hParent, bool Shown = true);
	void ShowDlg(CStartLocation* _StartLocation);
};





#endif
