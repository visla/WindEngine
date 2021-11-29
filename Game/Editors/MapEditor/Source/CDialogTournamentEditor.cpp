/*
    CDialogTournamentEditor.cpp

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0

    
*/


#include <CDialogTournamentEditor.h>
#include <CTuberDescriptionManager.h>
#include <CTuberDescription.h>
#include <CEditorResourceManager.h>
#include <CMapEditorwindow.h>
#include <CDialogTournament.h>
#include <CTournamentManager.h>

//========================================================
//    TubersEditor Class functions
//=======================================================

CDialogTournamentEditor* CDialogTournamentEditor::_instance = NULL;

CDialogTournamentEditor* CDialogTournamentEditor::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTournamentEditor;
	return _instance;
}


bool CDialogTournamentEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TOURNAMENTEDITOR, hParent, Show))
	{
		return false;
	}

	TournamentList = new CListBox(this, IDC_TOURNAMENTSLIST);
	AddTournament = new CButton(this, IDC_ADDTOURNAMENT);
	EditTournament = new CButton(this, IDC_EDITTOURNAMENT);
	RemoveTournament = new CButton(this, IDC_REMOVETOURNAMENT);
	OKButton = new CButton(this, IDOK);

	return true;
}


void CDialogTournamentEditor::OnShow()
{
	TournamentList->Clear();
	for (CTournamentManager::Instance->Tournaments.First(); !CTournamentManager::Instance->Tournaments.End(); CTournamentManager::Instance->Tournaments.Next())
	{
		TournamentList->AddItem(CTournamentManager::Instance->Tournaments.GetItem().Name);
	}
}

void CDialogTournamentEditor::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID())
	{
		Hide();
	}

	if (ID == AddTournament->GetID())
	{
		CTournament *t;
		t = new CTournament;
		CDialogTournament::Instance()->Tournament = t;
		if (CDialogTournament::Instance()->ShowModal(GetHWND()) == MR_OK)
		{
			CTournamentManager::Instance->AddTournament(*t);
			TournamentList->AddItem(t->Name, *((DWORD*)&t));
		}
		delete t;
	}

	if (ID == RemoveTournament->GetID())
	{
		CTournamentManager::Instance->Remove(TournamentList->GetSelectionText());
		TournamentList->RemoveItem(TournamentList->GetSelectionIndex());
	}

	if (ID == EditTournament->GetID())
	{
		CTournament *t;
		t = CTournamentManager::Instance->FindTournament(TournamentList->GetSelectionText());
		if (t)
		{
			CDialogTournament::Instance()->Tournament = t;
			if (CDialogTournament::Instance()->ShowModal(GetHWND()) == MR_OK)
			{
				TournamentList->RemoveItem(TournamentList->GetSelectionIndex());
				TournamentList->AddItem(t->Name, 0);
				CTournamentManager::Instance->Save();
			}
		}

	}
}



