/*
    CDialogAlign.h

	Autor: Nebojsa Vislavski
	Date: 16.12.2005.
	Version: 1.0
    

*/

#ifndef _CDIALOGALIGN_H
#define _CDIALOGALIGN_H

#include <CDialog.h>
#include <CComponent.h>


class CDialogAlign: public CDialog
{
protected:
	static CDialogAlign *_instance;


	void OnClick(DWORD ID);
	void OnShow();


public:
	static CDialogAlign *Instance();

	CButton *AlignButton;
	CButton *CancelButton;
	CCheckBox *XAxisAlign;
	CCheckBox *YAxisAlign;
	CCheckBox *ZAxisAlign;
	CCheckBox *YawAlign;
	CCheckBox *PitchAlign;
	CCheckBox *RollAlign;

	CCheckBox *AxisLocalAlign;

	bool alX;
	bool alY;
	bool alZ;

	bool alYaw;
	bool alPitch;
	bool alRoll;

	bool alAxis;
    bool Create(HWND hParent, bool Show = true);
};



#endif