#ifndef _DIALOG_ABOUT
#define _DIALOG_ABOUT


#include <CDialog.h>
#include <CComponent.h>


extern class CDialogAbout* DialogAbout;

class CDialogAbout : public CDialog
{
protected:
	void OnShow();
	void OnClick(DWORD ID);
	void OnOK();

	CButton* OkButton;

public:
	CDialogAbout();

	bool Create(HWND hParent, bool Shown = true);
};


#endif
