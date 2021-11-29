/*
    CDialogTournamentMapSettings.h

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0


*/

#ifndef _CDialogTournamentMapSettings_H
#define _CDialogTournamentMapSettings_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTournament.h>

class CDialogTournamentMapSettings : public CDialog
{
protected:
	void OnClick(DWORD ID);
	
		
	static CDialogTournamentMapSettings *_instance;

public:
	static CDialogTournamentMapSettings *Instance();
	CDialogTournamentMapSettings(){}

	CTournamentMapSettings	*MapStates;

	CEditBox	*StateName;
	CComboBox	*MapName;
	CEditBox*	LoadingMusic;
	CEditBox*	LoadingImage;
	CEditBox*	MapDescriptionImage;
	CEditBox*	LoadingText;
	CEditBox*	MapDescription;
	CComboBox*	MapType;
	CComboBox*	EndCondition;
	CCheckBox*	RespawnPlayers;
	CCheckBox*	Weapons;
	CCheckBox*	Opened;
	CEditBox*	TimeForRace;
	CEditBox*	MaxTrials;
	CCheckBox*	MapReverse;
	CEditBox*	LapCount;
	CEditBox*	Bonus1st;
	CEditBox*	Bonus2nd;
	CEditBox*	Bonus3rd;
	CListBox*	ToOpenListBox;
	CListBox*	ToCloseListBox;
	CListBox*	AIProfiles;
	CButton*	AddOpenList;
	CButton*	RemoveOpenList;
	CButton*	AddCloseList;
	CButton*	RemoveCloseList;
	CButton*	AddAI;
	CButton*	RemoveAI;
	CButton*	EditAI;
	CButton*	OKButton;
	CButton*	CancelButton;
	CButton*	MapDescriptionBrowse;
	CButton*	LoadingImageBrowse;
	CCheckBox*	MarkEndOfGame;
	CEditBox*	EndGameMusic;
	CEditBox*	EndGameImage;
	CEditBox*	EndGameText;
	CButton*	EndGameImageBrowse;



	void		OnOKButton();    
	bool		Create(HWND hParent, bool Show = true);	
	void		Update(CTournamentMapSettings *tms);
	void		OnShow();
};



#endif