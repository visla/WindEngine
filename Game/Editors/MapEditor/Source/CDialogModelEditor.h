#ifndef _DIALOG_MODELEDITOR
#define _DIALOG_MODELEDITOR


#include <CDialog.h>
#include <CComponent.h>
#include <WeMesh.h>


extern class CDialogModelEditor* ModelEditor;


class CDialogModelEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnShow();

public:
	CDialogModelEditor(){}

	CEditBox*  MeshName;
	CEditBox*  QueryMeshName;
	CGroupBox* PreviewGroup;
	CComboBox* MeshPreviewCombo;
	CButton*   MoreButton;

	WeMesh* Mesh;

	bool AllVisible;   // Is rendered part off a window visible or not
	bool WireFrame;
	
	bool Create(HWND hParent, bool Show = true);
};


#endif
