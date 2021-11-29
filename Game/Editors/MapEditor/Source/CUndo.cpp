/*
    CUndo.cpp

	Autor: Nebojsa Vislavski
	Date: 20.12.2005.
	Version: 1.0


*/


#include <CUndo.h>
#include <CMapEditorWindow.h>
#include <ActorTags.h>
#include <CEditorMap.h>
#include <WeStaticModel.h>



CUndo *CUndo::_instance = NULL;
CUndo *CUndo::Instance()
{
	if (_instance) return _instance;
	_instance = new CUndo;
	return _instance;
}


CUndo::CUndo()
{
	UndoIndex = 0;
}


void CUndo::MakeUndoItem(UNDOTYPE UndoType, WeActor *Param1)
{
	CUndoBatch UndoBatch;
	CUndoItem UndoItem;


	RedoItems.Clear();

	if (UndoItems.GetItemsCount() > MAX_UNDO_HISTORY)
	{
		UndoItems.Last();
		UndoItems.Delete();
	}

	//UndoItem = new CUndoItem;
	UndoItem.Type = UndoType;
	// save state for all selected actors

	switch (UndoType)
	{
	case UNDO_CREATE:
		//UndoBatch = new CUndoBatch;
		UndoBatch.RefActor = Param1;
		UndoItem.UndoBatches.PushBack(UndoBatch);		
		break;

	case UNDO_ACTORSTATECHANGE:
		for (EditorWindow->Selections.Selections.First(); !EditorWindow->Selections.Selections.End(); EditorWindow->Selections.Selections.Next())
		{
			//UndoBatch = new CUndoBatch;			
			SaveState(&UndoBatch, EditorWindow->Selections.Selections.GetItem()->SelectedActor);
			UndoItem.UndoBatches.PushBack(UndoBatch);
		}
		break;

	case UNDO_DELETE:

		for (EditorWindow->Selections.Selections.First(); !EditorWindow->Selections.Selections.End(); EditorWindow->Selections.Selections.Next())
		{
			//UndoBatch = new CUndoBatch;			
			UndoBatch.ClonedActor = EditorWindow->Selections.CreateNewActor(EditorWindow->Selections.Selections.GetItem()->SelectedActor->Type);				
			EditorWindow->Selections.Selections.GetItem()->SelectedActor->Clone(UndoBatch.ClonedActor, false);
			SAFE_DELETE(UndoBatch.ClonedActor->PhysicHandle);			
			UndoItem.UndoBatches.PushBack(UndoBatch);
		}
		break;
	}

	UndoItems.PushFront(UndoItem);
}






void CUndo::SaveState(CUndoBatch *UndoBatch, WeActor *Actor)
{
	UndoBatch->ClonedActor = EditorWindow->Selections.CreateNewActor(Actor->Type);	
	UndoBatch->RefActor = Actor;
	UndoBatch->RefActor->Clone(UndoBatch->ClonedActor, false);
	SAFE_DELETE(UndoBatch->ClonedActor->PhysicHandle);	
}



void CUndo::RestoreState(CUndoBatch *UndoBatch, UNDOTYPE UndoType)
{
	switch (UndoType)
	{
	case UNDO_ACTORSTATECHANGE:
			UndoBatch->ClonedActor->Clone(UndoBatch->RefActor);
		break;

	case UNDO_DELETE:		  		   
		// treba da napravimo objekat				
		UndoBatch->RefActor = EditorWindow->Selections.CreateNewActor(UndoBatch->ClonedActor->Type);	
		UndoBatch->ClonedActor->Clone(UndoBatch->RefActor, false);				
		EditorWindow->MapEditorWorld->AddActor(UndoBatch->RefActor);
		break;

	case UNDO_CREATE:
		// treba da obrisemo objekat ali i da ga sacuvamo
		UndoBatch->ClonedActor = EditorWindow->Selections.CreateNewActor(UndoBatch->RefActor->Type);	
		UndoBatch->RefActor->Clone(UndoBatch->ClonedActor, false);				
		SAFE_DELETE(UndoBatch->ClonedActor->PhysicHandle);
		for (EditorWindow->Selections.Selections.First(); !EditorWindow->Selections.Selections.End(); EditorWindow->Selections.Selections.Next())
		{
			if (EditorWindow->Selections.Selections.GetItem()->SelectedActor == UndoBatch->RefActor)
			{
				SAFE_DELETE(EditorWindow->Selections.Selections.GetItem());
				EditorWindow->Selections.Selections.Delete();
				break;
			}
		}
		SAFE_DELETE(UndoBatch->RefActor);		
		break;
	}

}

void CUndo::Reset()
{
	//for (UndoItems.First(); !UndoItems.End(); UndoItems.Next())
	//{
	//	for (UndoItems.GetItem().UndoBatches.First(); !UndoItems.GetItem().UndoBatches.End(); UndoItems.GetItem().UndoBatches.Delete())
	//	{
	//		delete UndoItems.GetItem()->UndoBatches.GetItem();
	//	}		
	//	delete UndoItems.GetItem();
	//}
	UndoItems.Clear();
}



// This will return to previous saved state
void CUndo::Undo()
{
	if (UndoItems.Empty()) return;

	UndoItems.First();

	for (UndoItems.GetItem().UndoBatches.First(); !UndoItems.GetItem().UndoBatches.End(); UndoItems.GetItem().UndoBatches.Next())
	{		
		RestoreState(&UndoItems.GetItem().UndoBatches.GetItem(), UndoItems.GetItem().Type);
	}	
	RedoItems.PushFront(UndoItems.GetItem());
	UndoItems.Delete();
}

void CUndo::Redo()
{
	if (RedoItems.Empty()) return;

	RedoItems.First();
	CUndoBatch *UndoBatch;

	for (RedoItems.GetItem().UndoBatches.First(); !RedoItems.GetItem().UndoBatches.End(); RedoItems.GetItem().UndoBatches.Next())
	{
		UndoBatch = &RedoItems.GetItem().UndoBatches.GetItem();
		switch (RedoItems.GetItem().Type)
		{
		case UNDO_ACTORSTATECHANGE:
			UndoBatch->RefActor->Clone(UndoBatch->ClonedActor);		
			break;

		case UNDO_DELETE:		  		   
			// treba da obrisemo objekat		
			UndoBatch->ClonedActor = EditorWindow->Selections.CreateNewActor(UndoBatch->RefActor->Type);	
			UndoBatch->RefActor->Clone(UndoBatch->ClonedActor, false);				
			SAFE_DELETE(UndoBatch->ClonedActor->PhysicHandle);
			SAFE_DELETE(UndoBatch->RefActor);			
			break;

		case UNDO_CREATE:
			// treba da kreiramo objekat			
			UndoBatch->RefActor = EditorWindow->Selections.CreateNewActor(UndoBatch->ClonedActor->Type);	
			UndoBatch->ClonedActor->Clone(UndoBatch->RefActor, false);				
			EditorWindow->MapEditorWorld->AddActor(UndoBatch->RefActor);			
			break;
		}
	}

	UndoItems.PushFront(RedoItems.GetItem());
	RedoItems.Delete();
}