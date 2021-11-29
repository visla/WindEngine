/*
     CDialogPathNode.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/


#ifndef _CDIALOGPATHNODE_H
#define _CDIALOGPATHNODE_H


#include <CDialog.h>
#include <CComponent.h>
#include <CEditorResourceManager.h>
#include <WePathNode.h>
#include <CMapEditorWindow.h>

class CDialogPathNode: public CDialog
{
protected:
	static CDialogPathNode *_instance;

    void OnClick(DWORD ID);
	void OnChange(DWORD ID);

	void OnPositionChange();
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }


	bool CanChange;

public:
	static CDialogPathNode *Instance();

	CEditBox *PositionXEdit;
	CEditBox *PositionYEdit;
	CEditBox *PositionZEdit;

	CEditBox *SequenceNumber;
	CEditBox *FHDRI;
	CEditBox *HealingRange;
	CEditBox *HealingSpeed;
	CComboBox *HeadLight;
	CComboBox *NodeType;
	CEditBox *LeftNum;
	CEditBox *RightNum;
	CEditBox *FollowTime;
	CCheckBox *Refraction;
	CCheckBox *Shaking;

	CButton  *ResetButton;
	CButton  *OKButton;

	WePathNode *PathNode;

	bool Create(HWND hParent, bool Show = true);
	void Show(WePathNode *pathNode);
	void Update(WePathNode *pathNode);
};

#endif