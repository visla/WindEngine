/*
     CDialogLight.h

	 Autor: Nebojsa Vislavski
	 Date: 31.1.2006.
	 Version: 1.0

	 

*/

#ifndef _CDIALOGLIGHT_H
#define _CDIALOGLIGHT_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeLight.h>
#include <CMapEditorWindow.h>

class CDialogLight: public CDialog
{
protected:
	static CDialogLight *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnShow();
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }

	void OnStateListChange();
	void OnLightTypeChange();

public:
	static CDialogLight *Instance();

	CButton  *AddStateButton;
	CButton  *RemoveStateButton;
	CButton  *OKButton;
	CButton  *CancelButton;
	CButton	 *DiffusePickButton;
	CButton  *SpecularPickButton;
	CButton  *MoveUpButton;
	CButton  *MoveDownButton;
	CButton  *DuplicateStateButton;
	CButton  *ProjectorBrowseButton;

	CListBox *StateListBox;
	CLabel   *DiffuseStatic;
	CLabel	 *SpecularStatic;

	CEditBox *SpotAngle;
	CEditBox *SpotAngleFalloff;
	CEditBox *Range;
	CEditBox *Duration;
	CEditBox *StartFalloff;
	CEditBox *ProjectorMaterial;
	CEditBox *LightIntensity;
	CEditBox *SpecularIntensity;

	CComboBox *LightType;

	CCheckBox *StaticLight;
	CCheckBox *CastShadows;
	CCheckBox *Projector;
	CCheckBox *ProjectorIsOmni;
	CCheckBox *InterpolateStates;
	CCheckBox *AffectsDynamicActors;
	CCheckBox *Flare;
	CCheckBox *Corona;

	CEditBox *CoronaTexture;
	CEditBox *FlareTexture1;
	CEditBox *FlareTexture2;
	CEditBox *FlareTexture3;
	CEditBox *FlareTexture4;
	CEditBox *FlareTexture5;

	CButton  *CoronaBrowse;
	CButton	 *FlareBrowse1;
	CButton	 *FlareBrowse2;
	CButton	 *FlareBrowse3;
	CButton	 *FlareBrowse4;
	CButton	 *FlareBrowse5;

	CEditBox *CoronaAlpha;
	CEditBox *FlareAlpha[LIGHT_FLARE_TEXTURES];

	CEditBox *LightSizeX;
	CEditBox *LightSizeY;
	CLabel	 *SizeXStatic;
	CLabel	 *SizeYStatic;

	CEditBox *FlareSizeX;
	CEditBox *FlareSizeY;
	CEditBox *CoronaSizeX;
	CEditBox *CoronaSizeY;

	CCheckBox *CoronaFixedSize;
	CCheckBox *CoronaRotation;
	CCheckBox *AlwaysShowFlares;

	CEditBox *FadeInTime;
	CEditBox *FadeOutTime;

	WeLight		 *Light;
	WeLightState *CurrentState;
	bool			CanChange;
	
    bool Create(HWND hParent, bool Show = true);
	void Show(WeLight *light);
	void Update(WeLight *light);

};





#endif