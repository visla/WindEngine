

#include <CDialogCheckPoint.h>


CDialogCheckPoint *CDialogCheckPoint::_instance = NULL;


CDialogCheckPoint *CDialogCheckPoint::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogCheckPoint;
	return _instance;
}


bool CDialogCheckPoint::Create(HWND hParent, bool Show)
{
	//if(!CDialog::Create(IDD_CHECKPOINT_DIALOG, hParent, Show))
	//{
	//	return false;
	//}


  /*  PositionXEdit = new CEditBox(this, IDC_POSITIONX);
	PositionYEdit = new CEditBox(this, IDC_POSITIONY);
	PositionZEdit = new CEditBox(this, IDC_POSITIONZ);


	DimensionX = new CEditBox(this, IDC_DIMENSIONX);
	DimensionY = new CEditBox(this, IDC_DIMENSIONY);
	DimensionZ = new CEditBox(this, IDC_DIMENSIONZ);

	LapCounter = new CCheckBox(this, IDC_LAPCOUNTER);
	Index = new CEditBox(this, IDC_INDEX);

	OKButton = new CButton(this, IDC_OK);*/
		
	return true;
}



void CDialogCheckPoint::OnShow()
{
}


void CDialogCheckPoint::OnClick(DWORD ID)
{
}

void CDialogCheckPoint::OnChange(DWORD ID)
{

}