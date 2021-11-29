/*
    CDialogMaterialPresets.h

	Autor: NEbojsa Vislavski
	Date: 27.1.2006.
	Version: 1.0

    
*/


#ifndef _CDIALOGMATERIALPRESET_H
#define _CDIALOGMATERIALPRESET_H


#include <CDialog.h>
#include <CComponent.h>
#include <WeMesh.h>

class CDialogMaterialPresets: public CDialog
{
protected:
	static CDialogMaterialPresets *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnShow();

	void MakePresetLabel();
	void SavePresets();
	void LoadPresets();

public:
	static CDialogMaterialPresets *Instance();

	CButton *OKButton;
	CButton *CancelButton;
	CButton *LoadButton;
	CButton *SaveButton;
	CButton *MaterialBrowse;
	CComboBox *PresetCombo;
	CEditBox  *MaterialEdit;
	CLabel	  *PresetStatic;

	String MaterialNames[MAX_MATERIAL_PRESETS];

    bool Create(HWND hParent, bool Show = true);
};



#endif