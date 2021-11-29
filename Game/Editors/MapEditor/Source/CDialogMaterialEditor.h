/*
    CDialogMaterialEditor.h

	Autor: Nebojsa Vislavski
	Date: 27.12.2005.
	Version: 1.0


*/

#ifndef _CDIALOGMATERIALEDITOR_H
#define _CDIALOGMATERIALEDITOR_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeMaterialEditor.h>
#include <WeCamera.h>
#include <WeActor.h>
#include <WeWorld.h>

extern class CDialogMaterialEditor *MaterialEditor;


class CDialogMaterialEditor : public CDialog
{
protected:
	static CDialogMaterialEditor *_instance;

	void OnShow();
	void OnHide();
	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnSaveMaterial(StringParam SaveName);
	void OnNewMaterial();
	void OnEffectBrowse();
	void OnPickColor();
	void OnSetTexture(int i);
	void OnFindTexture(int i);
	void OnApply();
	void OnRefrsh();
	void OnChangeRot();
	void OnChangeObject();
	void OnExit();
	void OnTexAnimComboClick();

	CButton* EffectBrowse;
	CButton* SetTexture[8];
	CButton* FindTexture[8];

	CButton* Apply;
	CButton* Refresh;
	CButton* MaterialColorChoose;

	CCheckBox* XRot;
	CCheckBox* YRot;
	CCheckBox* ZRot;

	CLabel	*TexAnimParam1Label;
	CLabel	*TexAnimParam2Label;
	CLabel	*TexAnimParam3Label;
	CLabel	*TexAnimParam4Label;
	CLabel	*TexAnimParam5Label;
	CLabel  *MatColorLabel;

	CCheckBox *Glowing;
	CCheckBox *DoubleSided;

	CEditBox *TexAnimParam1;
	CEditBox *TexAnimParam2;
	CEditBox *TexAnimParam3;
	CEditBox *TexAnimParam4;
	CEditBox *TexAnimParam5;
	CEditBox *BumpIntensity;
	CEditBox *SpecularPow;
	CEditBox *AlphaRef;
	CEditBox *MaterialAlpha;

	CEditBox *MatParam1;
	CEditBox *MatParam2;
	CEditBox *MatParam3;
	CEditBox *MatParam4;

	CComboBox* TexAnimCombo;
	CComboBox* Objects;

	CButton* SaveSelected;
	CButton* RemoveSelected;

	bool XRotation;
	bool YRotation;
	bool ZRotation;
	bool Created;

	void ClearAllNames();

	WeMaterialEditor* Material;
	
public:

	static CDialogMaterialEditor *Instance();

	CTreeView* MaterialList;
	CTreeView* TextureList;
	CTreeView* EffectList;


	CComboBox* MaterialType;
	CComboBox* BlendType;
	CComboBox* MaterialReflection;
	CCheckBox* AcceptLights;

	CEditBox* EffectName;
	CEditBox* TexName[8];

	WeWorld* MEWorld;
	WeCamera* MECamera;
	WeActor* BoxPreview;
	WeActor* SpherePreview;
	WeActor* BoxPreviewNMap;
	WeActor* SpherePreviewNMap;

	CCheckBox *TexPreview;
	CButton* PreviewArea;
	CButton* TexturePreviewArea;

	CDialogMaterialEditor(){}
	
	void FillComboBoxes();
	bool Create(HWND hParent, bool Show = true);
	void InitializeWorldAndObjects();
	void UpdateRotations();
	void UpdateComponents();
};




#endif