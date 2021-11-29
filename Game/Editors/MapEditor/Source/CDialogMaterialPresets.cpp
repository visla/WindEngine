/*
    CDialogMaterialPresets.h

	Autor: NEbojsa Vislavski
	Date: 27.1.2006.
	Version: 1.0

    
*/


#include <CDialogMaterialPresets.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CDialogInsert.h>


CDialogMaterialPresets *CDialogMaterialPresets::_instance = NULL;
CDialogMaterialPresets *CDialogMaterialPresets::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogMaterialPresets();
	return _instance;
}

bool CDialogMaterialPresets::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_MATERIALPRESETDIALOG, hParent))
	{
		return false;
	}
	LoadPresets();
	return true;
}


void CDialogMaterialPresets::OnShow()
{
	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);
	LoadButton = new CButton(this, IDC_LOADPRESETBUTTON);
	SaveButton = new CButton(this, IDC_SAVEPRESETBUTTON);

	MaterialEdit = new CEditBox(this, IDC_MATERIALEDIT);	
	MaterialBrowse = new CButton(this, IDC_BROWSEMATERIALBUTTON);


	PresetCombo = new CComboBox(this, IDC_PRESETCOMBO);
	PresetStatic = new CLabel(this, IDC_PRESETSTATIC);

	for (int i = 0; i < MAX_MATERIAL_PRESETS; i++)
	{
		char tmp[100];
		sprintf(tmp, "Preset %d", i);
		PresetCombo->AddItem(tmp, i);
	}
	PresetCombo->SetSelection(0);
	MakePresetLabel();
}


void CDialogMaterialPresets::LoadPresets()
{
	WeFileStream FS;
	if (!FS.Exists("matpresets.ini")) return;
	if (!FS.Open("matpresets.ini")) return;

	for (int i = 0; i < MAX_MATERIAL_PRESETS; i++)
	{
		if (FS.Eof()) break;

		MaterialNames[i] = FS.ReadString();
	}
	FS.Close();
}


void CDialogMaterialPresets::SavePresets()
{
	WeFileStream FS;
	if (!FS.OpenNew("matpresets.ini")) return;

	for (int i = 0; i < MAX_MATERIAL_PRESETS; i++)
	{		
		FS.WriteString(MaterialNames[i]);
	}
	FS.Close();
}

void CDialogMaterialPresets::MakePresetLabel()
{
   String s;
   char tmp[100];

   for (int i = 0; i < MAX_MATERIAL_PRESETS; i++)
   {
	   sprintf(tmp, "Preset %d: ", i);
	   s = s + tmp;
	   s = s + MaterialNames[i] + "\r";	   	   
   }
   PresetStatic->SetText(s);
}


void CDialogMaterialPresets::OnChange(DWORD ID)
{
	if (ID == MaterialEdit->GetID())
	{
		MaterialNames[PresetCombo->GetSelectionData()] = MaterialEdit->GetText();
		MakePresetLabel();
	}

}


void CDialogMaterialPresets::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 
	{
		SavePresets();
		SetModalResult(MR_OK);
	}
	if (ID == CancelButton->GetID()) SetModalResult(MR_CANCEL);


	if (ID == PresetCombo->GetID())
	{
		MaterialEdit->SetText(MaterialNames[PresetCombo->GetSelectionData()]);
		MakePresetLabel();
	}

	if (ID == MaterialBrowse->GetID()) 
	{
		InsertDialog->Type = PACKAGEITEM_MATERIAL;
		if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			MaterialNames[PresetCombo->GetSelectionData()] = InsertDialog->HoleName;
			MaterialEdit->SetText(InsertDialog->HoleName);
			MakePresetLabel();
		}
	}
}

