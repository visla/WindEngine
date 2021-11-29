/*
    CDialogCloth.cpp

	Autor: Nebojsa Vislavski
	Date: 26.5.2006.

*/

#include <CDialogCloth.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <CDialogInsert.h>


CDialogCloth *CDialogCloth::_instance = NULL;
CDialogCloth *CDialogCloth::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogCloth();
	return _instance;
}

bool CDialogCloth::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_CLOTHDIALOG, hParent, Show))
	{
		return false;
	}

	LowModel = new CEditBox(this, IDC_LOWMODELEDIT);
	MediumModel = new CEditBox(this, IDC_MEDIUMMODELEDIT);
	HighModel = new CEditBox(this, IDC_HIGHMODELEDIT);
	
	BendingStiffness = new CEditBox(this, IDC_BENDINGSTIFFNESS);
	StretchingStiffness = new CEditBox(this, IDC_STRETCHINGSTIFFNESS);
	DampingCoef = new CEditBox(this, IDC_DAMPINGCOEF);
	Friction = new CEditBox(this, IDC_FRICTION);
	Pressure = new CEditBox(this, IDC_PRESSURE);
	TearFactor = new CEditBox(this, IDC_TEARFACTOR);
	Thickness = new CEditBox(this, IDC_THICKNESS);
	AttachmentResponseCoef = new CEditBox(this, IDC_ATTACHMENTRESPONSECOEF);
	CollisionResponseCoef = new CEditBox(this, IDC_COLLISIONRESPONSECOEF);

	OKButton = new CButton(this, IDOK);
	CancelButton = new CButton(this, IDCANCEL);

	LowModelBrowse = new CButton(this, IDC_LOWEMODELBUTTON);
	MediumModelBrowse = new CButton(this, IDC_MEDIUMMODELBUTTON);
	HighModelBrowse = new CButton(this, IDC_HIGHMODELBUTTON);   

	AutoAttach = new CCheckBox(this, IDC_AUTOATTACH);
	Group = new CEditBox(this, IDC_CLOTHGROUP);
	Tearable = new CCheckBox(this, IDC_TEARABLE);
	Density = new CEditBox(this, IDC_DENSITY);
	Gravity = new CCheckBox(this, IDC_GRAVITY);
	OnlyHardware = new CCheckBox(this, IDC_ONLYPPU);
	
	return true;
}


void CDialogCloth::Update(WeCloth *cloth)
{
	CanChange = false;

	cloth->LowModelName = cloth->ModelName;
	LowModel->SetText(cloth->LowModelName);
	MediumModel->SetText(cloth->MediumModelName);
	HighModel->SetText(cloth->HighModelName);

	BendingStiffness->SetText(cloth->BendingStiffness);
	StretchingStiffness->SetText(cloth->StretchingStiffness);
	DampingCoef->SetText(cloth->DampingCoef);
	Friction->SetText(cloth->Friction);
	Pressure->SetText(cloth->Pressure);
	TearFactor->SetText(cloth->TearFactor);
	Thickness->SetText(cloth->Thickness);
	AttachmentResponseCoef->SetText(cloth->AttachmentResponseCoef);
	CollisionResponseCoef->SetText(cloth->CollisionResponseCoef);    

	AutoAttach->SetChecked(cloth->AutoAttachToColliding);
	Group->SetText(cloth->Group);
	Tearable->SetChecked(cloth->Tearable);
	Density->SetText(cloth->Density);
	Gravity->SetChecked(cloth->Gravity);
	OnlyHardware->SetChecked(cloth->OnlyHardware);



	CanChange = true;
}


void CDialogCloth::Show(WeCloth *cloth)
{
	if (!cloth) return;
	Cloth = cloth;
	Update(Cloth);    		
	CDialog::Show();
}


void CDialogCloth::OnOKClick()
{
	Hide();
}



void CDialogCloth::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) OnOKClick();	
	if (ID == AutoAttach->GetID()) Cloth->AutoAttachToColliding = AutoAttach->GetChecked();		
	if (ID == Tearable->GetID()) Cloth->Tearable = Tearable->GetChecked();		
	if (ID == Gravity->GetID()) Cloth->Gravity = Gravity->GetChecked();		
	if (ID == OnlyHardware->GetID()) Cloth->OnlyHardware = OnlyHardware->GetChecked();		

	if (ID == LowModelBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			CanChange = false;

			LowModel->SetText(InsertDialog->HoleName);
			SAFE_DELETE(Cloth->Model);
			Cloth->ModelName = LowModel->GetText();
			Cloth->LowModelName = LowModel->GetText();
			Cloth->Model = WeModelManager::Instance()->Load(LowModel->GetText());
			//EditorWindow->Selection->SetSelectedActor(TubePart);
			CanChange = true;
		}
	}
	if (ID == MediumModelBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			CanChange = false;

			MediumModel->SetText(InsertDialog->HoleName);
			Cloth->MediumModelName = MediumModel->GetText();			
			//EditorWindow->Selection->SetSelectedActor(TubePart);
			CanChange = true;
		}
	}
	if (ID == HighModelBrowse->GetID())
	{
		InsertDialog->Type = PACKAGEITEM_MODEL;
		if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
		{
			CanChange = false;

			MediumModel->SetText(InsertDialog->HoleName);
			Cloth->HighModelName = HighModel->GetText();			
			//EditorWindow->Selection->SetSelectedActor(TubePart);
			CanChange = true;
		}
	}

}

void CDialogCloth::OnChange(DWORD ID)
{
	if (!CanChange) return;


	if (ID == LowModel->GetID()) 
	{
		Cloth->ModelName = LowModel->GetText();
		Cloth->LowModelName = LowModel->GetText();
	}
	if (ID == MediumModel->GetID()) Cloth->MediumModelName = MediumModel->GetText();
	if (ID == HighModel->GetID()) Cloth->HighModelName = HighModel->GetText();

	if (ID == BendingStiffness->GetID()) Cloth->BendingStiffness = BendingStiffness->GetTextAsFloat();
	if (ID == StretchingStiffness->GetID()) Cloth->StretchingStiffness = StretchingStiffness->GetTextAsFloat();
	if (ID == DampingCoef->GetID()) Cloth->DampingCoef = DampingCoef->GetTextAsFloat();
	if (ID == Friction->GetID()) Cloth->Friction = Friction->GetTextAsFloat();
	if (ID == Pressure->GetID()) Cloth->Pressure = Pressure->GetTextAsFloat();
	if (ID == TearFactor->GetID()) Cloth->TearFactor = TearFactor->GetTextAsFloat();
	if (ID == Thickness->GetID()) Cloth->Thickness = Thickness->GetTextAsFloat();
	if (ID == AttachmentResponseCoef->GetID()) Cloth->AttachmentResponseCoef = AttachmentResponseCoef->GetTextAsFloat();
	if (ID == CollisionResponseCoef->GetID()) Cloth->CollisionResponseCoef = CollisionResponseCoef->GetTextAsFloat();
	if (ID == Group->GetID()) Cloth->Group = Group->GetTextAsInt();
	if (ID == Density->GetID()) Cloth->Density = Density->GetTextAsFloat();

}