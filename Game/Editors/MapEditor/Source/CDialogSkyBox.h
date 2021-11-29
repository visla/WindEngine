/*
     CDialogSkyBox.h

	 Autor: Nebojsa Vislavski
	 Date: 3.3.2006.
	 Version: 1.0

	 

*/

#ifndef _CDIALOGSKYBOX_H
#define _CDIALOGSKYBOX_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeSkyBox.h>

class CDialogSkyBox: public CDialog
{
protected:
	static CDialogSkyBox *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnShow();

	void OnStateListChange();

public:
	static CDialogSkyBox *Instance();

	CButton *OKButton;
	CButton *TopMaterialChoose;
	CButton *BottomMaterialChoose;
	CButton *LeftMaterialChoose;
	CButton *RightMaterialChoose;
	CButton *FrontMaterialChoose;
	CButton *BackMaterialChoose;

	CEditBox *TopMaterial;
	CEditBox *BottomMaterial;
	CEditBox *LeftMaterial;
	CEditBox *RightMaterial;
	CEditBox *FrontMaterial;
	CEditBox *BackMaterial;

	CEditBox *DimX;
	CEditBox *DimY;
	CEditBox *DimZ;
    
	bool CanChange;
	WeSkyBox *SkyBox;
	
    bool Create(HWND hParent, bool Show = true);
	void Show(WeSkyBox *skybox);
	void Update(WeSkyBox *skybox);

};





#endif

