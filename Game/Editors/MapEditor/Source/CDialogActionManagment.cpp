/*
    CDialogActionManagment.cpp

	Autor: Nebojsa Vislavski
	Date: 16.12.2005.
	Version: 1.0
    

*/


#include <CDialogActionManagment.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CActionManager.h>


CDialogActionManagment *CDialogActionManagment::_instance = NULL;
CDialogActionManagment *CDialogActionManagment::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogActionManagment();
	return _instance;
}

bool CDialogActionManagment::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_ACTIOMANAGERDIALOG, hParent, Show))
	{
		return false;
	}
	NewButton = new CButton(this, IDC_NEWBUTTON);
	DeleteButton = new CButton(this, IDC_DELETEBUTTON);
	PlayButton = new CButton(this, IDC_PLAYACTIONBUTTON);
	StopRecButton = new CButton(this, IDC_STOPREC);

	ActionCombo = new CComboBox(this, IDC_ACTIONCOMBO);	

	CActionManager::Instance()->LoadActions();
	FillActionCombo();
	return true;
}


void CDialogActionManagment::FillActionCombo()
{
	String s;

	ActionCombo->Clear();
	for (CActionManager::Instance()->Actions.First(); !CActionManager::Instance()->Actions.End(); CActionManager::Instance()->Actions.Next())
	{
		s = CActionManager::Instance()->Actions.GetItem()->Name;
		ActionCombo->AddItem(s);
	}

	ActionCombo->SetSelection(0);
}

void CDialogActionManagment::OnDeleteButtonClick()
{
	// TODO: Add action deleting
	if (MessageBox(EditorWindow->GetHWND(), "Are you sure you want to delete action?", "Answer", MB_ICONINFORMATION | MB_YESNO) == IDNO) 
		return;

	CActionManager::Instance()->DeleteAction(ActionCombo->GetText());
	FillActionCombo();


}

void CDialogActionManagment::OnNewButtonClick()
{
	CActionManager::Instance()->StartRecording(ActionCombo->GetText());
}

void CDialogActionManagment::OnPlayButtonClick()
{
	String s;	
	s = ActionCombo->GetSelectionText();
	CActionManager::Instance()->PlayAction(s);
}

void CDialogActionManagment::OnStopRecButtonClick()
{
	if (!CActionManager::Instance()->Recording()) return;
	CActionManager::Instance()->StopRecording();
}

void CDialogActionManagment::OnClick(DWORD ID)
{
	if (ID == NewButton->GetID()) OnNewButtonClick();
	if (ID == DeleteButton->GetID()) OnDeleteButtonClick();
	if (ID == PlayButton->GetID()) OnPlayButtonClick();
	if (ID == StopRecButton->GetID()) OnStopRecButtonClick();
	
	
}

