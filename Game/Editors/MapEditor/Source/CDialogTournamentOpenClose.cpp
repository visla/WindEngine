/*
    CDialogTournamentOpenClose.cpp

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0

    
*/


#include <CDialogTournamentOpenClose.h>
#include <CTuberDescriptionManager.h>
#include <CTuberDescription.h>
#include <CEditorResourceManager.h>
#include <CMapEditorwindow.h>
#include <CTournamentManager.h>

//========================================================
//    TubersEditor Class functions
//=======================================================

CDialogTournamentOpenClose* CDialogTournamentOpenClose::_instance = NULL;

CDialogTournamentOpenClose* CDialogTournamentOpenClose::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTournamentOpenClose;
	return _instance;
}


bool CDialogTournamentOpenClose::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TOPENCLOSESTATE, hParent, Show))
	{
		return false;
	}


	return true;
}


void CDialogTournamentOpenClose::OnShow()
{
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	Tournaments = new CComboBox(this, IDC_TOURNAMENTCOMBO);
	States = new CComboBox(this, IDC_STATECOMBO);

	Tournaments->Clear();
	for (CTournamentManager::Instance->Tournaments.First(); !CTournamentManager::Instance->Tournaments.End(); CTournamentManager::Instance->Tournaments.Next())
	{
		Tournaments->AddItem(CTournamentManager::Instance->Tournaments.GetItem().Name);
	}
	Tournaments->SetSelection(0);

	States->Clear();
}


void CDialogTournamentOpenClose::Update(CTournament *Tournament)
{
}

void CDialogTournamentOpenClose::OnOKButton()
{
	SetModalResult(true);
}

void CDialogTournamentOpenClose::OnClick(DWORD ID)
{
	if (ID == Tournaments->GetID())
	{
		CTournament *t;		
		t = CTournamentManager::Instance->FindTournament(Tournaments->GetSelectionText());                
		if (t)
		{
			States->Clear();
			for (t->Maps.First(); !t->Maps.End(); t->Maps.Next())
			{
				States->AddItem(t->Maps.GetItem().StateName);
			}
			States->SetSelection(0);
		}
	}

	if (ID == OKButton->GetID())
	{
		Tournament = Tournaments->GetText();
		StateName = States->GetText();
		SetModalResult(MR_OK);
	}

	if (ID == CancelButton->GetID())
	{
		SetModalResult(MR_CANCEL);
	}


}



