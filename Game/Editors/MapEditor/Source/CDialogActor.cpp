

#include <CDialogActor.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePathNode.h>
#include <CEditorMap.h>
//#include <CDialogMaterialPresets.h>
#include <CDialogInsert.h>
#include <CDialogPhysicProperties.h>
#include <CDialogSave.h>
#include <WeMesh.h>
#include <direct.h>
#include <commdlg.h>


CDialogActor *CDialogActor::_instance = NULL;
CDialogActor *CDialogActor::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogActor();
	return _instance;
}

bool CDialogActor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_ACTOR, hParent, Show))
	{
		return false;
	}

	Renderable = new CCheckBox(this, IDC_RENDERABLE);
	OKButton = new CButton(this, IDC_OK);
	ModelNameEdit = new CEditBox(this, IDC_MODELNAME);
	ModelBrowseButton = new CButton(this, IDC_MODELNAMEBROWSE);
	PositionXEdit = new CEditBox(this, IDC_POSITIONX);
	PositionYEdit = new CEditBox(this, IDC_POSITIONY);
	PositionZEdit = new CEditBox(this, IDC_POSITIONZ);
	YawEdit = new CEditBox(this, IDC_YAW);
	PitchEdit = new CEditBox(this, IDC_PITCH);
	RollEdit = new CEditBox(this, IDC_ROLL);
	MaterialPreset = new CComboBox(this, IDC_MATERIALPRESETCOMBO);


	PhysicModelEdit = new CEditBox(this, IDC_PMODELNAME);
	PhysicModelButton = new CButton(this, IDC_PMODELNAMEBROWSE);

	TypeCombo = new CComboBox(this, IDC_TYPECOMBO);
	ReceiveProjector = new CCheckBox(this, IDC_RECEIVEPROJECTOR);
	CastShadow = new CCheckBox(this, IDC_CASTSHADOW);
	ReceiveShadow = new CCheckBox(this, IDC_RECEIVESHADOW);
	Teleport = new CCheckBox(this, IDC_TELEPORTCHECK);
	TeleportRelative = new CCheckBox(this, IDC_TELEPORTRELATIVE);
	TeleportRadius = new CEditBox(this, IDC_TELEPORTRADIUS);
	DestinationNameEdit = new CEditBox(this, IDC_DESTINATIONNAME);
	IsTrigger = new CCheckBox(this, IDC_ISTRIGGERCHECKBOX);
	GhostObject = new CCheckBox(this, IDC_GHOSTOBJECT);
	NameEdit = new CEditBox(this, IDC_NAME);
	LifeTime = new CEditBox(this, IDC_LIFETIME);
	PassFlares = new CCheckBox(this, IDC_PASSFLARES);
	LightOccluder = new CCheckBox(this, IDC_LIGHTOCCLUDER);
	MaxRenderDistance = new CEditBox(this, IDC_MAXRENDERDISTANCE);	
	ApplyMaxRenderDistance = new CButton(this, IDC_APPLYTOALLOBJECTS);
	SoundName = new CEditBox(this, IDC_SOUND);
	SoundDistance = new CEditBox(this, IDC_SOUNDDISTANCE);
	PhysicsButton = new CButton(this, IDC_PHYSICSBUTTON);


	TypeCombo->Clear();
	TypeCombo->AddItem("Dynamic");
	TypeCombo->AddItem("Static");
	TypeCombo->SetSelection(0);


	CanChange = false;

	Actor = NULL;


	for (int i = 0; i < MAX_MATERIAL_PRESETS2; i++)
	{
		char tmp[100];
		sprintf(tmp, "Preset %d", i);
		MaterialPreset->AddItem(tmp, i);
	}
	MaterialPreset->SetSelection(0);
	
	return true;
}




void CDialogActor::Update(WeActor *actor)
{
	CanChange = false;

	Renderable->SetChecked(actor->Renderable);
	LifeTime->SetText(actor->LifeTime);
	RollEdit->SetText(actor->Roll*180 / D3DX_PI);
	PitchEdit->SetText(actor->Pitch*180 / D3DX_PI);
	YawEdit->SetText(actor->Yaw*180 / D3DX_PI);
	PositionXEdit->SetText(actor->Location.x);
	PositionYEdit->SetText(actor->Location.y);
	PositionZEdit->SetText(actor->Location.z);
	ModelNameEdit->SetText(Actor->ModelName);
	PhysicModelEdit->SetText(Actor->PhysicModelName);
	MaterialPreset->SetSelection(Actor->MaterialPreset);
	Teleport->SetChecked(Actor->Teleport);
	TeleportRelative->SetChecked(Actor->TeleportRelative);
	TeleportRadius->SetText(Actor->TeleportRadius);
	IsTrigger->SetChecked(Actor->IsTrigger);
	GhostObject->SetChecked(Actor->GhostObject);
	NameEdit->SetText(Actor->Name);
	DestinationNameEdit->SetText(Actor->TeleportDestination);
	ReceiveProjector->SetChecked(Actor->ReceiveProjectors);
	ReceiveShadow->SetChecked(Actor->ReceiveShadowVolume);
	CastShadow->SetChecked(Actor->CastShadowVolume);
	PassFlares->SetChecked(Actor->PassFlares);
	LightOccluder->SetChecked(Actor->LightOccluder);
	MaxRenderDistance->SetText(Actor->MaxRenderDistance);
	SoundName->SetText(Actor->SoundName);
	SoundDistance->SetText(Actor->SoundDistance);

	if (Actor->IsPrefab)
		TypeCombo->SetSelection(1);
	else
		TypeCombo->SetSelection(0);

	CanChange = true;
}


void CDialogActor::Show(WeActor *actor)
{
	ActorList.Clear();
	if (!actor) return;
	Actor = actor;
	Update(Actor);    		
	CDialog::Show();
}


void CDialogActor::Show(CSmartList <WeActor *> &actorList)
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

void CDialogActor::UpdateFromList()
{
	CanChange = false;


	ActorList.First();
	Actor = ActorList.GetItem();
	bool bRenderable;
	float fLifeTime;
	int iMaterialPreset;
	bool bTeleport;
	bool bTeleportRelative;
	float fTeleportRadius;
	bool bIsTrigger;
	bool bGhostObject;
	String sTeleportDestination;
	bool bReceiveProjector;
	bool bReceiveShadow;
	bool bCastShadowVolume;
	bool bPassFlares;
	bool bLightOccluder;
	float fMaxRenderDistance;
	String sSoundName;
	float fSoundDistance;

	ActorList.First();

	bRenderable = ActorList.GetItem()->Renderable;	
	fLifeTime = ActorList.GetItem()->LifeTime;
	iMaterialPreset = ActorList.GetItem()->MaterialPreset;
	bTeleport= ActorList.GetItem()->Teleport;
	bTeleportRelative = ActorList.GetItem()->TeleportRelative;
	fTeleportRadius = ActorList.GetItem()->TeleportRadius;
	bIsTrigger = ActorList.GetItem()->IsTrigger;
	bGhostObject = ActorList.GetItem()->GhostObject;
	sTeleportDestination = ActorList.GetItem()->TeleportDestination;
	bReceiveProjector = ActorList.GetItem()->ReceiveProjectors;
	bReceiveShadow = ActorList.GetItem()->ReceiveShadowVolume;
	bCastShadowVolume = ActorList.GetItem()->CastShadowVolume;
	bPassFlares = ActorList.GetItem()->PassFlares;
	bLightOccluder = ActorList.GetItem()->LightOccluder;
	fMaxRenderDistance = ActorList.GetItem()->MaxRenderDistance;
	sSoundName = ActorList.GetItem()->SoundName;
	fSoundDistance = ActorList.GetItem()->SoundDistance;

	Renderable->SetChecked(bRenderable);
	LifeTime->SetText(fLifeTime);
	MaterialPreset->SetSelection(iMaterialPreset);
	Teleport->SetChecked(bTeleport);
	TeleportRelative->SetChecked(bTeleportRelative);
	TeleportRadius->SetText(fTeleportRadius);
	IsTrigger->SetChecked(bIsTrigger);
	GhostObject->SetChecked(bGhostObject);
	DestinationNameEdit->SetText(sTeleportDestination);
	ReceiveProjector->SetChecked(bReceiveProjector);
	ReceiveShadow->SetChecked(bReceiveShadow);
	CastShadow->SetChecked(bCastShadowVolume);
    PassFlares->SetChecked(bPassFlares);
	LightOccluder->SetChecked(bLightOccluder);
	MaxRenderDistance->SetText(fMaxRenderDistance);
	SoundName->SetText(sSoundName);
	SoundDistance->SetText(fSoundDistance);



	ActorList.Next();

	for (; !ActorList.End(); ActorList.Next())
	{
		if (bRenderable != ActorList.GetItem()->Renderable) Renderable->SetChecked(false);
		if (fLifeTime != ActorList.GetItem()->LifeTime) LifeTime->SetText("");
		if (iMaterialPreset != ActorList.GetItem()->MaterialPreset) MaterialPreset->SetText("");
		if (bTeleport != ActorList.GetItem()->Teleport) Teleport->SetChecked(false);
		if (bTeleportRelative != ActorList.GetItem()->TeleportRelative) TeleportRelative->SetChecked(false);
		if (fTeleportRadius != ActorList.GetItem()->TeleportRadius) TeleportRadius->SetText(0.0f);
		if (bIsTrigger != ActorList.GetItem()->IsTrigger) IsTrigger->SetChecked(false);
		if (bGhostObject != ActorList.GetItem()->GhostObject) GhostObject->SetChecked(false);
		if (sTeleportDestination != ActorList.GetItem()->TeleportDestination) DestinationNameEdit->SetText("");
		if (bReceiveProjector != ActorList.GetItem()->ReceiveProjectors) ReceiveProjector->SetChecked(false);
		if (bReceiveShadow != ActorList.GetItem()->ReceiveShadowVolume) ReceiveShadow->SetChecked(false);
		if (bCastShadowVolume != ActorList.GetItem()->CastShadowVolume) CastShadow->SetChecked(false);
		if (bPassFlares != ActorList.GetItem()->PassFlares) PassFlares->SetChecked(false);
		if (bLightOccluder != ActorList.GetItem()->LightOccluder) LightOccluder->SetChecked(false);
		if (fMaxRenderDistance != ActorList.GetItem()->MaxRenderDistance) MaxRenderDistance->SetText("");
		if (sSoundName != ActorList.GetItem()->SoundName) SoundName->SetText("");
		if (fSoundDistance != ActorList.GetItem()->SoundDistance) SoundDistance->SetText("");	
	}


	CanChange = true;

}

void CDialogActor::OnModelNameChange()
{
	SAFE_DELETE(Actor->Model);
	Actor->Model = WeModelManager::Instance()->Load(ModelNameEdit->GetText());
	Actor->ModelName = ModelNameEdit->GetText();
}

void CDialogActor::OnPositionChange()
{
	WeVector Loc;
	Loc.x = PositionXEdit->GetTextAsFloat();
	Loc.y = PositionYEdit->GetTextAsFloat();
	Loc.z = PositionZEdit->GetTextAsFloat();
	Actor->MoveTo(Loc);
}

void CDialogActor::OnOrientationChange()
{
	float y,p,r;
	y = YawEdit->GetTextAsFloat()*D3DX_PI / 180.0f;
	p = PitchEdit->GetTextAsFloat()*D3DX_PI / 180.0f;
	r = RollEdit->GetTextAsFloat()*D3DX_PI / 180.0f;

	Actor->SetOrientation(y, p, r);
}

void CDialogActor::OnRenderableChange()
{	
	Actor->Renderable = Renderable->GetChecked();
	for (ActorList.First(); !ActorList.End(); ActorList.Next())
		ActorList.GetItem()->Renderable = Renderable->GetChecked();

}

void CDialogActor::OnOKClick()
{
	Actor->InitializeSound();
	for (ActorList.First(); !ActorList.End(); ActorList.Next())
		ActorList.GetItem()->InitializeSound();
	ActorList.Clear();
	Hide();
}

void CDialogActor::OnModelBrowseClick()
{
	InsertDialog->Type = PACKAGEITEM_MODEL;
	if (InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		CanChange = false;

		ModelNameEdit->SetText(InsertDialog->HoleName);
		SAFE_DELETE(Actor->Model);
		Actor->ModelName = ModelNameEdit->GetText();
		Actor->Model = WeModelManager::Instance()->Load(ModelNameEdit->GetText());
		//EditorWindow->Selection->SetSelectedActor(TubePart);

		CanChange = true;
	}
}


void CDialogActor::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID()) OnOKClick();	
	if (ID == ModelBrowseButton->GetID()) OnModelBrowseClick();	
	if (ID == MaterialPreset->GetID())
	{		
		Actor->MaterialPreset = MaterialPreset->GetSelectionData();		
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->MaterialPreset = MaterialPreset->GetSelectionData();		

	}
	if (ID == ApplyMaxRenderDistance->GetID())
	{
		EditorWindow->MapEditorWorld->SetMaxRenderDistanceToAll(MaxRenderDistance->GetTextAsFloat());

	}

	if (ID == PhysicsButton->GetID())
	{
		if (ActorList.GetItemsCount() >= 2)
			CDialogPhysicProperties::Instance()->Show(ActorList);		
		else
			CDialogPhysicProperties::Instance()->Show(Actor);		

	}

	if (ID == Teleport->GetID()) 
	{
		Actor->Teleport = Teleport->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->Teleport = Teleport->GetChecked();
	}

	if (ID == LightOccluder->GetID())
	{
		Actor->LightOccluder = LightOccluder->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->LightOccluder = LightOccluder->GetChecked();
	}

	if (ID == GhostObject->GetID())
	{
		Actor->GhostObject = GhostObject->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->GhostObject = GhostObject->GetChecked();
	}

	if (ID == TeleportRelative->GetID())
	{
		Actor->TeleportRelative = TeleportRelative->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->TeleportRelative = TeleportRelative->GetChecked();
	}

	if (ReceiveProjector->GetID() == ID)
	{
		Actor->ReceiveProjectors = ReceiveProjector->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->ReceiveProjectors = ReceiveProjector->GetChecked();

	}
	if (ReceiveShadow->GetID() == ID)
	{
		Actor->ReceiveShadowVolume = ReceiveShadow->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->ReceiveShadowVolume= ReceiveShadow->GetChecked();

	}
	if (CastShadow->GetID() == ID)
	{
		Actor->CastShadowVolume = CastShadow->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->CastShadowVolume = CastShadow->GetChecked();
	}
	if (ID == PassFlares->GetID())
	{
		Actor->PassFlares = PassFlares->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->PassFlares = PassFlares->GetChecked();

	}

	if (ID == Renderable->GetID())
		OnRenderableChange();

	/*
	if (ID == SoundBrowse->GetID())
	{
		char OldDir[MAX_PATH];
		getcwd(OldDir, MAX_PATH);

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		//HANDLE hf;              // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetHWND();
		ofn.lpstrFile = szFile;
		ofn.lpstrInitialDir = OldDir;
		//
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "WAV Files *.wav\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrDefExt = "wav";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn))
		{
			String FileName = ofn.lpstrFile;	
			String l, l2;
			int tmp = 0;
			while (1)
			{
				l = FileName.Extract('\\', tmp);
				tmp++;
				if (l.GetLen() == 0)  { FileName = l2; break; }
				l2 = l;
			}
			
			Actor->SoundName = ".\\Sounds\\Ambient\\";
			Actor->SoundName = Actor->SoundName + FileName;
			Actor->InitializeSound();
			Sound->SetText(FileName);
		}

		chdir(OldDir);
	}
	*/

	if (ID == TypeCombo->GetID())
	{
		bool IsPrefab = (TypeCombo->GetSelectionIndex() == 0);
		if (Actor->IsPrefab != IsPrefab)
		{
			if (Actor->GetWorld())
				Actor->GetWorld()->RemoveActor(Actor);
			Actor->IsPrefab = IsPrefab;
			if (Actor->GetWorld())
				Actor->GetWorld()->AddActor(Actor);
		}

		for (ActorList.First(); !ActorList.End(); ActorList.Next())
		{
			WeActor *actor = ActorList.GetItem();
			if (actor->IsPrefab != IsPrefab)
			{
				if (actor->GetWorld())
					actor->GetWorld()->RemoveActor(actor);
				actor->IsPrefab = IsPrefab;
				if (actor->GetWorld())
					actor->GetWorld()->AddActor(actor);
			}
		}
	}
}

void CDialogActor::OnChange(DWORD ID)
{
	if (!CanChange) return;


	if (ID == SoundDistance->GetID()) 
	{
		Actor->SoundDistance = SoundDistance->GetTextAsFloat();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->SoundDistance = SoundDistance->GetTextAsFloat();

	}

	if (ID == ModelNameEdit->GetID()) OnModelNameChange();
	if (ID == PhysicModelEdit->GetID()) 
	{
		Actor->PhysicModelName = PhysicModelEdit->GetText();
	}

	if (ID == MaxRenderDistance->GetID())
	{
		Actor->MaxRenderDistance = MaxRenderDistance->GetTextAsFloat();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->MaxRenderDistance = MaxRenderDistance->GetTextAsFloat();

	}

	if (ID == TeleportRadius->GetID())
	{
		Actor->TeleportRadius = TeleportRadius->GetTextAsFloat();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->TeleportRadius = TeleportRadius->GetTextAsFloat();
	}

	if (ID == NameEdit->GetID()) 
	{
		Actor->Name = NameEdit->GetText();
	}
	if (ID == DestinationNameEdit->GetID()) 
	{
		Actor->TeleportDestination = DestinationNameEdit->GetText();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->TeleportDestination = DestinationNameEdit->GetText();

	}

	if (ID == PositionXEdit->GetID()) OnPositionChange();
	if (ID == PositionYEdit->GetID()) OnPositionChange();
	if (ID == PositionZEdit->GetID()) OnPositionChange();
	if (ID == YawEdit->GetID()) OnOrientationChange();
	if (ID == PitchEdit->GetID()) OnOrientationChange();
	if (ID == RollEdit->GetID()) OnOrientationChange();
	if (ID == IsTrigger->GetID())
	{
		Actor->IsTrigger = IsTrigger->GetChecked();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->IsTrigger = IsTrigger->GetChecked();

	}

	if (ID == SoundName->GetID())
	{
		Actor->SoundName = SoundName->GetText();
		for (ActorList.First(); !ActorList.End(); ActorList.Next())
			ActorList.GetItem()->SoundName = SoundName->GetText();
	}
}


