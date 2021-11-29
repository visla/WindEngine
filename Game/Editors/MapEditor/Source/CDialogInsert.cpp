#include <CDialogInsert.h>
#include <CEditorResourceManager.h>
#include <CDialogInfo.h>
#include <CDialogTools.h>



//========================================================
//    InsertDialog Class functions
//========================================================

CDialogInsert* InsertDialog = NULL;

CDialogInsert::CDialogInsert()
{
	HoleName = "";
}





bool CDialogInsert::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_INSERT_DIALOG, hParent, Show))
	{
		return false;
	}

	Extension = "*.wep";

	return true;
}


void CDialogInsert::OnTreeViewClick()
{
}



void CDialogInsert::OnClick(DWORD ID)
{
	if(ID == InsertButton->GetID())
	{
		InsertButtonClick();
		return;
	}

	else if(ID == PackageTree->GetID())
	{
		OnTreeViewClick();
	}

	else if(ID == CancelButton->GetID())
	{
		CancelButtonClick();
		return;
	}
}



void CDialogInsert::InsertButtonClick()
{
	if (!PackageTree->GetSelected() || (PackageTree->GetSelected()->GetLevel() != 2))
	{
		MessageBox(GetHWND(), "You need to select a name!","Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	String PackageName = PackageTree->GetSelected()->GetParent()->GetParent()->GetText();
	String GroupName = PackageTree->GetSelected()->GetParent()->GetText();
	String Name = PackageTree->GetSelected()->GetText();
	
	HoleName = PackageName + "." + GroupName + "." + Name;
	Extension = "*.wep";

	SetModalResult(MR_OK);
}


void CDialogInsert::CancelButtonClick()
{
	Extension = "*.wep";
	SetModalResult(MR_CANCEL);
}



void CDialogInsert::OnShow()
{
	PackageTree = new CTreeView(this, ID_INSERT_TREEVIEW);
	InsertButton = new CButton(this, ID_INSERT_INSERTBUTTON);
	CancelButton = new CButton(this, ID_INSERT_CANCELBUTTON);

	if(Extension == (String)"*.wep")
	{
		FillTree(PackageTree, Type);
	}
	else
	{
		FillTree(PackageTree, Type, true, NULL, Extension);
	}

	CTreeViewItem *Item;
	Item = FindItem(PackageTree, HoleName);

	if (Item)
	{
		PackageTree->SelectItem(Item);
	}
}
