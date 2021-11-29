/*
CDialogBaking.h


Autor: Nebojsa Vislavski
Date: 13.9.2006.
Version: 1.0
Platform: Win32

*/

#ifndef _CDialogBaking_H
#define _CDialogBaking_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeActor.h>


class CDialogBaking: public CDialog
{
protected:
	static CDialogBaking *_instance;


	void OnClick(DWORD ID);


public:
	static CDialogBaking *Instance();

	static bool	Canceled;
	CButton  *CancelButton;
	static CProgressBar* LoadProgress;
	static CLabel*	BakingLabel;

	bool Create(HWND hParent, bool Show = true);
	void OnShow();
	static void OnLoadProgress(int FacesLit, float Progress, bool &Stop);
	static void OnMapLoadProgress(float Progress, bool &Stop);
	static void OnResaveProgress(StringParam MeshName, float Progress, bool &Stop);

};


#endif