#ifndef _DIALOG_RENDERPHASES
#define _DIALOG_RENDERPHASES


#include <CDialog.h>
#include <CComponent.h>
#include <WeWorld.h>


extern class CDialogRenderPhases* DialogRenderPhases;

class CDialogRenderPhases: public CDialog
{
protected:
	CCheckBox *(CheckRenderPhase[RENDERPHASE_COUNT]);

public:
	bool Create(HWND hParent, bool Show = true);

	void OnShow();
	void OnHide();
	void OnClick(DWORD ID);

};


#endif
