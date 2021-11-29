/*
    CDialogTubersEditor.h

	Autor: Nebojsa Vislavski
	Date: 16.3.2006.
	Version: 1.0


*/

#ifndef _CDIALOGTUBERSEDITOR_H
#define _CDIALOGTUBERSEDITOR_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTuberDescription.h>

class CDialogTubersEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	
	void OnSave();
	void OnLoad();
	void OnNew();
	void OnRemove();
		
	CTuberDescription TuberDesc;
	static CDialogTubersEditor *_instance;

public:
	static CDialogTubersEditor *Instance();
	CDialogTubersEditor(){}


	CButton*	SaveButton;
	CButton*	RemoveButton;


	CEditBox*	TuberNameEdit;
	CEditBox*	ModelNameEdit;
	CButton*	ModelBrowseButton;
	CEditBox*	MaxSpeedEdit;
	CEditBox*	MaxAccelerationEdit;
	CEditBox*	ManouverabilityEdit;
	CComboBox*	WeaponName;
	CComboBox*	MineName;
	CEditBox*	HP;
	CEditBox*	Price;
	CComboBox*	TournamentRequired;
	CComboBox*	DescList;
	CEditBox*	Description;
	CEditBox*	PhysicModel;
	CButton*	PhysicModelButton;



	
	CEditBox*	PartModelName;
	CButton*	PartModelBrowseButton;
	CEditBox*	PartQnt;	
	CComboBox*	PartNum;
	

	void		FillWeaponCombo();
	bool		Create(HWND hParent, bool Show = true);	
	void		FillTubersList();
	void		ShowPartProps();
	void		UpdateDescription(StringParam TuberName);
	void		ShowDesc();
	
};



#endif
