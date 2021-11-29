/*
    CDialogTournamentOpenClose.h

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0


*/

#ifndef _CDialogTournamentOpenClose_H
#define _CDialogTournamentOpenClose_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTournament.h>

class CDialogTournamentOpenClose : public CDialog
{
protected:
	void OnClick(DWORD ID);
	
		
	static CDialogTournamentOpenClose *_instance;

public:
	static CDialogTournamentOpenClose *Instance();
	CDialogTournamentOpenClose(){}


	CButton		*OKButton;
	CButton		*CancelButton;
	CComboBox	*Tournaments;
	CComboBox	*States;


	String		Tournament;
	String		StateName;
	

	void		OnOKButton();    
	bool		Create(HWND hParent, bool Show = true);	
	void		Update(CTournament *tournament);
	void		OnShow();
};



#endif