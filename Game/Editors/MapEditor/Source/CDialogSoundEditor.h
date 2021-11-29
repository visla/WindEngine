#ifndef _DIALOG_SOUNDEDITOR
#define _DIALOG_SOUNDEDITOR


#include <CDialog.h>
#include <CComponent.h>
#include <WeSoundEditor.h>


extern class CDialogSoundEditor* DialogSoundEditor;

class CDialogSoundEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);

	CButton* Close;
	CButton* Load;
	CButton* Save;
	CButton* Play;
	CButton* Browse;
	CEditBox* SoundName;

	WeSoundEditor* Sound;

public:
	CDialogSoundEditor();
	~CDialogSoundEditor();

	void OnCloseClick();
	void OnBrowse();
	void OnLoad();
	void OnSave();
	void OnPlay();

	bool Create(HWND hParent, bool Shown);
};


#endif
