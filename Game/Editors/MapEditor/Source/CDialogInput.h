/*
    CDialogInput.h

	Autor: Nebojsa Vislavski
	Date: 16.12.2005.
	Version: 1.0
    

*/

#ifndef _CDialogInput_H
#define _CDialogInput_H

#include <CDialog.h>
#include <CComponent.h>

class CDialogInput: public CDialog
{
protected:
	static CDialogInput *_instance;


	void OnClick(DWORD ID);
	void OnShow();


public:
	static CDialogInput *Instance();

	CButton  *OKButton;
	CButton  *CancelButton;
	CEditBox *EditBox;
	CLabel	 *Static;

	String  InputText;


    bool Create(HWND hParent, bool Show = true);
	int  Show(StringParam StaticText);

};



#endif