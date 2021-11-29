/*
    CMassSelection.h

	Autor: Nebojsa Vislavski
	Date: 14.12.2005.
	Version: 1.0

	This is mass selection class

*/

#ifndef _CMASSSELECTION_H
#define _CMASSSELECTION_H

#include <CSelection.h>
#include <SmartList.h>
#include <WeMatrix.h>

#define ALIGN_NONE		0
#define ALIGN_PIVOT_X 1
#define ALIGN_PIVOT_Y 2
#define ALIGN_PIVOT_Z 4

#define ALIGN_ROTATION_YAW 8
#define ALIGN_ROTATION_PITCH 16
#define ALIGN_ROTATION_ROLL 32

#define ALIGN_AXIS 64

#define IS_PICKUP(a) if(a>10000 && a<11000)

#include <CSelection.h>


class CMassSelection
{
protected:
	void	MakeGroupPivot();

	WeVector Ref;
	//float RefX;
	//float RefY;
	//float RefYaw;
	//float RefPitch;
	//float RefRoll;


public:
	CMassSelection();
	
	// origin of all selected items. it is avarage of all selected locations
	WeVector GroupPivot;   
	WeVector GroupPivotAxisX;
	WeVector GroupPivotAxisY;
	WeVector GroupPivotAxisZ;

	WeVector DeltaMoving;
    WeVector StartLocation;
	WeVector StartRotation;
	WeVector DeltaRotation;
	CSmartList<WeMatrix> OldTransformations;

	bool	SelectPrefabs;
	bool	SelectLights;
	bool	SelectPathNodes;
	bool	SelectStartLocations;
	bool	SelectParticleSystems;

	void	Align(int AlignType); 

	// this will delete all selected items from world
	void	MakeSelection(WeActor *actorhit, bool AddToSelection);
	void	DeleteSelected();
	void	DoSelection(int x, int y, bool AddToSelection);

	void	ShowSelectionProperties();
	void	HideSelectionProperties();

	void	DeselectAll();
	void	Deselect(CSelection *selected);	
	void	Move(int mx, int my, bool LeftDown, bool RightDown, bool LockX = false, bool LockY = false, bool LocalMove = false);
	void	Rotate(int mx, int my, bool LeftDown, bool RightDown, bool LockX = false, bool LockY = false, bool LocalRotate = false);
	void	RotateCW();
	void	RotateCWYAxis();
	void	CloneSelection();
	void	CloneArray(int NumObjects);
	void	CloneRadialArray(int NumObjects);
	WeActor *CreateNewActor(int TypeOfNew);

	CSmartList <CSelection *> Selections;
	int ShownPropertiesType;
};

#endif