#include <CDialogSave.h>
#include <CDialogTools.h>
#include <WePackage.h>


//========================================================
//    SaveDialog Class functions
//========================================================

CDialogSave* SaveDialog = NULL;

CDialogSave::CDialogSave()
{
	HoleName = "";
}


bool CDialogSave::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_SAVE_DIALOG, hParent, Show))
	{
		return false;
	}

	Extension = "*.wep";

	return true;
}



void CDialogSave::OnShow()
{
	PackageTree = new CTreeView(this, ID_SAVE_TREEVIEW);
	Save = new CButton(this, ID_SAVE_SAVEBUTTON);
	Cancel = new CButton(this, IDC_CLOSE);
	Delete = new CButton(this, IDC_DELETE);
	PackageName = new CComboBox(this, IDC_PACKAGENAME);
	GroupName = new CComboBox(this, IDC_GROUPNAME);
	Name = new CComboBox(this, IDC_NAME);

	if(Extension.EqualCaseInsensitive((String)"*.wep"))
		FillTree(PackageTree, Type, true, PackageName);
	else
		FillTree(PackageTree, Type, true, PackageName, Extension);

	String aPackageName, aGroupName, aName;

	aPackageName = HoleName.Extract('.', 0);
	if (aPackageName.GetLen() != 0)
		PackageName->SetText(aPackageName);

	aGroupName = HoleName.Extract('.', 1);
	if (aGroupName.GetLen() != 0)
		GroupName->SetText(aGroupName);

	aName = HoleName.Extract('.', 2);
	if (aName.GetLen() != 0)
		Name->SetText(aName);

	CTreeViewItem *Item;

	Item = FindItem(PackageTree, HoleName);
	if (!Item)
	{
		Item = FindItem(PackageTree, aPackageName + "." + aGroupName);
		if (!Item)
			Item = FindItem(PackageTree, aPackageName);
	}

	if (Item)
		PackageTree->SelectItem(Item);
}



void CDialogSave::OnHide()
{
	delete PackageTree;
	delete Save;
	delete Cancel;
	delete Delete;
	delete PackageName;
	delete GroupName;
	delete Name;
}



void CDialogSave::OnClick(DWORD ID)
{
	if(ID == Save->GetID())
	{
		SaveButtonClick();
		return;
	}

	else if(ID == Cancel->GetID())
	{
		CancelButtonClick();
		return;
	}

	else if(ID == Delete->GetID())
	{
		DeleteButtonClick();
		return;
	}

	else if (ID == PackageTree->GetID())
	{
		OnPackageTreeClick();
		return;
	}

	else if (ID == PackageName->GetID()) 
	{
		PackageName->SetSelection(PackageName->GetSelectionIndex());
		FillGroupNames(PackageName->GetText());
		return;
	}
	
	else if (ID == GroupName->GetID()) 
	{
		GroupName->SetSelection(GroupName->GetSelectionIndex());
		FillNames(GroupName->GetText());
		return;
	}

	else if (ID == Name->GetID())
	{
		Name->SetSelection(Name->GetSelectionIndex());
		return;
	}

}





void CDialogSave::OnPackageTreeClick()
{
//	MessageBox(0, "CLICK", 0, 0);
	if (!PackageTree->GetSelected()) return;

	if(PackageTree->GetSelected()->GetLevel() == 0)
	{
		PackageName->SetSelection(PackageTree->GetSelected()->GetText());
		//MessageBox(0, PackageTree->GetSelected()->GetText(), 0, 0);
		FillGroupNames(PackageTree->GetSelected()->GetText());
	}

	else if(PackageTree->GetSelected()->GetLevel() == 1)
	{
		PackageName->SetSelection(PackageTree->GetSelected()->GetParent()->GetText());
		FillGroupNames(PackageTree->GetSelected()->GetParent()->GetText());

		GroupName->SetSelection(PackageTree->GetSelected()->GetText());
		//MessageBox(0, PackageTree->GetSelected()->GetText(), 0, 0);
		FillNames(PackageTree->GetSelected()->GetText());
	}

	else if(PackageTree->GetSelected()->GetLevel() == 2)
	{
		PackageName->SetSelection(PackageTree->GetSelected()->GetParent()->GetParent()->GetText());
		//MessageBox(0, PackageTree->GetSelected()->GetText(), 0, 0);
		FillGroupNames(PackageTree->GetSelected()->GetParent()->GetParent()->GetText());

		GroupName->SetSelection(PackageTree->GetSelected()->GetParent()->GetText());
		FillNames(PackageTree->GetSelected()->GetParent()->GetText());

		Name->SetSelection(PackageTree->GetSelected()->GetText());
	}
}




void CDialogSave::FillGroupNames(StringParam Package)
{
	String SavedText = GroupName->GetText();
	GroupName->Clear();

	Strings* GroupNames;
	GroupNames = WePackage::Instance()->GetGroupNames(Package, Type);

	for(GroupNames->First(); !GroupNames->End(); GroupNames->Next())
	{
		GroupName->AddItem(GroupNames->GetItem());
	}
	//GroupName->SetSelection(0);
	GroupName->SetText(SavedText);
}


void CDialogSave::FillNames(StringParam Group)
{
	String SavedText = Name->GetText();
	Name->Clear();

	Strings* NameList;
	NameList = WePackage::Instance()->GetItemsNames(PackageName->GetSelectionText(), Group, Type);
	for(NameList->First(); !NameList->End(); NameList->Next())
	{
		Name->AddItem(NameList->GetItem());
	}
	//Name->SetSelection(0);
	Name->SetText(SavedText);
}


void CDialogSave::SaveButtonClick()
{
    if(PackageName->GetText() == (String)"") { MessageBox(GetHWND(), "Packagage text is empty!", "Save", MB_OK | MB_ICONERROR); return; }
	if(GroupName->GetText() == (String)"") { MessageBox(GetHWND(), "Group text is empty!", "Save", MB_OK | MB_ICONERROR); return; }
	if(Name->GetText() == (String)"") { MessageBox(GetHWND(), "Name text is empty!", "Save", MB_OK | MB_ICONERROR); return; }

	String NewPackage;
	String NewGroup;
	String NewName;

	NewPackage = PackageName->GetText();
	NewGroup = GroupName->GetText();
	NewName = Name->GetText();

	HoleName = NewPackage + "." + NewGroup + "." + NewName;
	Extension = "*.wep";

	SetModalResult(MR_OK);
}




void CDialogSave::CancelButtonClick()
{
	Extension = "*.wep";
	SetModalResult(MR_CANCEL);
}



void CDialogSave::DeleteButtonClick()
{
	if (!PackageTree->GetSelected() || (PackageTree->GetSelected()->GetLevel() == 1))
	{
		MessageBox(NULL,"You need to select a name or Package!","Warning",MB_OK | MB_ICONWARNING);
		return;
	}

	if(PackageTree->GetSelected()->GetLevel() == 0)
	{
		String DeleteFileName = PackageTree->GetSelected()->GetText() + Extension;
		if(IDYES == MessageBox(GetHWND(), DeleteFileName, "Delete", MB_YESNO | MB_ICONQUESTION))
		{
			DeleteFile(DeleteFileName);
			FillTree(PackageTree, Type, true, PackageName, Extension);
		}
		return;
	}

	String newPackageName = PackageTree->GetSelected()->GetParent()->GetParent()->GetText();
	String newGroupName = PackageTree->GetSelected()->GetParent()->GetText();
	String newName = PackageTree->GetSelected()->GetText();
	String DeleteName = newPackageName + "." + newGroupName + "." + newName;

	String tmp = "Delete: ";
	tmp = tmp + DeleteName;
	if(IDYES == MessageBox(GetHWND(), tmp, "Delete", MB_YESNO | MB_ICONQUESTION))
	{
		WePackage::Instance()->Remove(DeleteName, Type);
		FillTree(PackageTree, Type, true, PackageName, Extension);
	}
}
