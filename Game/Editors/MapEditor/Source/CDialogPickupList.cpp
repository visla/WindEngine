/*
		File: CDialogPickupList.cpp
		Author: Tomislav Kukic
		Date: 24.03.2006
		Version: 1.0
*/


#include <CEditorResourceManager.h>
#include <CComponent.h>
#include <CDialogPickupList.h>
#include <CPickupFactory.h>


CDialogPickupList* CDialogPickupList::Instance = new CDialogPickupList();


CDialogPickupList::CDialogPickupList()
{
	Type = -2;
}


bool CDialogPickupList::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_PICKUPLIST, hParent))
	{
		return false;
	}
	return true;
}


void CDialogPickupList::OnClick(DWORD ID)
{
	if(ID == PickupList->GetID()) Type = (int)PickupList->GetSelectionData();
	if(ID == IDC_INSERT) OnOk();
	if(ID == IDC_CANCEL) OnCancel();
}


void CDialogPickupList::OnShow()
{
	Type = -2;
	PickupList = new CListBox(this, IDC_PICKUPLIST);
	//PickupList->AddItem("Health",PICKUPTYPE_HEALTH);
	PickupList->AddItem("Blur mine",PICKUPTYPE_BLURMINE);
	PickupList->AddItem("Electric mine",PICKUPTYPE_ELECTRICMINE);
	PickupList->AddItem("Forcefield mine",PICKUPTYPE_FORCEFIELDMINE);
	PickupList->AddItem("Fronfield generator",PICKUPTYPE_FRONTFIELDGENERATOR);
	PickupList->AddItem("Invisible mine",PICKUPTYPE_INVISIBLEMINE);
	PickupList->AddItem("Mini gun",PICKUPTYPE_MINIGUN);
	PickupList->AddItem("Mini rockets",PICKUPTYPE_MINIROCKETS);
	PickupList->AddItem("Moving mine",PICKUPTYPE_MOVINGMINE);
	PickupList->AddItem("Nuke rocket",PICKUPTYPE_NUKEROCKET);
	PickupList->AddItem("Quantum laser",PICKUPTYPE_QUANTUMLASER);
	PickupList->AddItem("Random weapon",PICKUPTYPE_RANDOMWEAPON);
	PickupList->AddItem("Slowing rocket",PICKUPTYPE_SLOWINGROCKET);
	PickupList->AddItem("Super nuke mine",PICKUPTYPE_SUPERNUKEMINE);
	PickupList->AddItem("Translocator",PICKUPTYPE_TRANSLOCATOR);
	PickupList->AddItem("Gravity mine",PICKUPTYPE_GRAVITYMINE);
	PickupList->AddItem("Homing mine",PICKUPTYPE_HOMINGMINE);
	PickupList->AddItem("Proximity mine",PICKUPTYPE_PROXIMITYMINE);
	PickupList->AddItem("Slowing mine",PICKUPTYPE_SLOWINGMINE);
	PickupList->AddItem("Smoke mine",PICKUPTYPE_SMOKEMINE);
	PickupList->AddItem("Standard mine",PICKUPTYPE_STANDARDMINE);
	PickupList->AddItem("Standard rocket",PICKUPTYPE_STANDARDROCKET);
	PickupList->AddItem("Standard bolt",PICKUPTYPE_STANDARDLASER);
	PickupList->AddItem("Double bolt",PICKUPTYPE_DOUBLELASER);
	PickupList->AddItem("Quad bolt",PICKUPTYPE_QUADLASER);
	PickupList->AddItem("Small Coin", PICKUPTYPE_COINSMALL);
	PickupList->AddItem("Medium Coin", PICKUPTYPE_COINMEDIUM);
	PickupList->AddItem("Large Coin", PICKUPTYPE_COINLARGE);
	PickupList->AddItem("Invisibility", PICKUPTYPE_INVISIBILITY);
	PickupList->AddItem("Invulnerability", PICKUPTYPE_INVULNERABILITY);
	PickupList->AddItem("Double Damage", PICKUPTYPE_DOUBLEDAMAGE);
	PickupList->AddItem("Quad Damage", PICKUPTYPE_QUADDAMAGE);
}


void CDialogPickupList::OnOk()
{
	SetModalResult(MR_OK);
}


void CDialogPickupList::OnCancel()
{
	Type = -2;
	SetModalResult(MR_CANCEL);
}

