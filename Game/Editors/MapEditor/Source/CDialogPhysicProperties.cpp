

#include <CDialogPhysicProperties.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePathNode.h>
#include <CEditorMap.h>
//#include <CDialogMaterialPresets.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>
#include <WePhysicModel.h>
#include <WePhysics.h>
#include <WeMesh.h>
#include <direct.h>
#include <commdlg.h>


CDialogPhysicProperties *CDialogPhysicProperties::_instance = NULL;
CDialogPhysicProperties *CDialogPhysicProperties::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogPhysicProperties();
	return _instance;
}

bool CDialogPhysicProperties::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_PHYSICPROPERTIES, hParent, Show))
	{
		return false;
	}

	TypeCombo = new CComboBox(this, IDC_TYPECOMBO);
	BoxDimensionX = new CEditBox(this, IDC_BOXDIMENSIONX);
	BoxDimensionY = new CEditBox(this, IDC_BOXDIMENSIONY);
	BoxDimensionZ = new CEditBox(this, IDC_BOXDIMENSIONZ);
	SphereRadius = new CEditBox(this, IDC_SPHERERADIUS);
	CapsuleHeight = new CEditBox(this, IDC_CAPSULEHEIGHT);
	CapsuleRadius = new CEditBox(this, IDC_CAPSULERADIUS);
	SleepOnStart = new CCheckBox(this, IDC_SLEEPONSTART);
	Mass = new CEditBox(this, IDC_MASS);
	Group = new CEditBox(this, IDC_GROUP);
	TriggerTypeCombo = new CComboBox(this, IDC_TRIGGERTYPE);
	OK = new CButton(this, IDOK);
	Cancel = new CButton(this, IDCANCEL);
	GetFromObject = new CButton(this, IDC_GETFROMOBJECT);

	TypeCombo->Clear();
	TypeCombo->AddItem("NONE", PHYSICMODEL_NONE);
	TypeCombo->AddItem("STATIC SHAPE", PHYSICMODEL_STATIC_SHAPE);
	TypeCombo->AddItem("DYNAMIC BOX", PHYSICMODEL_DYNAMIC_BOX);
	TypeCombo->AddItem("DYNAMIC SPHERE", PHYSICMODEL_DYNAMIC_SPHERE);
	TypeCombo->AddItem("DYNAMIC CAPSULE", PHYSICMODEL_DYNAMIC_CYLINDER);
	TypeCombo->SetSelection(0);

	TriggerTypeCombo->Clear();
	TriggerTypeCombo->AddItem("None", 0);
	TriggerTypeCombo->AddItem("Always", WETRIGGER_ON_ENABLE);
	TriggerTypeCombo->AddItem("On Enter", WETRIGGER_ON_ENTER);
	TriggerTypeCombo->AddItem("On Leave", WETRIGGER_ON_LEAVE);
	TriggerTypeCombo->AddItem("On Stay", WETRIGGER_ON_STAY);
	TriggerTypeCombo->SetSelection(0);

	CanChange = false;

	Actor = NULL;	
	return true;
}

void CDialogPhysicProperties::OnShow()
{

}

void CDialogPhysicProperties::Update(WeActor *actor)
{
	CanChange = false;

	switch (actor->PhysicModel.Type)
	{
	case PHYSICMODEL_NONE:
		TypeCombo->SetSelection(0);
		break;

	case PHYSICMODEL_STATIC_SHAPE:
		TypeCombo->SetSelection(1);				
		break;
	case PHYSICMODEL_DYNAMIC_BOX:
		TypeCombo->SetSelection(2);
		break;

	case PHYSICMODEL_DYNAMIC_SPHERE:
		TypeCombo->SetSelection(3);
		break;
	case PHYSICMODEL_DYNAMIC_CYLINDER:
		TypeCombo->SetSelection(4);
		break;
	}

	switch (actor->PhysicModel.TriggerType)
	{
	case 0: TriggerTypeCombo->SetSelection(0); break;
	case WETRIGGER_ON_ENABLE: TriggerTypeCombo->SetSelection(1); break;
	case WETRIGGER_ON_ENTER: TriggerTypeCombo->SetSelection(2); break;
	case WETRIGGER_ON_LEAVE: TriggerTypeCombo->SetSelection(3); break;
	case WETRIGGER_ON_STAY: TriggerTypeCombo->SetSelection(4); break;
	}

	BoxDimensionX->SetText(actor->PhysicModel.BoxDimensions.x);
	BoxDimensionY->SetText(actor->PhysicModel.BoxDimensions.y);
	BoxDimensionZ->SetText(actor->PhysicModel.BoxDimensions.z);

	SphereRadius->SetText(actor->PhysicModel.SphereRadius);
	CapsuleHeight->SetText(actor->PhysicModel.CapsuleHeight);
	CapsuleRadius->SetText(actor->PhysicModel.CapsuleRadius);

	Group->SetText(actor->PhysicModel.ShapeGroup);
	SleepOnStart->SetChecked(actor->PhysicModel.SleepOnStart);
	Mass->SetText(actor->PhysicModel.Mass);
	
	CanChange = true;
}


void CDialogPhysicProperties::UpdateFromList()
{
	CanChange = false;

	PHYSICMODEL_TYPE pType;
	WORD wTriggerType;
	WeVector vBoxDimensions;
	float fSphereRadius;
	float fCapsuleHeight;
	float fCapsuleRadius;
	WORD wShapeGroup;
	float fMass;
	bool bSleepOnStart;

	ActorList.First();
	Actor = ActorList.GetItem();

	pType = ActorList.GetItem()->PhysicModel.Type;
	wTriggerType = ActorList.GetItem()->PhysicModel.TriggerType;
	vBoxDimensions = ActorList.GetItem()->PhysicModel.BoxDimensions;
	fSphereRadius = ActorList.GetItem()->PhysicModel.SphereRadius;
	fCapsuleHeight = ActorList.GetItem()->PhysicModel.CapsuleHeight;
	fCapsuleRadius = ActorList.GetItem()->PhysicModel.CapsuleRadius;
	wShapeGroup = ActorList.GetItem()->PhysicModel.ShapeGroup;
	fMass = ActorList.GetItem()->PhysicModel.Mass;
	bSleepOnStart = ActorList.GetItem()->PhysicModel.SleepOnStart;


	switch (pType)
	{
	case PHYSICMODEL_NONE:
		TypeCombo->SetSelection(0);
		break;

	case PHYSICMODEL_STATIC_SHAPE:
		TypeCombo->SetSelection(1);				
		break;
	case PHYSICMODEL_DYNAMIC_BOX:
		TypeCombo->SetSelection(2);
		break;

	case PHYSICMODEL_DYNAMIC_SPHERE:
		TypeCombo->SetSelection(3);
		break;
	case PHYSICMODEL_DYNAMIC_CYLINDER:
		TypeCombo->SetSelection(4);
		break;
	}

	switch (wTriggerType)
	{
	case 0: TriggerTypeCombo->SetSelection(0); break;
	case WETRIGGER_ON_ENABLE: TriggerTypeCombo->SetSelection(1); break;
	case WETRIGGER_ON_ENTER: TriggerTypeCombo->SetSelection(2); break;
	case WETRIGGER_ON_LEAVE: TriggerTypeCombo->SetSelection(3); break;
	case WETRIGGER_ON_STAY: TriggerTypeCombo->SetSelection(4); break;
	}

	BoxDimensionX->SetText(vBoxDimensions.x);
	BoxDimensionY->SetText(vBoxDimensions.y);
	BoxDimensionZ->SetText(vBoxDimensions.z);

	SphereRadius->SetText(fSphereRadius);
	CapsuleHeight->SetText(fCapsuleHeight);
	CapsuleRadius->SetText(fCapsuleRadius);

	Group->SetText(wShapeGroup);
	SleepOnStart->SetChecked(bSleepOnStart);
	Mass->SetText(fMass);

	for (; !ActorList.End(); ActorList.Next())
	{
		if (pType != ActorList.GetItem()->PhysicModel.Type) TypeCombo->SetText("");
		if (wTriggerType != ActorList.GetItem()->PhysicModel.TriggerType) TriggerTypeCombo->SetText("");
		if (vBoxDimensions.x != ActorList.GetItem()->PhysicModel.BoxDimensions.x) BoxDimensionX->SetText("");
		if (vBoxDimensions.y != ActorList.GetItem()->PhysicModel.BoxDimensions.y) BoxDimensionY->SetText("");
		if (vBoxDimensions.z != ActorList.GetItem()->PhysicModel.BoxDimensions.z) BoxDimensionZ->SetText("");
		if (fSphereRadius != ActorList.GetItem()->PhysicModel.SphereRadius) SphereRadius->SetText("");
		if (fCapsuleHeight != ActorList.GetItem()->PhysicModel.CapsuleHeight) CapsuleHeight->SetText("");
		if (fCapsuleRadius != ActorList.GetItem()->PhysicModel.CapsuleRadius) CapsuleRadius->SetText("");
		if (wShapeGroup != ActorList.GetItem()->PhysicModel.ShapeGroup) Group->SetText("");
		if (bSleepOnStart != ActorList.GetItem()->PhysicModel.SleepOnStart) SleepOnStart->SetChecked(false);
		if (fMass != ActorList.GetItem()->PhysicModel.Mass) Mass->SetText("");
	}

	CanChange = true;
}

void CDialogPhysicProperties::Show(WeActor *actor)
{
	ActorList.Clear();

	if (!actor) return;
	Actor = actor;
	Update(Actor);    		
	CDialog::Show();
}

void CDialogPhysicProperties::Show(CSmartList <WeActor *> &actorList)
{
	ActorList.Clear();
	for (actorList.First(); !actorList.End(); actorList.Next())
	{
		if (actorList.GetItem()->Type == WETYPE_ACTOR)
			ActorList.PushBack(actorList.GetItem());
	}

	UpdateFromList();
	CDialog::Show();	
}


void CDialogPhysicProperties::OnClick(DWORD ID)
{
	if (ID == OK->GetID()) 
	{
		Actor->PhysicModel.Type = (PHYSICMODEL_TYPE) TypeCombo->GetSelectionData();
		Actor->PhysicModel.BoxDimensions.x = BoxDimensionX->GetTextAsFloat();
		Actor->PhysicModel.BoxDimensions.y = BoxDimensionY->GetTextAsFloat();
		Actor->PhysicModel.BoxDimensions.z = BoxDimensionZ->GetTextAsFloat();
		Actor->PhysicModel.SphereRadius = SphereRadius->GetTextAsFloat();
		Actor->PhysicModel.CapsuleRadius = CapsuleRadius->GetTextAsFloat();
		Actor->PhysicModel.CapsuleHeight = CapsuleHeight->GetTextAsFloat();
		Actor->PhysicModel.Mass = Mass->GetTextAsFloat();
		Actor->PhysicModel.ShapeGroup = Group->GetTextAsInt();
		Actor->PhysicModel.TriggerType = (WORD)TriggerTypeCombo->GetSelectionData();
		Actor->PhysicModel.SleepOnStart = SleepOnStart->GetChecked();
		Actor->Initialize();

		WeActor *tmpA;
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
		{
			tmpA = ActorList.GetItem();
			tmpA->PhysicModel.Type = (PHYSICMODEL_TYPE) TypeCombo->GetSelectionData();
			tmpA->PhysicModel.BoxDimensions.x = BoxDimensionX->GetTextAsFloat();
			tmpA->PhysicModel.BoxDimensions.y = BoxDimensionY->GetTextAsFloat();
			tmpA->PhysicModel.BoxDimensions.z = BoxDimensionZ->GetTextAsFloat();
			tmpA->PhysicModel.SphereRadius = SphereRadius->GetTextAsFloat();
			tmpA->PhysicModel.CapsuleRadius = CapsuleRadius->GetTextAsFloat();
			tmpA->PhysicModel.CapsuleHeight = CapsuleHeight->GetTextAsFloat();
			tmpA->PhysicModel.Mass = Mass->GetTextAsFloat();
			tmpA->PhysicModel.ShapeGroup = Group->GetTextAsInt();
			tmpA->PhysicModel.TriggerType = (WORD)TriggerTypeCombo->GetSelectionData();
			tmpA->PhysicModel.SleepOnStart = SleepOnStart->GetChecked();
			tmpA->Initialize();
		}
		Hide();
	}

	if (ID == GetFromObject->GetID())
	{
		WeBox BBox;
		WeSphere BSphere;
		if (Actor->Model && Actor->Model->Mesh)
		{
			BBox = Actor->Model->Mesh->BoundingBox;		
			BSphere = Actor->Model->Mesh->BSphere;

			Actor->PhysicModel.BoxDimensions.x = (BBox.Max.x - BBox.Min.x) / 2.0f;
			Actor->PhysicModel.BoxDimensions.y = (BBox.Max.y - BBox.Min.y) / 2.0f;
			Actor->PhysicModel.BoxDimensions.z = (BBox.Max.z - BBox.Min.z) / 2.0f;
			Actor->PhysicModel.SphereRadius = BSphere.Radius;
			Actor->PhysicModel.CapsuleRadius = BSphere.Radius;
			Actor->PhysicModel.CapsuleHeight = BSphere.Radius;
		}

		WeActor *tmpA;
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
		{
			tmpA = ActorList.GetItem();			
			if (tmpA->Model && tmpA->Model->Mesh)
			{
				BBox = tmpA->Model->Mesh->BoundingBox;
				BSphere = tmpA->Model->Mesh->BSphere;

				tmpA->PhysicModel.BoxDimensions.x = (BBox.Max.x - BBox.Min.x) / 2.0f;
				tmpA->PhysicModel.BoxDimensions.y = (BBox.Max.y - BBox.Min.y) / 2.0f;
				tmpA->PhysicModel.BoxDimensions.z = (BBox.Max.z - BBox.Min.z) / 2.0f;
				tmpA->PhysicModel.SphereRadius = BSphere.Radius;
				tmpA->PhysicModel.CapsuleRadius = BSphere.Radius;
				tmpA->PhysicModel.CapsuleHeight = BSphere.Radius;
			}
		}
		if (ActorList.GetItemsCount() >= 2)
			UpdateFromList();
		else
			Update(Actor);
	}


	if (ID == Cancel->GetID())
	{
		Hide();
	}

}

void CDialogPhysicProperties::OnChange(DWORD ID)
{
	if (!CanChange) return;


}


