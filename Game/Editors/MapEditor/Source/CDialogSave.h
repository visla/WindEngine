#ifndef _DIALOG_SAVE
#define _DIALOG_SAVE


#include <CDialog.h>
#include <CComponent.h>
#include <CString.h>
#include <CEditorResourceManager.h>



extern class CDialogSave* SaveDialog;

class CDialogSave : public CDialog
{
protected:
	void OnClick(DWORD ID);

	void SaveButtonClick();
	void CancelButtonClick();
	void DeleteButtonClick();
	void OnPackageTreeClick();

	void FillGroupNames(StringParam Package);
	void FillNames(StringParam Group);

public:
	CDialogSave();

	CTreeView*	PackageTree;
	CComboBox*	PackageName;
	CComboBox*	GroupName;
	CComboBox*	Name;
	CButton*	Save;
	CButton*	Cancel;
	CButton*	Delete;

	int  Type;
	String HoleName;
	String Extension;

    bool Create(HWND hParent, bool Show = true);
	void OnShow();
	void OnHide();
};


#endif
