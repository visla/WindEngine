/*
    CDialogTournament.cpp

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0

    
*/


#include <CDialogTournament.h>
#include <CTuberDescriptionManager.h>
#include <CTuberDescription.h>
#include <CEditorResourceManager.h>
#include <CMapEditorwindow.h>
#include <CDialogTournamentMapSettings.h>


//========================================================
//    TubersEditor Class functions
//=======================================================

CDialogTournament* CDialogTournament::_instance = NULL;

CDialogTournament* CDialogTournament::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTournament;
	return _instance;
}


bool CDialogTournament::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TURNAMENTPROPERTIES, hParent, Show))
	{
		return false;
	}


	return true;
}


void CDialogTournament::OnShow()
{
	Name = new CEditBox(this, IDC_TOURNAMENTNAME);
	Type = new CComboBox(this, IDC_TOURNAMENTTYPE);
	Opened = new CCheckBox(this, IDC_OPENED);

	MapStates = new CListBox(this, IDC_MAPSTATESLIST);
	AddMapState = new CButton(this, IDC_ADDRMAPSTATE);
	RemoveMapState = new CButton(this, IDC_REMOVEMAPSTATE);
	EditMapState = new CButton(this, IDC_EDITMAPSTATE);
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);

	Type->Clear();
	Type->AddItem("Single Player", CTournament::TT_SINGLEPLAYER);
	Type->AddItem("Multi Player", CTournament::TT_MULTIPLAYER);
	Type->SetSelection(0);

	Update(Tournament);
}


void CDialogTournament::Update(CTournament *Tournament)
{
	MapStates->Clear();
	Name->SetText(Tournament->Name);
	Opened->SetChecked(Tournament->Opened);
	Type->SetSelection(Tournament->Type);

	for (Tournament->Maps.First(); !Tournament->Maps.End(); Tournament->Maps.Next())
	{
		MapStates->AddItem(Tournament->Maps.GetItem().StateName);
	}
}

void CDialogTournament::OnOKButton()
{
	Tournament->Name = Name->GetText();
	Tournament->Type = (CTournament::TOURNAMENTTYPE)Type->GetSelectionData();
	Tournament->Opened = Opened->GetChecked();
	SetModalResult(true);
}

void CDialogTournament::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID())
	{
		OnOKButton();
	}
	if (ID == CancelButton->GetID())
	{
		SetModalResult(MR_CANCEL);
	}

	CTournamentMapSettings *tms;
	if (ID == AddMapState->GetID())
	{
		tms = new CTournamentMapSettings;
		CDialogTournamentMapSettings::Instance()->MapStates = tms;
		if (CDialogTournamentMapSettings::Instance()->ShowModal(GetHWND()) == MR_OK)
		{
			MapStates->AddItem(tms->StateName);
			Tournament->AddMapSettings(*tms);
		}
		delete tms;
	}

	if (ID == RemoveMapState->GetID())
	{
		if (Tournament->RemoveMapSettings(MapStates->GetSelectionText()))
		{
			MapStates->RemoveItem(MapStates->GetSelectionIndex());
		}
	}

	if (ID == EditMapState->GetID())
	{
		tms = Tournament->FindMapSettings(MapStates->GetSelectionText());
		if (tms)
		{
			CDialogTournamentMapSettings::Instance()->MapStates = tms;
			if (CDialogTournamentMapSettings::Instance()->ShowModal(GetHWND()) == MR_OK)
			{
				MapStates->RemoveItem(MapStates->GetSelectionIndex());
				MapStates->AddItem(tms->StateName);				
			} 
		}
	}
}



