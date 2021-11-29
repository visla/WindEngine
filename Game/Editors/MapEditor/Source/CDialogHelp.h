#ifndef _DIALOG_HELP
#define _DIALOG_HELP


#include <CDialog.h>
#include <CComponent.h>


extern class CDialogHelp* DialogHelp;

class CDialogHelp : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnOK();

	CButton* OkButton;
	CEditBox* HelpText;

public:
	CDialogHelp();

	bool Create(HWND hParent, bool Shown = true);
};


#endif
