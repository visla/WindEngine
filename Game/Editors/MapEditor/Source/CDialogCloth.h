/*
    CDialogCloth.h

	Autor: Nebojsa Vislavki
	Date: 26.6.2006.
	Version: 1.0
    
*/

#ifndef _CDialogCloth_H
#define _CDialogCloth_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeCloth.h>
#include <CMapEditorWindow.h>

class CDialogCloth: public CDialog
{
protected:
	static CDialogCloth *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }

	void OnModelNameChange();
	void OnPositionChange();
	void OnOrientationChange();
	void OnRenderableChange();
	void OnOKClick();
	void OnModelBrowseClick();	

	bool CanChange;


public:
	static CDialogCloth *Instance();


	CEditBox *LowModel;
	CEditBox *MediumModel;
	CEditBox *HighModel;
	CEditBox *BendingStiffness;
	CEditBox *StretchingStiffness;
	CEditBox *DampingCoef;
	CEditBox *Friction;
	CEditBox *Pressure;
	CEditBox *TearFactor;
	CEditBox *Thickness;
	CEditBox *AttachmentResponseCoef;
	CEditBox *CollisionResponseCoef;
	CEditBox *Group;
	CEditBox *Density;
	
	

	
	CButton	*LowModelBrowse;
	CButton	*MediumModelBrowse;
	CButton	*HighModelBrowse;

	CCheckBox* AutoAttach;
	CCheckBox* Tearable;
	CCheckBox* Gravity;
	CCheckBox* OnlyHardware;



	CButton	*OKButton;
	CButton *CancelButton;

	WeCloth *Cloth;


    bool Create(HWND hParent, bool Show = true);
	void Show(WeCloth *);
	void Update(WeCloth *);
};


#endif

