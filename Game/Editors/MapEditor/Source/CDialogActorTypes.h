/*
    CDialogActorTypes.h

	Autor: Nebojsa Vislavski
	Date: 19.1.2006.
	Version: 1.0
    

*/

#ifndef _CDIALOGACTORTYPES_H
#define _CDIALOGACTORTYPES_H

#include <CDialog.h>
#include <CComponent.h>

class CDialogActorTypes: public CDialog
{
protected:
	static CDialogActorTypes *_instance;


	void OnClick(DWORD ID);
	void OnShow();


public:
	static CDialogActorTypes *Instance();

	CButton *OKButton;
	CComboBox *ActorTypesCombo;

	int TypeSelected;  

    bool Create(HWND hParent, bool Show = true);
};



#endif