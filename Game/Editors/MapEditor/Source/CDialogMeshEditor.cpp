/*
    CDialogMeshEditor.cpp

	Autor: Tomislav Kukic
	Date: 24.1.2006.
	Version: 1.0


*/

#include <CDialogMeshEditor.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CDialogTools.h>
#include <CDialogSave.h>
#include <WeMeshManager.h>
#include <WeMaterialManager.h>
#include <WePackage.h>
#include <WeStaticModel.h>
#include <WeModelManager.h>
#include <WeTimer.h>
#include <String.h>
#include <WeLoger.h>
#include <WeCamera.h>
#include <WeLight.h>
#include <WeStaticModel.h>
#include <WeMaterial.h>
#include <WeTextureManager.h>
#include <CDialogMaterialEditor.h>
#include <CDialogBaking.h>



//========================================================
//    MeshEditor Class functions
//========================================================


CDialogMeshEditor* MeshEditor = NULL;

bool CDialogMeshEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_MESH_EDITOR, hParent, Show))
	{
		return false;
	}

	cModel = NULL;
	cMeshEditor = NULL;
	EnableRotation = false;
	WireFrame = false;

	OldMouseX = 0;
	OldMouseY = 0;
	OldMouseZ = 0;

	MouseX = 0;
	MouseY = 0;
	MouseZ = 0;

	PreviewMaterial = NULL;
	PreviewTexture = NULL;

	TreeModels = new CTreeView(this, IDC_MESHED_MODELS);
	TreeMeshes = new CTreeView(this, IDC_MESHED_MESHES);
	MaterialPresetCombo = new CComboBox(this, IDC_MESHED_MATERIALPRESET);
	TreeMeshMaterials = new CTreeView(this, IDC_MESHED_ATTRIBUTES);
	FindButton = new CButton(this, IDC_MESHED_FINDMATERIAL);
	SetButton = new CButton(this, IDC_MESHED_SETMATERIAL);
	TreeMaterials = new CTreeView(this, IDC_MESHED_MATERIALS);
	MeshPreview = new CGroupBox(this, IDC_MESHED_MESHPREVIEW);
	UseNormalMap = new CCheckBox(this, IDC_MESHED_USENORMALMAP);
	CheckWireframe = new CCheckBox(this, IDC_MESHED_MESHVIEW_WIREFRAME);
	SaveSelected = new CButton(this, IDC_MESHED_SAVESELECTED);
	SaveSelectedAs = new CButton(this, IDC_MESHED_SAVESELECTED_AS);
	RemoveSelected = new CButton(this, IDC_MESHED_REMOVESELECTED);
	ImportXFile = new CButton(this, IDC_MESHED_IMPORTXFILE);
	RefreshPackage = new CButton(this, IDC_MESHED_REFRESHPACKAGE);
	CheckOldStyle = new CCheckBox(this, IDC_MESHED_OLD_STYLE);
	CheckOldStyle->SetChecked(true);
	CheckShowOnlySelected = new CCheckBox(this, IDC_MESHED_SHOWONLYSELECTEDMATERIAL);
	LightMapSize = new CComboBox(this, IDC_LIGHTMAPSIZE);
	ResaveButton = new CButton(this, IDC_TEMPBUTTON);

	DiffusePreview = new CButton(this, ID_MESHED_TEXPREVIEW);


	LightMapSize->AddItem("64x64", 64);
	LightMapSize->AddItem("128x128", 128);
	LightMapSize->AddItem("256x256", 256);
	LightMapSize->AddItem("512x512", 512);
	LightMapSize->AddItem("1024x1024", 1024);
	LightMapSize->SetSelection(2);


	char tmp[100];
	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		sprintf(tmp, "%d", i);
		MaterialPresetCombo->AddItem(tmp, i);
	}
	MaterialPresetCombo->SetSelection(0);

	World = new WeWorld;
	Camera = new WeCamera;
	Camera->Location = WeVector(0, 0, -50);

	Light = new WeLight;
	Light->AffectDynamicActors = true;
	Light->Location = WeVector(0, 100, -100);

	WeLightState LS;
	LS.Range = 1000;
	LS.Diffuse.r = 1;
	LS.Diffuse.g = 1;
	LS.Diffuse.b = 1;
	Light->AddLightState(&LS);	
	World->AddActor(Light);

	Actor = NULL;

	return true;
}



String CDialogMeshEditor::GetAttributeText(int MaterialPreset, int AttributeIndex)
{
	char tmpMat[100];
	String tmps;

	sprintf(tmpMat,"Attribute %d: ",AttributeIndex + 1);
	tmps = tmpMat;

	int n = AttributeIndex;
	CSmartListIterator<String> I;

	for (I = cMeshEditor->MaterialNames[MaterialPreset].FirstI();
		I != cMeshEditor->MaterialNames[MaterialPreset].EndI();
		++I)
	{
		if (n == 0)
		{
			tmps = tmps + I.GetItem();
			return tmps;
		}
		else
			n--;
	}

	tmps = tmps + "not set";
	return tmps;
}


void CDialogMeshEditor::FillMaterialTree(int MaterialPreset)
{
	TreeMeshMaterials->Clear();
	
	if (cMeshEditor)
	{
		int Count = 0;
		CSmartListIterator<WeAttribRange> I;

		//for (I = cMeshEditor->MeshAttributes.FirstI();
		//	I != cMeshEditor->MeshAttributes.EndI();
		//	++I)
		for (cMeshEditor->MeshAttributes.FirstI(I);
			!cMeshEditor->MeshAttributes.EndI(I);
			cMeshEditor->MeshAttributes.NextI(I))
		{
			String tmps = GetAttributeText(MaterialPreset, Count);
			TreeMeshMaterials->Add(NULL, tmps, Count);
			Count++;
		}
	}
}


void CDialogMeshEditor::OnShow()
{
	FillTree(TreeModels, PACKAGEITEM_MODEL);
	FillTree(TreeMeshes, PACKAGEITEM_MESH);
	FillTree(TreeMaterials, PACKAGEITEM_MATERIAL);

	if (MaterialEditor->GetVisible())
	{
		CTreeViewItem *ItemMat;
		ItemMat = MaterialEditor->MaterialList->GetSelected();

		if (ItemMat)
		{
			CTreeViewItem *Item;
			Item = FindItem(TreeMaterials, GetFullItemName(ItemMat));
			if (Item)
			{
				TreeMaterials->SelectItem(Item);
			}
		}
	}

	if (EditorWindow->Selections.Selections.GetItemsCount() != 0)
	{
		WeActor *SelectedActor;
		EditorWindow->Selections.Selections.First();
		SelectedActor = EditorWindow->Selections.Selections.GetItem()->SelectedActor;

		if (SelectedActor->IsPrefab)
		{
			WeStaticModel *Model;
			CTreeViewItem *Item;
			Model = SelectedActor->Model;
			if (Model)
			{
				Item = FindItem(TreeMeshes, Model->MeshName);
				if (Item)
				{
					TreeMeshes->SelectItem(Item);
				}
			}

			MaterialPresetCombo->SetSelection(SelectedActor->MaterialPreset);
			OnClick(MaterialPresetCombo->GetID());
		}
	}
}


void CDialogMeshEditor::OnClick(DWORD ID)
{
	if (ID == UseNormalMap->GetID())
	{
		if (cMeshEditor)
		{
			cMeshEditor->SetNewUseNormalMap(UseNormalMap->GetChecked());
		}
	}

	else if (ID == ResaveButton->GetID())
	{
		if (TreeMeshes->GetSelected() && (TreeMeshes->GetSelected()->GetLevel() == 0))
		{
			bool StopResave = false;

			String Package;
			String FullName;
			Strings *Groups, *Names;
			Package = TreeMeshes->GetSelected()->GetText();
			Groups = WePackage::Instance()->GetGroupNames(Package, PACKAGEITEM_MESH);

			String OldName;
			int TotalMeshes = 0;
			int SavedMeshes = 0;

			for (Groups->First(); !Groups->End(); Groups->Next())
			{
				Names = WePackage::Instance()->GetItemsNames(Package, Groups->GetItem(), PACKAGEITEM_MESH);
				TotalMeshes += Names->GetItemsCount();
			}

			for (Groups->First(); !Groups->End(); Groups->Next())
			{
				Names = WePackage::Instance()->GetItemsNames(Package, Groups->GetItem(), PACKAGEITEM_MESH);

				for (Names->First(); !Names->End(); Names->Next())
				{
					FullName = Package + ".";
					FullName = FullName + Groups->GetItem();
					FullName = FullName + ".";
					FullName = FullName + Names->GetItem();

					OldName = (String)"_OLD_" + FullName;

					if (WePackage::Instance()->BeginReading(OldName, PACKAGEITEM_MESH))
					{
						WePackage::Instance()->EndReading();
						cMeshEditor = (WeMeshEditor*)WeMeshManager::Instance()->Load(OldName, false);
					}
					else
						cMeshEditor = (WeMeshEditor*)WeMeshManager::Instance()->Load(FullName, false);

					if (cMeshEditor)
					{
						SaveAutoModel(cMeshEditor, FullName, true, CheckOldStyle->GetChecked(), true);
					}

					++SavedMeshes;
					CDialogBaking::Instance()->OnResaveProgress(FullName, (float)SavedMeshes / (float)TotalMeshes, StopResave);
					if (StopResave) break;
				}

				if (StopResave) break;
			}

			if (StopResave)
			{
				char ttt[1000];
				sprintf(ttt, "Resave canceled on mesh %s", FullName.GetString());
				MessageBox(0, ttt, "MeshEditor", 0);
			}
			else
				MessageBox(0, "Resave completed", "MeshEditor", 0);
		}						
	}

	else if (ID == MaterialPresetCombo->GetID())
	{
		FillMaterialTree(MaterialPresetCombo->GetSelectionData());
		if (Actor)
			Actor->MaterialPreset = MaterialPresetCombo->GetSelectionData();
	}

	else if (ID == ImportXFile->GetID())
	{
		CTreeViewItem* Item;
		Item = TreeMeshes->GetSelected();
		//if (Item && (Item->GetLevel() == 1))
		{
			GetFile("DirectX mesh files *.x\0*.x\0\0","");
			if(FileName.GetLen() == 0)
				return;

			WeMeshEditor *aMeshEditor;
			aMeshEditor = (WeMeshEditor*)WeMeshManager::Instance()->Load(FileName, true, false, true);

			if (aMeshEditor)
			{
				String temp = FileName;
				int p;

				while ((p = temp.SubPos("\\")) >= 0)
					temp = temp.Sub(p + 1, temp.GetLen());

				SaveDialog->HoleName =
					SaveDialog->HoleName.Extract('.', 0) + "." +
					SaveDialog->HoleName.Extract('.', 1) + "." +
					temp.Extract('.', 0);

				SaveDialog->Type = PACKAGEITEM_MESH;

				if (SaveDialog->ShowModal(GetHWND()) == MR_OK)
				{
					Strings MaterialNames[MAX_MATERIAL_PRESETS2];

					if (WePackage::Instance()->BeginReading(SaveDialog->HoleName, PACKAGEITEM_MESH))
					{
						WePackage::Instance()->EndReading();

						WeMesh *OldMesh;
						OldMesh = WeMeshManager::Instance()->Load(SaveDialog->HoleName, false, false, false);

						for (int j = 0; j < MAX_MATERIAL_PRESETS2; j++)
						{
							for (aMeshEditor->MaterialNames[j].First(), OldMesh->MaterialNames[j].First();
								!aMeshEditor->MaterialNames[j].End() && !OldMesh->MaterialNames[j].End();
								aMeshEditor->MaterialNames[j].Next(), OldMesh->MaterialNames[j].Next())
							{
								aMeshEditor->MaterialNames[j].GetItem() = OldMesh->MaterialNames[j].GetItem();
							}
						}
					}

					String SavedName = SaveAutoModel(aMeshEditor, SaveDialog->HoleName, true, CheckOldStyle->GetChecked());
					WeMeshManager::Instance()->Load(SaveDialog->HoleName, false, false, true);

					Item = FindItem(TreeMeshes, SavedName);
					if (Item)
					{
						TreeMeshes->SelectItem(Item);
						//OnClick(TreeMeshes->GetID());
					}
				}
			}

			//String PackageName, GroupName, Name, FullName;
			//PackageName = Item->GetParent()->GetText();
			//GroupName = Item->GetText();

			//Name = FileName;
			//int p;
			//while ((p = Name.SubPos((String)"\\")) >= 0)
			//{
			//	Name = Name.Sub(p + 1, Name.GetLen());
			//}
			//Name = Name.Extract('.', 0);

			//SaveAutoModel(cMeshEditor, PackageName + (String)"." + GroupName + (String)"." + Name, true);
		}
	}

	else if (ID == SaveSelected->GetID())
	{
		if (cMeshEditor)
		{
			SaveAutoModel(cMeshEditor, cMeshEditor->Name, true, CheckOldStyle->GetChecked());
		}
	}

	else if (ID == SaveSelectedAs->GetID())
	{
		if (cMeshEditor)
		{
			SaveDialog->HoleName = cMeshEditor->Name;
			SaveDialog->Type = PACKAGEITEM_MESH;
			if (SaveDialog->ShowModal(GetHWND()) == MR_OK)
			{
				SaveAutoModel(cMeshEditor, SaveDialog->HoleName, true, CheckOldStyle->GetChecked());
			}
		}
	}

	else if (ID == RemoveSelected->GetID())
	{
		CTreeViewItem* Item;
		Item = TreeMeshes->GetSelected();
		if (Item && (IDYES == MessageBox(GetHWND(), "Are you sure you want to remove selected item(s)?", "Remove", MB_YESNO | MB_ICONQUESTION)))
		{
			if (Item->GetLevel() == 0)
			{
				for (TreeMeshes->Items.First(); !TreeMeshes->Items.End(); TreeMeshes->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeMeshes->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent()->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MESH);
						if (cMeshEditor && (cMeshEditor->Name == ItemName))
						{
							cMeshEditor = NULL;
							FillMaterialTree(0);
						}
					}
				}
				TreeMeshes->DeleteSelected();
			}

			else if (Item->GetLevel() == 1)
			{
				for (TreeMeshes->Items.First(); !TreeMeshes->Items.End(); TreeMeshes->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeMeshes->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MESH);
						if (cMeshEditor && (cMeshEditor->Name == ItemName))
						{
							cMeshEditor = NULL;
							FillMaterialTree(0);
						}
					}
				}
				TreeMeshes->DeleteSelected();
			}

			else if (Item->GetLevel() == 2)
			{
				if (cMeshEditor)
				{
					WePackage::Instance()->Remove(cMeshEditor->Name, PACKAGEITEM_MESH);
					cMeshEditor = NULL;
					FillMaterialTree(0);
					TreeMeshes->DeleteSelected();
				}
			}
		}
	}

	else if (ID == TreeMeshes->GetID())
	{
		CTreeViewItem* Item;
		Item = TreeMeshes->GetSelected();
		bool ok = false;

		cMeshEditor = NULL;

		if (Item && (Item->GetLevel() == 2))
		{
			String PackageName, GroupName, Name, FullName;
			PackageName = Item->GetParent()->GetParent()->GetText();
			GroupName = Item->GetParent()->GetText();
			Name = Item->GetText();
			FullName = PackageName + "." + GroupName + "." + Name;

			cMeshEditor = (WeMeshEditor*)WeMeshManager::Instance()->Load(FullName, false);

			ok = true;
		}

		if (ok)
		{
			FillMaterialTree(MaterialPresetCombo->GetSelectionData());
			//UseNormalMap->SetChecked(cMeshEditor->GetUseNormalMap());
			if (cMeshEditor->LightMapSize == 64)
				LightMapSize->SetSelection(0);
			if (cMeshEditor->LightMapSize == 128)
				LightMapSize->SetSelection(1);
			if (cMeshEditor->LightMapSize == 256)
				LightMapSize->SetSelection(2);
			if (cMeshEditor->LightMapSize == 512)
				LightMapSize->SetSelection(3);
			if (cMeshEditor->LightMapSize == 1024)
				LightMapSize->SetSelection(4);


			SAFE_DELETE(Actor);
			Actor = new WeActor;
			Actor->Model = new WeStaticModel;
			if (Actor->Model)
			{
				Actor->Model->Mesh = cMeshEditor;
				Actor->Model->QueryMesh = cMeshEditor;
				for (int Preset = 0; Preset < MAX_MATERIAL_PRESETS2; ++Preset)
					Actor->Model->LoadMaterials(Preset);
			}
			Actor->Location = WeVector(0, 0, 0);
			Camera->Location.z = -cMeshEditor->BoundingBox.GetSize()*3;			
			Actor->MaterialPreset = MaterialPresetCombo->GetSelectionData();
			World->AddActor(Actor);
			Actor->LightMap.GenerateDefault(Actor);
		}
	}

	else if (ID == LightMapSize->GetID())
	{
		if (cMeshEditor)
		{			
			cMeshEditor->LightMapSize = LightMapSize->GetSelectionData();
		}
	}

	else if (ID == TreeMeshMaterials->GetID())
	{
		CTreeViewItem* MMItem = TreeMeshMaterials->GetSelected();
		if (cMeshEditor && MMItem)
		{
			Strings* MaterialNames = &cMeshEditor->MaterialNames[MaterialPresetCombo->GetSelectionData()];

			MaterialNames->First();
			for (int n = MMItem->GetData(); n > 0; n--)
				MaterialNames->Next();

			PreviewMaterial = WeMaterialManager::Instance()->Load(MaterialNames->GetItem());
			if (PreviewMaterial)
			{
				if (PreviewMaterial->Textures[0] && (PreviewMaterial->Textures[0]->Name.GetLen() != 0))
					PreviewTexture = WeTextureManager::Instance()->Load(PreviewMaterial->Textures[0]->Name, true);
			}
		}

		OnShowOnlySelected();
	}

	else if (ID == TreeMaterials->GetID())
	{
		CTreeViewItem* TexItem = TreeMaterials->GetSelected();
		if (TexItem && TexItem->GetLevel() == 2)
		{
			PreviewMaterial = WeMaterialManager::Instance()->Load(GetFullItemName(TexItem));
			if (PreviewMaterial->Textures[0] && (PreviewMaterial->Textures[0]->Name.GetLen() != 0))
				PreviewTexture = WeTextureManager::Instance()->Load(PreviewMaterial->Textures[0]->Name, true);
		}
	}

	else if (ID == CheckWireframe->GetID())
	{
		WireFrame = !WireFrame;
	}

	else if (ID == IDC_MESHED_SETMATERIAL)
	{
		CTreeViewItem* MatItem;
		CTreeViewItem* MMItem;

		MatItem = TreeMaterials->GetSelected();
		MMItem = TreeMeshMaterials->GetSelected();

		if (cMeshEditor && MatItem && MMItem)
		{
			String PackageName, GroupName, Name, FullName;
			//PackageName = MatItem->GetParent()->GetParent()->GetText();
			//GroupName = MatItem->GetParent()->GetText();
			//Name = MatItem->GetText();
			//FullName = PackageName + "." + GroupName + "." + Name;
			FullName = GetFullItemName(MatItem);

			Strings* MaterialNames = &cMeshEditor->MaterialNames[MaterialPresetCombo->GetSelectionData()];

			MaterialNames->First();
			for (int n = MMItem->GetData(); n > 0; n--)
				MaterialNames->Next();

			MaterialNames->GetItem() = FullName;
			cMeshEditor->Refresh();
			FillMaterialTree(MaterialPresetCombo->GetSelectionData());
		}
	}

	else if (ID == IDC_MESHED_FINDMATERIAL)
	{
		CTreeViewItem *MMItem;

		MMItem = TreeMeshMaterials->GetSelected();
		if (MMItem)
		{
			String MatName;
			MatName = MMItem->GetText();
			MatName = MatName.Sub(MatName.SubPos(": ") + 2, MatName.GetLen());

			CTreeViewItem* MatItem;
			MatItem = FindItem(TreeMaterials, MatName);

			if (MatItem)
			{
				TreeMaterials->SelectItem(MatItem);
			}
		}
	}

	else if (ID == IDC_MESHED_MODELS)
	{
		cModel = NULL;

		CTreeViewItem* Item;
		Item = TreeModels->GetSelected();
		if (Item && (Item->GetLevel() == 2))
		{
			String PackageName, GroupName, Name, FullName;
			PackageName = Item->GetParent()->GetParent()->GetText();
			GroupName = Item->GetParent()->GetText();
			Name = Item->GetText();
			FullName = PackageName + "." + GroupName + "." + Name;

			cModel = WeModelManager::Instance()->Load(FullName, false);
		}
	}

	else if (ID == IDC_MESHED_LOAD_MODEL)
	{
		if (cModel)
		{
			CTreeViewItem *Item = FindItem(TreeMeshes, cModel->MeshName);
			if (Item)
			{
				TreeMeshes->SelectItem(Item);
			}
		}
	}

	else if (ID == IDC_MESHED_SAVESELECTED_MODEL)
	{
		if (cModel && cMeshEditor)
		{
			cModel->MeshName = cMeshEditor->Name;
			cModel->QueryMeshName = cMeshEditor->Name;
			cModel->Save(cModel->Name, true);
		}
	}

	else if (ID == IDC_MESHED_SAVESELECTED_AS_MODEL)
	{
		if (cMeshEditor)
		{
			WeStaticModel* model = new WeStaticModel;
			model->MeshName = cMeshEditor->Name;
			model->QueryMeshName = cMeshEditor->Name;

			SaveDialog->HoleName = cMeshEditor->Name;
			SaveDialog->Type = PACKAGEITEM_MODEL;
			if (SaveDialog->ShowModal(GetHWND()) == MR_OK)
			{
				bool ok = false;

				if (model->Save(SaveDialog->HoleName, false))
					ok = true;
				else
				{
					if (IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?", "Save", MB_YESNO | MB_ICONQUESTION))
					{
						ok = model->Save(SaveDialog->HoleName, true);
					}
				}

				if (ok)
				{
					if (!FindItem(TreeModels, SaveDialog->HoleName))
						AddItem(TreeModels, SaveDialog->HoleName);
				}
			}

			delete model;
		}
	}

	else if (ID == IDC_MESHED_REMOVESELECTED_MODEL)
	{
		CTreeViewItem* Item;
		Item = TreeModels->GetSelected();
		if (Item && (IDYES == MessageBox(GetHWND(), "Are you sure you want to remove selected item(s)?", "Remove", MB_YESNO | MB_ICONQUESTION)))
		{
			if (Item->GetLevel() == 0)
			{
				for (TreeModels->Items.First(); !TreeModels->Items.End(); TreeModels->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeModels->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent()->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MESH);
						if (cModel && (cModel->Name == ItemName))
						{
							cModel = NULL;
						}
					}
				}
				TreeModels->DeleteSelected();
			}

			else if (Item->GetLevel() == 1)
			{
				for (TreeModels->Items.First(); !TreeModels->Items.End(); TreeModels->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeModels->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MESH);
						if (cModel && (cModel->Name == ItemName))
						{
							cModel = NULL;
						}
					}
				}
				TreeModels->DeleteSelected();
			}

			else if (Item->GetLevel() == 2)
			{
				if (cModel)
				{
					WePackage::Instance()->Remove(cModel->Name, PACKAGEITEM_MESH);
					cModel = NULL;
					TreeModels->DeleteSelected();
				}
			}
		}
	}

	else if (ID == RefreshPackage->GetID())
	{
		CTreeViewItem* Item;
		Item = TreeMeshes->GetSelected();
		if (Item && (Item->GetLevel() == 0))
			OnRefreshPackage(Item->GetText());
	}

	else if (ID == IDC_MESHED_SHOWONLYSELECTEDMATERIAL)
	{
		OnShowOnlySelected();
	}

	else if (ID == IDC_MESHED_COPYATTR)
	{
		CTreeViewItem *MMItem;
		MMItem = TreeMeshMaterials->GetSelected();

		if (MMItem)
		{
			String MatName;
			MatName = MMItem->GetText();
			ClipboardMaterial = MatName.Sub(MatName.SubPos(": ") + 2, MatName.GetLen());
		}
	}

	else if (ID == IDC_MESHED_PASTEATTR)
	{
		CTreeViewItem* MMItem;
		MMItem = TreeMeshMaterials->GetSelected();

		if (cMeshEditor && MMItem && FindItem(TreeMaterials, ClipboardMaterial))
		{
			Strings* MaterialNames = &cMeshEditor->MaterialNames[MaterialPresetCombo->GetSelectionData()];

			MaterialNames->First();
			for (int n = MMItem->GetData(); n > 0; n--)
				MaterialNames->Next();

			MaterialNames->GetItem() = ClipboardMaterial;
			cMeshEditor->Refresh();
			FillMaterialTree(MaterialPresetCombo->GetSelectionData());
		}
	}

	else if (ID == IDC_MESHED_COPYALLATTR)
	{
		ClipboardMaterials.Clear();
		for (TreeMeshMaterials->Items.First(); !TreeMeshMaterials->Items.End(); TreeMeshMaterials->Items.Next())
		{
			String MatName;
			MatName = TreeMeshMaterials->Items.GetItem().GetText();
			ClipboardMaterials.PushBack(MatName.Sub(MatName.SubPos(": ") + 2, MatName.GetLen()));
		}
	}

	else if (ID == IDC_MESHED_PASTEALLATTR)
	{
		if (cMeshEditor)
		{
			Strings* MaterialNames = &cMeshEditor->MaterialNames[MaterialPresetCombo->GetSelectionData()];

			for (MaterialNames->First(), ClipboardMaterials.First();
				!MaterialNames->End() && !ClipboardMaterials.End();
				MaterialNames->Next(), ClipboardMaterials.Next())
			{
				if (FindItem(TreeMaterials, ClipboardMaterials.GetItem()))
					MaterialNames->GetItem() = ClipboardMaterials.GetItem();
			}

			cMeshEditor->Refresh();
			FillMaterialTree(MaterialPresetCombo->GetSelectionData());
		}
	}
}





void CDialogMeshEditor::OnEveryFrame()
{
/*
	if(!TreeMeshMaterials->GetSelected())
	{
		ChangeButton->SetVisible(false);
		return;
	}

	if(TreeMeshMaterials->GetSelected()->GetLevel() == 1)
		ChangeButton->SetVisible(true);
	else
		ChangeButton->SetVisible(false);
*/
}



void CDialogMeshEditor::Tick()
{
	if (Actor)
		Actor->Yaw = WeTimer::Instance()->GetElapsedTimeF();
}


const float SPEED = 200.0;

void CDialogMeshEditor::OnLButtonDown(int x, int y)
{
	EnableRotation = true;

	float tmpX, tmpY;
	tmpX = (D3DX_PI / SPEED) * x;
	tmpY = (D3DX_PI / SPEED) * y;

	OldMouseX = tmpX;
	OldMouseY = tmpY;
}



void CDialogMeshEditor::OnLButtonUp(int x, int y)
{
	EnableRotation = false;

	MouseX = 0;
	MouseY = 0;
}



void CDialogMeshEditor::OnMouseMove(int x, int y)
{
	if (!EnableRotation)
		return;

	float tmpX, tmpY;
	tmpX = (D3DX_PI / SPEED) * x;
	tmpY = (D3DX_PI / SPEED) * y;

	MouseX = -(tmpX - OldMouseX);
	MouseY = -(tmpY - OldMouseY);

	OldMouseX = tmpX;
	OldMouseY = tmpY;
}



void CDialogMeshEditor::OnMouseWheel(WORD Position)
{
	if((int)Position > 0)
		MouseZ = 1;
	else if((int)Position < 0)
		MouseZ = -1;
}



void CDialogMeshEditor::Render()
{
	MeshEditor->World->Render(MeshEditor->Camera);
}


void RefreshMesh(StringParam ItemName, StringParam FileName, void *Param)
{
	WeMeshEditor* Mesh = NULL;
	Strings OldMaterialNames[MAX_MATERIAL_PRESETS2];

	// Try to load old materials
	Mesh = (WeMeshEditor*)WeMeshManager::Instance()->Load(ItemName, false);
	if (Mesh)
	{
		for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
		{
			for (Mesh->MaterialNames[i].First(); !Mesh->MaterialNames[i].End(); Mesh->MaterialNames[i].Next())
			{
				OldMaterialNames[i].PushBack(Mesh->MaterialNames[i].GetItem());
			}
		}
	}

	// Force load from file
	Mesh = (WeMeshEditor*)WeMeshManager::Instance()->Load(FileName, true, false, true);

	// Try to restore old material names
	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		for (OldMaterialNames[i].First(), Mesh->MaterialNames[i].First();
			!OldMaterialNames[i].End() && !Mesh->MaterialNames[i].End();
			OldMaterialNames[i].Next(), Mesh->MaterialNames[i].Next())
		{
			Mesh->MaterialNames[i].GetItem() = OldMaterialNames[i].GetItem();
		}
		OldMaterialNames[i].Clear();
	}

	// Save
	String SavedName = ((CDialogMeshEditor*)Param)->SaveAutoModel(Mesh, ItemName, false, ((CDialogMeshEditor*)Param)->CheckOldStyle->GetChecked());

	// Force load from package
	Mesh->Load(SavedName, false);

	Mesh->Refresh();
}


void CDialogMeshEditor::OnRefreshPackage(StringParam PackageName)
{
	RefreshPackageItems("_Meshes", PackageName, &RefreshMesh, this);
	MessageBox(0, ((String)"Package " + PackageName + (String)" refreshed").GetString(), "Mesh Editor", 0);
}


String CDialogMeshEditor::SaveAutoModel(WeMeshEditor* Mesh, StringParam MeshName, bool ShowMsgs, bool OldStyle, bool Silent)
{
	String aModelName = "", aMeshName = "";
	WeStaticModel* model = NULL;
	bool okModel = false, okMesh = false;


	// Calculate Model and Mesh name
	if (OldStyle)
	{
		aMeshName = MeshName;
	}
	else
	{
		aMeshName = MeshName;
		aModelName = MeshName;
	}


	// Create temporary model
	if (aModelName.GetLen() != 0)
	{
		model = new WeStaticModel;
		if (!model->Load(aModelName))
		{
			model->MeshName = aMeshName;
			model->QueryMeshName = aMeshName;
		}

		if ((model->MeshName == (String)"") || (aMeshName.GetLen() != 0))
		{
			model->MeshName = aMeshName;
			model->QueryMeshName = aMeshName;
		}
	}


	// Save Mesh
	if (aMeshName.GetLen() != 0)
	{
		if (Mesh->Save(aMeshName, !ShowMsgs))
		{
			okMesh = true;
		}
		else
		{
			if (IDYES == MessageBox(GetHWND(), "Mesh already exist, overwrite?", "Save Mesh", MB_YESNO | MB_ICONQUESTION))
			{
				if (Mesh->Save(aMeshName, true))
				{
					okMesh = true;
				}
			}
		}

		if (okMesh && !FindItem(TreeMeshes, aMeshName))
		{
			AddItem(TreeMeshes, aMeshName);
		}
	}


	// Save Model
	if (model)
	{
		if (model->Save(aModelName, !ShowMsgs))
		{
			okModel = true;
		}
		else
		{
			if (IDYES == MessageBox(GetHWND(), "Model already exist, overwrite?", "Save Model", MB_YESNO | MB_ICONQUESTION))
			{
				if (model->Save(aModelName, true))
				{
					okModel = true;
				}
			}
		}

		if (okModel)
		{
			CTreeViewItem *Item = FindItem(TreeModels, aModelName);
			if (!Item)
			{
				AddItem(TreeModels, aModelName);
			}
		}


		// Destroy temporary model
		delete model;
	}


	// MessageBox
	if (ShowMsgs && (okMesh || okModel))
	{
		String msg = "";
		if (okModel)
		{
			msg += (String)"Saved model: " + aModelName;
		}
		if (okMesh)
		{
			if (msg.GetLen() != 0)
			{
				msg += (char)10;
			}
			msg += (String)"Saved mesh: " + aMeshName;
		}

		if (!Silent)
			MessageBox(0, msg.GetString(), "Save Geometry", 0);
	}

	return aMeshName;
}



void CDialogMeshEditor::OnShowOnlySelected()
{
	if (cMeshEditor)
	{
		cMeshEditor->OnlyVisibleAttribute = -1;

		if (CheckShowOnlySelected->GetChecked())
		{
			CTreeViewItem *MMItem;
			MMItem = TreeMeshMaterials->GetSelected();
			if (MMItem)
			{
				cMeshEditor->OnlyVisibleAttribute = MMItem->GetData();
			}
		}
	}
}
