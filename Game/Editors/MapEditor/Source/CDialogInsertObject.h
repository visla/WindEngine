#ifndef _DIALOG_INSERTOBJECT
#define _DIALOG_INSERTOBJECT


#include <CDialog.h>
#include <CComponent.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeActor.h>


class CDialogInsertObject:public CDialog
{
protected:
	static CDialogInsertObject* _instance;

	WeMatrix Transform;

	void OnClick(DWORD ID);

	void OnInsert();

public:
	static CDialogInsertObject* Instance();

	WeWorld* PreviewWorld;
	WeCamera* PreviewCamera;
	WeActor* PreviewActor;

	CTreeView* PackageTree;
	CListView* ObjectList;
	CButton* Select;
	CButton* Deselect;
	CButton* Insert;
	CButton* Cancel;
	CButton* PreviewArea;
	CCheckBox* IsAnimatedModel;
	CCheckBox* UseMultiInsert;
	CEditBox* MultiMask;

	int SelectedType;

	String HoleName;
	void UpdateHoleName();

	CDialogInsertObject();
	~CDialogInsertObject(){}

	void UpdateActor();
	void OnAnimatedModel();

	bool Create(HWND hParent, bool Shown = true);
	void OnShow();
};


#endif
