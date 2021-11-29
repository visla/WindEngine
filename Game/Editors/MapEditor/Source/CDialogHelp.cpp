#include <CDialogHelp.h>
#include <CEditorResourceManager.h>


//========================================================
//    HelpDialog Class functions
//=======================================================

CDialogHelp* DialogHelp = NULL;

CDialogHelp::CDialogHelp()
{
}



bool CDialogHelp::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_HELP, hParent, Shown))
	{
		return false;
	}

	OkButton = new CButton(this, IDC_OK);
	HelpText = new CEditBox(this, IDC_HELPTEXT);

	FILE *f = fopen("Help.txt","rb");

	if(f)
	{
		String tmpText = "";
		char tmpChar[2];
		int ch;
		while(1)
		{
			ch = fgetc(f);
			if(feof(f)) break;
			tmpChar[0] = (char)ch;
			tmpChar[1] = '\0';
			tmpText = tmpText + tmpChar;
		}
		HelpText->SetText(tmpText);
		fclose(f);
	}else{
		HelpText->SetText("NO HELP FILE!!!");
	}

	return true;
}




void CDialogHelp::OnClick(DWORD ID)
{
	if(ID == OkButton->GetID()) OnOK();
}




void CDialogHelp::OnOK()
{
	SetVisible(false);
}

