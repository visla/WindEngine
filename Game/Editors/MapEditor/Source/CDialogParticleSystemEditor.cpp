#include <CDialogParticleSystemEditor.h>
#include <CMapEditorWindow.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>
#include <WeMaterialManager.h>
#include <Commdlg.h>



//========================================================
//    ParticleSystemEditor Class functions
//=======================================================


CDialogParticleSystemEditor* ParticleSystemEditor = NULL;

CDialogParticleSystemEditor::CDialogParticleSystemEditor()
{
}




bool CDialogParticleSystemEditor::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_PARTICLESYSTEMEDITOR, hParent, Shown))
	{
		return false;
	}


	Close = new CButton(this, IDC_CLOSE);
	Browse = new CButton(this, IDC_MATERIALBROWSE);
	ColorFromChose = new CButton(this, IDC_COLORFROMCHOSEB);
	ColorToChose = new CButton(this, IDC_COLORTOCHOSEB);

	MaxParticles = new CEditBox(this, IDC_MAXPARTICLES);
	EmitRate = new CEditBox(this, IDC_EMITRATE);
	SpownVolumeSize = new CEditBox(this, IDC_SPOWNVOLUMESIZE);
	AlphaFrom = new CEditBox(this, IDC_ALPHAFROM);
	AlphaTo = new CEditBox(this, IDC_ALPHATO);
	GrowFrom = new CEditBox(this, IDC_GROWFROM);
	GrowTo = new CEditBox(this, IDC_GROWTO);
	GravityX = new CEditBox(this, IDC_GRAVITYX);
	GravityY = new CEditBox(this, IDC_GRAVITYY);
	GravityZ = new CEditBox(this, IDC_GRAVITYZ);
	StartVelociy = new CEditBox(this, IDC_STARTVELOCITY);
	Spread = new CEditBox(this, IDC_SPREAD);
	LifeTime = new CEditBox(this, IDC_LIFETIME);
	Material = new CEditBox(this, IDC_MATERIALNAME);
	SpawingType = new CComboBox(this, IDC_SPAWINGTYPE);
	SpawingType->AddItem("Point");
	SpawingType->AddItem("Line");
	SpawingType->AddItem("Plane");
	SpawingType->AddItem("Sphere");
	MovingType = new CComboBox(this, IDC_MOVINGTYPE);
	MovingType->AddItem("NONE");
	MovingType->AddItem("Directional");
	MovingType->AddItem("Spherical");
	MovingType->AddItem("Chaotic");
	ColorFromLabel = new CLabel(this, IDC_COLORFROMLABEL);
	ColorFromLabel->SetText("R: 255, G: 255, B: 255");
	ColorToLabel = new CLabel(this, IDC_COLORTOLABEL);
	ColorToLabel->SetText("R: 0, G: 0, B: 0");
	PlaneWidth = new CEditBox(this, IDC_SPOWNPLANEWIDTH);
	PlaneHeight = new CEditBox(this, IDC_SPOWNPLANEHEIGHT);
	LineX = new CEditBox(this, IDC_SPOWNLINEX);
	LineY = new CEditBox(this, IDC_SPOWNLINEY);
	LineZ = new CEditBox(this, IDC_SPOWNLINEZ);
	DirectionX = new CEditBox(this, IDC_SPAWNDIRECTIONX);
	DirectionY = new CEditBox(this, IDC_SPAWNDIRECTIONY);
	DirectionZ = new CEditBox(this, IDC_SPAWNDIRECTIONZ);

	ShowHideButton = new CButton(this, IDC_PREVIEW);
	Preview = new CButton(this, IDC_PREVIEWAREA);

	PSEditorWorld = new WeWorld();
	PSEditorCamera = new WeCamera();
	PSEditorCamera->Location = WeVector(0.0,0.0,-10.0);
	PSEditorCamera->Direction = WeVector(0.0,0.0,1.0);

	PreY = 0;

	ParticleSystem = NULL;
	Created = false;
	MouseMove = false;
	ParticleSystem = NULL;
	
	this->SetSize(485,GetHeight());
	return true;
}



void CDialogParticleSystemEditor::OnMouseMove(int x, int y)
{
	if(!MouseMove) return;

	if (PreY == 0) { PreY = y; return; }
	int tmpY = y - PreY;

	PSEditorCamera->Location.z += (float)tmpY / 10.0f;
	PreY = y;
}



void CDialogParticleSystemEditor::OnClick(DWORD ID)
{
	if(ID == Close->GetID() || ID == IDM_PARTICLEEDITOR_EXIT) OnCloseClick();
	
	if(ID == IDM_PARTICLEEDITOR_NEW) OnNew();
	if(ID == IDM_PARTICLEEDITOR_LOAD) OnLoad();
	if(ID == IDM_PARTICLEEDITOR_SAVE) OnSave();
	if(ID == IDC_PREVIEW) OnShowHide();

	if(!ParticleSystem)
	{
		MessageBox(NULL,"You need to create particle first!",0,0);
		return;

	}
	if(ID == Browse->GetID()) OnBrowse();
	if(ID == IDC_PREVIEWAREA) OnPreviewClick();
	if(ID == IDC_COLORFROMCHOSEB) OnColorFromChose();
	if(ID == IDC_COLORTOCHOSEB) OnColorToChose();
	if(SpawingType->GetID() == ID)
	{
		ParticleSystem->Properties.SpawnType = SpawingType->GetSelectionIndex();
	}
	if(MovingType->GetID() == ID)
	{
		ParticleSystem->Properties.MoveType = MovingType->GetSelectionIndex();
	}
}



void CDialogParticleSystemEditor::OnShowHide()
{
	if(ShowHideButton->GetText() == (String)"Show Preview >>")
	{
		ShowHideButton->SetText("Hide Preview <<");
		SetSize(800, GetHeight());
	}else{
		ShowHideButton->SetText("Show Preview >>");
		SetSize(485,GetHeight());
	}
}


void CDialogParticleSystemEditor::OnPreviewClick()
{
	if(!MouseMove) 
		MouseMove = true;
	else
	{
		MouseMove = false;
		PreY = 0;
	}
	// TO DO:
	// Write code here...
}



void CDialogParticleSystemEditor::OnCloseClick()
{
	SetVisible(false);
}


void CDialogParticleSystemEditor::OnBrowse()
{
	InsertDialog->Type = PACKAGEITEM_MATERIAL;
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		Material->SetText(InsertDialog->HoleName);
		ParticleSystem->Properties.Material = WeMaterialManager::Instance()->Load(InsertDialog->HoleName);
		ParticleSystem->Properties.MaterialName = InsertDialog->HoleName;
	}
}


void CDialogParticleSystemEditor::OnLoad()
{
	if(ParticleSystem)
	{
		PSEditorWorld->RemoveActor(ParticleSystem);
	}
	ParticleSystem = new WeParticleSystem();

	InsertDialog->Type = PACKAGEITEM_PARTICLESYS;
	InsertDialog->Extension = "*.ppp";
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		ParticleSystem->LoadPreset(InsertDialog->HoleName);

		char tmp[100];
		int R, G, B;
		R = (int)(ParticleSystem->Properties.ColorFrom.r * 255.0f);
		G = (int)(ParticleSystem->Properties.ColorFrom.g * 255.0f);			
		B = (int)(ParticleSystem->Properties.ColorFrom.b * 255.0f);
		sprintf(tmp, "R: %d, G: %d, B: %d", R, G, B);
		ColorFromLabel->SetText(tmp);

		R = (int)(ParticleSystem->Properties.ColorTo.r * 255.0f);
		G = (int)(ParticleSystem->Properties.ColorTo.g * 255.0f);			
		B = (int)(ParticleSystem->Properties.ColorTo.b * 255.0f);
		sprintf(tmp, "R: %d, G: %d, B: %d", R, G, B);
		ColorToLabel->SetText(tmp);

		ParticleSystem->Properties.Material = WeMaterialManager::Instance()->Load(ParticleSystem->Properties.MaterialName);
	}

	PSEditorWorld->AddActor(ParticleSystem);
	RefreshItems();
	Created = true;
}


void CDialogParticleSystemEditor::OnSave()
{

	bool Res = false;
	while(!Res)
	{
		SaveDialog->Type = PACKAGEITEM_PARTICLESYS;
		SaveDialog->Extension = "*.ppp";
		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			if(ParticleSystem->SavePreset(SaveDialog->HoleName))
			{
				Res = true;   // Save is OK!!!
			}else{
				if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save", MB_YESNO | MB_ICONQUESTION))
				{
					ParticleSystem->SavePreset(SaveDialog->HoleName, true);
					Res = true;		// Save is OK!!!
				}else Res = false;	// Don't wan't to replase so enter new name
			}
		}else Res = true;  //  Cancel was pressed
	}
}


void CDialogParticleSystemEditor::OnNew()
{
	if(ParticleSystem)
		PSEditorWorld->RemoveActor(ParticleSystem);
	ParticleSystem = new WeParticleSystem();

	ParticleSystem->Properties.Material = WeMaterialManager::Instance()->Load("Materials.Default.Default");
	ParticleSystem->Properties.MaterialName = "Materials.Default.Default";
	
	RefreshItems();

	PSEditorWorld->AddActor(ParticleSystem);
	Created = true;
}




void CDialogParticleSystemEditor::RefreshItems()
{
	MaxParticles->SetText(ParticleSystem->Properties.MaxParticles);
	EmitRate->SetText(ParticleSystem->Properties.EmitRate);
	SpownVolumeSize->SetText(ParticleSystem->Properties.SpawnVolumeSize);
	AlphaFrom->SetText(ParticleSystem->Properties.AlphaFrom);
	AlphaTo->SetText(ParticleSystem->Properties.AlphaTo);
	GrowFrom->SetText(ParticleSystem->Properties.SizeFrom);
	GrowTo->SetText(ParticleSystem->Properties.SizeTo);
	GravityX->SetText(ParticleSystem->Properties.Gravity.x);
	GravityY->SetText(ParticleSystem->Properties.Gravity.y);
	GravityZ->SetText(ParticleSystem->Properties.Gravity.z);
	StartVelociy->SetText(ParticleSystem->Properties.StartVelocity);
	Spread->SetText(ParticleSystem->Properties.Spread);
	LifeTime->SetText(ParticleSystem->Properties.LifeTime);
	Material->SetText(ParticleSystem->Properties.MaterialName);
	SpawingType->SetSelection(ParticleSystem->Properties.SpawnType);
	MovingType->SetSelection(ParticleSystem->Properties.MoveType);
	PlaneWidth->SetText(ParticleSystem->Properties.SpawnPlaneWidth);
	PlaneHeight->SetText(ParticleSystem->Properties.SpawnPlaneHeight);
	DirectionX->SetText(ParticleSystem->Properties.ParticleDirection.x);
	DirectionY->SetText(ParticleSystem->Properties.ParticleDirection.y);
	DirectionZ->SetText(ParticleSystem->Properties.ParticleDirection.z);
	LineX->SetText(ParticleSystem->Properties.SpawnLine.x);
	LineY->SetText(ParticleSystem->Properties.SpawnLine.y);
	LineZ->SetText(ParticleSystem->Properties.SpawnLine.z);
}




void CDialogParticleSystemEditor::OnChange(DWORD ID)
{
	if(!Created) return;
	if(!ParticleSystem)
	{
		MessageBox(GetHWND(),"You need to create particle first!",0,0);
		return;
	}

	if(Spread->GetID() == ID) 
	{
		ParticleSystem->Properties.Spread = Spread->GetTextAsFloat();
	}

	if(LifeTime->GetID() == ID) 
	{
		ParticleSystem->Properties.LifeTime = LifeTime->GetTextAsFloat();
	}

	if(EmitRate->GetID() == ID) 
	{
		ParticleSystem->Properties.EmitRate = (int)EmitRate->GetTextAsFloat();
	}

	if(GravityX->GetID()  == ID || GravityY->GetID()  == ID || GravityZ->GetID() == ID)
	{
		ParticleSystem->Properties.Gravity.x = GravityX->GetTextAsFloat();
		ParticleSystem->Properties.Gravity.y = GravityY->GetTextAsFloat();
		ParticleSystem->Properties.Gravity.z = GravityZ->GetTextAsFloat();
	}

	if(StartVelociy->GetID() == ID)
	{
		ParticleSystem->Properties.StartVelocity = StartVelociy->GetTextAsFloat();
	}

	if(GrowFrom->GetID() == ID)
	{
		ParticleSystem->Properties.SizeFrom = GrowFrom->GetTextAsFloat();
	}

	if(GrowTo->GetID() == ID)
	{
		ParticleSystem->Properties.SizeTo = GrowTo->GetTextAsFloat();
	}

	if(MaxParticles->GetID() == ID)
	{
		if(MaxParticles->GetTextAsFloat() > EmitRate->GetTextAsFloat())
		{
			ParticleSystem->setMaxParticles((int)MaxParticles->GetTextAsFloat());
		}
	}

	if(SpownVolumeSize->GetID() == ID)
	{
		ParticleSystem->Properties.SpawnVolumeSize = SpownVolumeSize->GetTextAsFloat();
	}

	if(PlaneWidth->GetID() == ID)
	{
		ParticleSystem->Properties.SpawnPlaneWidth = PlaneWidth->GetTextAsFloat();
	}

	if(PlaneHeight->GetID() == ID)
	{
		ParticleSystem->Properties.SpawnPlaneHeight = PlaneHeight->GetTextAsFloat();
	}

	if(AlphaFrom->GetID() == ID)
	{
		ParticleSystem->Properties.AlphaFrom = AlphaFrom->GetTextAsFloat();
	}

	if(AlphaTo->GetID() == ID)
	{
		ParticleSystem->Properties.AlphaTo = AlphaTo->GetTextAsFloat();
	}

	if(LineX->GetID()  == ID || LineY->GetID()  == ID || LineZ->GetID() == ID)
	{
		ParticleSystem->Properties.SpawnLine.x = LineX->GetTextAsFloat();
		ParticleSystem->Properties.SpawnLine.y = LineY->GetTextAsFloat();
		ParticleSystem->Properties.SpawnLine.z = LineZ->GetTextAsFloat();
	}

	if(DirectionX->GetID()  == ID || DirectionY->GetID()  == ID || DirectionZ->GetID() == ID)
	{
		ParticleSystem->Properties.ParticleDirection.x = DirectionX->GetTextAsFloat();
		ParticleSystem->Properties.ParticleDirection.y = DirectionY->GetTextAsFloat();
		ParticleSystem->Properties.ParticleDirection.z = DirectionZ->GetTextAsFloat();
	}
}



void CDialogParticleSystemEditor::OnColorFromChose()
{
	CHOOSECOLOR col;
	COLORREF colRef[16];
	col.lStructSize = sizeof(col);
	col.hwndOwner = GetHWND();
	col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	col.lpCustColors = colRef;
	col.lpTemplateName = "Pick Color";				
	col.rgbResult = RGB(ParticleSystem->Properties.ColorFrom.r*255.0f, ParticleSystem->Properties.ColorFrom.g*255.0f, ParticleSystem->Properties.ColorFrom.b*255.0f);
	
	if (ChooseColor(&col))
	{
		char tmp[100];
		int R, G, B;
		R = GetRValue(col.rgbResult);
		G = GetGValue(col.rgbResult);			
		B = GetBValue(col.rgbResult);
		ParticleSystem->Properties.ColorFrom.r = R / 255.0f;
		ParticleSystem->Properties.ColorFrom.g = G / 255.0f;
		ParticleSystem->Properties.ColorFrom.b = B / 255.0f;

		sprintf(tmp, "R: %d, G: %d, B: %d", R, G, B);
		ColorFromLabel->SetText(tmp);
	}
}


void CDialogParticleSystemEditor::OnColorToChose()
{
	CHOOSECOLOR col;
	COLORREF colRef[16];
	col.lStructSize = sizeof(col);
	col.hwndOwner = GetHWND();
	col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	col.lpCustColors = colRef;
	col.lpTemplateName = "Pick Color";				
	col.rgbResult = RGB(ParticleSystem->Properties.ColorTo.r*255.0f, ParticleSystem->Properties.ColorTo.g*255.0f, ParticleSystem->Properties.ColorTo.b*255.0f);
	
	if (ChooseColor(&col))
	{
		char tmp[100];
		int R, G, B;
		R = GetRValue(col.rgbResult);
		G = GetGValue(col.rgbResult);			
		B = GetBValue(col.rgbResult);
		ParticleSystem->Properties.ColorTo.r = R / 255.0f;
		ParticleSystem->Properties.ColorTo.g = G / 255.0f;
		ParticleSystem->Properties.ColorTo.b = B / 255.0f;

		sprintf(tmp, "R: %d, G: %d, B:% d", R, G, B);
		ColorToLabel->SetText(tmp);
	}
}

