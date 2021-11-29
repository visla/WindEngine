#ifndef _DIALOG_POSTPROCESS
#define _DIALOG_POSTPROCESS


#include <CDialog.h>
#include <CComponent.h>


class CDialogPostProceses : public CDialog
{
protected:
	void OnClick(DWORD ID);

	void OnAdd();
	void OnRemove();
	void OnPreview();
	void OnClearAll();
	void OnCloseClick();

	CButton* AddEffect;
	CButton* RemoveEffect;
	CButton* ClearAll;
	CButton* Preview;
	CButton* Close;
	CEditBox* ScaleX;
	CEditBox* ScaleY;
	CListBox* EffectList;

private:
	static CDialogPostProceses* _instance;

public:
	CDialogPostProceses();
	~CDialogPostProceses();
	static CDialogPostProceses* Instance();

	bool Create(HWND hParent, bool Shown = true);
};



#endif
