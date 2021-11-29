/*
    CDialogTubeConnect.h

	Autor: Nebojsa Vislavski
	Date: 22.12.2005.
	Version: 1.0


*/

#ifndef _CDIALOGTUBECONNECT_H
#define _CDIALOGTUBECONNECT_H

#include <CDialog.h>
#include <CComponent.h>
#include <CTubeSegment.h>

class CDialogTubeConnect: public CDialog
{
protected:
	static CDialogTubeConnect *_instance;


	void OnClick(DWORD ID);
	void OnShow();


public:
	static CDialogTubeConnect *Instance();

	CButton *OkButton;
	CComboBox *ConnectCombo;

	CTubeSegmentInfo *Sinfo;
	int PivotIndex;

    bool Create(HWND hParent, bool Show = true);
};


#endif