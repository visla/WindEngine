/*
    CDialogInput.cpp

	Autor: Nebojsa Vislavski
	Date: 20.1.2006.
	Version: 1.0
    

*/


#include <CDialogInput.h>
#include <CEditorResourceManager.h>


CDialogInput *CDialogInput::_instance = NULL;
CDialogInput *CDialogInput::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogInput();
	return _instance;
}

bool CDialogInput::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_INPUTDIALOG, hParent))
	{
		return false;
	}
	return true;
}


void CDialogInput::OnShow()
{
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);

	EditBox = new CEditBox(this, IDC_INPUTEDIT);
	Static = new CLabel(this, IDC_INPUTSTATIC);

	EditBox->Clear();
	EditBox->SetFocused();
}





void CDialogInput::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 
	{
		InputText = EditBox->GetText();
		SetModalResult(MR_OK);
	}
	if (ID == CancelButton->GetID()) SetModalResult(MR_CANCEL);
}

