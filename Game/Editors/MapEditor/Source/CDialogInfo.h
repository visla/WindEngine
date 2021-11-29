#ifndef _DIALOG_INFO
#define _DIALOG_INFO


#include <CDialog.h>
#include <CComponent.h>
#include <CString.h>


extern class CDialogInfo* DialogInfo;

class CDialogInfo : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnCloseClick();

	void OnShow();

public:
	CDialogInfo(){}

	String Name;
	String OutText;
	bool FromPackage;

	CEditBox* InfoText;

	bool Create(HWND hParent, bool Shown = true);
	bool OpenInfoFile();
};


#endif