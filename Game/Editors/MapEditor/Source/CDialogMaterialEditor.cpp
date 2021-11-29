/*
    CDialogMaterialEditor.cpp

	Autor: Nebojsa Vislavski
	Date: 27.12.2005.
	Version: 1.0


*/


#include <CDialogMaterialEditor.h>
#include <CMapEditorWindow.h>
#include <CEditorResourceManager.h>
#include <WePackage.h>
#include <WeMaterialEditor.h>
#include <WeMaterialManager.h>
#include <WeTextureManager.h>
#include <WeShaderManager.h>
#include <WeMaterial.h>
#include <WeModelManager.h>
#include <WeTimer.h>
#include <d3dx9.h>
#include <commdlg.h>
#include <WeLight.h>
#include <WeMesh.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>
#include <CDialogTools.h>
#include <CDialogMeshEditor.h>
#include <CDialogTextureEditor.h>



CDialogMaterialEditor *MaterialEditor = NULL;


CDialogMaterialEditor *CDialogMaterialEditor::_instance = NULL;
CDialogMaterialEditor *CDialogMaterialEditor::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogMaterialEditor();
	return _instance;
}



bool CDialogMaterialEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_MATERIAL_EDITOR,EditorWindow->GetHWND(),false))
	{
		return false;
	}
	
	Created = false;

	EffectBrowse = new CButton(this, IDC_EFFECTNAMEBROWSE);
	SetTexture[0] = new CButton(this, IDC_TEXTURE1SET);
	SetTexture[1] = new CButton(this, IDC_TEXTURE2SET);
	SetTexture[2] = new CButton(this, IDC_TEXTURE3SET);
	SetTexture[3] = new CButton(this, IDC_TEXTURE4SET);
	SetTexture[4] = new CButton(this, IDC_TEXTURE5SET);
	SetTexture[5] = new CButton(this, IDC_TEXTURE6SET);
	SetTexture[6] = new CButton(this, IDC_TEXTURE7SET);
	SetTexture[7] = new CButton(this, IDC_TEXTURE8SET);
	FindTexture[0] = new CButton(this, IDC_TEXTURE1FIND);
	FindTexture[1] = new CButton(this, IDC_TEXTURE2FIND);
	FindTexture[2] = new CButton(this, IDC_TEXTURE3FIND);
	FindTexture[3] = new CButton(this, IDC_TEXTURE4FIND);
	FindTexture[4] = new CButton(this, IDC_TEXTURE5FIND);
	FindTexture[5] = new CButton(this, IDC_TEXTURE6FIND);
	FindTexture[6] = new CButton(this, IDC_TEXTURE7FIND);
	FindTexture[7] = new CButton(this, IDC_TEXTURE8FIND);
	Apply = new CButton(this, IDC_APPLY);
	Refresh = new CButton(this, IDC_REFRESH);
	PreviewArea = new CButton(this, IDC_PREVIEWAREA);
	BlendType = new CComboBox(this, IDC_BLENDTYPE);
	
	MatParam1 = new CEditBox(this, IDC_MATPARAM1);
	MatParam2 = new CEditBox(this, IDC_MATPARAM2);
	MatParam3 = new CEditBox(this, IDC_MATPARAM3);
	MatParam4 = new CEditBox(this, IDC_MATPARAM4);
	AcceptLights = new CCheckBox(this, IDC_ACCEPTLIGHTS);

	TexAnimCombo = new CComboBox(this, IDC_TEXANIMCOMBO);
	TexAnimParam1 = new CEditBox(this, IDC_TEXANIMPARAM1);
	TexAnimParam2 = new CEditBox(this, IDC_TEXANIMPARAM2);
	TexAnimParam3 = new CEditBox(this, IDC_TEXANIMPARAM3);
	TexAnimParam4 = new CEditBox(this, IDC_TEXANIMPARAM4);
	TexAnimParam5 = new CEditBox(this, IDC_TEXANIMPARAM5);

	TexAnimParam1Label = new CLabel(this, IDC_TEXANIMPARAM1LABEL);
	TexAnimParam2Label = new CLabel(this, IDC_TEXANIMPARAM2LABEL);
	TexAnimParam3Label = new CLabel(this, IDC_TEXANIMPARAM3LABEL);
	TexAnimParam4Label = new CLabel(this, IDC_TEXANIMPARAM4LABEL);
	TexAnimParam5Label = new CLabel(this, IDC_TEXANIMPARAM5LABEL);

	Glowing = new CCheckBox(this, IDC_GLOWING);
	DoubleSided = new CCheckBox(this, IDC_DOUBLESIDED);

	Objects = new CComboBox(this, IDC_OBJECT);
	Objects->AddItem("Box");
	Objects->AddItem("Sphere");
	Objects->AddItem("Box (normal map)");
	Objects->AddItem("Sphere (normal map)");
	Objects->SetSelection(3);

	XRot = new CCheckBox(this, IDC_XROT);
	YRot = new CCheckBox(this, IDC_YROT);
	ZRot = new CCheckBox(this, IDC_ZROT);


	EffectName = new CEditBox(this, IDC_EFFECTNAME);
	TexName[0] = new CEditBox(this, IDC_TEXTURE1NAME);
	TexName[1] = new CEditBox(this, IDC_TEXTURE2NAME);
	TexName[2] = new CEditBox(this, IDC_TEXTURE3NAME);
	TexName[3] = new CEditBox(this, IDC_TEXTURE4NAME);
	TexName[4] = new CEditBox(this, IDC_TEXTURE5NAME);
	TexName[5] = new CEditBox(this, IDC_TEXTURE6NAME);
	TexName[6] = new CEditBox(this, IDC_TEXTURE7NAME);
	TexName[7] = new CEditBox(this, IDC_TEXTURE8NAME);
	SpecularPow = new CEditBox(this ,IDC_SPECULARPOW);
	BumpIntensity = new CEditBox(this, IDC_BUMPINTENSITY);
	AlphaRef = new CEditBox(this, IDC_ALPHAREF);
	MaterialAlpha = new CEditBox(this, IDC_MATERIALALPHA);

	MaterialType = new CComboBox(this, IDC_MATERIALTYPE);
	MaterialReflection = new CComboBox(this, IDC_METERIALREFLECTION);

	MaterialList = new CTreeView(this, IDC_MATED_MATERIALLIST);
	TextureList = new CTreeView(this, IDC_MATED_TEXTURELIST);
	EffectList = new CTreeView(this, IDC_MATED_EFFECTLIST);

	TexPreview = new CCheckBox(this, IDC_MATED_TEXPREVIEW);
	TexPreview->SetChecked(true);
	TexturePreviewArea = new CButton(this, ID_MATED_TEXPREVIEW);

	MatColorLabel = new CLabel(this, IDC_MATCOLORSTATIC);

	XRotation = false;
	YRotation = false;
	ZRotation = false;

	TexName[1]->SetText("MapEditor.Textures.DefaultNormalMapBlue1x1");
	TexName[2]->SetText("MapEditor.Textures.DefaultWhite1x1");
	TexName[3]->SetText("MapEditor.Textures.DefaultBlack1x1");

	PrepareTextureList(TextureList);

	FillComboBoxes();

	ClearAllNames();
	Material = new WeMaterialEditor;
	UpdateComponents();

	Created = true;
	return true;
}



void CDialogMaterialEditor::FillComboBoxes()
{
	MaterialType->AddItem("Normal");
	MaterialType->AddItem("Transparent");
	MaterialType->AddItem("Alpha blended");
	MaterialType->AddItem("Refractive");
	MaterialType->AddItem("Foreground");
	MaterialType->SetSelection(0);

	BlendType->Clear();
	BlendType->AddItem("Standard", WEMATERIAL_BLEND_STANDARD);
	BlendType->AddItem("Additive", WEMATERIAL_BLEND_ADDITIVE);
	BlendType->SetSelection(0);


	MaterialReflection->AddItem("None");
	MaterialReflection->AddItem("Realtime");
	MaterialReflection->AddItem("Supplied");
	MaterialReflection->SetSelection(0);

	TexAnimCombo->Clear();
	TexAnimCombo->AddItem("None", WETEXTUREANIMATION_NONE);
	TexAnimCombo->AddItem("Scroll", WETEXTUREANIMATION_SCROLL);
	TexAnimCombo->AddItem("Rotate", WETEXTUREANIMATION_ROTATE);
	TexAnimCombo->AddItem("RotateCenter", WETEXTUREANIMATION_ROTATECENTER);
	TexAnimCombo->AddItem("Slide show", WETEXTUREANIMATION_SLIDESHOW);
	TexAnimCombo->AddItem("Shake", WETEXTUREANIMATION_SHAKE);
	TexAnimCombo->AddItem("Zoom", WETEXTUREANIMATION_ZOOM);
	TexAnimCombo->AddItem("Slide show random", WETEXTUREANIMATION_SLIDESHOWRANDOM);
	TexAnimCombo->AddItem("UV Scale", WETEXTUREANIMATION_UVSCALE);
	TexAnimCombo->SetSelection(0);
}


void CDialogMaterialEditor::OnShow()
{
	FillTree(MaterialList,PACKAGEITEM_MATERIAL);
	FillTree(TextureList,PACKAGEITEM_TEXTURE);
	UpdateTextureList(TextureList);
	FillTree(EffectList,PACKAGEITEM_EFFECT);

	OnChangeObject();

	CTreeViewItem *Item;
	String AutoMaterialName;
	AutoMaterialName = "";

	if (EditorWindow->Selections.Selections.GetItemsCount() != 0)
	{
		WeActor *SelectedActor;
		EditorWindow->Selections.Selections.First();
		SelectedActor = EditorWindow->Selections.Selections.GetItem()->SelectedActor;

		if (SelectedActor->IsPrefab)
		{
			WeStaticModel *Model;
			Model = SelectedActor->Model;
			if (Model && Model->Mesh)
			{
				CSmartList<WeMaterial*> *Materials = &Model->Mesh->Materials[SelectedActor->MaterialPreset];
				if (!Materials->Empty())
				{
					Materials->First();
					AutoMaterialName = Materials->GetItem()->Name;
				}
			}
		}
	}

	if ((AutoMaterialName.GetLen() == 0) && MeshEditor/* && MeshEditor->GetVisible()*/)
	{
		if (MeshEditor->PreviewMaterial)
		{
			AutoMaterialName = MeshEditor->PreviewMaterial->Name;
		}
	}

	if (AutoMaterialName.GetLen() != 0)
	{
		Item = FindItem(MaterialList, AutoMaterialName);
		if (Item)
		{
			MaterialList->SelectItem(Item);
		}
	}

	Item = TextureEditor->TreeTextures->GetSelected();
	if (Item)
	{
		String TexName = GetFullItemName(Item);
		Item = FindItem(TextureList, TexName);
		if (Item)
			TextureList->SelectItem(Item);
	}
}



void CDialogMaterialEditor::OnHide()
{
}



void CDialogMaterialEditor::OnClick(DWORD ID)
{
	
	if(ID == TexAnimCombo->GetID()) 	OnTexAnimComboClick();	
	if(ID == IDC_MATED_NEW) OnNewMaterial();
	if(ID == IDC_MATED_SAVESELECTED)
	{
		CTreeViewItem *Item;
		Item = MaterialList->GetSelected();
		if (Item && Item->GetLevel() == 2)
		{
			OnSaveMaterial(GetFullItemName(Item));
		}
	}

	if(ID == Glowing->GetID()) 
	{
		if (Material) Material->Glowing = Glowing->GetChecked();
	}
	if(ID == DoubleSided->GetID()) 
	{
		if (Material) Material->DoubleSided = DoubleSided->GetChecked();
	}

	//if(ID == Refractive->GetID()) 
	//{
	//	if (Material) Material->Refractive = Refractive->GetChecked();
	//}
	

	if(ID == IDC_MATED_SAVESELECTED_AS) OnSaveMaterial("");
	if(ID == IDC_MATED_REMOVESELECTED)
	{
		CTreeViewItem *Item;
		Item = MaterialList->GetSelected();
		String FullName = GetFullItemName(Item);

		if (Item && (IDYES == MessageBox(GetHWND(), "Are you sure you want to remove selected item(s)?", "Remove", MB_YESNO | MB_ICONQUESTION)))
		{
			if (Item->GetLevel() == 0)
			{
				for (MaterialList->Items.First(); !MaterialList->Items.End(); MaterialList->Items.Next())
				{
					CTreeViewItem *Item2 = &MaterialList->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent()->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MATERIAL);
					}
				}
				MaterialList->DeleteSelected();
			}

			else if (Item->GetLevel() == 1)
			{
				for (MaterialList->Items.First(); !MaterialList->Items.End(); MaterialList->Items.Next())
				{
					CTreeViewItem *Item2 = &MaterialList->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MATERIAL);
					}
				}
				MaterialList->DeleteSelected();
			}

			else if (Item->GetLevel() == 2)
			{
				String ItemName = GetFullItemName(Item);
				WePackage::Instance()->Remove(ItemName, PACKAGEITEM_MATERIAL);
			}
		}
	}

	if(ID == EffectBrowse->GetID()) OnEffectBrowse();
	if(ID == IDC_MATCOLORBUTTON) OnPickColor();
	if(ID == SetTexture[0]->GetID()) OnSetTexture(0);
	if(ID == SetTexture[1]->GetID()) OnSetTexture(1);
	if(ID == SetTexture[2]->GetID()) OnSetTexture(2);
	if(ID == SetTexture[3]->GetID()) OnSetTexture(3);
	if(ID == SetTexture[4]->GetID()) OnSetTexture(4);
	if(ID == SetTexture[5]->GetID()) OnSetTexture(5);
	if(ID == SetTexture[6]->GetID()) OnSetTexture(6);
	if(ID == SetTexture[7]->GetID()) OnSetTexture(7);
	if(ID == FindTexture[0]->GetID()) OnFindTexture(0);
	if(ID == FindTexture[1]->GetID()) OnFindTexture(1);
	if(ID == FindTexture[2]->GetID()) OnFindTexture(2);
	if(ID == FindTexture[3]->GetID()) OnFindTexture(3);
	if(ID == FindTexture[4]->GetID()) OnFindTexture(4);
	if(ID == FindTexture[5]->GetID()) OnFindTexture(5);
	if(ID == FindTexture[6]->GetID()) OnFindTexture(6);
	if(ID == FindTexture[7]->GetID()) OnFindTexture(7);
	if(ID == Apply->GetID()) OnApply();
	if(ID == Refresh->GetID()) OnRefrsh();
	if(ID == Objects->GetID()) OnChangeObject();
	if (ID == AcceptLights->GetID()) 
	{
		if (Material)
		{
			Material->AcceptLights = AcceptLights->GetChecked();
		}
	}
	if(ID ==  XRot->GetID() || ID == YRot->GetID() || ID == ZRot->GetID()) OnChangeRot();

	if (ID == MaterialList->GetID())
	{
		if (MaterialList->GetSelected() && (MaterialList->GetSelected()->GetLevel() == 2))
		{
			String aMaterialName = GetFullItemName(MaterialList->GetSelected());
			ClearAllNames();
			Material = (WeMaterialEditor*)WeMaterialManager::Instance()->Load(aMaterialName, true);
			UpdateComponents();
			OnApply();
		}
	}
	if(ID == BlendType->GetID()) 
	{
		if (Material)
		{
			Material->BlendType = (WEMATERIAL_BLEND_TYPE)BlendType->GetSelectionData();			
		}
	}
}


void CDialogMaterialEditor::OnTexAnimComboClick()
{
	// change param texts 
	Material->TextureAnimation = TexAnimCombo->GetSelectionData();
	switch (TexAnimCombo->GetSelectionData())
	{
	case WETEXTUREANIMATION_NONE: 
		TexAnimParam1Label->SetText("Not used");
		TexAnimParam2Label->SetText("Not used");
		TexAnimParam3Label->SetText("Not used");
		TexAnimParam4Label->SetText("Not used");
		TexAnimParam5Label->SetText("Not used");
		

		break;
	case WETEXTUREANIMATION_SCROLL: 
		TexAnimParam1Label->SetText("U scroll/s");
		TexAnimParam2Label->SetText("V scroll/s");
		TexAnimParam3Label->SetText("Not used");
		TexAnimParam4Label->SetText("Not used");
		TexAnimParam5Label->SetText("Not used");
		break;
	case WETEXTUREANIMATION_SLIDESHOW: 
		TexAnimParam1Label->SetText("Slice w (max 1.0)");
		TexAnimParam2Label->SetText("Slice h (max 1.0");
		TexAnimParam3Label->SetText("Num slices");
		TexAnimParam4Label->SetText("Speed (slice/s)");
		TexAnimParam5Label->SetText("Not used");

		break;

	case WETEXTUREANIMATION_SLIDESHOWRANDOM: 
		TexAnimParam1Label->SetText("Slice w (max 1.0)");
		TexAnimParam2Label->SetText("Slice h (max 1.0");
		TexAnimParam3Label->SetText("Num slices");
		TexAnimParam4Label->SetText("Speed (slice/s)");
		TexAnimParam5Label->SetText("Not used");

		break;
	case WETEXTUREANIMATION_SHAKE: 

		TexAnimParam1Label->SetText("Chaos factor");
		TexAnimParam2Label->SetText("Not used");
		TexAnimParam3Label->SetText("Not used");
		TexAnimParam4Label->SetText("Not used");
		TexAnimParam5Label->SetText("Not used");
		break;

	case WETEXTUREANIMATION_ZOOM: 

		TexAnimParam1Label->SetText("Zoom from");
		TexAnimParam2Label->SetText("Zoom to");
		TexAnimParam3Label->SetText("Speed (cycle/s)");
		TexAnimParam4Label->SetText("Not used");
		TexAnimParam5Label->SetText("Not used");
		break;

	case WETEXTUREANIMATION_UVSCALE:
		TexAnimParam1Label->SetText("U scale");
		TexAnimParam2Label->SetText("V scale");
		TexAnimParam3Label->SetText("W scale");
		TexAnimParam4Label->SetText("Not used");
		TexAnimParam5Label->SetText("Not used");
		

		break;

	}
}


void CDialogMaterialEditor::OnChange(DWORD ID)
{
	if(!Created) return;

	if(ID ==  XRot->GetID() || ID == YRot->GetID() || ID == ZRot->GetID()) OnChangeRot();

	if(ID == TexAnimParam1->GetID())
	{
		if (TexAnimParam1->GetTextAsFloat() >= -100.0f && TexAnimParam1->GetTextAsFloat() < 100.0f)
			Material->TextureAnimParam1 = TexAnimParam1->GetTextAsFloat();
	}
	if(ID == TexAnimParam2->GetID())
	{
		if (TexAnimParam2->GetTextAsFloat() >= -100.0f && TexAnimParam2->GetTextAsFloat() < 100.0f)
			Material->TextureAnimParam2 = TexAnimParam2->GetTextAsFloat();
	}
	if(ID == TexAnimParam3->GetID())
	{
		if (TexAnimParam3->GetTextAsFloat() >= -100.0f && TexAnimParam3->GetTextAsFloat() < 100.0f)
			Material->TextureAnimParam3 = TexAnimParam3->GetTextAsFloat();
	}
	if(ID == TexAnimParam4->GetID())
	{
		if (TexAnimParam4->GetTextAsFloat() >= 0.0f && TexAnimParam4->GetTextAsFloat() < 100.0f)
			Material->TextureAnimParam4 = TexAnimParam4->GetTextAsFloat();
	}
	if(ID == TexAnimParam5->GetID())
	{
		if (TexAnimParam5->GetTextAsFloat() >= 0.0f && TexAnimParam5->GetTextAsFloat() < 100.0f)
			Material->TextureAnimParam5 = TexAnimParam5->GetTextAsFloat();
	}
	if (ID == SpecularPow->GetID())
	{
		Material->SpecularPow = SpecularPow->GetTextAsFloat();
	}
	if (ID == BumpIntensity->GetID())
	{
		Material->BumpIntensity = BumpIntensity->GetTextAsFloat();
	}
	if (ID == AlphaRef->GetID())
	{
		Material->AlphaRef = AlphaRef->GetTextAsInt();
	}
	if (ID == MaterialAlpha->GetID())
	{
		Material->MaterialColor.a = MaterialAlpha->GetTextAsFloat() / 255.0f;
	}

	if(ID == MatParam1->GetID())	Material->Param1 = MatParam1->GetTextAsFloat();
	if(ID == MatParam2->GetID())	Material->Param2 = MatParam2->GetTextAsFloat();
	if(ID == MatParam3->GetID())	Material->Param3 = MatParam3->GetTextAsFloat();
	if(ID == MatParam4->GetID())	Material->Param4 = MatParam4->GetTextAsFloat();

}


void CDialogMaterialEditor::OnApply()
{
	if(EffectName->GetText().GetLen() == 0)
	{
		MessageBox(GetHWND(),"You didn't set Effect for this material...",0,0);
		return;
	}

	for(BoxPreview->Model->Mesh->Materials->First(); !BoxPreview->Model->Mesh->Materials->End(); BoxPreview->Model->Mesh->Materials->Next())
	{
		BoxPreview->Model->Mesh->Materials->GetItem() = (WeMaterial*)Material;
	}

	for(BoxPreviewNMap->Model->Mesh->Materials->First(); !BoxPreviewNMap->Model->Mesh->Materials->End(); BoxPreviewNMap->Model->Mesh->Materials->Next())
	{
		BoxPreviewNMap->Model->Mesh->Materials->GetItem() = (WeMaterial*)Material;
	}

	for(SpherePreview->Model->Mesh->Materials->First(); !SpherePreview->Model->Mesh->Materials->End(); SpherePreview->Model->Mesh->Materials->Next())
	{
		SpherePreview->Model->Mesh->Materials->GetItem() = (WeMaterial*)Material;
	}

	for(SpherePreviewNMap->Model->Mesh->Materials->First(); !SpherePreviewNMap->Model->Mesh->Materials->End(); SpherePreviewNMap->Model->Mesh->Materials->Next())
	{
		SpherePreviewNMap->Model->Mesh->Materials->GetItem() = (WeMaterial*)Material;
	}
}


void CDialogMaterialEditor::OnRefrsh()
{
	XRotation = false;
	XRot->SetChecked(false);
	YRotation = false;
	YRot->SetChecked(false);
	ZRotation = false;
	ZRot->SetChecked(false);

	BoxPreview->Yaw = 0;
	BoxPreview->Pitch = 0;
	BoxPreview->Roll = 0;

	BoxPreviewNMap->Yaw = 0;
	BoxPreviewNMap->Pitch = 0;
	BoxPreviewNMap->Roll = 0;

	SpherePreview->Yaw = 0;
	SpherePreview->Pitch = 0;
	SpherePreview->Roll = 0;

	SpherePreviewNMap->Yaw = 0;
	SpherePreviewNMap->Pitch = 0;
	SpherePreviewNMap->Roll = 0;

	Objects->SetSelection(3);
	OnChangeObject();
}



void CDialogMaterialEditor::OnChangeRot()
{
	if(XRot->GetChecked())
		XRotation = true;
	else
		XRotation = false;

	if(YRot->GetChecked())
		YRotation = true;
	else
		YRotation = false;

	if(ZRot->GetChecked())
		ZRotation = true;
	else
		ZRotation = false;
}



void CDialogMaterialEditor::OnChangeObject()
{
	int Sel = Objects->GetSelectionIndex();

	if(Sel == 0)
	{ 
		BoxPreview->Renderable = true; 
		SpherePreview->Renderable = false; 
		BoxPreviewNMap->Renderable = false;
		SpherePreviewNMap->Renderable = false;
	}
	if(Sel == 1)
	{
		BoxPreview->Renderable = false; 
		SpherePreview->Renderable = true; 
		BoxPreviewNMap->Renderable = false;
		SpherePreviewNMap->Renderable = false;
	}

	if(Sel == 2)
	{
		BoxPreview->Renderable = false; 
		SpherePreview->Renderable = false; 
		BoxPreviewNMap->Renderable = true;
		SpherePreviewNMap->Renderable = false;
	}
	if(Sel == 3)
	{
		BoxPreview->Renderable = false; 
		SpherePreview->Renderable = false; 
		BoxPreviewNMap->Renderable = false;
		SpherePreviewNMap->Renderable = true;
	}
}



void CDialogMaterialEditor::InitializeWorldAndObjects()
{
	MEWorld = new WeWorld();

	MECamera = new WeCamera();
	MECamera->Location = WeVector(0.0f,0.0f,-15.0f);

	SpherePreviewNMap = new WeActor();
	D3DXMatrixIdentity(&SpherePreviewNMap->TM);
	SpherePreviewNMap->Renderable = false;
	SpherePreviewNMap->Model = WeModelManager::Instance()->Load("MapEditor.Geometry.SphereNMap");

	BoxPreview = new WeActor();
	D3DXMatrixIdentity(&BoxPreview->TM);
	BoxPreview->Renderable = true;
	BoxPreview->Model = WeModelManager::Instance()->Load("MapEditor.Geometry.Box");

	BoxPreviewNMap = new WeActor();
	D3DXMatrixIdentity(&BoxPreviewNMap->TM);
	BoxPreviewNMap->Renderable = true;
	BoxPreviewNMap->Model = WeModelManager::Instance()->Load("MapEditor.Geometry.BoxNMap");

	SpherePreview = new WeActor();
	D3DXMatrixIdentity(&SpherePreview->TM);
	SpherePreview->Renderable = false;
	SpherePreview->Model = WeModelManager::Instance()->Load("MapEditor.Geometry.Sphere");


	MEWorld->AddActor(SpherePreviewNMap);
	MEWorld->AddActor(SpherePreview);
	MEWorld->AddActor(BoxPreview);
	MEWorld->AddActor(BoxPreviewNMap);

	WeLight *l = new WeLight;
	WeLightState ls;
	ls.Diffuse.r = 1.0f;
	ls.Diffuse.g = 1.0f;
	ls.Diffuse.b = 1.0f;
	ls.Diffuse.a = 1.0f;

	ls.Range = 1000;
	l->Location = WeVector(0, 20, -10);
	l->AddKeyState(ls, 100);
	l->AffectDynamicActors = true;
	
	MEWorld->AddActor(l);
}




void CDialogMaterialEditor::UpdateRotations()
{
	if(XRotation)
	{
		BoxPreview->Pitch = BoxPreview->Pitch + (D3DX_PI *2) / 500;
		SpherePreview->Pitch = SpherePreview->Pitch + (D3DX_PI *2) / 500;
		BoxPreviewNMap->Pitch = SpherePreview->Pitch + (D3DX_PI *2) / 500;
		SpherePreviewNMap->Pitch = SpherePreview->Pitch + (D3DX_PI *2) / 500;
	}

	if(YRotation)
	{
		BoxPreview->Yaw = BoxPreview->Yaw + (D3DX_PI *2) / 500;
		SpherePreview->Yaw = BoxPreview->Yaw + (D3DX_PI *2) / 500;
		BoxPreviewNMap->Yaw = BoxPreview->Yaw + (D3DX_PI *2) / 500;
		SpherePreviewNMap->Yaw = BoxPreview->Yaw + (D3DX_PI *2) / 500;
	}

	if(ZRotation)
	{
		BoxPreview->Roll = BoxPreview->Roll + (D3DX_PI *2) / 500;
		SpherePreview->Roll = BoxPreview->Roll + (D3DX_PI *2) / 500;
		BoxPreviewNMap->Roll = BoxPreview->Roll + (D3DX_PI *2) / 500;
		SpherePreviewNMap->Roll = BoxPreview->Roll + (D3DX_PI *2) / 500;
	}
}




void CDialogMaterialEditor::ClearAllNames()
{
	EffectName->SetText("");
	for (int i = 0; i < 8; i++)
	{
		TexName[i]->SetText("");
	}
}


void CDialogMaterialEditor::OnExit()
{
	ClearAllNames();
	SetVisible(false);
}


void CDialogMaterialEditor::UpdateComponents()
{
	MaterialType->SetSelection(Material->Type);
	MaterialReflection->SetSelection(Material->ReflectionType);

	if (!Material->Shader)
	{
		Material->Shader = WeShaderManager::Instance()->DefaultShader;
	}
	EffectName->SetText(Material->Shader->Name);		

	for (int i = 0; i < 8; i++)
	{
		if(Material->Textures[i]) TexName[i]->SetText(Material->Textures[i]->Name);
	}

	char temp[100];
	sprintf(temp, "R: %d G: %d B: %d",
		(int)(Material->MaterialColor.r * 255.0f + 0.49f),
		(int)(Material->MaterialColor.g * 255.0f + 0.49f),
		(int)(Material->MaterialColor.b * 255.0f + 0.49f));
	MatColorLabel->SetText(temp);

	if (TexName[1]->GetText().GetLen() == 0) 
	{
		TexName[1]->SetText("MapEditor.Textures.DefaultNormalMapBlue1x1");
		Material->Textures[1] = WeTextureManager::Instance()->Load(TexName[1]->GetText(), true);
	}
	if (TexName[2]->GetText().GetLen() == 0) 
	{
		TexName[2]->SetText("MapEditor.Textures.DefaultWhite1x1");
		Material->Textures[2] = WeTextureManager::Instance()->Load(TexName[2]->GetText(), true);
	}
	if (TexName[3]->GetText().GetLen() == 0) 
	{
		TexName[3]->SetText("MapEditor.Textures.DefaultBlack1x1");
		Material->Textures[3] = WeTextureManager::Instance()->Load(TexName[3]->GetText(), true);
	}

	AcceptLights->SetChecked(Material->AcceptLights);
	BlendType->SetSelection(Material->BlendType);
	TexAnimCombo->SetSelection(Material->TextureAnimation);
	TexAnimParam1->SetText(Material->TextureAnimParam1);
	TexAnimParam2->SetText(Material->TextureAnimParam2);
	TexAnimParam3->SetText(Material->TextureAnimParam3);
	TexAnimParam4->SetText(Material->TextureAnimParam4);
	TexAnimParam5->SetText(Material->TextureAnimParam5);
	BumpIntensity->SetText(Material->BumpIntensity);
	SpecularPow->SetText(Material->SpecularPow);
	Glowing->SetChecked(Material->Glowing);
	DoubleSided->SetChecked(Material->DoubleSided);
	//Refractive->SetChecked(Material->Refractive);
	AlphaRef->SetText((int)Material->AlphaRef);
	MaterialAlpha->SetText(Material->MaterialColor.a*255.0f);

	OnTexAnimComboClick();

	MatParam1->SetText(Material->Param1);
	MatParam2->SetText(Material->Param2);
	MatParam3->SetText(Material->Param3);
	MatParam4->SetText(Material->Param4);
}


void CDialogMaterialEditor::OnPickColor()
{
	CHOOSECOLOR col;
	COLORREF colRef[16];
	col.lStructSize = sizeof(col);
	col.hwndOwner = GetHWND();
	col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	col.lpCustColors = colRef;
	col.lpTemplateName = "Pick Color";				
	col.rgbResult = RGB(Material->MaterialColor.r * 255.0f, Material->MaterialColor.g * 255.0f, Material->MaterialColor.b * 255.0f);
	
	if (ChooseColor(&col))
	{
		//char tmp[100];
		int R, G, B;
		R = GetRValue(col.rgbResult);
		G = GetGValue(col.rgbResult);
		B = GetBValue(col.rgbResult);
		
		Material->MaterialColor.r = (float)R / 255.0f;
		Material->MaterialColor.g = (float)G / 255.0f;
		Material->MaterialColor.b = (float)B / 255.0f;

		UpdateComponents();
	}
}





void CDialogMaterialEditor::OnSaveMaterial(StringParam SaveName)
{
	String Name = SaveName;

	Material->Type = MaterialType->GetSelectionIndex();
	Material->ReflectionType = MaterialReflection->GetSelectionIndex();
	
	if (Name.GetLen() == 0)
	{
		SaveDialog->Type = PACKAGEITEM_MATERIAL;
		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			Name = SaveDialog->HoleName;
		}
	}

	if (Name.GetLen() != 0)
	{
		bool ok = false;

		if (Material->Save(Name, (SaveName.GetLen() != 0)))
		{
			ok = true;
		}
		else
		{
			if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
			{
				ok = Material->Save(Name, true);
			}
		}

		if (ok)
		{
			CTreeViewItem *Item = FindItem(MaterialList, Name);
			if (!Item)
				AddItem(MaterialList, Name);
		}
	}
}


void CDialogMaterialEditor::OnNewMaterial()
{
	if(IDYES == MessageBox(GetHWND(), "Are you sure you want to create new material?","New material",MB_YESNO | MB_ICONQUESTION))
	{
		ClearAllNames();
		Material = new WeMaterialEditor;
		UpdateComponents();
		OnApply();
	}
}



void CDialogMaterialEditor::OnSetTexture(int i)
{
	if (Material && TextureList->GetSelected() && (TextureList->GetSelected()->GetLevel() == 2))
	{
		String aTextureName = GetFullItemName(TextureList->GetSelected());

		TexName[i]->SetText(aTextureName);
		Material->Textures[i] = WeTextureManager::Instance()->Load(TexName[i]->GetText(), true);

		if (i == 0)
		{
			int p = aTextureName.SubPosCaseInsensitive("Diffuse");
			if (p == aTextureName.GetLen() - 7)
			{
				aTextureName = aTextureName.Sub(0, p);

				if (FindItem(TextureList, aTextureName + "Normal"))
				{
					TexName[1]->SetText(aTextureName + "Normal");
					Material->Textures[1] = WeTextureManager::Instance()->Load(TexName[1]->GetText(), true);
				}
				else if (FindItem(TextureList, aTextureName + "normal"))
				{
					TexName[1]->SetText(aTextureName + "normal");
					Material->Textures[1] = WeTextureManager::Instance()->Load(TexName[1]->GetText(), true);
				}

				if (FindItem(TextureList, aTextureName + "Specular"))
				{
					TexName[2]->SetText(aTextureName + "Specular");
					Material->Textures[2] = WeTextureManager::Instance()->Load(TexName[2]->GetText(), true);
				}
				else if (FindItem(TextureList, aTextureName + "specular"))
				{
					TexName[2]->SetText(aTextureName + "specular");
					Material->Textures[2] = WeTextureManager::Instance()->Load(TexName[2]->GetText(), true);
				}

				if (FindItem(TextureList, aTextureName + "Glow"))
				{
					TexName[3]->SetText(aTextureName + "Glow");
					Material->Textures[3] = WeTextureManager::Instance()->Load(TexName[3]->GetText(), true);
				}
				else if (FindItem(TextureList, aTextureName + "glow"))
				{
					TexName[3]->SetText(aTextureName + "glow");
					Material->Textures[3] = WeTextureManager::Instance()->Load(TexName[3]->GetText(), true);
				}

				if (FindItem(TextureList, aTextureName + "Env"))
				{
					TexName[4]->SetText(aTextureName + "Env");
					Material->Textures[4] = WeTextureManager::Instance()->Load(TexName[4]->GetText(), true);
				}
				else if (FindItem(TextureList, aTextureName + "env"))
				{
					TexName[4]->SetText(aTextureName + "env");
					Material->Textures[4] = WeTextureManager::Instance()->Load(TexName[4]->GetText(), true);
				}
			}
		}
	}
}


void CDialogMaterialEditor::OnFindTexture(int i)
{
	if (Material)
	{
		CTreeViewItem *Item;
		Item = FindItem(TextureList, TexName[i]->GetText());
		if (Item)
			TextureList->SelectItem(Item);
	}
}


void CDialogMaterialEditor::OnEffectBrowse()
{
	if (EffectList->GetSelected() && (EffectList->GetSelected()->GetLevel() == 2))
	{
		String aEffectName = GetFullItemName(EffectList->GetSelected());
		EffectName->SetText(aEffectName);
		Material->Shader = WeShaderManager::Instance()->Load(EffectName->GetText(), false, true);
	}
}

