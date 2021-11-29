/*
		File: CDialogAnimatedMesh.cpp
		Authoe: Tomislav kukic
		Date: 11.07.2006
		Version: 1.0
*/





#include <CDialogAnimatedMesh.h>
#include <CEditorResourceManager.h>
#include <WeRenderDevice.h>
#include <WeWorld.h>
#include <CMapEditorWindow.h>
#include <WeModelManager.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>
#include <CDialogTools.h>




CDialogAnimatedMesh* CDialogAnimatedMesh::_instance = NULL;
CDialogAnimatedMesh* CDialogAnimatedMesh::Instance()
{
	if(_instance) return _instance;
	_instance = new CDialogAnimatedMesh();
	return _instance;
}





CDialogAnimatedMesh::CDialogAnimatedMesh()
{
	Model = NULL;
}





bool CDialogAnimatedMesh::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_ANIMATEDMESH, hParent, Shown))
	{
		return false;
	}

	FrameTree = new CTreeView(this, IDC_ANIMESHED_FRAMETREE);
	PreviewArea = new CButton(this, IDC_ANIMESHED_PREVIEWAREA);

	PreviewWorld = new WeWorld();
	PreviewCamera = new WeCamera();
	PreviewCamera->Location = WeVector(0.0,0.0,-150.0);
	Actor = new WeActor();
	//Actor->ModelName = "tmp.tmp.ProbaRotationModel";
	//Actor->Initialize();
	PreviewWorld->AddActor(Actor);

	MeshList = new CTreeView(this, IDC_ANIMESHED_ANIMESHLIST);
	MaterialPresetCombo = new CComboBox(this, IDC_ANIMESHED_MATERIALPRESET);
	MaterialList = new CTreeView(this, IDC_ANIMESHED_MATERIALLIST);
	AnimationsList = new CListBox(this, IDC_ANIMESHED_ANIMATIONLIST);
	AnimSpeed = new CEditBox(this, IDC_ANIMESHED_ANIMATIONSPEED);
	EditTime = new CEditBox(this, IDC_ANIMESHED_TIME);

	char tmp[100];
	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		sprintf(tmp, "%d", i);
		MaterialPresetCombo->AddItem(tmp, i);
	}
	MaterialPresetCombo->SetSelection(0);
	

	return true;
}





void CDialogAnimatedMesh::OnClick(DWORD ID)
{
	switch(ID)
	{
	case ID_FILE_EXIT:
		Hide();
		break;

	case ID_FILE_LOADANIMMESHFROMXFILE:
		if(GetFile("DirectX *.x files\0*.x\0\0","*.x"))
		{
			LoadMeshFromX(FileName);
		}
		break;

	case ID_FILE_SAVETOPACKAGE:
		{
			if(!Model) return; 
			bool Res = false;

			SaveDialog->Type = PACKAGEITEM_ANIMMODEL;
			if (SaveDialog->ShowModal(GetHWND()) == MR_OK)
			{
				if (Model->Save(SaveDialog->HoleName, false))
					Res = true;
				else
				{
					if (IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
					{
						Res = Model->Save(SaveDialog->HoleName, true);
					}
				}

				if (Res)
				{
					if (!FindItem(MeshList, SaveDialog->HoleName))
					{
						AddItem(MeshList, SaveDialog->HoleName);
					}
				}
			}
		}
		break;

	case ID_ANIMATION_NEXTANIMATION:
		Model->SetAnimation(Model->GetAnimation() + 1);
		break;

	case ID_ANIMATION_PREVANIMATION:
		Model->SetAnimation(Model->GetAnimation() - 1);
		break;

	case IDC_ANIMESHED_FRAMETREE:
		OnChooseMesh();
		break;

	case IDC_ANIMESHED_ANIMATIONLIST:
		Model->SetAnimation(AnimationsList->GetSelectionData());
		break;

	case IDC_ANIMESHED_CHANGEBUTTON:
		OnChangeButton();
		break;

	case ID_ANIMATION_SPEEDUP:
		Model->AnimationSpeed += 0.5f;
		break;

	case ID_ANIMATION_SPEEDDOWN:
		Model->AnimationSpeed -= 0.5;
		if(Model->AnimationSpeed < 0.0f)
			Model->AnimationSpeed = 0.0f;
		break;

	case IDC_ANIMESHED_ANIMESHLIST:
		CTreeViewItem* Item;
		Item = MeshList->GetSelected();

		if (Item && (Item->GetLevel() == 2))
		{
			String PackageName, GroupName, Name, FullName;
			PackageName = Item->GetParent()->GetParent()->GetText();
			GroupName = Item->GetParent()->GetText();
			Name = Item->GetText();
			FullName = PackageName + "." + GroupName + "." + Name;

			Actor->InEditor = true;
			Actor->ModelName = FullName;
			Actor->Initialize();
			Model = (WeModel*)Actor->Model;
			FrameTree->Clear();
			AnimationsList->Clear();
			FillFrames(Model->RootFrame, NULL);
			FillAnimations();
			SAFE_DELETE(Actor->PhysicHandle);
			AnimSpeed->SetText(Model->AnimationSpeed);

			AnimationsList->SetSelection(0);
		}
		break;

	case IDC_ANIMESHED_MATERIALPRESET:
		Actor->MaterialPreset = MaterialPresetCombo->GetSelectionData();
		OnChooseMesh();
		break;

	}
}



void CDialogAnimatedMesh::OnShow()
{
	FillTree(MeshList, PACKAGEITEM_ANIMMODEL);
}


void CDialogAnimatedMesh::OnChange(DWORD ID)
{
	if(ID == AnimSpeed->GetID())
	{
		Model->AnimationSpeed = AnimSpeed->GetTextAsFloat();
	}
}





void CDialogAnimatedMesh::OnChooseMesh()
{
	MaterialList->Clear();
	if(FrameTree->GetSelected())
	{
		DWORD Data;
		WeMesh* mesh;

		Data = FrameTree->GetSelected()->GetData();
		mesh = *((WeMesh**)&Data);

		if (mesh)
		{
			FillMaterialTree(mesh, Actor->MaterialPreset);
		}
	}
}








WeMesh* CDialogAnimatedMesh::FindMesh(WeModelFrame* pFrame, StringParam Name)
{
	if(pFrame->pMeshContainer)
	{
		if(((WeMeshContainer*)(pFrame->pMeshContainer))->Mesh->Name == Name)
		{
			return ((WeMeshContainer*)(pFrame->pMeshContainer))->Mesh;
		}
	}

	WeMesh* mesh = NULL;

	// Recurse for sibblings
	if (pFrame->pFrameSibling != NULL)
	{
		mesh = FindMesh((WeModelFrame*)pFrame->pFrameSibling, Name);
		if (mesh)
		{
			return mesh;
		}
	}

    // Recurse for children
	if (pFrame->pFrameFirstChild != NULL)
	{
        mesh = FindMesh((WeModelFrame*)pFrame->pFrameFirstChild, Name);
		if (mesh)
		{
			return mesh;
		}
	}

	return NULL;
}








void CDialogAnimatedMesh::FillMaterialTree(WeMesh *mesh, int MaterialPreset)
{
	//RootName = RootName_;
	//CTreeViewItem* Root = NULL;
	MaterialList->Clear();
	//Root = MaterialList->Add(NULL, "Root");
	

	int Count = 1;
	for(mesh->MaterialNames[MaterialPreset].First(), mesh->MeshAttributes.First(); !mesh->MeshAttributes.End(); mesh->MeshAttributes.Next(), mesh->MaterialNames[MaterialPreset].Next() )
	{
		CTreeViewItem* MaterialItem;
		//CTreeViewItem* EffectItem;
		//CTreeViewItem* TextureItem;

		char tmpMat[100];
		String tmps;

		sprintf(tmpMat,"Attribute %d: ",Count);
		tmps = tmpMat;
		if (!mesh->MaterialNames[MaterialPreset].End())
		{
			tmps = tmps + mesh->MaterialNames[MaterialPreset].GetItem();
		} else
		{
			tmps = tmps  + "not set";
		}
		MaterialItem = MaterialList->Add(NULL, tmps, Count);

		//EffectItem = MaterialList->Add(MaterialItem, "Effects");
		//TextureItem = MaterialList->Add(MaterialItem, "Textures");
		//if(!cMeshEditor->Materials[MaterialPreset].End())
		//	MaterialList->Add(EffectItem, cMeshEditor->Materials[MaterialPreset].GetItem()->Shader->Name);


		//if(!cMeshEditor->Materials[MaterialPreset].End())
		//{
		//	for(int i=0; i<16; i++)
		//	{
		//		if(cMeshEditor->Materials[MaterialPreset].GetItem()->Textures[i])
		//			MaterialList->Add(TextureItem, cMeshEditor->Materials[MaterialPreset].GetItem()->Textures[i]->Name);
		//		else
		//			MaterialList->Add(TextureItem, "No Texture");
		//	}
		//}
		Count++;
	}
}





void CDialogAnimatedMesh::OnChangeButton()
{
	if (!MaterialList->GetSelected())
	{
		MessageBox(0, "You need to select attribute of mesh", "Animated Mesh", 0);
		return;
	}

	if (FrameTree->GetSelected())
	{
		DWORD Data;
		WeMesh* mesh;

		Data = FrameTree->GetSelected()->GetData();
		mesh = *((WeMesh**)&Data);

		if (mesh)
		{
			int tmpNum, preset;
			//sscanf(MaterialList->GetSelected()->GetText().Extract('_',1).GetString(),"%d", &tmpNum);
			tmpNum = MaterialList->GetSelected()->GetData();
			preset = MaterialPresetCombo->GetSelectionData();

			InsertDialog->Type = PACKAGEITEM_MATERIAL;
			if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
			{
				mesh->MaterialNames[preset].First();
				for(int i=0; i<tmpNum-1; i++)
				{
					mesh->MaterialNames[preset].Next();
				}

				mesh->MaterialNames[preset].GetItem() = InsertDialog->HoleName;
				mesh->Refresh();

				//cMeshEditor = (WeMeshEditor*)WeMeshManager::Instance()->Load(cMeshEditor->Name,false,false,MaterialPresetCombo->GetSelectionData());		
				FillMaterialTree(mesh, preset);
			}
		}
	}
}






void CDialogAnimatedMesh::FillFrames(WeModelFrame* pFrame, CTreeViewItem* Parent)
{
	CTreeViewItem* Item = NULL;
	String ItemName;
	WeMesh* Data = NULL;

	if(!pFrame) return;

	ItemName = pFrame->Name;
	if (pFrame->pMeshContainer)
	{
		ItemName += (String)"*";
		Data = ((WeMeshContainer*)pFrame->pMeshContainer)->Mesh;
	}

	Item = FrameTree->Add(Parent, ItemName, *((DWORD*)&Data));

	//if(!pFrame->pMeshContainer)
	//	tmpItem = FrameTree->Add(Parent, pFrame->Name);
	//else
	//{
	//	char tmpC[100];
	//	sprintf(tmpC, "%s_%d", pFrame->pMeshContainer->Name, Index);
	//	tmpItem = FrameTree->Add(Parent, tmpC);
	//	((WeMeshContainer*)(pFrame->pMeshContainer))->Mesh->Name = tmpC;
	//	FramesList->AddItem(tmpC);
	//	Index++;
	//}
	
		
	// Recurse for sibblings
	if (pFrame->pFrameSibling != NULL)
		FillFrames((WeModelFrame*)pFrame->pFrameSibling, Parent);

    // Recurse for children
	if (pFrame->pFrameFirstChild != NULL)
        FillFrames((WeModelFrame*)pFrame->pFrameFirstChild, Item);
}





void CDialogAnimatedMesh::FillAnimations()
{
	Strings* Names = Model->GetAnimationNames();
	int i=0;
	for(Names->First(); !Names->End(); Names->Next())
	{
		AnimationsList->AddItem(Names->GetItem(),i);
		i++;
	}
}




void CDialogAnimatedMesh::LoadMeshFromX(StringParam name)
{    
	SAFE_DELETE(Model);
	Model = new WeModel();
	if (Model->Load(name, false, false))
	{
		FrameTree->Clear();
		AnimationsList->Clear();
		FillFrames(Model->RootFrame, NULL);
		FillAnimations();
		AnimSpeed->SetText(Model->AnimationSpeed);

		Actor->Model = Model;
		Actor->Renderable = true;
		SAFE_DELETE(Actor->PhysicHandle);

		Model->ComputeBoundingBox(Actor);
		PreviewCamera->Location.z = -(Actor->BSphere.Radius * 2.0f);
		//PreviewCamera->Location.y = tmp2;

		AnimationsList->SetSelection(0);
	}
	else
		MessageBox(GetHWND(),"Mesh doesn't have animation,\nor mesh file is wrong!!!",0,0);
}



