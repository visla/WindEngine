/*
    CDialogActorTypes.cpp

	Autor: Nebojsa Vislavski
	Date: 19.12.2005.
	Version: 1.0
    

*/


#include <CDialogActorTypes.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CActorTypesSupplier.h>


CDialogActorTypes *CDialogActorTypes::_instance = NULL;
CDialogActorTypes *CDialogActorTypes::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogActorTypes();
	return _instance;
}

bool CDialogActorTypes::Create(HWND hParent, bool Show)
{
	if(!CDialog::CreateModal(IDD_ACTORTYPEDIALOG, hParent))
	{
		return false;
	}
	return true;
}


void CDialogActorTypes::OnShow()
{
	OKButton = new CButton(this, IDOK);
	ActorTypesCombo = new CComboBox(this, IDC_ACTORTYPESCOMBO);

	for (int i = 0; i < MAX_ACTOR_TYPES; i++)
	{
		ActorTypesCombo->AddItem(CActorTypesSupplier::Instance()->Types[i].Description, CActorTypesSupplier::Instance()->Types[i].Type);
	}
	ActorTypesCombo->SetSelection(0);


}





void CDialogActorTypes::OnClick(DWORD ID)
{
	if (ID == ActorTypesCombo->GetID())
	{
		
		
	}
	if (ID == OKButton->GetID()) 
	{
		TypeSelected = ActorTypesCombo->GetSelectionData();
		SetModalResult(MR_OK);
	}
}

