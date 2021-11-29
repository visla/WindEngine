

#include <CDialogPickupProperties.h>
#include <CEditorResourceManager.h>
#include <CPickUp.h>


CDialogPickupProperties *CDialogPickupProperties::_instance = NULL;
CDialogPickupProperties *CDialogPickupProperties::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogPickupProperties();
	return _instance;
}

bool CDialogPickupProperties::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_DIALOGPICKUPPROPERTIES, hParent, Show))
	{
		return false;
	}

	OKButton = new CButton(this, IDC_BUTTONOK);
	Active = new CCheckBox(this, IDC_CHECKACTIVE);
	ShouldTerminate = new CCheckBox(this, IDC_CHECKSHOULDTERMINATE);
	RespawnTime = new CEditBox(this, IDC_RESPAWNTIMEEDIT);
   	
	return true;
}


void CDialogPickupProperties::Update()
{
	PickUp->Active = Active->GetChecked();
	PickUp->ShouldTerminate = ShouldTerminate->GetChecked();
	PickUp->RespawnTime = RespawnTime->GetTextAsFloat();
}


void CDialogPickupProperties::Show(CPickUp* _PickUp)
{
	CDialog::Show();
	PickUp = _PickUp;
	Active->SetChecked(PickUp->Active);
	ShouldTerminate->SetChecked(PickUp->ShouldTerminate);
	RespawnTime->SetText(PickUp->RespawnTime);
}



void CDialogPickupProperties::OnClick(DWORD ID)
{
	Update();
	if (ID == OKButton->GetID())
	{
        Hide();
	}
}
