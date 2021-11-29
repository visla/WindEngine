#include <CDialogInsertObject.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CDialogTools.h>
#include <WeLight.h>
#include <WeModelManager.h>


//========================================================
//    InsertObjectDialog Class functions
//========================================================



CDialogInsertObject* CDialogInsertObject::_instance = NULL;

CDialogInsertObject* CDialogInsertObject::Instance()
{
	if(_instance) return _instance;
	_instance = new CDialogInsertObject();
	return _instance;
}



CDialogInsertObject::CDialogInsertObject()
{
	PreviewWorld = NULL;
	PreviewCamera = NULL;
	PreviewActor = NULL;
	HoleName = "";
	SelectedType = WETYPE_ACTOR;
}



bool CDialogInsertObject::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_OBJECTINSERTDILOG, hParent, Shown))
	{
		return true;
	}

	PackageTree = new CTreeView(this, IDC_INSERT_PACKAGETREE);
//	ObjectList = new CListView(this, IDC_INSERT_OBJECTLIST);
//	Select = new CButton(this, IDC_INSERT_SELECT);
//	Deselect = new CButton(this, IDC_INSERT_DESELECT);
	Cancel = new CButton(this,IDC_CANCEL);
	Insert = new CButton(this,IDC_OK);
	PreviewArea = new CButton(this, IDC_PREVBUTTON);
	IsAnimatedModel = new CCheckBox(this, IDC_ANIMATEDMESHES);
	UseMultiInsert = new CCheckBox(this, IDC_INSERT_MULTI);
	MultiMask = new CEditBox(this, IDC_INSERT_MULTIMASK);

	
	return true;
}



void CDialogInsertObject::OnShow()
{
	if(!PreviewWorld)
	{
		PreviewWorld = new WeWorld();

		PreviewCamera = new WeCamera();
		PreviewCamera->Location = WeVector(0,0,-15);

		//PreviewActor = new WeActor();
		//PreviewActor->Location = WeVector(0,0,0);
		//D3DXMatrixIdentity(&PreviewActor->TM);
		//PreviewActor->Renderable = true;

		//PreviewWorld->AddActor(PreviewActor);
		WeLight *tmpL;
		tmpL = new WeLight;
		WeLightState LS;				
		LS.Duration = 100;	
		tmpL->AffectDynamicActors = true;
		tmpL->Range = 1000.0f;
		LS.Diffuse.r = LS.Diffuse.g = LS.Diffuse.b = 1.0f;
		tmpL->AddKeyState(LS, 100);
		PreviewWorld->AddActor(tmpL);
	}

	//FillTree(PackageTree,PACKAGEITEM_MODEL);
	OnAnimatedModel();

	CTreeViewItem *Item;
	Item = FindItem(PackageTree, HoleName);
	if (Item)
		PackageTree->SelectItem(Item);

	UseMultiInsert->SetChecked(false);
}



void CDialogInsertObject::UpdateHoleName()
{
	//if(PackageTree->GetSelected())
	//{
	//	if(PackageTree->GetSelected()->GetLevel() != 2)
	//	{
	//		HoleName = "";
	//		return;
	//	}
	//}else{
	//	HoleName = "";
	//	return;
	//}

	//String Package = PackageTree->GetSelected()->GetParent()->GetParent()->GetText();
	//String Group = PackageTree->GetSelected()->GetParent()->GetText();
	//String Name = PackageTree->GetSelected()->GetText();

	//HoleName = Package + "." + Group + "." + Name;
}



void CDialogInsertObject::UpdateActor()
{
//	UpdateHoleName();
//	//static float Rotation = 0.0f;
//	
//	/*D3DXMatrixIdentity(&Transform);
//	if(PreviewActor->Renderable)
//	{
//		Rotation += (D3DX_PI)*WeTimer::Instance()->GetDeltaTimeF();
//		PreviewActor->SetOrientation(Rotation,0.0f,0.0f);
//	}*/
//
//	if(HoleName == (String)"") 
//	{
//		PreviewActor->Renderable = false;
////		Rotation = 0.0f;
//		return;
//	}
//
//	if(PreviewActor->ModelName == HoleName)
//	{
//		return;
//	}else{
//		PreviewActor->ModelName = HoleName;
//		PreviewActor->Initialize();
//		//PreviewActor->Model = WeModelManager::Instance()->Load(HoleName);
//		PreviewActor->Renderable = true;
//		PreviewActor->Model->ComputeBoundingBox(PreviewActor);
//		
//
//		float tempX = 0.0f;
//		float tempY = 0.0f;
//		float tempZ = 0.0f;
//
//		tempX = (PreviewActor->BBox.Max.x - PreviewActor->BBox.Min.x);
//		tempY = (PreviewActor->BBox.Max.y - PreviewActor->BBox.Min.y);
//		tempZ = PreviewActor->BBox.Min.z;
//		if(tempZ > 0) tempZ = 0;
//		else
//			tempZ *= -1;
//		
//
//		PreviewCamera->Location.z = -((tempX + tempY + tempZ)*1.3f);
//	}
}


void CDialogInsertObject::OnAnimatedModel()
{
	if(IsAnimatedModel->GetChecked())
		FillTree(PackageTree, PACKAGEITEM_ANIMMODEL);
	else
		FillTree(PackageTree, PACKAGEITEM_MODEL);
}


void CDialogInsertObject::OnClick(DWORD ID)
{
	if (ID == IDC_CANCEL) 
	{
		SAFE_DELETE(PreviewActor);
		Hide();
	}
	if (ID == IDC_OK) OnInsert();
	if (ID == IsAnimatedModel->GetID()) OnAnimatedModel();
	if (ID == PackageTree->GetID()) 
	{
		CTreeViewItem *Item;
		SAFE_DELETE(PreviewActor);

		Item = PackageTree->GetSelected();
		if (Item && (Item->GetLevel() == 2))
		{
			HoleName = GetFullItemName(Item);
			MultiMask->SetText(HoleName);

			PreviewActor = new WeActor();
			PreviewActor->Model = WeModelManager::Instance()->Load(HoleName);

			PreviewWorld->AddActor(PreviewActor);
			PreviewActor->Model->ComputeBoundingBox(PreviewActor);

			float tempX = 0.0f;
			float tempY = 0.0f;
			float tempZ = 0.0f;
	
			tempX = (PreviewActor->BBox.Max.x - PreviewActor->BBox.Min.x);
			tempY = (PreviewActor->BBox.Max.y - PreviewActor->BBox.Min.y);
			tempZ = PreviewActor->BBox.Min.z;
			if(tempZ > 0) tempZ = 0;
			else
				tempZ *= -1;			
	
			PreviewCamera->Location.z = -((tempX + tempY + tempZ)*1.3f);
		}
	}
}




void CDialogInsertObject::OnInsert()
{
	String MultiText = MultiMask->GetText();
	if (UseMultiInsert->GetChecked() && (MultiText.GetLen() != 0))
	{
		for (PackageTree->Items.First(); !PackageTree->Items.End(); PackageTree->Items.Next())
		{
			CTreeViewItem *Item;
			Item = &PackageTree->Items.GetItem();
			if (Item->GetLevel() == 2)
			{
				String ItemName = GetFullItemName(Item);
				if (ItemName.SubPos(MultiText) == 0)
					EditorWindow->InsertObject(SelectedType, ItemName);
			}
		}
	}
	else
	{
		CTreeViewItem *Item;
		Item = PackageTree->GetSelected();
		if (Item && (Item->GetLevel() == 2))
		{
			String ItemName = GetFullItemName(Item);
			EditorWindow->InsertObject(SelectedType, ItemName);
		}
	}

	SAFE_DELETE(PreviewActor);		
	IsAnimatedModel->SetChecked(false);
	Hide();
}

