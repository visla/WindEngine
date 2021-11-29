

#include <CDialogZone.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePathNode.h>
#include <CEditorMap.h>


CDialogZone *CDialogZone::_instance = NULL;
CDialogZone *CDialogZone::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogZone();
	return _instance;
}

bool CDialogZone::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_ZONEDIALOG, hParent, Show))
	{
		return false;
	}

	BoxSizeX = new CEditBox(this, IDC_SIZEX);
	BoxSizeY = new CEditBox(this, IDC_SIZEY);
	BoxSizeZ = new CEditBox(this, IDC_SIZEZ);
	Name = new CEditBox(this, IDC_NAME);
	Portals = new CListBox(this, IDC_PORTALLIST);
	OKButton = new CButton(this, IDC_OK);
	
	return true;
}


void CDialogZone::Update(WeZone *zone)
{
	CanChange = false;

	Name->SetText(zone->Name);	
	float size;
	size = zone->Box.Max.x - zone->Box.Min.x;
	BoxSizeX->SetText(size);
	size = zone->Box.Max.y - zone->Box.Min.y;
	BoxSizeY->SetText(size);
	size = zone->Box.Max.z - zone->Box.Min.z;
	BoxSizeZ->SetText(size);
	CanChange = true;
}


void CDialogZone::Show(WeZone *zone)
{
	if (!zone) return;
	Zone = zone;
	Update(Zone);    		
	CDialog::Show();
}






void CDialogZone::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) 	Hide();
}

void CDialogZone::OnChange(DWORD ID)
{
	if (!CanChange) return;

	if (ID == Name->GetID()) Zone->Name = Name->GetText();
	if (ID == BoxSizeX->GetID() || ID == BoxSizeY->GetID() || ID == BoxSizeZ->GetID()) 
	{
		if (BoxSizeX->GetTextAsFloat() != 0)
		{
			Zone->Box.Min.x = -BoxSizeX->GetTextAsFloat() / 2.0f;
			Zone->Box.Max.x = BoxSizeX->GetTextAsFloat() / 2.0f;
		}
		if (BoxSizeY->GetTextAsFloat() != 0)
		{
			Zone->Box.Min.y = -BoxSizeY->GetTextAsFloat() / 2.0f;
			Zone->Box.Max.y = BoxSizeY->GetTextAsFloat() / 2.0f;
		}
		if (BoxSizeZ->GetTextAsFloat() != 0)
		{
			Zone->Box.Min.z = -BoxSizeZ->GetTextAsFloat() / 2.0f;
			Zone->Box.Max.z = BoxSizeZ->GetTextAsFloat() / 2.0f;
		}
	}


}


