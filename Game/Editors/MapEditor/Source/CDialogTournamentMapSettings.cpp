/*
    CDialogTournamentMapSettings.cpp

	Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0

    
*/


#include <CDialogTournamentMapSettings.h>
#include <CTuberDescriptionManager.h>
#include <CTuberDescription.h>
#include <CEditorResourceManager.h>
#include <CMapEditorwindow.h>
#include <CMapProvider.h>
#include <CProfile.h>
#include <CTournamentOpenCloseState.h>
#include <CDialogTournamentOpenClose.h>
#include <CDialogInsert.h>
#include <CDialogAIProfile.h>


//========================================================
//    TubersEditor Class functions
//=======================================================

CDialogTournamentMapSettings* CDialogTournamentMapSettings::_instance = NULL;

CDialogTournamentMapSettings* CDialogTournamentMapSettings::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTournamentMapSettings;
	return _instance;
}


bool CDialogTournamentMapSettings::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TMSETTINGS, hParent, Show))
	{
		return false;
	}

	return true;
}


void CDialogTournamentMapSettings::OnShow()
{

	StateName = new CEditBox(this, IDC_STATENAME);
	MapName = new CComboBox(this, IDC_MAPNAMECOMBO);
	LoadingMusic = new CEditBox(this, IDC_LOADINGMUSIC);    
	LoadingImage = new CEditBox(this, IDC_LOADINGIMAGE);
	MapDescriptionImage = new CEditBox(this, IDC_MAPDESCRIPTIONIMAGE);    
	LoadingText = new CEditBox(this, IDC_LOADINGTEXT);
	MapDescription = new CEditBox(this, IDC_MAPDESCRIPTION);
	MapType = new CComboBox(this, IDC_MAPTYPE);
	EndCondition = new CComboBox(this, IDC_ENDCONDITION);
	RespawnPlayers = new CCheckBox(this, IDC_RESPAWNPLAYERS);
	Weapons = new CCheckBox(this, IDC_WEAPONSINRACE);
	Opened = new CCheckBox(this, IDC_OPENED);

	TimeForRace = new CEditBox(this, IDC_TIMEFORRACE);
	MaxTrials = new CEditBox(this, IDC_MAXTRIALS);
	MapReverse = new CCheckBox(this, IDC_MAPREVERSE);
	LapCount = new CEditBox(this, IDC_LAPCOUNT);
	Bonus1st = new CEditBox(this, IDC_BONUS1ST);
	Bonus2nd = new CEditBox(this, IDC_BONUS2ND);
	Bonus3rd = new CEditBox(this, IDC_BONUS3RD);
	
	ToOpenListBox = new CListBox(this, IDC_OPENLIST);
	ToCloseListBox = new CListBox(this, IDC_CLOSELIST);
	AIProfiles = new CListBox(this, IDC_AIPROFILES);
	AddOpenList = new CButton(this, IDC_ADDOPENLIST);
	RemoveOpenList = new CButton(this, IDC_REMOVEOPENLIST);
	AddCloseList = new CButton(this, IDC_ADDCLOSELIST);
	RemoveCloseList = new CButton(this, IDC_REMOVECLOSELIST);
	AddAI = new CButton(this, IDC_ADDAI);
	RemoveAI = new CButton(this, IDC_REMOVEAI);
	EditAI = new CButton(this, IDC_EDITAI);	

	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	AIProfiles =  new CListBox(this, IDC_AIPROFILES);

	MapDescriptionBrowse = new CButton(this, IDC_MAPDESCRIPTIONBROWSE);
	LoadingImageBrowse = new CButton(this, IDC_LOADINGIMAGEBROWSE);

	MarkEndOfGame = new CCheckBox(this, IDC_MARKENDOFGAME);
	EndGameImage = new CEditBox(this, IDC_ENDGAMEIMAGE);
	EndGameMusic = new CEditBox(this, IDC_ENDGAMEMUSIC);
	EndGameImageBrowse = new CButton(this, IDC_ENDGAMEIMAGEBROWSE);
	EndGameText = new CEditBox(this, IDC_ENDGAMETEXT);

    
	MapName->Clear();
	for (CMapProvider::Instance->MapNames.First(); !CMapProvider::Instance->MapNames.End(); CMapProvider::Instance->MapNames.Next())
	{
		MapName->AddItem(CMapProvider::Instance->MapNames.GetItem());
	}


	MapType->Clear();
	MapType->AddItem("Standard race", RACETYPE_STANDARD);
	MapType->AddItem("Time trial", RACETYPE_TIMETRIAL);
	MapType->AddItem("Last out", RACETYPE_LASTOUT);
	MapType->SetSelection(0);


	EndCondition->Clear();
	EndCondition->AddItem("Finish 1st", RACEENDCONDITION_1);
	EndCondition->AddItem("Finish 1st, 2nd", RACEENDCONDITION_12);
	EndCondition->AddItem("Finish 1st 2nd 3rd", RACEENDCONDITION_123);
	EndCondition->AddItem("Finish 2nd", RACEENDCONDITION_2);
	EndCondition->AddItem("Finish 2nd 3rd", RACEENDCONDITION_23);
	EndCondition->AddItem("Finish 3rd", RACEENDCONDITION_3);
	EndCondition->AddItem("Finish in time", RACEENDCONDITION_TIME);
	EndCondition->SetSelection(0);	

	Update(MapStates);
}


void CDialogTournamentMapSettings::Update(CTournamentMapSettings *MapStatess)
{
	StateName->SetText(MapStates->StateName);
	MapName->SetSelection(MapStates->MapName);
	LoadingMusic->SetText(MapStates->LoadingMusic);
	LoadingImage->SetText(MapStates->LoadingImage);
	LoadingText->SetText(MapStates->LoadingText);
	MapDescription->SetText(MapStates->MapDescription);
	MapDescriptionImage->SetText(MapStates->MapDescriptionImage);
	MapType->SetSelection(MapStates->RaceType);
	EndCondition->SetSelection(MapStates->EndCondition);
	RespawnPlayers->SetChecked(MapStates->RespawnPlayers);
	Weapons->SetChecked(MapStates->Weapons);
	Opened->SetChecked(MapStates->Opened);
	TimeForRace->SetText(MapStates->TimeForRace);
	MaxTrials->SetText(MapStates->MaxTrialsCount);
	MapReverse->SetChecked(MapStates->MapReverse);
	LapCount->SetText(MapStates->LapCount);
	Bonus1st->SetText(MapStates->Bonus1stCash);
	Bonus2nd->SetText(MapStates->Bonus2ndCash);
	Bonus3rd->SetText(MapStates->Bonus3rdCash);
	MarkEndOfGame->SetChecked(MapStates->MarksEndOfGame);
	EndGameImage->SetText(MapStates->EndGameImage);
	EndGameMusic->SetText(MapStates->EndGameMusic);
	EndGameText->SetText(MapStates->EndGameText);
	

	ToOpenListBox->Clear();
	String s;
	for (MapStates->ToOpen.First(); !MapStates->ToOpen.End(); MapStates->ToOpen.Next())
	{
		s = MapStates->ToOpen.GetItem().TournamentName + "-";
		s = s + MapStates->ToOpen.GetItem().StateName;
		ToOpenListBox->AddItem(s);
	}

	ToCloseListBox->Clear();
	for (MapStates->ToClose.First(); !MapStates->ToClose.End(); MapStates->ToClose.Next())
	{
		s = MapStates->ToClose.GetItem().TournamentName + "-"; 
		s = s + MapStates->ToClose.GetItem().StateName;
		ToCloseListBox->AddItem(s);
	}

	AIProfiles->Clear();
	for (MapStates->AIProfiles.First(); !MapStates->AIProfiles.End(); MapStates->AIProfiles.Next())
	{
		AIProfiles->AddItem(MapStates->AIProfiles.GetItem()->Name);
	}
}

void CDialogTournamentMapSettings::OnOKButton()
{
	MapStates->StateName = StateName->GetText();
	MapStates->MapName = MapName->GetText();
	MapStates->LoadingMusic = LoadingMusic->GetText();
	MapStates->LoadingText = LoadingText->GetText();
	MapStates->LoadingImage = LoadingImage->GetText();
	MapStates->MapDescriptionImage = MapDescriptionImage->GetText();
	MapStates->MapDescription = MapDescription->GetText();
	MapStates->RaceType = (RACETYPE)MapType->GetSelectionData();
	MapStates->EndCondition = (RACEENDCONDITION)EndCondition->GetSelectionData();
	MapStates->RespawnPlayers = RespawnPlayers->GetChecked();
	MapStates->Weapons = Weapons->GetChecked();
	MapStates->Opened = Opened->GetChecked();
	MapStates->TimeForRace = TimeForRace->GetTextAsFloat();
	MapStates->MaxTrialsCount = MaxTrials->GetTextAsInt();
	MapStates->MapReverse = MapReverse->GetChecked();
	MapStates->LapCount = LapCount->GetTextAsInt();
	MapStates->Bonus1stCash = Bonus1st->GetTextAsInt();
	MapStates->Bonus2ndCash = Bonus2nd->GetTextAsInt();
	MapStates->Bonus3rdCash = Bonus3rd->GetTextAsInt();
	MapStates->MarksEndOfGame = MarkEndOfGame->GetChecked();
	MapStates->EndGameImage = EndGameImage->GetText();
	MapStates->EndGameMusic = EndGameMusic->GetText();
	MapStates->EndGameText = EndGameText->GetText();
	SetModalResult(MR_OK);
}

void CDialogTournamentMapSettings::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID())
	{
		OnOKButton();
	}
	if (ID == CancelButton->GetID())
	{
		SetModalResult(MR_CANCEL);
	}

	if (ID == AddOpenList->GetID())
	{
		CTournamentOpenCloseState toc;
		String s;
		if (CDialogTournamentOpenClose::Instance()->ShowModal(GetHWND()) == MR_OK)
		{
			toc.StateName = CDialogTournamentOpenClose::Instance()->StateName;
			toc.TournamentName = CDialogTournamentOpenClose::Instance()->Tournament;
			MapStates->ToOpen.PushBack(toc);
			s = toc.TournamentName + "-";
			s = s + toc.StateName;
			ToOpenListBox->AddItem(s);
		}
	}

	if (ID == RemoveOpenList->GetID())
	{
		String s, s2;
		s = ToOpenListBox->GetSelectionText();
		s2 = s.Extract('-', 0);
		s = s.Extract('-', 1);


		for (MapStates->ToOpen.First(); !MapStates->ToOpen.End(); MapStates->ToOpen.Next())
		{
			if (MapStates->ToOpen.GetItem().StateName == s && MapStates->ToOpen.GetItem().TournamentName == s2)
			{
				MapStates->ToOpen.Delete();
				ToOpenListBox->RemoveItem(ToOpenListBox->GetSelectionIndex());
				break;
			}            
		}
	}

	if (ID == EndGameImageBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			EndGameImage->SetText(InsertDialog->HoleName);
		}
	}

	if (ID == AddCloseList->GetID())
	{
		CTournamentOpenCloseState toc;
		String s;
		if (CDialogTournamentOpenClose::Instance()->ShowModal(GetHWND()) == MR_OK)
		{
			toc.StateName = CDialogTournamentOpenClose::Instance()->StateName;
			toc.TournamentName = CDialogTournamentOpenClose::Instance()->Tournament;
			MapStates->ToClose.PushBack(toc);
			s = toc.TournamentName + "-";
			s = s + toc.StateName;
			ToCloseListBox->AddItem(s);
		}
	}

	if (ID == RemoveCloseList->GetID())
	{
		String s, s2;
		s = ToCloseListBox->GetSelectionText();
		s2 = s.Extract('-', 0);
		s = s.Extract('-', 1);

		for (MapStates->ToClose.First(); !MapStates->ToClose.End(); MapStates->ToClose.Next())
		{
			if (MapStates->ToClose.GetItem().StateName == s && MapStates->ToClose.GetItem().TournamentName == s2)
			{
				MapStates->ToClose.Delete();
				ToCloseListBox->RemoveItem(ToCloseListBox->GetSelectionIndex());
				break;
			}            
		}
	}

	if (ID == AddAI->GetID())
	{
		if (MapStates->AIProfiles.GetItemsCount() == 15)
		{
			MessageBox(GetHWND(), "Maximum allowed AI profiles is 15", "Info", MB_OK | MB_ICONINFORMATION);
		} else
		{
			CProfile *AIProfile;
			AIProfile = new CProfile;
			AIProfile->AIProfile = true;
			CDialogAIProfile::Instance()->Profile = AIProfile;
			if (CDialogAIProfile::Instance()->ShowModal(GetHWND()) == MR_OK)
			{
				AIProfiles->AddItem(CDialogAIProfile::Instance()->Profile->Name);
				MapStates->AIProfiles.PushBack(CDialogAIProfile::Instance()->Profile);
			} else
			{
				delete AIProfile;
			}
		}
	}

	if (ID == RemoveAI->GetID())
	{
		for (MapStates->AIProfiles.First(); !MapStates->AIProfiles.End(); MapStates->AIProfiles.Next())
		{
			if (MapStates->AIProfiles.GetItem()->Name == AIProfiles->GetSelectionText())
			{
				MapStates->AIProfiles.Delete();
				break;
			}
		}
		AIProfiles->RemoveItem(AIProfiles->GetSelectionIndex());
	}

	if (ID == EditAI->GetID())
	{
		for (MapStates->AIProfiles.First(); !MapStates->AIProfiles.End(); MapStates->AIProfiles.Next())
		{
			if (MapStates->AIProfiles.GetItem()->Name == AIProfiles->GetSelectionText())
			{
				CDialogAIProfile::Instance()->Profile = MapStates->AIProfiles.GetItem();
				if (CDialogAIProfile::Instance()->ShowModal(GetHWND()) == MR_OK)
				{
					AIProfiles->RemoveItem(AIProfiles->GetSelectionIndex());
					AIProfiles->AddItem(CDialogAIProfile::Instance()->Profile->Name);
					MapStates->AIProfiles.PushBack(CDialogAIProfile::Instance()->Profile);
				}
				break;
			}
		}		
	}

	if (ID == LoadingImageBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			LoadingImage->SetText(InsertDialog->HoleName);
		}
	}

	if (ID == MapDescriptionBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			MapDescriptionImage->SetText(InsertDialog->HoleName);
		}
	}

}
