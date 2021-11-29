/*
     CDialogPhysicModelEditor.h


	 Autor: Nebojsa Vislavski
	 Date: 27.4.2006.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogPhysicModelEditor_H
#define _CDialogPhysicModelEditor_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeActor.h>
#include <WePhysicModel.h>

class CDialogPhysicModelEditor: public CDialog
{
protected:
	static CDialogPhysicModelEditor *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);

	bool CanChange;

	//WePhysicModel *PModel;

public:
	static CDialogPhysicModelEditor *Instance();


	CListBox*	DescList;
	CCheckBox*	OnlyOnHardware;
	CCheckBox*	SleepOnStart;
	CEditBox*	Name;
	CButton*	AssingModelName;
	CButton*	AssingModelNameAll;
	CEditBox*	GroupEdit;

    
    bool Create(HWND hParent, bool Show = true);
	void UpdateComponents();
	void FillListView();
};


#endif