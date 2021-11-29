/*
    CDialogActionManagment.h

	Autor: Nebojsa Vislavski
	Date: 27.12.2005.
	Version: 1.0
    

*/

#ifndef _CDIALOGACTIONMANAGMENT_H
#define _CDIALOGACTIONMANAGMENT_H

#include <CDialog.h>
#include <CComponent.h>

class CDialogActionManagment: public CDialog
{
protected:
	static CDialogActionManagment *_instance;


	void OnClick(DWORD ID);



	void FillActionCombo();

public:
	static CDialogActionManagment *Instance();

	void OnDeleteButtonClick();
	void OnNewButtonClick();
	void OnPlayButtonClick();
	void OnStopRecButtonClick();

	CButton *NewButton;
	CButton *PlayButton;
	CButton *DeleteButton;
	CButton *StopRecButton;
	CComboBox *ActionCombo;


    bool Create(HWND hParent, bool Show = true);
	void Show();
};



#endif