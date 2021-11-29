

#include <CDialogPhysicModelEditor.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePhysicModelManager.h>


CDialogPhysicModelEditor *CDialogPhysicModelEditor::_instance = NULL;
CDialogPhysicModelEditor *CDialogPhysicModelEditor::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogPhysicModelEditor();
	return _instance;
}

bool CDialogPhysicModelEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_PHYSIC_EDITOR, hParent, Show))
	{
		return false;
	}

	CanChange = false;

	Name = new CEditBox(this, IDC_PHYSICMODELNAME);
	OnlyOnHardware = new CCheckBox(this, IDC_ONLYHARDWARE);
	SleepOnStart = new CCheckBox(this, IDC_SLEEPONSTART);
	DescList = new CListBox(this, IDC_ACTORDESC_LIST);
	AssingModelName = new CButton(this, IDC_ASSIGN_MODELNAME);
	AssingModelNameAll = new CButton(this, IDC_ASSIGN_MODELNAME_ALL);
	GroupEdit = new CEditBox(this, IDC_GROUPEDIT);
    
	//PModel = NULL;


	return true;
}


void CDialogPhysicModelEditor::UpdateComponents()
{
	//if (!PModel) return; 
	//Name->SetText(PModel->Name);
	//OnlyOnHardware->SetChecked(PModel->OnlyHardware);
	//SleepOnStart->SetChecked(PModel->SleepOnStart);
	//GroupEdit->SetText(PModel->Group);
 //   
	//FillListView();
}

void CDialogPhysicModelEditor::FillListView()
{
	//DescList->Clear();

	//for (int i = 0; i < PModel->GetActorsCount(); i++)
	//{
	//	char tmp[100];
	//	sprintf(tmp, "Actor %d [%s]", i, PModel->ModelNames[i].GetString());
	//	DescList->AddItem(tmp);
	//}
}



void CDialogPhysicModelEditor::OnClick(DWORD ID)
{
	//if (ID == ID_FILE_OPENFROMSSB)
	//{
	//	if (GetFile("Physic model (from MAX)\0*.ssb\0\0",""))
	//	{
	//		PModel = WePhysicModelManager::Instance()->Load(FileName);
	//		UpdateComponents();
	//	}
	//}
	//if (ID == ID_FILE_OPENFROMPACKAGE)
	//{
	//	InsertDialog->Type = PACKAGEITEM_PHYSICMODEL;
	//	if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
	//	{
	//		PModel = WePhysicModelManager::Instance()->Load(InsertDialog->HoleName, true);
	//		UpdateComponents();
	//	}

	//}

	//if (ID == ID_SAVE_TO_PACKAGE)
	//{
	//	bool Res = false;
	//	while(!Res)
	//	{
	//		SaveDialog->Type = PACKAGEITEM_PHYSICMODEL;
	//		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
	//		{
	//			if(PModel->Save(SaveDialog->HoleName))
	//			{
	//				Res = true;   // Save is OK!!!
	//			}else{
	//				if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
	//				{
	//					PModel->Save(SaveDialog->HoleName, true);
	//					Res = true;		// Save is OK!!!
	//				}else Res = false;	// Don't wan't to replase so enter new name
	//			}
	//		}else Res = true;  //  Cancel was pressed
	//	}
	//}

	//if (ID == AssingModelName->GetID())
	//{
	//	if (PModel)
	//	{
	//		InsertDialog->Type = PACKAGEITEM_MODEL;
	//		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
	//		{
	//			int index;
	//			index = DescList->GetSelectionIndex();
	//            
	//			PModel->ModelNames[index] = InsertDialog->HoleName;
	//			UpdateComponents();
	//		}
	//	}
	//}

	//if (ID == AssingModelNameAll->GetID())
	//{
	//	if (PModel)
	//	{
	//		InsertDialog->Type = PACKAGEITEM_MODEL;
	//		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
	//		{
	//			for (int i = 0; i < PModel->GetActorsCount(); i++)
	//			{
	//				PModel->ModelNames[i] = InsertDialog->HoleName;
	//			}            
	//			
	//			UpdateComponents();
	//		}
	//	}
	//}



	//if (ID == SleepOnStart->GetID()) 
	//{
	//	if (PModel) PModel->SleepOnStart = SleepOnStart->GetChecked();
	//}
	//if (ID == OnlyOnHardware->GetID()) 
	//{
	//	if (PModel) PModel->OnlyHardware = OnlyOnHardware->GetChecked();
	//}

	//if (ID == ID_FILE_EXIT) Hide();
}

void CDialogPhysicModelEditor::OnChange(DWORD ID)
{
	//if (ID == GroupEdit->GetID())
	//{
	//	if (PModel) PModel->Group = GroupEdit->GetTextAsInt();
	//}

}