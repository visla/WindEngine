#include <CDialogEffectEditor.h>
#include <CEditorResourceManager.h>
#include <CDialogSave.h>
#include <CDialogInfo.h>
#include <CDialogInsert.h>
#include <CDialogTools.h>
#include <CMapEditorWindow.h>
#include <WePackage.h>


//========================================================
//    EffectEditor Class functions
//=======================================================

CDialogEffectEditor* EffectEditor = NULL;


bool CDialogEffectEditor::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_EFFECTEDITOR, hParent, Shown))
	{
		return false;
	}

	Save = new CButton(this, IDC_SAVE);
	Load = new CButton(this, IDC_LOAD);
	Browse = new CButton(this, IDC_EFFECTNAMEBROWSE);
	Close = new CButton(this, IDC_CLOSE);
	EffectName = new CLabel(this, IDC_EFFECTNAME);
	EffectName->SetText("NONE");

	EffectForEditor = new WeEffectEditor();

	return true;
}




void CDialogEffectEditor::OnClick(DWORD ID)
{
	if (ID == Save->GetID())
		OnSave();
	if (ID == Load->GetID())
		OnLoad();
	if (ID == Browse->GetID())
		OnBrowse();
	if (ID == Close->GetID())
		OnOk();
	if (ID == IDC_EFFECTS_REFRESH)
		RefreshEffects();
}




void CDialogEffectEditor::OnSave()
{
	SaveDialog->Type = PACKAGEITEM_EFFECT;
	if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
	{
		if (EffectForEditor->Save(SaveDialog->HoleName, false))
		{
			EffectName->SetText(SaveDialog->HoleName);
		}
		else
		{
			if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
			{
				if (EffectForEditor->Save(SaveDialog->HoleName, true))
				{
					EffectName->SetText(SaveDialog->HoleName);
				}
				else
				{
					MessageBox(0, "Failed writing", 0, 0);
				}
			}
		}
	}
}




void CDialogEffectEditor::OnLoad()
{
	InsertDialog->Type = PACKAGEITEM_EFFECT;
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		if (EffectForEditor->Load(InsertDialog->HoleName, false))
			EffectName->SetText(InsertDialog->HoleName);
	}
}





void CDialogEffectEditor::OnBrowse()
{
	if(GetFile("Effect files *.fx\0*.fx\0\0",""))
	{
		if(FAILED(D3DXCreateEffectCompilerFromFile(FileName, NULL, NULL, 0, &EffectForEditor->EffectCompiler, NULL)))
		{
			MessageBox(GetHWND(), "Error compiling effect!", "Error", MB_OK | MB_ICONERROR);
			return;
		}
		
		//MessageBox(GetHWND(),"Compile OK!", "Compile!", MB_OK | MB_ICONINFORMATION);
		EffectName->SetText(FileNameNoPath);		
	}
}



void CDialogEffectEditor::OnOk()
{
	EffectName->SetText("NONE");
	SetVisible(false);
}



void RefreshEffect(StringParam ItemName, StringParam FileName, void *Param)
{
	if (!FAILED(D3DXCreateEffectCompilerFromFile(FileName, NULL, NULL, 0, &EffectEditor->EffectForEditor->EffectCompiler, NULL)))
		EffectEditor->EffectForEditor->Save(ItemName, true);
}



void CDialogEffectEditor::RefreshEffects()
{
	RefreshPackageItems("_Effects", "Effects", &RefreshEffect, this);
}

