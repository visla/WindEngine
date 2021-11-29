/*
   CDialogCheckPoint.h

   Autor: Nebojsa Vislavski
   Date: 10.12.2005.
   Verision:1.0
   Platform: Win32
   
*/

#ifndef _CDIALOGCHECKPOINT_H
#define _CDIALOGCHEcKPOINT_H

#include <CDialog.h>
#include <CComponent.h>
#include <CEditorResourceManager.h>


class CDialogCheckPoint: public CDialog
{
protected:
	static CDialogCheckPoint *_instance;

	void OnClick(DWORD ID);
	void OnChange(DWORD ID);



public:	
	static CDialogCheckPoint *Instance();


	CEditBox *PositionXEdit;
	CEditBox *PositionYEdit;
	CEditBox *PositionZEdit;

	CEditBox *DimensionX;
	CEditBox *DimensionY;
	CEditBox *DimensionZ;

	CEditBox *Index;
	CCheckBox *LapCounter;


	CButton *OKButton;
	
	bool Create(HWND hParent, bool Show = true);
	void OnShow();

};



#endif