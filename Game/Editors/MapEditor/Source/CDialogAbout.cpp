#include <CDialogAbout.h>
#include <CEditorResourceManager.h>


//========================================================
//    AboutBox Class functions
//=======================================================


CDialogAbout* DialogAbout = NULL;


CDialogAbout::CDialogAbout()
{
}



bool CDialogAbout::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_ABOUT, hParent, Shown))
	{
		return false;
	}

	return true;
}


void CDialogAbout::OnShow()
{
	OkButton = new CButton(this, IDC_OK);
}


void CDialogAbout::OnClick(DWORD ID)
{
	if(ID == OkButton->GetID()) OnOK();
}


void CDialogAbout::OnOK()
{
	SetModalResult(MR_OK);
}

