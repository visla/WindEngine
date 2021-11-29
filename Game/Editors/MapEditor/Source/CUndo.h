/*
    CUndo.h

	Autor: Nebojsa Vislavski
	Date: 20.12.2005.
	Version: 1.0


*/

#ifndef _CUNDO_H
#define _CUNDO_H


#include <SmartList.h>
#include <WeActor.h>
#include <CString.h>

#define MAX_UNDO_HISTORY 20


enum UNDOTYPE
{
	UNDO_DELETE,
	UNDO_CREATE,
	UNDO_ACTORSTATECHANGE	
};


class CUndoBatch
{
public:
	WeActor *RefActor;    // what is reference to world actor
	WeActor *ClonedActor; // this is not in world actor for holding data    
};



// holds one state for all selected objects
class CUndoItem
{
public:
	UNDOTYPE Type;
	CSmartList <CUndoBatch> UndoBatches;
};

class CUndo
{
private:
	static CUndo *_instance;
protected:
	CSmartList <CUndoItem> UndoItems;
	CSmartList <CUndoItem> RedoItems;


	// this will save state for actor
	void	SaveState(CUndoBatch *UndoBatch, WeActor *Actor);  
	void	RestoreState(CUndoBatch *UndoBatch, UNDOTYPE UndoType);	
	
	int		UndoIndex;


public:
	static CUndo*	Instance();
					CUndo();

	void			MakeCreateUndoItem();
	void			MakeUndoItem(UNDOTYPE UndoType, WeActor *Param1 = NULL);
	void			Undo();
	void			Redo();
	void			Reset();

};


#endif