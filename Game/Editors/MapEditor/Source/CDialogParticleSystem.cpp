


#include <CDialogParticleSystem.h>
#include <CMapEditorWindow.h>
#include <commdlg.h>
#include <WeMaterialManager.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>

CDialogParticleSystem *CDialogParticleSystem::_instance = NULL;

CDialogParticleSystem *CDialogParticleSystem::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogParticleSystem;
	return _instance;
}


bool CDialogParticleSystem::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_PARTICLESYSTEMPROPERTIES, hParent, Shown))
	{
		return false;
	}

	Close = new CButton(this, IDC_CLOSE);
	MaterialBrowse = new CButton(this, IDC_MATERIALBROWSE);
	Load = new CButton(this, IDC_LOAD);
	Save = new CButton(this, IDC_SAVE);
	ColorFromButton = new CButton(this, IDC_COLORFROMCHOOSE);
	ColorToButton = new CButton(this, IDC_COLORTOCHOOSE);

	SpawnType = new CComboBox(this, IDC_SPAWNTYPECOMBO);
	ParticleMoveType = new CComboBox(this, IDC_MOVINGTYPECOMBO);

	SpawnParam1 = new CEditBox(this, IDC_SPAWNPARAM1);
	SpawnParam2 = new CEditBox(this, IDC_SPAWNPARAM2);
	SpawnParam3 = new CEditBox(this, IDC_SPAWNPARAM3);
	SpawnParam4 = new CEditBox(this, IDC_SPAWNPARAM4);

	SpawnDirectionX = new CEditBox(this, IDC_SPAWNDIRECTIONX);
	SpawnDirectionY = new CEditBox(this, IDC_SPAWNDIRECTIONY);
	SpawnDirectionZ = new CEditBox(this, IDC_SPAWNDIRECTIONZ);

	ColorFromLabel = new CLabel(this, IDC_COLORFROMSTATIC);
	ColorToLabel = new CLabel(this, IDC_COLORTOSTATIC);
        
	MaxParticles = new CEditBox(this, IDC_MAXPARTICLES);
	EmitRate = new CEditBox(this, IDC_EMITRATE);
	EmitRateRandomness = new CEditBox(this, IDC_EMITRATERANDOMNESS);
	AlphaFrom = new CEditBox(this, IDC_ALPHAFROM);
	AlphaTo = new CEditBox(this, IDC_ALPHATO);
	SizeFrom = new CEditBox(this, IDC_GROWFROM);
	SizeTo = new CEditBox(this, IDC_GROWTO);
	SizeRandomness = new CEditBox(this, IDC_SIZERANDOMNESS);
	GravityX = new CEditBox(this, IDC_GRAVITYX);
	GravityY = new CEditBox(this, IDC_GRAVITYY);
	GravityZ = new CEditBox(this, IDC_GRAVITYZ);
	StartVelociy = new CEditBox(this, IDC_STARTVELOCITY);
	StartVelociyRandomness = new CEditBox(this, IDC_STARTVELOCITYRANDOMNESS);
	Spread = new CEditBox(this, IDC_SPREAD);
	LifeTime = new CEditBox(this, IDC_LIFETIME);
	LifeTimeRandomness = new CEditBox(this, IDC_LIFETIMERANDOMNESS);
	Material = new CEditBox(this, IDC_MATERIALNAME);
	ParticleAngle = new CComboBox(this, IDC_PARTICLEANGLE);
	SpawnParam1Label = new CLabel(this, IDC_STATICPARAM1);
	SpawnParam2Label = new CLabel(this, IDC_STATICPARAM2);
	SpawnParam3Label = new CLabel(this, IDC_STATICPARAM3);
	SpawnParam4Label = new CLabel(this, IDC_STATICPARAM4);
	BlendType = new CComboBox(this, IDC_PARTICLEBLENDTYPE);
	SpinFrom = new CEditBox(this, IDC_SPINFROM);
	SpinTo = new CEditBox(this, IDC_SPINTO);
	SpinRandomness = new CEditBox(this, IDC_SPINRANDOMNESS);
	SpinningInLife = new CCheckBox(this, IDC_SPINNINGINLIFE);
	RelativeParticles = new CCheckBox(this, IDC_RELATIVECHECK);
	VisibleSphereRadius = new CEditBox(this, IDC_VISIBLESPHERERADIUS);


	SpawnParam1Label->SetVisible(false);
	SpawnParam2Label->SetVisible(false);
	SpawnParam3Label->SetVisible(false);
	SpawnParam4Label->SetVisible(false);
	// fill spawn types and move types
	ParticleAngle->AddItem("Align to motion", WEPARTICLE_ANGLE_ALIGN_TO_MOTION);
	ParticleAngle->AddItem("Random", WEPARTICLE_ANGLE_RANDOM);
	ParticleAngle->AddItem("Specified", WEPARTICLE_ANGLE_SPECIFY);	
	ParticleAngle->SetSelection(0);	

	BlendType->AddItem("Additive", WEPARTICLE_BLEND_ADDITIVE);
	BlendType->AddItem("Standard", WEPARTICLE_BLEND_STANDARD);
	BlendType->SetSelection(0);

	SpawnType->AddItem("From point", WE_PARTICLE_SPAWNTYPE_POINT);
	SpawnType->AddItem("From line", WE_PARTICLE_SPAWNTYPE_LINE);
	SpawnType->AddItem("From horiz plane", WE_PARTICLE_SPAWNTYPE_HORIZPLANE);
	SpawnType->AddItem("From Sphere", WE_PARTICLE_SPAWNTYPE_SPHERE);
	SpawnType->SetSelection(0);


	ParticleMoveType->AddItem("None", WE_PARTICLE_MOVETYPE_NONE);
	ParticleMoveType->AddItem("Directional", WE_PARTICLE_MOVETYPE_DIRECTIONAL);
	ParticleMoveType->AddItem("Spherical", WE_PARTICLE_MOVETYPE_SPHERICAL);
	ParticleMoveType->AddItem("Chaotic", WE_PARTICLE_MOVETYPE_CHAOTIC);
	ParticleMoveType->SetSelection(0);

	CanChange = false;

	return true;
}




void CDialogParticleSystem::OnClick(DWORD ID)
{
	if (ID == Close->GetID()) 
	{
		CanChange = false;
		Hide();
	}
	if (ID == MaterialBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MATERIAL;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			Material->SetText(InsertDialog->HoleName);
		}
	}

	if (ID == BlendType->GetID())
	{
		PS->Properties.BlendType = (WEPARTICLE_BLEND_TYPE)BlendType->GetSelectionData();
	}

	if (ID == RelativeParticles->GetID())
	{
		PS->Properties.RelativePosition = RelativeParticles->GetChecked();
	}
	if (ID == SpinningInLife->GetID())
	{
		PS->Properties.SpinningInLife = SpinningInLife->GetChecked();
	}

	if (ID == SpawnType->GetID())
	{
		PS->Properties.SpawnType = SpawnType->GetSelectionIndex();
		if (SpawnType->GetSelectionData() == WE_PARTICLE_SPAWNTYPE_POINT)
		{
			SpawnParam1Label->SetVisible(false);
			SpawnParam2Label->SetVisible(false);
			SpawnParam3Label->SetVisible(false);
			SpawnParam4Label->SetVisible(false);
		} 
		if (SpawnType->GetSelectionData() == WE_PARTICLE_SPAWNTYPE_SPHERE)
		{
			SpawnParam1Label->SetVisible(true);
			SpawnParam1Label->SetText("sphere size:");
			SpawnParam2Label->SetVisible(false);
			SpawnParam3Label->SetVisible(false);
			SpawnParam4Label->SetVisible(false);
		} 
		if (SpawnType->GetSelectionData() == WE_PARTICLE_SPAWNTYPE_HORIZPLANE)
		{
			SpawnParam1Label->SetVisible(true);
			SpawnParam1Label->SetText("plane width:");
			SpawnParam2Label->SetVisible(true);
			SpawnParam2Label->SetText("plane height:");
			SpawnParam3Label->SetVisible(false);
			SpawnParam4Label->SetVisible(false);
		} 
		if (SpawnType->GetSelectionData() == WE_PARTICLE_SPAWNTYPE_LINE)
		{
			SpawnParam1Label->SetVisible(true);
			SpawnParam1Label->SetText("line x:");
			SpawnParam2Label->SetVisible(true);
			SpawnParam2Label->SetText("line y:");
			SpawnParam3Label->SetVisible(true);
			SpawnParam2Label->SetText("line z:");
			SpawnParam4Label->SetVisible(false);
		} 
	}

	if (ID == ParticleMoveType->GetID())
	{
		PS->Properties.MoveType = ParticleMoveType->GetSelectionIndex();
	}

	if (ID == ParticleAngle->GetID())
	{
		PS->Properties.AngleType = (WEPARTICLE_ANGLE_TYPE)ParticleAngle->GetSelectionData();
	}

	if(ID == Load->GetID())
	{
		OnLoadPresset();
	}

	if(ID == Save->GetID())
	{
		OnSavePresset();
	}


	if (ID == ColorFromButton->GetID())
	{
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(PS->Properties.ColorFrom.r*255.0f, PS->Properties.ColorFrom.g*255.0f, PS->Properties.ColorFrom.b*255.0f);
		
		if (ChooseColor(&col))
		{
			char tmp[100];
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			PS->Properties.ColorFrom.r = R / 255.0f;
			PS->Properties.ColorFrom.g = G / 255.0f;
			PS->Properties.ColorFrom.b = B / 255.0f;

			sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
			ColorFromLabel->SetText(tmp);
		}
	}

	if (ID == ColorToButton->GetID())
	{
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(PS->Properties.ColorTo.r*255.0f, PS->Properties.ColorTo.g*255.0f, PS->Properties.ColorTo.b*255.0f);
		
		if (ChooseColor(&col))
		{
			char tmp[100];
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			PS->Properties.ColorTo.r = R / 255.0f;
			PS->Properties.ColorTo.g = G / 255.0f;
			PS->Properties.ColorTo.b = B / 255.0f;

			sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
			ColorToLabel->SetText(tmp);
		}
	}
}



void CDialogParticleSystem::OnLoadPresset()
{
	InsertDialog->Type = PACKAGEITEM_PARTICLESYS;
	InsertDialog->Extension = "*.ppp";
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		PS->LoadPreset(InsertDialog->HoleName);

		char tmp[100];
		int R, G, B;
		R = (int)(PS->Properties.ColorFrom.r * 255.0f);
		G = (int)(PS->Properties.ColorFrom.g * 255.0f);			
		B = (int)(PS->Properties.ColorFrom.b * 255.0f);
		sprintf(tmp, "R: %d, G: %d, B: %d", R, G, B);
		ColorFromLabel->SetText(tmp);

		R = (int)(PS->Properties.ColorTo.r * 255.0f);
		G = (int)(PS->Properties.ColorTo.g * 255.0f);			
		B = (int)(PS->Properties.ColorTo.b * 255.0f);
		sprintf(tmp, "R: %d, G: %d, B: %d", R, G, B);
		ColorToLabel->SetText(tmp);

		PS->Properties.Material = WeMaterialManager::Instance()->Load(PS->Properties.MaterialName);

		Update(PS);
	}
}



void CDialogParticleSystem::OnSavePresset()
{
	bool Res = false;
	while(!Res)
	{
		SaveDialog->Type = PACKAGEITEM_PARTICLESYS;
		SaveDialog->Extension = "*.ppp";
		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			if(PS->SavePreset(SaveDialog->HoleName))
			{
				Res = true;   // Save is OK!!!
			}else{
				if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save", MB_YESNO | MB_ICONQUESTION))
				{
					PS->SavePreset(SaveDialog->HoleName, true);
					Res = true;		// Save is OK!!!
				}else Res = false;	// Don't wan't to replase so enter new name
			}
		}else Res = true;  //  Cancel was pressed
	}
}



void CDialogParticleSystem::OnChange(DWORD ID)
{
	if (!CanChange) return;

	if (ID == VisibleSphereRadius->GetID())
	{
		PS->Properties.VisibleSphereRadius = VisibleSphereRadius->GetTextAsFloat();
	}
	if (ID == SpinFrom->GetID())
	{
		PS->Properties.SpinFrom = SpinFrom->GetTextAsFloat();
	}
	if (ID == SpinTo->GetID())
	{
		PS->Properties.SpinTo = SpinTo->GetTextAsFloat();
	}
	if (ID == SpinRandomness->GetID())
	{
		PS->Properties.SpinRandomness = SpinRandomness->GetTextAsFloat();
	}



	if (ID == EmitRate->GetID())
	{
		if (EmitRate->GetTextAsInt() > 0 && EmitRate->GetTextAsInt() < 50000)
			PS->Properties.EmitRate = EmitRate->GetTextAsInt();
	}
	if (ID == EmitRateRandomness->GetID())
	{
		PS->Properties.EmitRateRandomness = EmitRateRandomness->GetTextAsInt();
	}

	if (ID == MaxParticles->GetID())
	{
		if (MaxParticles->GetTextAsInt() > PS->Properties.ParticleCount && MaxParticles->GetTextAsInt() < 50000)		
		PS->setMaxParticles(MaxParticles->GetTextAsInt());
	}

	if (ID == LifeTime->GetID())
		PS->Properties.LifeTime = LifeTime->GetTextAsFloat();
	if (ID == LifeTimeRandomness->GetID())
		PS->Properties.LifeTimeRandomness = LifeTimeRandomness->GetTextAsFloat();

	if (ID == Spread->GetID())
	{
		PS->Properties.Spread = Spread->GetTextAsFloat();
	}

	if (ID == StartVelociy->GetID())
		PS->Properties.StartVelocity = StartVelociy->GetTextAsFloat();

	if (ID == StartVelociyRandomness->GetID())
		PS->Properties.StartVelocityRandomness = StartVelociyRandomness->GetTextAsFloat();

	if (ID == SizeFrom->GetID())
	{
		PS->Properties.SizeFrom = SizeFrom->GetTextAsFloat();
	}
	if (ID == SizeTo->GetID())
	{
		PS->Properties.SizeTo = SizeTo->GetTextAsFloat();
	}
	if (ID == SizeRandomness->GetID())
	{
		PS->Properties.SizeRandomenss = SizeRandomness->GetTextAsFloat();
	}
	
	if ((ID == SpawnDirectionX->GetID()) || (ID == SpawnDirectionY->GetID()) || (ID == SpawnDirectionZ->GetID()))
	{
		PS->Properties.ParticleDirection.x = SpawnDirectionX->GetTextAsFloat();
		PS->Properties.ParticleDirection.y = SpawnDirectionY->GetTextAsFloat();
		PS->Properties.ParticleDirection.z = SpawnDirectionZ->GetTextAsFloat();
	}

	if ((ID == GravityX->GetID()) || (ID == GravityY->GetID()) || (ID == GravityZ->GetID()))
	{
		PS->Properties.Gravity.x = GravityX->GetTextAsFloat();
		PS->Properties.Gravity.y = GravityY->GetTextAsFloat();
		PS->Properties.Gravity.z = GravityZ->GetTextAsFloat();
	}

	if (ID == SpawnParam1->GetID())
	{
		PS->Properties.SpawnParam1 = SpawnParam1->GetTextAsFloat();
	}
	if (ID == SpawnParam2->GetID())
	{
		PS->Properties.SpawnParam2 = SpawnParam2->GetTextAsFloat();
	}
	if (ID == SpawnParam3->GetID())
	{
		PS->Properties.SpawnParam3 = SpawnParam3->GetTextAsFloat();
	}
	if (ID == SpawnParam4->GetID())
	{
		PS->Properties.SpawnParam4 = SpawnParam4->GetTextAsFloat();
	}


	if (ID == Material->GetID())
	{
		PS->Properties.MaterialName = Material->GetText();
		PS->Initialize();
		//PS->Material = WeMaterialManager::Instance()->Load(Material->GetText());
	}

	if (ID == AlphaFrom->GetID())
	{
		PS->Properties.AlphaFrom = AlphaFrom->GetTextAsFloat();
	}

	if (ID == AlphaTo->GetID())
	{
		PS->Properties.AlphaTo = AlphaTo->GetTextAsFloat();
	}


	//TO DO:
	//Add code here...
}



void CDialogParticleSystem::Show(WeParticleSystem *ps)
{
	if (!ps) return;
	PS = ps;
	Update(PS);    		
	CDialog::Show();
}


void CDialogParticleSystem::Update(WeParticleSystem *ps)
{
	CanChange = false;

	SpawnType->SetSelection(ps->Properties.SpawnType);
	ParticleMoveType->SetSelection(ps->Properties.MoveType);
	MaxParticles->SetText(ps->Properties.MaxParticles);
	EmitRate->SetText(ps->Properties.EmitRate);
	EmitRateRandomness->SetText(ps->Properties.EmitRateRandomness);
	StartVelociy->SetText(ps->Properties.StartVelocity);
	StartVelociyRandomness->SetText(ps->Properties.StartVelocityRandomness);
	Spread->SetText(ps->Properties.Spread);
	LifeTime->SetText(ps->Properties.LifeTime);
	LifeTimeRandomness->SetText(ps->Properties.LifeTimeRandomness);
	SizeFrom->SetText(ps->Properties.SizeFrom);
	SizeTo->SetText(ps->Properties.SizeTo);
	SizeRandomness->SetText(ps->Properties.SizeRandomenss);
	SpawnDirectionX->SetText(ps->Properties.ParticleDirection.x);
	SpawnDirectionY->SetText(ps->Properties.ParticleDirection.y);
	SpawnDirectionZ->SetText(ps->Properties.ParticleDirection.z);
	GravityX->SetText(ps->Properties.Gravity.x);
	GravityY->SetText(ps->Properties.Gravity.y);
	GravityZ->SetText(ps->Properties.Gravity.z);
	Material->SetText(ps->Properties.MaterialName);
	SpawnParam1->SetText(ps->Properties.SpawnParam1);
	SpawnParam2->SetText(ps->Properties.SpawnParam2);
	SpawnParam3->SetText(ps->Properties.SpawnParam3);
	SpawnParam4->SetText(ps->Properties.SpawnParam4);
	BlendType->SetSelection(ps->Properties.BlendType);
	SpinFrom->SetText(ps->Properties.SpinFrom);
	SpinTo->SetText(ps->Properties.SpinTo);
	SpinRandomness->SetText(ps->Properties.SpinRandomness);
	RelativeParticles->SetChecked(ps->Properties.RelativePosition);
	SpinningInLife->SetChecked(ps->Properties.SpinningInLife);
	VisibleSphereRadius->SetText(ps->Properties.VisibleSphereRadius);

	char tmp[100];	
	int R,G,B;

	R = (int)(ps->Properties.ColorFrom.r*255.0f);
	G = (int)(ps->Properties.ColorFrom.g*255.0f); 
	B = (int)(ps->Properties.ColorFrom.b*255.0f);
	sprintf(tmp, "R:%d, G:%d, B:%d", R, G, B);
	ColorFromLabel->SetText(tmp);

	R = (int)(ps->Properties.ColorTo.r*255.0f);
	G = (int)(ps->Properties.ColorTo.g*255.0f);
	B = (int)(ps->Properties.ColorTo.b*255.0f);
	sprintf(tmp, "R: %d, G:%d, B:%d", R, G, B);
	ColorToLabel->SetText(tmp);

	AlphaFrom->SetText(ps->Properties.AlphaFrom);
	AlphaTo->SetText(ps->Properties.AlphaTo);
	
	CanChange = true;
}
