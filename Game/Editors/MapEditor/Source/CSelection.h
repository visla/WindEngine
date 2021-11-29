/*
       File: CSelection.h
	   Author: Tomislav Kukic
	   Date: 05.12.2005
	   Version: 1.0
*/




#ifndef _CSELECTION
#define _CSELECTION


#define SELECTIONTYPE_MODEL 1


#include <WeActor.h>


class CSelection
{
protected:
	int  Type;

public:
	CSelection(WeActor *Actor);
	~CSelection();
	
	WeActor*	SelectedActor;

	//void		SetSelectedActor(WeActor*);
	//WeActor*	GetSelectedActor();

	int			GetSelectionType();
	void		UpdateProperties();

	//void		Tick();
};



#endif