/*
    CDialogSkyBox.cpp

	Autor: Nebojsa Vislavski
	Date: 3.3.2006.
	Version: 1.0
    

*/


#include <CDialogSkyBox.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <commdlg.h>
#include <CEditorMap.h>
#include <CDialogInsert.h>


CDialogSkyBox *CDialogSkyBox::_instance = NULL;
CDialogSkyBox *CDialogSkyBox::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogSkyBox();
	return _instance;
}

bool CDialogSkyBox::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_SKYBOXDIALOG, hParent, Show))
	{
		return false;
	}

	OKButton = new CButton(this, IDOK);
	TopMaterial = new CEditBox(this, IDC_TOPMATERIALEDIT);
	TopMaterialChoose = new CButton(this, IDC_TOPMATERIALCHOOSE);

	CanChange = false;

	return true;
}


void CDialogSkyBox::OnShow()
{
	Show(EditorWindow->MapEditorWorld->GetSkyBox());
}




void CDialogSkyBox::Show(WeSkyBox *skybox)
{
	if (!skybox) return;
	SkyBox = skybox;
	Update(skybox);
	CDialog::Show();
}

void CDialogSkyBox::Update(WeSkyBox *skybox)
{	
	CanChange = false;

	TopMaterial->SetText(skybox->SkyBoxMeshName);

	CanChange = true;
}


void CDialogSkyBox::OnChange(DWORD ID)
{
	if (!CanChange) return;

	if (TopMaterial->GetID() == ID)
	{
		EditorWindow->MapEditorWorld->GetSkyBox()->SkyBoxMeshName = TopMaterial->GetText();
		EditorWindow->MapEditorWorld->GetSkyBox()->Initialize();
	}
}

void CDialogSkyBox::OnClick(DWORD ID)
{
	if (OKButton->GetID() == ID)
	{
		Hide();
	}

	if (TopMaterialChoose->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_MESH;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			TopMaterial->SetText(InsertDialog->HoleName);
		}
	}
}

