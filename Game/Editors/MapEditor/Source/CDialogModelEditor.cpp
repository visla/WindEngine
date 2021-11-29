#include <CDialogModelEditor.h>
#include <CEditorResourceManager.h>
#include <CDialog.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>
#include <CDialogMeshEditor.h>
#include <CMapEditorWindow.h>
#include <WeStaticModel.h>
#include <WeMeshManager.h>


//========================================================
//    ModelEditor Class functions
//========================================================

CDialogModelEditor* ModelEditor = NULL;

bool CDialogModelEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_MODEL_EDITOR, hParent, Show))
	{
		return false;
	}

	SetSize(335, GetHeight());
	AllVisible = false;
	WireFrame = false;

	MeshName = new CEditBox(this, ID_MODEDIT_MESHNAME);
	QueryMeshName = new CEditBox(this, ID_MODEDIT_QUERYMESHNAME);
	PreviewGroup = new CGroupBox(this, ID_MOEDIT_MESHPREVIEW);
	MoreButton = new CButton(this, ID_MOEDIT_MOREBUTTON);

	MeshPreviewCombo = new CComboBox(this, ID_MOEDIT_MESHPREVIEWCOMBO);
	MeshPreviewCombo->AddItem("Mesh");
	MeshPreviewCombo->AddItem("QueryMesh");
	MeshPreviewCombo->SetSelection(0);

	Mesh = new WeMesh();

	return true;
}



void CDialogModelEditor::OnClick(DWORD ID)
{
	switch(ID)
	{
	case ID_MOEDIT_FILE_EXIT:
		{
			MeshName->SetText("");
			QueryMeshName->SetText("");
			SetVisible(false);
		}
		break;

	case ID_MOEDIT_FILE_SAVE:
		{
			bool Res = false;
			while(!Res)
			{
				SaveDialog->Type = PACKAGEITEM_MODEL;
				if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
				{				
					WeStaticModel *model;
					model = new WeStaticModel;
					model->MeshName = MeshName->GetText();
					model->QueryMeshName = QueryMeshName->GetText();
					//model->ShadowMeshName = MeshName->GetText();
					if (!model->Save(SaveDialog->HoleName, false))
					{
						if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save", MB_YESNO | MB_ICONQUESTION))
						{
							model->Save(SaveDialog->HoleName, true);
							Res = true;   // Save is OK!!!
						}
					}
					Res = true;
					delete model;
				}else Res = true;  //  Cancel was pressed
			}
		}
		break;

	case ID_MOEDIT_FILE_OPEN:
		{

			InsertDialog->Type = PACKAGEITEM_MODEL;
			if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
			{
				WeStaticModel *model = new WeStaticModel;
				model->Load(InsertDialog->HoleName, false);
				MeshName->SetText(model->MeshName);
				QueryMeshName->SetText(model->QueryMeshName);
			//	if(WePackage::Instance()->BeginReading(InsertDialog->HoleName, 6))
			//	{
			//		MeshName->SetText(WePackage::Instance()->DataStream.ReadString());
			//		QueryMeshName->SetText(WePackage::Instance()->DataStream.ReadString());
			//	}
			//	WePackage::Instance()->EndReading();
			}
		}
		break;

	case ID_MOEDIT_MESHVIEW_WIREFRAME:
		{
			if(WireFrame) 
				WireFrame = false;
			else
				WireFrame = true;
		}
		break;

	case ID_MODEDIT_EDITMESHNAME:
		{
			InsertDialog->Type = PACKAGEITEM_MESH;
			if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
			{
				MeshName->SetText(InsertDialog->HoleName);
				QueryMeshName->SetText(InsertDialog->HoleName);
			}
		}
		break;

	case ID_MODEDIT_EDITQUERYMESHNAME:
		{
			InsertDialog->Type = PACKAGEITEM_MESH;
			if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
			{
				QueryMeshName->SetText(InsertDialog->HoleName);
			}
		}
		break;

	case ID_MOEDIT_MOREBUTTON:
		{
			if(GetWidth() > 600)
			{ 
				SetSize(335, GetHeight());
				MoreButton->SetText(">>");
				AllVisible = false;
			}else{
				SetSize(660, GetHeight());
				MoreButton->SetText("<<");
				AllVisible = true;
			}
		}
		break;

	case ID_MOEDIT_REFRESHBUTTON:
		{
			if(MeshPreviewCombo->GetSelectionIndex() == 0)
			{
				Mesh = WeMeshManager::Instance()->Load(MeshName->GetText(),false);
			}else{
				Mesh = WeMeshManager::Instance()->Load(QueryMeshName->GetText(),false);
			}
		}
		break;
	}
}


void CDialogModelEditor::OnShow()
{
	CTreeViewItem* Item;
	Item = MeshEditor->TreeMeshes->GetSelected();
	if (Item && (Item->GetLevel() == 2))
	{
		String PackageName, GroupName, Name, FullName;
		PackageName = Item->GetParent()->GetParent()->GetText();
		GroupName = Item->GetParent()->GetText();
		Name = Item->GetText();
		FullName = PackageName + "." + GroupName + "." + Name;
		MeshName->SetText(FullName);
		QueryMeshName->SetText(FullName);
	}
}
