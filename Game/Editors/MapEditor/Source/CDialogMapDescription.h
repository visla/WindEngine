/*
     CDialogMapDescription.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDialogMapDescription_H
#define _CDialogMapDescription_H

#include <CDialog.h>
#include <CComponent.h>
#include <CMap.h>

class CDialogMapDescription: public CDialog
{
protected:
	static CDialogMapDescription *_instance;

	void OnOKClick();

	void OnClick(DWORD ID);
	void OnChange(DWORD ID);

	bool CanChange;


public:
	static CDialogMapDescription *Instance();

	CButton  *OKButton;
	CButton  *CancelButton;

	CEditBox *MapDescription;
	CButton  *MapDescriptionImageBrowse;
	CEditBox *MapDescriptionImage;
	CEditBox *RaceTime;
	CEditBox *LoadingImage;
	CButton  *LoadingImageBrowse;
	CEditBox *StatMusic;

	CButton		*SkyColor;
	CButton		*GroundColor;
	CCheckBox	*FogCheck;
	CEditBox	*FogStart;
	CEditBox	*FogEnd;
	CButton		*FogColor;

	CEditBox	*ClipDistance;
	CEditBox	*MusicFile;

	CEditBox	*(FHDRI[3]);

	CMap *Map;


    bool Create(HWND hParent, bool Show = true);
	void Update(CMap *Map);	
	void Show(CMap *Map);	
};


#endif