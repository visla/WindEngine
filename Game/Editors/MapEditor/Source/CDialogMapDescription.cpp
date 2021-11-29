

#include <CDialogMapDescription.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <CMap.h>
#include <CMapHeader.h>
#include <CProfileManager.h>
#include <CProfile.h>
#include <CDialogInsert.h>
#include <WeRenderDevice.h>
#include <CMapEditorCamera.h>
#include <commdlg.h>



CDialogMapDescription *CDialogMapDescription::_instance = NULL;
CDialogMapDescription *CDialogMapDescription::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogMapDescription();
	return _instance;
}

bool CDialogMapDescription::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_MAPDESCRIPTIONDIALOG, hParent, Show))
	{
		return false;
	}

	MapDescription = new CEditBox(this, IDC_MAPDESCRIPTION);
	MapDescriptionImage = new CEditBox(this, IDC_MAPDESCRIPTIONIMAGE);
	MapDescriptionImageBrowse = new CButton(this, IDC_MAPDESCRIPTIONIMAGEBROWSE);
	LoadingImage = new CEditBox(this, IDC_LOADMAPIMAGE);
	LoadingImageBrowse = new CButton(this, IDC_LOADMAPIMAGEBROWSE);
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	RaceTime = new CEditBox(this,IDC_REACETIME);
	FogCheck = new CCheckBox(this, IDC_FOGCHECK);
	FogStart = new CEditBox(this, IDC_FOGSTART);
	FogEnd = new CEditBox(this, IDC_FOGEND);
	FogColor = new CButton(this, IDC_FOGCOLORBUTTON);
	SkyColor = new CButton(this, IDC_SKYCOLOR);
	GroundColor = new CButton(this, IDC_GROUNDCOLOR);
	ClipDistance = new CEditBox(this, IDC_MAP_CLIPDISTANCE);
	MusicFile = new CEditBox(this, IDC_MAP_MUSIC);
	StatMusic = new CEditBox(this, IDC_MAP_STATMUSIC);
	FHDRI[0] = new CEditBox(this, IDC_MAP_FHDRI0);
	FHDRI[1] = new CEditBox(this, IDC_MAP_FHDRI1);
	FHDRI[2] = new CEditBox(this, IDC_MAP_FHDRI2);

	return true;
}


void CDialogMapDescription::Update(CMap *Map)
{
	MapDescription->SetText(Map->MapHeader.MapDescription);
	MapDescriptionImage->SetText(Map->MapHeader.MapDescriptionImage);			
	LoadingImage->SetText(Map->MapHeader.LoadingImage);
	RaceTime->SetText(Map->MapHeader.RaceTime);
	FogCheck->SetChecked(Map->MapHeader.FogEnabled);
	FogStart->SetText(Map->MapHeader.FogStart);
	FogEnd->SetText(Map->MapHeader.FogEnd);	
	ClipDistance->SetText(Map->MapHeader.ClippingDistance);
	MusicFile->SetText(Map->MapHeader.MusicFile);
	StatMusic->SetText(Map->MapHeader.StatMusic);
	FHDRI[0]->SetText(Map->MapHeader.FHDRI[0]);
	FHDRI[1]->SetText(Map->MapHeader.FHDRI[1]);
	FHDRI[2]->SetText(Map->MapHeader.FHDRI[2]);
}


void CDialogMapDescription::Show(CMap *map)
{
	if (!map) return;
	Map = map;
	Update(Map);    		
	CDialog::Show();
}


void CDialogMapDescription::OnOKClick()
{
	// make changes to Map.
	
	Map->MapHeader.MapDescription = MapDescription->GetText();	
	Map->MapHeader.MapDescriptionImage = MapDescriptionImage->GetText();
	Map->MapHeader.LoadingImage = LoadingImage->GetText();
	Map->MapHeader.RaceTime = RaceTime->GetTextAsFloat();
	Map->MapHeader.MusicFile = MusicFile->GetText();
	Map->MapHeader.StatMusic = StatMusic->GetText();
	Map->MapHeader.FHDRI[0] = FHDRI[0]->GetTextAsFloat();
	Map->MapHeader.FHDRI[1] = FHDRI[1]->GetTextAsFloat();
	Map->MapHeader.FHDRI[2] = FHDRI[2]->GetTextAsFloat();

	SetVisible(false);    
}


void CDialogMapDescription::OnChange(DWORD ID)
{
	if (Map)
	{
		if (ID == FogStart->GetID())
		{
			Map->MapHeader.FogStart = FogStart->GetTextAsFloat();
			WeRenderDevice::Instance()->SetFogStart(Map->MapHeader.FogStart);
		}
		if (ID == FogEnd->GetID())
		{
			Map->MapHeader.FogEnd = FogEnd->GetTextAsFloat();
			WeRenderDevice::Instance()->SetFogEnd(Map->MapHeader.FogEnd);
		}
		if (ID == ClipDistance->GetID())
		{
			float Distance = ClipDistance->GetTextAsFloat();
			if (Distance <= 0.0f)
				Distance = 1000.0f;
			Map->MapHeader.ClippingDistance = Distance;
			EditorWindow->Cam->SetFarClip(Map->MapHeader.ClippingDistance);
		}
	}
}



void CDialogMapDescription::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 
	{
		OnOKClick();	
	}
	if (ID == CancelButton->GetID())
	{
		SetVisible(false);
	}
	if (ID == SkyColor->GetID())
	{
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(Map->MapHeader.SkyColor.r*255.0f, Map->MapHeader.SkyColor.g*255.0f, Map->MapHeader.SkyColor.b*255.0f);

		if (ChooseColor(&col))
		{
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			Map->MapHeader.SkyColor.r = R / 255.0f;
			Map->MapHeader.SkyColor.g = G / 255.0f;
			Map->MapHeader.SkyColor.b = B / 255.0f;
			WeRenderDevice::Instance()->SetSkyColor(Map->MapHeader.SkyColor);
		}
	}
	if (ID == GroundColor->GetID())
	{
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(Map->MapHeader.GroundColor.r*255.0f, Map->MapHeader.GroundColor.g*255.0f, Map->MapHeader.GroundColor.b*255.0f);

		if (ChooseColor(&col))
		{
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			Map->MapHeader.GroundColor.r = R / 255.0f;
			Map->MapHeader.GroundColor.g = G / 255.0f;
			Map->MapHeader.GroundColor.b = B / 255.0f;
			WeRenderDevice::Instance()->SetGroundColor(Map->MapHeader.GroundColor);
		}
	}
	if (ID == FogCheck->GetID())
	{
		Map->MapHeader.FogEnabled = FogCheck->GetChecked();
		WeRenderDevice::Instance()->SetFog(Map->MapHeader.FogEnabled, Map->MapHeader.FogColor, 0);
	}
	if (ID == FogColor->GetID())
	{
		CHOOSECOLOR col;
		COLORREF colRef[16];
		col.lStructSize = sizeof(col);
		col.hwndOwner = GetHWND();
		col.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
		col.lpCustColors = colRef;
		col.lpTemplateName = "Pick Color";				
		col.rgbResult = RGB(Map->MapHeader.FogColor.r*255.0f, Map->MapHeader.FogColor.g*255.0f, Map->MapHeader.FogColor.b*255.0f);

		if (ChooseColor(&col))
		{
			int R, G, B;
			R = GetRValue(col.rgbResult);
			G = GetGValue(col.rgbResult);			
			B = GetBValue(col.rgbResult);
			Map->MapHeader.FogColor.r = R / 255.0f;
			Map->MapHeader.FogColor.g = G / 255.0f;
			Map->MapHeader.FogColor.b = B / 255.0f;
			WeRenderDevice::Instance()->SetFog(Map->MapHeader.FogEnabled, Map->MapHeader.FogColor, 0);
		}
	}
	if (ID == MapDescriptionImageBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			MapDescriptionImage->SetText(InsertDialog->HoleName);
		}
	}
	if (ID == LoadingImageBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_TEXTURE;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			LoadingImage->SetText(InsertDialog->HoleName);
		}
	}
}
