/*
    CDialogAlign.cpp

	Autor: Nebojsa Vislavski
	Date: 16.12.2005.
	Version: 1.0
    

*/


#include <CDialogAlign.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>


CDialogAlign *CDialogAlign::_instance = NULL;
CDialogAlign *CDialogAlign::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogAlign();
	return _instance;
}

bool CDialogAlign::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_OBJECTALIGN, hParent))
	{
		return false;
	}
	return true;
}


void CDialogAlign::OnShow()
{
	AlignButton = new CButton(this, IDC_ALIGNBUTTON);
	CancelButton = new CButton(this, IDC_CANCEL);

	XAxisAlign = new CCheckBox(this, IDC_XAXISCHECK);
	XAxisAlign->SetChecked(true);
	YAxisAlign = new CCheckBox(this, IDC_YAXISCHECK);
	YAxisAlign->SetChecked(true);
	ZAxisAlign = new CCheckBox(this, IDC_ZAXISCHECK);
	ZAxisAlign->SetChecked(true);

	YawAlign = new CCheckBox(this, IDC_YAWCHECK);
	YawAlign->SetChecked(true);
	PitchAlign = new CCheckBox(this, IDC_PITCHCHECK);
	PitchAlign->SetChecked(true);
	RollAlign = new CCheckBox(this, IDC_ROLLCHECK);
	RollAlign->SetChecked(true);

	AxisLocalAlign = new CCheckBox(this, IDC_LOCALAXIS);
	AxisLocalAlign->SetChecked(true);
}





void CDialogAlign::OnClick(DWORD ID)
{
	if (ID == AlignButton->GetID()) 
	{
		alX = XAxisAlign->GetChecked();
		alY = YAxisAlign->GetChecked();
		alZ = ZAxisAlign->GetChecked();
		alYaw = YawAlign->GetChecked();
		alPitch = PitchAlign->GetChecked();
		alRoll = RollAlign->GetChecked();
		alAxis = AxisLocalAlign->GetChecked();

		SetModalResult(MR_OK);
	}
	if (ID == CancelButton->GetID()) SetModalResult(MR_CANCEL);
}

