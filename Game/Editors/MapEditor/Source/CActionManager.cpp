/*

    CActionManager.h

	Autor: Nebojsa Vislavski
	Date: 26.12.2005.
	Version: 1.0

	This is action manager used to record actions in editor

*/


#include <CActionManager.h>
#include <stdio.h>
#include <ActorTags.h>
#include <CMapEditorWindow.h>
#include <CEditorMap.h>
#include <WeStaticModel.h>
#include <CMapEditorCamera.h>


CActionManager *CActionManager::_instance = NULL;

CActionManager *CActionManager::Instance()
{
	if (_instance) return _instance;
	_instance = new CActionManager;
	return _instance;
}


CActionManager::CActionManager()
{
	CurrentAction = NULL;
	MakingAction = false;
	Actions.Clear();
}


void CActionManager::StartRecording(StringParam Name)
{
	// if existst then nothing
	for (Actions.First(); !Actions.End(); Actions.Next())
	{
		if (Actions.GetItem()->Name == Name)
		{
			MessageBox(0, "Action with this name already exists", "Note", MB_ICONINFORMATION);
			return;
		}
	}

	CurrentAction = new CAction;
	CurrentAction->Name = Name;			
	MessageBox(0, "Recording...", "Info", MB_ICONINFORMATION);

	MakingAction = true;
}

void CActionManager::RecordAction(WeActor *Actor)
{
	if (!MakingAction) return;
	if (!CurrentAction) return;

	CAction::CActorPart *ActionPart;
	ActionPart = new CAction::CActorPart;

	ActionPart->Actor = Actor;
	CurrentAction->ActionParts.PushBack(ActionPart);    
	
}


void CActionManager::RemoveRecordedAction(WeActor *actor)
{
	if (!MakingAction) return;
	if (!CurrentAction) return;

	for (CurrentAction->ActionParts.First(); !CurrentAction->ActionParts.End(); CurrentAction->ActionParts.Next())
	{
		if (actor == CurrentAction->ActionParts.GetItem()->Actor)		
		{
			CurrentAction->ActionParts.Delete();
			return;
		}
	}
}

// This will extract local transformation matrix and return matrix that is relative from TM
void ExtractLocalTransformation(WeVector &result, WeVector &v, WeMatrix &TM)
{
	float proj;
	WeVector tmpV, XAxis, YAxis, ZAxis;

	XAxis.x = TM._11; XAxis.y = TM._12; XAxis.z = TM._13;
	YAxis.x = TM._21; YAxis.y = TM._22; YAxis.z = TM._23;
	ZAxis.x = TM._31; ZAxis.y = TM._32; ZAxis.z = TM._33;

	proj = D3DXVec3Dot(&v, &XAxis);	
	result.x = proj; //tmpV.x; result._12 = tmpV.y; result._13 = tmpV.z;

	proj = D3DXVec3Dot(&v, &YAxis);
	//tmpV = YAxis*proj;
	result.y = proj; //tmpV.x; result._22 = tmpV.y; result._23 = tmpV.z;

	proj = D3DXVec3Dot(&v, &ZAxis);
	//tmpV = ZAxis*proj;
	result.z = proj; //tmpV.x; result._32 = tmpV.y; result._33 = tmpV.z;
    
}


void CActionManager::DeleteAction(StringParam Name)
{
	// this will delete from file action with supported name
	WeFileStream FS;
	bool Found = false;
	
	for (Actions.First(); !Actions.End(); Actions.Next())
	{
		if (Actions.GetItem()->Name == Name)
		{
			Found = true;
		}
	}
	if (!Found) return;

	FS.OpenNew("actions.tmp.act");
	for (Actions.First(); !Actions.End(); )
	{
		if (Actions.GetItem()->Name == (String)Name)
		{
			Actions.Delete();
		} else
		{
			FS.WriteString(Actions.GetItem()->Name);
			FS.WriteInt(Actions.GetItem()->ActionParts.GetItemsCount());
			for (Actions.GetItem()->ActionParts.First(); !Actions.GetItem()->ActionParts.End(); Actions.GetItem()->ActionParts.Next())
			{
				FS.WriteInt(Actions.GetItem()->ActionParts.GetItem()->Actor->Type);
				FS.WriteVector(Actions.GetItem()->ActionParts.GetItem()->LocationV);
				FS.WriteVector(Actions.GetItem()->ActionParts.GetItem()->XAxisV);
				FS.WriteVector(Actions.GetItem()->ActionParts.GetItem()->YAxisV);
				FS.WriteVector(Actions.GetItem()->ActionParts.GetItem()->ZAxisV);
				Actions.GetItem()->ActionParts.GetItem()->Actor->Serialize(FS);
			}
			Actions.Next();
		}
	}
	FS.Close();            			
	DeleteFile("actions.act");
	MoveFile("actions.tmp.act", "actions.act");
}

void CActionManager::StopRecording()
{
	// Save this actions if has part to file
	if (!MakingAction) return;
	if (CurrentAction->ActionParts.GetItemsCount() == 0) 
	{
		MakingAction = false;
		return;
	}
	if (EditorWindow->Selections.Selections.GetItemsCount() != 1) 
	{
		MessageBox(0, "You need to have only one object selected for action to be played", "Note!", MB_ICONWARNING);
		return;
	}

	WeActor *Selected;
	WeVector locX, locY, locZ, DeltaLocation, tmpV, SelXAxis, SelYAxis, SelZAxis;
	WeVector LocationV, XAxisV, YAxisV, ZAxisV;
	//float proj;
	
    
	EditorWindow->Selections.Selections.First();
	Selected = EditorWindow->Selections.Selections.GetItem()->SelectedActor;


	
	WeFileStream FS;
	if (!FS.Append("actions.act")) return;
	FS.WriteString(CurrentAction->Name);
	FS.WriteInt(CurrentAction->ActionParts.GetItemsCount());

	for (CurrentAction->ActionParts.First(); !CurrentAction->ActionParts.End(); CurrentAction->ActionParts.Next())
	{
		DeltaLocation = CurrentAction->ActionParts.GetItem()->Actor->Location - Selected->Location;
		Selected->GetLocalAxis(SelXAxis, SelYAxis, SelZAxis);

		CurrentAction->ActionParts.GetItem()->Actor->GetLocalAxis(locX, locY, locZ);

		ExtractLocalTransformation(LocationV, DeltaLocation, Selected->TM);
		ExtractLocalTransformation(XAxisV, locX, Selected->TM);
		ExtractLocalTransformation(YAxisV, locY, Selected->TM);
		ExtractLocalTransformation(ZAxisV, locZ, Selected->TM);

		FS.WriteInt(CurrentAction->ActionParts.GetItem()->Actor->Type);
		FS.WriteVector(LocationV);
		FS.WriteVector(XAxisV);
		FS.WriteVector(YAxisV);
		FS.WriteVector(ZAxisV);
		CurrentAction->ActionParts.GetItem()->Actor->Serialize(FS);
	}

	FS.Close();


	Actions.PushBack(CurrentAction);
	MakingAction = false;	
	MessageBox(0, "Recorded. Recorded actions can be played only after editor restart!", 0, MB_ICONINFORMATION);
}


void CActionManager::PlayAction(StringParam ActionName)
{
	if (MakingAction) return;
	
	CActionManager::CAction *Action = NULL;

	for (Actions.First(); !Actions.End(); Actions.Next())
	{
		if (Actions.GetItem()->Name == ActionName)
		{
			// do current action
			Action = Actions.GetItem();
			break;
		}
	}
	if (!Action) return;


	if (EditorWindow->Selections.Selections.GetItemsCount() != 1) 
	{
		MessageBox(0, "You must select only one object for action to be player", "Note", MB_ICONINFORMATION);
		return;
	}

	WeActor *Selected;	
	WeActor *tmpA;
	WeVector tmpV, SelXAxis, SelYAxis, SelZAxis;


	EditorWindow->Selections.Selections.First();
	Selected = EditorWindow->Selections.Selections.GetItem()->SelectedActor;

	for (Action->ActionParts.First(); !Action->ActionParts.End(); Action->ActionParts.Next())
	{
		tmpV = Action->ActionParts.GetItem()->LocationV;
        
		Selected->GetLocalAxis(SelXAxis, SelYAxis, SelZAxis);      	
		tmpV = Action->ActionParts.GetItem()->LocationV.x * SelXAxis;
		tmpV += Action->ActionParts.GetItem()->LocationV.y * SelYAxis;
		tmpV += Action->ActionParts.GetItem()->LocationV.z * SelZAxis;
		
		//tmpV = Action->ActionParts.GetItem()->Actor->Location.x * SelXAxis;
		//tmpV += Action->ActionParts.GetItem()->Actor->Location.y * SelYAxis;
		//tmpV += Action->ActionParts.GetItem()->Actor->Location.z * SelZAxis;
        
		Action->ActionParts.GetItem()->Actor->Initialize();
		SAFE_DELETE(Action->ActionParts.GetItem()->Actor->PhysicHandle); // We dont need physic handle for this
		tmpA = EditorWindow->Selections.CreateNewActor(Action->ActionParts.GetItem()->Actor->Type);		
		Action->ActionParts.GetItem()->Actor->Clone(tmpA);
		tmpA->InEditor = true;

		tmpV = Selected->Location + tmpV;

		tmpA->Location = tmpV;	
		tmpA->TM._41 = tmpV.x; tmpA->TM._42 = tmpV.y; tmpA->TM._43 = tmpV.z; 
		

		tmpV = Action->ActionParts.GetItem()->XAxisV.x * SelXAxis;
		tmpV += Action->ActionParts.GetItem()->XAxisV.y * SelYAxis;
		tmpV += Action->ActionParts.GetItem()->XAxisV.z * SelZAxis;
        tmpA->TM._11 = tmpV.x; tmpA->TM._12 = tmpV.y; tmpA->TM._13 = tmpV.z;        

		tmpV = Action->ActionParts.GetItem()->YAxisV.x * SelXAxis;
		tmpV += Action->ActionParts.GetItem()->YAxisV.y * SelYAxis;
		tmpV += Action->ActionParts.GetItem()->YAxisV.z * SelZAxis;
        tmpA->TM._21 = tmpV.x; tmpA->TM._22 = tmpV.y; tmpA->TM._23 = tmpV.z;        

		tmpV = Action->ActionParts.GetItem()->ZAxisV.x * SelXAxis;
		tmpV += Action->ActionParts.GetItem()->ZAxisV.y * SelYAxis;
		tmpV += Action->ActionParts.GetItem()->ZAxisV.z * SelZAxis;
        tmpA->TM._31 = tmpV.x; tmpA->TM._32 = tmpV.y; tmpA->TM._33 = tmpV.z;        
		

		//WeMatrix M;
		//D3DXMatrixRotationZ(&M, 2.13f);
		//tmpA->TM = M;
		tmpA->Initialize();
		EditorWindow->MapEditorWorld->AddActor(tmpA);
		tmpA->SetTransformation(tmpA->TM);
		if (tmpA->IsPrefab) tmpA->LightMap.GenerateDefault(tmpA);	   
	}
    
}

void CActionManager::LoadActions()
{	
	SAFE_DELETE(CurrentAction);
	MakingAction = false;

	for (Actions.First(); !Actions.End(); Actions.Delete())
	{
		delete Actions.GetItem();
	}

	WeFileStream FS;

	if (!FS.Exists("actions.act")) return;
	if (!FS.Open("actions.act")) return;

	CAction *Action;
	int partcnt;
	//WeActor *tmpA;
	WeVector LocationV, XAxisV, YAxisV, ZAxisV;

	int Type;
	CAction::CActorPart *Part;

	while (!FS.Eof())
	{
		Action = new CAction;
		Action->Name = FS.ReadString();

		if (FS.Eof()) { delete Action; break; }

		Actions.PushBack(Action);
		partcnt = FS.ReadInt();
      
		for (int i = 0; i < partcnt; i++)
		{
			Part = new CAction::CActorPart;

			Type = FS.ReadInt();
			FS.ReadVector(Part->LocationV);
			FS.ReadVector(Part->XAxisV);
			FS.ReadVector(Part->YAxisV);
			FS.ReadVector(Part->ZAxisV);			
			if (Type == WETYPE_PREFAB) Type = WETYPE_ACTOR;
			Part->Actor = EditorWindow->Selections.CreateNewActor(Type);			        
			Part->Actor->Deserialize(FS);
			Part->Actor->InEditor = true;
		
			Action->ActionParts.PushBack(Part);
		}
	}
	FS.Close();

	SAFE_DELETE(CurrentAction);
	MakingAction = false;	
}

