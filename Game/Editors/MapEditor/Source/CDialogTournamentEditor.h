/*
    CDialogTournamentEditor.h

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0


*/

#ifndef _CDialogTournamentEditor_H
#define _CDialogTournamentEditor_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTuberDescription.h>

class CDialogTournamentEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);
	
		
	static CDialogTournamentEditor *_instance;

public:
	static CDialogTournamentEditor *Instance();
	CDialogTournamentEditor(){}


	CListBox*	TournamentList;
	CButton*	AddTournament;
	CButton*	EditTournament;
	CButton*	RemoveTournament;
	CButton*	OKButton;

	bool		Create(HWND hParent, bool Show = true);	
	void		OnShow();
};



#endif