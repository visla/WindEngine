

#include <CDialogPortal.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePathNode.h>
#include <CEditorMap.h>
#include <WeZone.h>


CDialogPortal *CDialogPortal::_instance = NULL;
CDialogPortal *CDialogPortal::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogPortal();
	return _instance;
}

bool CDialogPortal::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_PORTALDIALOG, hParent, Show))
	{
		return false;
	}

	OKButton = new CButton(this, IDC_OK);
	Zone1Name = new CComboBox(this, IDC_ZONE1NAME);
	Zone2Name = new CComboBox(this, IDC_ZONE2NAME);
	Name = new CEditBox(this, IDC_NAME);
	TwoWayPortal = new CCheckBox(this, IDC_TWOWAY);
	SizeX = new CEditBox(this, IDC_SIZEX);
	SizeY = new CEditBox(this, IDC_SIZEY);

	CanChange = false;	
	return true;
}


void CDialogPortal::Update(WePortal *portal)
{
	CanChange = false;

	Zone1Name->Clear();
	Zone2Name->Clear();

	Strings sl;
	
	for (EditorWindow->MapEditorWorld->Zones.First(); !EditorWindow->MapEditorWorld->Zones.End(); EditorWindow->MapEditorWorld->Zones.Next())
	{
		Zone1Name->AddItem(EditorWindow->MapEditorWorld->Zones.GetItem()->Name);
		Zone2Name->AddItem(EditorWindow->MapEditorWorld->Zones.GetItem()->Name);
	}

	MessageBox(0, portal->Zone1Name, 0, 0);
	MessageBox(0, portal->Zone2Name, 0, 0);

	Zone1Name->SetSelection(portal->Zone1Name);
	Zone2Name->SetSelection(portal->Zone2Name);
	Name->SetText(portal->Name);
	TwoWayPortal->SetChecked(portal->TwoWayPortal);
	SizeX->SetText(portal->SizeX);
	SizeY->SetText(portal->SizeY);
	CanChange = true;
}


void CDialogPortal::Show(WePortal *portal)
{
	if (!portal) return;
	Portal = portal;
	Update(Portal);    		
	CDialog::Show();
}





void CDialogPortal::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) Hide();
	if (ID == Zone1Name->GetID()) Portal->Zone1Name = Zone1Name->GetSelectionText();
	if (ID == Zone2Name->GetID()) Portal->Zone2Name = Zone2Name->GetSelectionText();
}

void CDialogPortal::OnChange(DWORD ID)
{
	if (!CanChange) return;

	if (ID == Name->GetID()) Portal->Name = Name->GetText();
	if (ID == TwoWayPortal->GetID()) Portal->TwoWayPortal = TwoWayPortal->GetChecked();
	if (ID == SizeX->GetID()) Portal->SizeX = SizeX->GetTextAsFloat();
	if (ID == SizeY->GetID()) Portal->SizeY = SizeY->GetTextAsFloat();	

}


