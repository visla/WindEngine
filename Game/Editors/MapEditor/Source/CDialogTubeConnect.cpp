/*
    CDialogTubeConnect.cpp

	Autor: Nebojsa Vislavski
	Date: 22.12.2005.
	Version: 1.0

    
*/

#include <CDialogTubeConnect.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>


CDialogTubeConnect *CDialogTubeConnect::_instance = NULL;
CDialogTubeConnect *CDialogTubeConnect::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTubeConnect();
	return _instance;
}

bool CDialogTubeConnect::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_TUBEADDCHOOSEDIALOG, hParent))
	{
		return false;
	}
	return true;
}


void CDialogTubeConnect::OnShow()
{
	OkButton = new CButton(this, IDC_OK);
	ConnectCombo = new CComboBox(this, IDC_TUNNELCONNECTCOMBO);

	// Fill combo box with values
	if (!Sinfo) return;

	ConnectCombo->Clear();
	for (int i = 0; i < Sinfo->NumPivots; i++)
	{
		ConnectCombo->AddItem(Sinfo->PivotNames[i], i);
	}
	ConnectCombo->SetSelection(0);
}





void CDialogTubeConnect::OnClick(DWORD ID)
{
	if (ID == OkButton->GetID()) 
	{
		PivotIndex = ConnectCombo->GetSelectionIndex();
		SetModalResult(MR_OK);
	}
}

