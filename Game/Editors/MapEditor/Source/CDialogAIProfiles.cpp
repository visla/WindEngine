

#include <CDialogAIProfiles.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <CMap.h>
#include <CMapHeader.h>
#include <CTuberDescriptionManager.h>
#include <CAIProfileManager.h>
#include <CDialogAIProfile.h>


CDialogAIProfiles *CDialogAIProfiles::_instance = NULL;
CDialogAIProfiles *CDialogAIProfiles::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogAIProfiles();
	return _instance;
}

bool CDialogAIProfiles::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_AIPROFILES, hParent, Show))
	{
		return false;
	}

	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	AddAIProfile = new CButton(this, IDC_ADDAIBUTTON);
	RemoveAIProfile = new CButton(this, IDC_REMOVEAIBUTTON);
	EditAIProfile = new CButton(this, IDC_EDITAIBUTTON);

	AIList = new CListBox(this, IDC_AILIST);

	FillAIList();

	return true;
}


void CDialogAIProfiles::FillAIList()
{
	AIList->Clear();

	CAIProfileManager::Instance->LoadProfiles();
	for (CAIProfileManager::Instance->Profiles.First(); !CAIProfileManager::Instance->Profiles.End(); CAIProfileManager::Instance->Profiles.Next())
	{
		AIList->AddItem(CAIProfileManager::Instance->Profiles.GetItem()->Name);
	}
}

void CDialogAIProfiles::OnShow()
{
	
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
   	

}



void CDialogAIProfiles::OnOKClick()
{
}

void CDialogAIProfiles::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 
	{
		Hide();
	}
	if (ID == CancelButton->GetID())
	{
		Hide();
	}

	if (ID == AddAIProfile->GetID())
	{
		CProfile *AIProfile;
		AIProfile = new CProfile;
		AIProfile->AIProfile = true;
		CDialogAIProfile::Instance()->Profile = AIProfile;
		if (CDialogAIProfile::Instance()->ShowModal(GetHWND()) == MR_OK)
		{
			if (CAIProfileManager::Instance->AddProfile(CDialogAIProfile::Instance()->Profile))
				AIList->AddItem(CDialogAIProfile::Instance()->Profile->Name);			
		} else
		{
			delete AIProfile;
		}
	}


	if (ID == RemoveAIProfile->GetID())
	{
		if (CAIProfileManager::Instance->RemoveProfile(AIList->GetSelectionText()))		
			AIList->RemoveItem(AIList->GetSelectionIndex());
	}

	if (ID == EditAIProfile->GetID())
	{
		CDialogAIProfile::Instance()->Profile = CAIProfileManager::Instance->GetProfile(AIList->GetSelectionText());
		if (CDialogAIProfile::Instance()->Profile)
		{
			if (CDialogAIProfile::Instance()->ShowModal(GetHWND()) == MR_OK)
			{
				AIList->RemoveItem(AIList->GetSelectionIndex());
				AIList->AddItem(CDialogAIProfile::Instance()->Profile->Name);				
				CAIProfileManager::Instance->SaveProfiles();
			}
		}
	}


}


