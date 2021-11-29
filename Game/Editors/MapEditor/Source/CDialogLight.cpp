/*
    CDialogLight.cpp

	Autor: Nebojsa Vislavski
	Date: 16.12.2005.
	Version: 1.0
    

*/


#include <CDialogLight.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <commdlg.h>
#include <CDialogInsert.h>


CDialogLight *CDialogLight::_instance = NULL;
CDialogLight *CDialogLight::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogLight();
	return _instance;
}

bool CDialogLight::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_LIGHTDIALOG, hParent, Show))
	{
		return false;
	}


	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	DiffusePickButton = new CButton(this, IDC_DIFFUSEPICKBUTTON);
	SpecularPickButton = new CButton(this, IDC_SPECULARPICKBUTTON);
	AddStateButton = new CButton(this, IDC_ADDSTATEBUTTON);
	RemoveStateButton = new CButton(this, IDC_DELETESTATEBUTTON);
	DuplicateStateButton = new CButton(this, IDC_DUPLICATESTATEBUTTON);
	MoveUpButton = new CButton(this, IDC_MOVEUPBUTTON);
	MoveDownButton = new CButton(this, IDC_MOVEDOWNBUTTON);
	ProjectorBrowseButton = new CButton(this, IDC_BROWSEPROJECTORMATERIAL);


	StateListBox = new CListBox(this, IDC_STATELIST);

	DiffuseStatic = new CLabel(this, IDC_DIFFUSESTATIC);
	SpecularStatic = new CLabel(this, IDC_SPECULARSTATIC);

	Range = new CEditBox(this, IDC_RANGEEDIT);
	SpotAngle = new CEditBox(this, IDC_SPOTANGLE);
	SpotAngleFalloff = new CEditBox(this, IDC_SPOTANGLEFALLOFF);
	ProjectorMaterial = new CEditBox(this, IDC_PROJECTORMATERIAL);
	Duration = new CEditBox(this, IDC_TIMEEDIT);
	StartFalloff = new CEditBox(this, IDC_STARTFALLOFFRANGE);
	LightIntensity = new CEditBox(this, IDC_LIGTHINTENSITY);
	SpecularIntensity = new CEditBox(this, IDC_SPECULARINTENSITY);

	StaticLight = new CCheckBox(this, IDC_STATICCHECK);
	LightType = new CComboBox(this, IDC_LIGHTTYPECOMBO);
	AffectsDynamicActors = new CCheckBox(this, IDC_AFFECTDYNAMICCHECK);
	InterpolateStates = new CCheckBox(this, IDC_INTERPOLATESTATES);
	CastShadows = new CCheckBox(this, IDC_CASTSHADOWS);
	Projector = new CCheckBox(this, IDC_PROJECTORCHECK);
	ProjectorIsOmni = new CCheckBox(this, IDC_PROJECTOROMNI);

	Flare = new CCheckBox(this, IDC_FLARECHECK);
	Corona = new CCheckBox(this, IDC_CORONACHECK);
	CoronaTexture = new CEditBox(this, IDC_CORONATEXTURE);
	FlareTexture1 = new CEditBox(this, IDC_FLARETEXTURE1);
	FlareTexture2 = new CEditBox(this, IDC_FLARETEXTURE2);
	FlareTexture3 = new CEditBox(this, IDC_FLARETEXTURE3);
	FlareTexture4 = new CEditBox(this, IDC_FLARETEXTURE4);
	FlareTexture5 = new CEditBox(this, IDC_FLARETEXTURE5);
	CoronaBrowse = new CButton(this, IDC_BROWSECORONATEXTURE);
	FlareBrowse1 = new CButton(this, IDC_BROWSEFLARETEXTURE1);
	FlareBrowse2 = new CButton(this, IDC_BROWSEFLARETEXTURE2);
	FlareBrowse3 = new CButton(this, IDC_BROWSEFLARETEXTURE3);
	FlareBrowse4 = new CButton(this, IDC_BROWSEFLARETEXTURE4);
	FlareBrowse5 = new CButton(this, IDC_BROWSEFLARETEXTURE5);

	CoronaAlpha = new CEditBox(this, IDC_CORONAALPHA);
	FlareAlpha[0] = new CEditBox(this, IDC_FLAREALPHA1);
	FlareAlpha[1] = new CEditBox(this, IDC_FLAREALPHA2);
	FlareAlpha[2] = new CEditBox(this, IDC_FLAREALPHA3);
	FlareAlpha[3] = new CEditBox(this, IDC_FLAREALPHA4);
	FlareAlpha[4] = new CEditBox(this, IDC_FLAREALPHA5);

	CoronaSizeX = new CEditBox(this, IDC_CORONASIZEX);
	CoronaSizeY = new CEditBox(this, IDC_CORONASIZEY);
	FlareSizeX = new CEditBox(this, IDC_FLARESIZEX);
	FlareSizeY = new CEditBox(this, IDC_FLARESIZEY);

	CoronaFixedSize = new CCheckBox(this, IDC_LIGHTED_FIXEDSIZE);
	CoronaRotation = new CCheckBox(this, IDC_LIGHT_ROTATION);
	AlwaysShowFlares = new CCheckBox(this, IDC_LIGHT_ALWAYSSHOWFLARES);

	LightSizeX = new CEditBox(this, IDC_LIGHTSIZEX);
	LightSizeY = new CEditBox(this, IDC_LIGHTSIZEY);
	SizeXStatic = new CLabel(this, IDC_SIZEXSTATIC);
	SizeYStatic = new CLabel(this, IDC_SIZEYSTATIC);


	FadeInTime = new CEditBox(this, IDC_FADEINTIME);
	FadeOutTime = new CEditBox(this, IDC_FADEOUTTIME);

	Light = NULL;
	CanChange = false;
	CurrentState = NULL;

	LightType->Clear();
	LightType->AddItem("Direct", LIGHT_DIRECTIONAL);
	LightType->AddItem("Point", LIGHT_POINT);
	LightType->AddItem("Spot", LIGHT_SPOT);
	LightType->AddItem("Projector Only", LIGHT_PROJECTORONLY);
	LightType->AddItem("Linear Static Light", LIGHT_STATIC_LINEAR);
	LightType->AddItem("Area Static Light", LIGHT_STATIC_AREA);
	LightType->AddItem("Beam Static Light", LIGHT_STATIC_BEAM);

	return true;
}


void CDialogLight::OnShow()
{
}



void CDialogLight::OnStateListChange()
{
	int i,j;

	CurrentState = NULL;
	i = StateListBox->GetSelectionIndex();
	j = 0;
	for (Light->LightStates.First(); !Light->LightStates.End(); Light->LightStates.Next())
	{
		if (j == i) 
		{
			CurrentState =  &Light->LightStates.GetItem();
			break;
		}
		j++;			
	}


	if (CurrentState)
	{
		char tmp[100];
		int R,G,B;
		R = (int)(CurrentState->Diffuse.r*255.0f);
		G = (int)(CurrentState->Diffuse.g*255.0f);
		B = (int)(CurrentState->Diffuse.b*255.0f);
		sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
		DiffuseStatic->SetText(tmp);		

		R = (int)(CurrentState->Specular.r*255.0f);
		G = (int)(CurrentState->Specular.g*255.0f);
		B = (int)(CurrentState->Specular.b*255.0f);
		sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
		SpecularStatic->SetText(tmp);		
		
		Duration->SetText(CurrentState->Duration);
		LightIntensity->SetText(CurrentState->Intensity);
		SpecularIntensity->SetText(CurrentState->SpecularIntensity);
		CoronaAlpha->SetText(CurrentState->CoronaAlpha);
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
		{
			FlareAlpha[i]->SetText(CurrentState->FlareAlpha[i]);
		}
	}
}

void CDialogLight::Show(WeLight *light)
{
	if (!light) return;
	Light = light;
	Update(Light);    		
	CDialog::Show();


}


void CDialogLight::OnLightTypeChange()
{
	if (Light->LightType == LIGHT_STATIC_LINEAR)
	{
		SizeXStatic->SetVisible(true);
		SizeXStatic->SetText("Length:");
		SizeYStatic->SetVisible(false);
		LightSizeX->SetVisible(true);
		LightSizeY->SetVisible(false);		
	}
	else if (Light->LightType == LIGHT_STATIC_AREA)
	{
		SizeXStatic->SetVisible(true);
		SizeYStatic->SetVisible(true);
		LightSizeX->SetVisible(true);
		LightSizeY->SetVisible(true);
		SizeXStatic->SetText("SizeX:");
		SizeYStatic->SetText("SizeY:");
	}
	else if (Light->LightType == LIGHT_STATIC_BEAM)
	{
		SizeXStatic->SetVisible(true);
		SizeYStatic->SetVisible(false);
		LightSizeX->SetVisible(true);
		LightSizeY->SetVisible(false);
		SizeXStatic->SetText("Radius:");		
	}
	else
	{
		SizeXStatic->SetVisible(false);
		SizeYStatic->SetVisible(false);
		LightSizeX->SetVisible(false);
		LightSizeY->SetVisible(false);
	}

}

void CDialogLight::Update(WeLight *light)
{	
	CanChange = false;

	StateListBox->Clear();
	int i = 0;
	char tmp[100];

	for (Light->LightStates.First(); !Light->LightStates.End(); Light->LightStates.Next())
	{
		sprintf(tmp, "State %d", i+1);		
		StateListBox->AddItem(tmp, i);
		i++;
	}

	StaticLight->SetChecked(!Light->IsDynamic);
	AffectsDynamicActors->SetChecked(Light->AffectDynamicActors);		
	LightType->SetSelection(Light->LightType);
	StartFalloff->SetText(Light->StartFalloff);
	Projector->SetChecked(Light->Projector);
	ProjectorIsOmni->SetChecked(Light->ProjectorIsOmni);
	ProjectorMaterial->SetText(Light->ProjectorMaterialName);
	InterpolateStates->SetChecked(Light->InterpolateStates);
	SpotAngle->SetText(Light->SpotAngle*180.0f/D3DX_PI);
	SpotAngleFalloff->SetText(Light->SpotAngleFalloff*180.0f/D3DX_PI);
	Range->SetText(Light->Range);
	CastShadows->SetChecked(Light->CastShadowVolume);
	Corona->SetChecked(Light->Corona);
	Flare->SetChecked(Light->Flares);
	FlareSizeX->SetText(Light->FlareSizeX);
	FlareSizeY->SetText(Light->FlareSizeY);
	CoronaSizeX->SetText(Light->CoronaSizeX);
	CoronaSizeY->SetText(Light->CoronaSizeY);
	CoronaTexture->SetText(Light->CoronaTextureName);
	FlareTexture1->SetText(Light->FlareTextureNames[0]);
	FlareTexture2->SetText(Light->FlareTextureNames[1]);
	FlareTexture3->SetText(Light->FlareTextureNames[2]);
	FlareTexture4->SetText(Light->FlareTextureNames[3]);
	FlareTexture5->SetText(Light->FlareTextureNames[4]);	
	FadeInTime->SetText(Light->FadeInTime);
	FadeOutTime->SetText(Light->FadeOutTime);
	CoronaFixedSize->SetChecked(Light->CoronaFixedSize);
	CoronaRotation->SetChecked(Light->CoronaRotation);
	AlwaysShowFlares->SetChecked(Light->AlwaysShowFlares);
	LightSizeX->SetText(Light->SizeX);
	LightSizeY->SetText(Light->SizeY);

	OnLightTypeChange();	

	CanChange = true;
	StateListBox->SetSelection(0);
	OnStateListChange();

}


void CDialogLight::OnChange(DWORD ID)
{
	
	if (Range->GetID() == ID)
	{
		Light->Range = Range->GetTextAsFloat();        
	}

	if (SpotAngle->GetID() == ID)
	{		
		Light->SpotAngle = D3DX_PI*SpotAngle->GetTextAsFloat() / 180.0f;        
	}
	if (SpotAngleFalloff->GetID() == ID)
	{		
		Light->SpotAngleFalloff = D3DX_PI*SpotAngleFalloff->GetTextAsFloat() / 180.0f;        
	}

	if (StartFalloff->GetID() == ID)
	{
		Light->StartFalloff = StartFalloff->GetTextAsFloat();
	}

	if (Duration->GetID() == ID)
	{
		if (!CurrentState) return;	
		CurrentState->Duration = Duration->GetTextAsFloat();
	}
	if (LightIntensity->GetID() == ID)
	{
		if (!CurrentState) return;	
		CurrentState->Intensity = LightIntensity->GetTextAsFloat();
	}	
	if (SpecularIntensity->GetID() == ID)
	{
		if (!CurrentState) return;	
		CurrentState->SpecularIntensity = SpecularIntensity->GetTextAsFloat();
	}	

	if (LightSizeX->GetID() == ID)
	{
		Light->SizeX = LightSizeX->GetTextAsFloat();
	}
	if (LightSizeY->GetID() == ID)
	{
		Light->SizeY = LightSizeY->GetTextAsFloat();
	}

	if (CoronaAlpha->GetID() == ID)
	{
		if (!CurrentState) return;	
		CurrentState->CoronaAlpha = CoronaAlpha->GetTextAsFloat();
	}		


	for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
	{
		if (FlareAlpha[i]->GetID() == ID)
		{
			if (!CurrentState) return;	
			CurrentState->FlareAlpha[i] = FlareAlpha[i]->GetTextAsFloat();
		}		
	}


	if (CoronaSizeX->GetID() == ID)
		Light->CoronaSizeX = CoronaSizeX->GetTextAsFloat();

	if (CoronaSizeY->GetID() == ID)
		Light->CoronaSizeY = CoronaSizeY->GetTextAsFloat();

	if (FlareSizeX->GetID() == ID)
		Light->FlareSizeX = FlareSizeX->GetTextAsFloat();

	if (FlareSizeY->GetID() == ID)
		Light->FlareSizeY = FlareSizeY->GetTextAsFloat();

	if (FadeInTime->GetID() == ID)
		Light->FadeInTime = FadeInTime->GetTextAsFloat();

	if (FadeOutTime->GetID() == ID)
		Light->FadeOutTime = FadeOutTime->GetTextAsFloat();
}

void CDialogLight::OnClick(DWORD ID)
{
	if (OKButton->GetID() == ID) 
	{
		CanChange = false;
		Hide();
	}
	if (CancelButton->GetID() == ID) 
	{
		CanChange = false;
		Hide();
	}

	if (LightType->GetID() == ID)
	{
		Light->LightType = (WeLightType)LightType->GetSelectionData();
		OnLightTypeChange();
	}

	if (AddStateButton->GetID() == ID) 
	{
		int i;
		WeLightState LS;

		i = Light->LightStates.GetItemsCount();
		char tmp[100];
		sprintf(tmp, "State %d", i+1);		
		StateListBox->AddItem(tmp, i);
		Light->AddKeyState(LS, 1.0f); // LightStates.PushBack(LS);
	}

	if (RemoveStateButton->GetID() == ID)
	{
		int i;

		Light->RemoveLightState(CurrentState);
		CurrentState = NULL;
		i = StateListBox->GetSelectionIndex();
		StateListBox->RemoveItem(i);		
		
		//for (Light->LightStates.First(); !Light->LightStates.End(); Light->LightStates.Next())
		//{
		//	if (&Light->LightStates.GetItem() == CurrentState)
		//	{
		//		
		//		Light->LightStates.Delete();
		//		CurrentState = NULL;
		//		break;
		//	}
		//}
	}

	if (DuplicateStateButton->GetID() == ID)
	{
		if (CurrentState) 
		{
			int i;
			WeLightState LS;
			LS = *CurrentState;

			i = Light->LightStates.GetItemsCount();
			char tmp[100];
			sprintf(tmp, "State %d", i+1);		
			StateListBox->AddItem(tmp, i);
			Light->AddKeyState(LS, LS.Duration); // LightStates.PushBack(LS);
		}
	}

	if (InterpolateStates->GetID() == ID)
	{
		Light->InterpolateStates = InterpolateStates->GetChecked();
	}

	if (CastShadows->GetID() == ID)
	{
		Light->CastShadowVolume = CastShadows->GetChecked();
	}

	if (Projector->GetID() == ID)
	{
		Light->Projector = Projector->GetChecked();
	}

	if (ProjectorIsOmni->GetID() == ID)
	{
		Light->ProjectorIsOmni = ProjectorIsOmni->GetChecked();
	}


	if (Corona->GetID() == ID)
	{
		Light->Corona = Corona->GetChecked();
	}

	if (Flare->GetID() == ID)
	{
		Light->Flares = Flare->GetChecked();
	}

	if (CoronaBrowse->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			CoronaTexture->SetText(InsertDialog->HoleName);
			Light->CoronaTextureName = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}

	if (FlareBrowse1->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			FlareTexture1->SetText(InsertDialog->HoleName);
			Light->FlareTextureNames[0] = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}
	if (FlareBrowse2->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			FlareTexture2->SetText(InsertDialog->HoleName);
			Light->FlareTextureNames[1] = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}
	if (FlareBrowse3->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			FlareTexture3->SetText(InsertDialog->HoleName);
			Light->FlareTextureNames[2] = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}
	if (FlareBrowse4->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			FlareTexture4->SetText(InsertDialog->HoleName);
			Light->FlareTextureNames[3] = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}
	if (FlareBrowse5->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			FlareTexture5->SetText(InsertDialog->HoleName);
			Light->FlareTextureNames[4] = InsertDialog->HoleName;
			Light->Initialize();
		}		
	}

	if (CoronaFixedSize->GetID() == ID)
	{
		Light->CoronaFixedSize = CoronaFixedSize->GetChecked();
	}

	if (CoronaRotation->GetID() == ID)
	{
		Light->CoronaRotation = CoronaRotation->GetChecked();
	}

	if (AlwaysShowFlares->GetID() == ID)
	{
		Light->AlwaysShowFlares = AlwaysShowFlares->GetChecked();
	}

	if (DiffusePickButton->GetID() == ID)
	{
		if (!CurrentState) return;
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(CurrentState->Diffuse.r*255.0f, CurrentState->Diffuse.g*255.0f, CurrentState->Diffuse.b*255.0f);
		
		if (ChooseColor(&col))
		{
			char tmp[100];
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			CurrentState->Diffuse.r = R / 255.0f;
			CurrentState->Diffuse.g = G / 255.0f;
			CurrentState->Diffuse.b = B / 255.0f;

			sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
			DiffuseStatic->SetText(tmp);
		}
	}

    
	if (SpecularPickButton->GetID() == ID)
	{
		if (!CurrentState) return;
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(CurrentState->Specular.r*255.0f, CurrentState->Specular.g*255.0f, CurrentState->Specular.b*255.0f);
		
		if (ChooseColor(&col))
		{
			char tmp[100];
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			CurrentState->Specular.r = R / 255.0f;
			CurrentState->Specular.g = G / 255.0f;
			CurrentState->Specular.b = B / 255.0f;

			sprintf(tmp, "R:%d G:%d B:%d", R, G, B);
			SpecularStatic->SetText(tmp);
		}

	}

	if (ProjectorBrowseButton->GetID() == ID)
	{
		InsertDialog->Type = PACKAGEITEM_MATERIAL;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			ProjectorMaterial->SetText(InsertDialog->HoleName);
			Light->ProjectorMaterialName = InsertDialog->HoleName;
			Light->Initialize();
		}
	}


	if (StaticLight->GetID() == ID)
	{
		Light->IsDynamic = !StaticLight->GetChecked();
	}

	if (AffectsDynamicActors->GetID() == ID)
	{
		Light->AffectDynamicActors = AffectsDynamicActors->GetChecked();
	}

	if (StateListBox->GetID() == ID) 
	{
		OnStateListChange();
	}

	if (MoveUpButton->GetID() == ID)
	{
		if (CurrentState)
		{
			for (Light->LightStates.First(); !Light->LightStates.End(); Light->LightStates.Next())
			{
				if (&Light->LightStates.GetItem() == CurrentState)
				{
					Light->LightStates.MovePrev();
				}
			}
		}
	}

	if (MoveDownButton->GetID() == ID)
	{
		if (CurrentState)
		{
			for (Light->LightStates.First(); !Light->LightStates.End(); Light->LightStates.Next())
			{
				if (&Light->LightStates.GetItem() == CurrentState)
				{
					Light->LightStates.MoveNext();
				}
			}
		}
	}
}

