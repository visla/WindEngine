/*
    CDialogTournament.h

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0


*/

#ifndef _CDialogTournament_H
#define _CDialogTournament_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTournament.h>

class CDialogTournament : public CDialog
{
protected:
	void OnClick(DWORD ID);
	
		
	static CDialogTournament *_instance;

public:
	static CDialogTournament *Instance();
	CDialogTournament(){}


	CEditBox*	Name;
	CCheckBox*	Opened;
	CComboBox*	Type;

	CListBox*	MapStates;
	CButton*	AddMapState;
	CButton*	RemoveMapState;
	CButton*	EditMapState;
	CButton*	OKButton;
	CButton*	CancelButton;

	CTournament	*Tournament;

	void		OnOKButton();    
	bool		Create(HWND hParent, bool Show = true);	
	void		Update(CTournament *tournament);
	void		OnShow();
};



#endif