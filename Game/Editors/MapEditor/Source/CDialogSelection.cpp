#include <CDialogSelection.h>
#include <CMapEditorWindow.h>
#include <CEditorResourceManager.h>


CDialogSelection *CDialogSelection::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogSelection;
	return _instance;
}

CDialogSelection *CDialogSelection::_instance = NULL;


bool CDialogSelection::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_SELECTION, hParent, Show))
	{
		return false;
	}

	Prefabs = new CCheckBox(this, IDC_SELECTION_ACTORS);
	Prefabs->SetChecked(true);

	Lights = new CCheckBox(this, IDC_SELECTION_LIGHTS);
	Lights->SetChecked(true);

	PathNodes = new CCheckBox(this, IDC_SELECTION_PATHNODES);
	PathNodes->SetChecked(true);
	
	StartLocations = new CCheckBox(this, IDC_SELECTION_STARTLOCATIONS);
	StartLocations->SetChecked(true);

	ParticleSystems = new CCheckBox(this, IDC_SELECTION_PARTICLESYSTEMS);
	ParticleSystems->SetChecked(true);

	return true;
}


void CDialogSelection::OnShow()
{
	Prefabs->SetChecked(EditorWindow->Selections.SelectPrefabs);
	Lights->SetChecked(EditorWindow->Selections.SelectLights);
	PathNodes->SetChecked(EditorWindow->Selections.SelectPathNodes);
	StartLocations->SetChecked(EditorWindow->Selections.SelectStartLocations);
	ParticleSystems->SetChecked(EditorWindow->Selections.SelectParticleSystems);
}



void CDialogSelection::OnClick(DWORD ID)
{
	switch (ID)
	{

	case IDOK:
		EditorWindow->Selections.SelectPrefabs = Prefabs->GetChecked();
		EditorWindow->Selections.SelectLights = Lights->GetChecked();
		EditorWindow->Selections.SelectPathNodes = PathNodes->GetChecked();
		EditorWindow->Selections.SelectStartLocations = StartLocations->GetChecked();
		EditorWindow->Selections.SelectParticleSystems = ParticleSystems->GetChecked();
		Hide();
		break;

	case IDC_SELECTION_ALL:
		Prefabs->SetChecked(true);
		Lights->SetChecked(true);
		PathNodes->SetChecked(true);
		StartLocations->SetChecked(true);
		ParticleSystems->SetChecked(true);
		break;

	case IDC_SELECTION_NONE:
		Prefabs->SetChecked(false);
		Lights->SetChecked(false);
		PathNodes->SetChecked(false);
		StartLocations->SetChecked(false);
		ParticleSystems->SetChecked(false);
		break;

	case IDC_SELECTION_INVERT:
		Prefabs->SetChecked(!Prefabs->GetChecked());
		Lights->SetChecked(!Lights->GetChecked());
		PathNodes->SetChecked(!PathNodes->GetChecked());
		StartLocations->SetChecked(!StartLocations->GetChecked());
		ParticleSystems->SetChecked(!ParticleSystems->GetChecked());
		break;

	}
}
