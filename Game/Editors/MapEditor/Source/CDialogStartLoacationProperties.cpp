/*
		File: CDialogStartLoacationProperties.cpp
		Author: Tomislav Kukic
		Date: 05.04.2006
		Version: 1.0
*/



#include <CComponent.h>
#include <CEditorResourceManager.h>
#include <CDialogStartLoacationProperties.h>
#include <CStartLocation.h>
#include <Constants.h>


CDialogStartLoacationProperties* CDialogStartLoacationProperties::_instance = NULL; 

CDialogStartLoacationProperties* CDialogStartLoacationProperties::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogStartLoacationProperties();
	return _instance;
}


CDialogStartLoacationProperties::CDialogStartLoacationProperties()
{
	StartLocation = NULL;
}


CDialogStartLoacationProperties::~CDialogStartLoacationProperties()
{
}


bool CDialogStartLoacationProperties::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_STARTLOACATION,hParent,Shown))
	{
		return false;
	}
	
	TeamNum = new CComboBox(this,IDC_COMBOTEAMNUM);
	for(int i=0; i<MAX_PLAYERS; i++)
	{
		char tmp[100];
		sprintf(tmp,"%d",(i+1));
		TeamNum->AddItem(tmp);
	}

	return true;
}




void CDialogStartLoacationProperties::OnClick(DWORD ID)
{
	if(ID == IDC_COMBOTEAMNUM)
	{
		//StartLocation->TeamNumber = TeamNum->GetSelectionIndex()+1;
	}

	if(ID == IDC_BUTTONOK) { StartLocation->TeamNumber = TeamNum->GetSelectionIndex() + 1; Hide(); }
}




void CDialogStartLoacationProperties::ShowDlg(CStartLocation* _StartLocation)
{
	StartLocation = _StartLocation;
	SetVisible(true);
	TeamNum->SetSelection(_StartLocation->TeamNumber-1);
}



