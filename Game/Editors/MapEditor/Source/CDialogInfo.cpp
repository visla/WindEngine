#include <CDialogInfo.h>
#include <CEditorResourceManager.h>
#include <WePackage.h>


//========================================================
//    InfoDialog Class functions
//========================================================

CDialogInfo* DialogInfo = NULL;

bool CDialogInfo::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_INFO, hParent, Shown))
	{
		return false;
	}

	return true;
}



void CDialogInfo::OnClick(DWORD ID)
{
	if(ID == IDC_CLOSE)
	{
		SetModalResult(MR_CANCEL);
	}
}


void CDialogInfo::OnShow()
{
	InfoText = new CEditBox(this, IDC_INFO);

	if(!OpenInfoFile()) SetModalResult(MR_CANCEL);
}



bool CDialogInfo::OpenInfoFile()
{

	if(FromPackage)
	{
		if(WePackage::Instance()->BeginReading(Name, 0))
		{
			String tmpText = WePackage::Instance()->DataStream.ReadString();
			InfoText->SetText(tmpText);
			WePackage::Instance()->EndReading();
			return true;
		}else return false;
	}else{
		FILE* f;
		f = fopen(Name,"rb");
		
		if(!f) return false;

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
		fclose(f);

		InfoText->SetText(tmpText);
		OutText = tmpText;
		return true;
	}
}
