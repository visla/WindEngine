#ifndef _DIALOG_INSERT
#define _DIALOG_INSERT


#include <CDialog.h>
#include <CComponent.h>
#include <CString.h>


extern class CDialogInsert* InsertDialog;

class CDialogInsert : public CDialog
{
protected:
	void OnClick(DWORD ID);
	
	void InsertButtonClick();
	void CancelButtonClick();
	void OnTreeViewClick();

public:
	CDialogInsert();

	CTreeView* PackageTree;
	//CTreeViewItem* SelectedItem;
	CButton *InsertButton;
	CButton *CancelButton;

	int Type;
	String HoleName;
	String Extension;

	bool Create(HWND hParent, bool Show = true);
	void OnShow();
};


#endif
