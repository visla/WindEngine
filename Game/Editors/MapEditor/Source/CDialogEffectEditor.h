#ifndef _DIALOG_EFFECTEDITOR
#define _DIALOG_EFFECTEDITOR


#include <CDialog.h>
#include <CComponent.h>
#include <WeEffectEditor.h>


extern class CDialogEffectEditor* EffectEditor;

class CDialogEffectEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnSave();
	void OnLoad();
	void OnBrowse();
	void OnOk();
	void OnAtachInfo();

	CButton* Save;
	CButton* Load;
	CButton* Browse;
	CButton* Close;
	CLabel*  EffectName;

public:
	CDialogEffectEditor(){}

	bool Create(HWND hParent, bool Shown = true);

	WeEffectEditor* EffectForEditor;

	void RefreshEffects();
};


#endif
