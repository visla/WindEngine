

#include <CDialogAIProfile.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <CMap.h>
#include <CMapHeader.h>
#include <CTuberDescriptionManager.h>
#include <CWeaponDescriptionManager.h>
#include <CDialogInsert.h>


CDialogAIProfile *CDialogAIProfile::_instance = NULL;
CDialogAIProfile *CDialogAIProfile::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogAIProfile();
	return _instance;
}

bool CDialogAIProfile::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_AIPROFILEDIALOG, hParent, Show))
	{
		return false;
	}

	return true;
}


void CDialogAIProfile::Update(CProfile *Profile)
{
	NameEdit->SetText(Profile->Name);
	TeamNumber->SetSelection(Profile->TeamNumber);
	CashCategory->SetText(Profile->CashCategory);

	TuberName->SetSelection(Profile->TuberDescription.Name);
	ModelName->SetText(Profile->TuberDescription.Model);
	Speed->SetText(Profile->TuberDescription.MaxSpeed);
	Acceleration->SetText(Profile->TuberDescription.MaxAcceleration);
	Manouverability->SetText(Profile->TuberDescription.Manouverability);
	WeaponName->SetSelection(Profile->TuberDescription.WeaponName);
	MineName->SetSelection(Profile->TuberDescription.MineName);
	HP->SetText(Profile->TuberDescription.HP);
	WeaponUpgradeLevel->SetText(Profile->TuberDescription.WeaponUpgradeLevel);
	MineUpgradeLevel->SetText(Profile->TuberDescription.MineUpgradeLevel);
	TurboUpgradeLevel->SetText(Profile->TuberDescription.TurboUpgradeLevel);
	PartModelName->SetText(Profile->TuberDescription.DestructionModel[PartNum->GetSelectionData()].Model);
	PartCount->SetText(Profile->TuberDescription.DestructionModel[PartNum->GetSelectionData()].Quantity);
}


void CDialogAIProfile::OnShow()
{
	NameEdit = new CEditBox(this, IDC_PROFILENAMEEDIT);
	TeamNumber = new CComboBox(this, IDC_TEAMNUMBERCOMBO);
	CashCategory = new CEditBox(this, IDC_PROFILECASH);
	TuberName = new CComboBox(this, IDC_TUBERNAMECOMBO);
	ModelName = new CEditBox(this, IDC_MODELNAME);
	Speed = new CEditBox(this, IDC_MAXSPEED);
	Acceleration = new CEditBox(this, IDC_MAXACCELERATION);
	Manouverability = new CEditBox(this, IDC_MANOUVERABILITY);
	WeaponName = new CComboBox(this, IDC_WEAPONNAME);
	MineName = new CComboBox(this, IDC_MINENAME);
	HP = new CEditBox(this, IDC_HP);
	WeaponUpgradeLevel = new CEditBox(this, IDC_WEAPONUPGRADELEVEL);
	MineUpgradeLevel = new CEditBox(this, IDC_MINEUPGRADELEVEL);
	TurboUpgradeLevel = new CEditBox(this, IDC_TURBOUPGRADELEVEL);
	PartNum = new CComboBox(this, IDC_RNUM2);
	PartModelName = new CEditBox(this, IDC_PARTMODELNAME);
	PartCount = new CEditBox(this, IDC_PARTQNT);

	ModelBrowse = new CButton(this, IDC_MODELNAMEBROWSE);
	PartModelBrowse = new CButton(this, IDC_PARTMODELBROWSE);
	
	
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
   	

	TeamNumber->Clear();
	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		char tmp[10];
		sprintf(tmp, "%d", i);
		TeamNumber->AddItem(tmp, i);
	}
	TeamNumber->SetSelection(0);
	

	PartNum->Clear();
	for (int i = 0; i < 10; i++)
	{
		char tmp[10];
		sprintf(tmp, "%d", i);
		PartNum->AddItem(tmp, i);
	}
	PartNum->SetSelection(0);

	TuberName->Clear();
	for (CTuberDescriptionManager::Instance()->Descriptions.First();
		!CTuberDescriptionManager::Instance()->Descriptions.End();
		CTuberDescriptionManager::Instance()->Descriptions.Next())
	{
		TuberName->AddItem(CTuberDescriptionManager::Instance()->Descriptions.GetItem().Name);
	}
	TuberName->SetSelection(0);

	FillWeaponCombo();
    Update(Profile);  
}



void CDialogAIProfile::FillWeaponCombo()
{
	WeaponName->Clear();
	MineName->Clear();

	CWeaponDescriptionManager::Instance->ReloadDescriptions();
	for (CWeaponDescriptionManager::Instance->Descriptions.First(); !CWeaponDescriptionManager::Instance->Descriptions.End(); CWeaponDescriptionManager::Instance->Descriptions.Next())
	{
		WeaponName->AddItem(CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Name, CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Type);        
		MineName->AddItem(CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Name, CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Type);        
	}
	WeaponName->SetSelection(0);
	MineName->SetSelection(0);

}

void CDialogAIProfile::OnOKClick()
{
	Profile->Name = NameEdit->GetText();
	Profile->TeamNumber = TeamNumber->GetSelectionIndex();
	Profile->CashCategory = CashCategory->GetTextAsInt();

	Profile->TuberDescription.Name = TuberName->GetText();
	Profile->TuberDescription.Model = ModelName->GetText();
	Profile->TuberDescription.MaxSpeed = Speed->GetTextAsFloat();
	Profile->TuberDescription.MaxAcceleration = Acceleration->GetTextAsFloat();
	Profile->TuberDescription.Manouverability = Manouverability->GetTextAsFloat();
	Profile->TuberDescription.WeaponName = WeaponName->GetText();
	Profile->TuberDescription.MineName = MineName->GetText();
	Profile->TuberDescription.HP = HP->GetTextAsInt();
	Profile->TuberDescription.WeaponUpgradeLevel = WeaponUpgradeLevel->GetTextAsInt();
	Profile->TuberDescription.MineUpgradeLevel = MineUpgradeLevel->GetTextAsInt();
	Profile->TuberDescription.TurboUpgradeLevel = TurboUpgradeLevel->GetTextAsInt();

	SetModalResult(MR_OK);
}

void CDialogAIProfile::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 
	{
		OnOKClick();	
	}

	else if (ID == CancelButton->GetID())
	{
		SetModalResult(MR_CANCEL);
	}
	
	else if (ID == ModelBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{			
			ModelName->SetText(InsertDialog->HoleName);
		}
	}

	else if (ID == TuberName->GetID())
	{
		UpdateTuberComponents();
	}

	else if (ID == PartModelBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			Profile->TuberDescription.DestructionModel[PartNum->GetSelectionData()].Model = InsertDialog->HoleName;
			PartModelName->SetText(InsertDialog->HoleName);
		}				
	}
}


void CDialogAIProfile::UpdateTuberComponents()
{
	// find tuber.
	CTuberDescription td;
	for (CTuberDescriptionManager::Instance()->Descriptions.First();
		!CTuberDescriptionManager::Instance()->Descriptions.End();
		CTuberDescriptionManager::Instance()->Descriptions.Next())
	{
		if (CTuberDescriptionManager::Instance()->Descriptions.GetItem().Name == TuberName->GetText())
		{
			td = CTuberDescriptionManager::Instance()->Descriptions.GetItem();
			Profile->TuberDescription = td;
			ModelName->SetText(td.Model);
			Speed->SetText(td.MaxSpeed);
			Acceleration->SetText(td.MaxAcceleration);
			Manouverability->SetText(td.Manouverability);
			WeaponName->SetSelection(td.WeaponName);
			MineName->SetSelection(td.MineName);
			HP->SetText(td.HP);
			PartNum->SetSelection(0);
		}
	}
}