/*
    CDialogTubersEditor.cpp

	Autor: Nebojsa Vislavski
	Date: 16.3.2006.
	Version: 1.0

    
*/


#include <CDialogTubersEditor.h>
#include <CTuberDescriptionManager.h>
#include <CTuberDescription.h>
#include <CEditorResourceManager.h>
#include <CMapEditorwindow.h>
#include <CWeaponDescriptionManager.h>
#include <CDialogInsert.h>



//========================================================
//    TubersEditor Class functions
//=======================================================

CDialogTubersEditor* CDialogTubersEditor::_instance = NULL;

CDialogTubersEditor* CDialogTubersEditor::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogTubersEditor;
	return _instance;
}


bool CDialogTubersEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TUBERS_EDITOR_DIALOG, hParent, Show))
	{
		return false;
	}

	ModelNameEdit = new CEditBox(this, IDC_MODELNAME);
	TuberNameEdit = new CEditBox(this, IDC_TUBERNAME);
	MaxSpeedEdit = new CEditBox(this, IDC_MAXSPEED);
	MaxAccelerationEdit = new CEditBox(this, IDC_MAXACCELERATION);
	ManouverabilityEdit =  new CEditBox(this, IDC_MANOUVERABILITY);
	WeaponName = new CComboBox(this, IDC_WEAPONNAME);
	MineName = new CComboBox(this, IDC_MINENAME);
	HP = new CEditBox(this, IDC_HP);
	Price = new CEditBox(this, IDC_PRICE);
	TournamentRequired = new CComboBox(this, IDC_TOURNAMENTREQUIRED);
	PartNum = new CComboBox(this, IDC_RNUM);
	PartModelName = new CEditBox(this, IDC_PARTMODELNAME);
	PartModelBrowseButton = new CButton(this, IDC_PARTMODELBROWSE);
	PartQnt = new CEditBox(this, IDC_PARTQNT);
	DescList = new CComboBox(this, IDC_DESCLIST);
	SaveButton = new CButton(this, IDC_SAVEBUTTON);
	RemoveButton = new CButton(this, IDC_REMOVEBUTTON);
	ModelBrowseButton = new CButton(this, IDC_MODELNAMEBROWSE);
	Description = new CEditBox(this, IDC_DESCRIPTION);
	PhysicModel = new CEditBox(this, IDC_PHYSICMODELNAME);
	PhysicModelButton = new CButton(this, IDC_PHYSICMODELNAMEBROWSE);


	char tmpNum[3];
	for(int i=0; i<10; i++)
	{
		sprintf(tmpNum,"%d",i+1);
		PartNum->AddItem(tmpNum, i);
	}

	FillWeaponCombo();
	PartNum->SetSelection(0);
	OnLoad();
	return true;
}


void CDialogTubersEditor::FillWeaponCombo()
{
	WeaponName->Clear();
	MineName->Clear();

	CWeaponDescriptionManager::Instance->ReloadDescriptions();
	for (CWeaponDescriptionManager::Instance->Descriptions.First(); !CWeaponDescriptionManager::Instance->Descriptions.End(); CWeaponDescriptionManager::Instance->Descriptions.Next())
	{
		WeaponName->AddItem(CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Name, CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Type);        
		MineName->AddItem(CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Name, CWeaponDescriptionManager::Instance->Descriptions.GetItem()->Type);        
	}

	MineName->SetSelection(0);
	WeaponName->SetSelection(0);
}



void CDialogTubersEditor::OnChange(DWORD ID)
{
	if (ID == PartQnt->GetID())
	{
		TuberDesc.DestructionModel[PartNum->GetSelectionData()].Quantity = PartQnt->GetTextAsInt();
	}
	if (ID == PartModelName->GetID())
	{
		TuberDesc.DestructionModel[PartNum->GetSelectionData()].Model = PartModelName->GetText();
	}
}

void CDialogTubersEditor::OnClick(DWORD ID)
{
	switch(ID)
	{
	case ID_TUBERSEDITOR_FILE_EXIT:
		SetVisible(false);
		break;

	case ID_TUBERSEDITOR_FILE_LOAD:
		OnLoad();
		break;

	case ID_TUBERSEDITOR_FILE_SAVE:		
		OnSave();
		break;

	case ID_TUBERSEDITOR_FILE_NEW:
		OnNew();
		break;
	}

	if (ID == PartNum->GetID())
	{
		ShowPartProps();
	}

	if (ID == ModelBrowseButton->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			TuberDesc.Model = InsertDialog->HoleName;
			ModelNameEdit->SetText(TuberDesc.Model);
		}
	}

	if (ID == PhysicModelButton->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_PHYSICMODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			TuberDesc.PhysicModel = InsertDialog->HoleName;
			PhysicModel->SetText(TuberDesc.PhysicModel);
		}
	}


	if (ID == PartModelBrowseButton->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			TuberDesc.DestructionModel[PartNum->GetSelectionData()].Model = InsertDialog->HoleName;
			PartModelName->SetText(InsertDialog->HoleName);
		}
		
		
	}

	if (ID == SaveButton->GetID())
	{
		OnSave();
	}
	if (ID == RemoveButton->GetID())
	{
		OnRemove();
	}
	if (ID == DescList->GetID())
	{
		DescList->SetSelection(DescList->GetSelectionIndex());
		UpdateDescription(DescList->GetText());
	}
}


void CDialogTubersEditor::UpdateDescription(StringParam Name)
{
	for (CTuberDescriptionManager::Instance()->Descriptions.First();
		!CTuberDescriptionManager::Instance()->Descriptions.End();
		CTuberDescriptionManager::Instance()->Descriptions.Next())
	{
		if (CTuberDescriptionManager::Instance()->Descriptions.GetItem().Name == (String)Name)
		{
			TuberDesc = CTuberDescriptionManager::Instance()->Descriptions.GetItem();
			ShowDesc();
		}
	}
	
}


void CDialogTubersEditor::ShowDesc()
{
	TournamentRequired->Clear();

	TuberNameEdit->SetText(TuberDesc.Name);
	ModelNameEdit->SetText(TuberDesc.Model);
	PhysicModel->SetText(TuberDesc.PhysicModel);
	MaxSpeedEdit->SetText(TuberDesc.MaxSpeed);
	MaxAccelerationEdit->SetText(TuberDesc.MaxAcceleration);
	ManouverabilityEdit->SetText(TuberDesc.Manouverability);	
	WeaponName->SetSelection(TuberDesc.WeaponName);
	MineName->SetSelection(TuberDesc.MineName);
	HP->SetText(TuberDesc.HP);
	Price->SetText(TuberDesc.Price);
	TournamentRequired->AddItem(TuberDesc.TournamentRequired);
	PartNum->SetSelection(0);
	Description->SetText(TuberDesc.Description);
	PartModelName->SetText(TuberDesc.DestructionModel[PartNum->GetSelectionData()].Model);
}

void CDialogTubersEditor::ShowPartProps()
{
	PartNum->SetSelection(PartNum->GetSelectionIndex());
	PartModelName->SetText(TuberDesc.DestructionModel[PartNum->GetSelectionData()].Model);
	PartQnt->SetText(TuberDesc.DestructionModel[PartNum->GetSelectionData()].Quantity);
}

void CDialogTubersEditor::OnRemove()
{
	CTuberDescriptionManager::Instance()->RemoveDescription(TuberDesc);
	FillTubersList();
}

void CDialogTubersEditor::OnSave()
{
	TuberDesc.Name = TuberNameEdit->GetText();
	TuberDesc.Model = ModelNameEdit->GetText();
	TuberDesc.MaxSpeed = MaxSpeedEdit->GetTextAsFloat();
	TuberDesc.MaxAcceleration = MaxAccelerationEdit->GetTextAsFloat();
	TuberDesc.Manouverability = ManouverabilityEdit->GetTextAsFloat();
	TuberDesc.WeaponName = WeaponName->GetText();
	TuberDesc.MineName = MineName->GetText();
	TuberDesc.HP = HP->GetTextAsInt();
	TuberDesc.Price = Price->GetTextAsInt();
	TuberDesc.Description = Description->GetText();	
	TuberDesc.TournamentRequired = TournamentRequired->GetText();
	TuberDesc.PhysicModel = PhysicModel->GetText();	

	CTuberDescriptionManager::Instance()->AddDescription(TuberDesc);
	CTuberDescriptionManager::Instance()->SaveDescriptions();
	FillTubersList();
}



void CDialogTubersEditor::OnLoad()
{
	CTuberDescriptionManager::Instance()->LoadDescriptions();
	FillTubersList();
}

void CDialogTubersEditor::FillTubersList()
{
	DescList->Clear();
	for (CTuberDescriptionManager::Instance()->Descriptions.First();
		!CTuberDescriptionManager::Instance()->Descriptions.End();
		CTuberDescriptionManager::Instance()->Descriptions.Next())
	{
		DescList->AddItem(CTuberDescriptionManager::Instance()->Descriptions.GetItem().Name);        
	}
}

void CDialogTubersEditor::OnNew()
{
}

