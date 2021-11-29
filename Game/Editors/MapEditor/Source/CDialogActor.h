/*
     CDialogActor.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/

#ifndef _CDIALOGACTOR_H
#define _CDIALOGACTOR_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeActor.h>
#include <CMapEditorWindow.h>
#include <SmartList.h>


class CDialogActor: public CDialog
{
protected:
	static CDialogActor *_instance;


	void OnClick(DWORD ID);
	void OnChange(DWORD ID);

	void OnModelNameChange();
	void OnPositionChange();
	void OnOrientationChange();
	void OnRenderableChange();
	void OnOKClick();
	void OnModelBrowseClick();	

	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }

	bool CanChange;


public:
	static CDialogActor *Instance();

	CComboBox *TypeCombo;
	CEditBox *ModelNameEdit;
	CButton  *ModelBrowseButton;
	CButton  *ApplyMaxRenderDistance;

	CEditBox *PhysicModelEdit;
	CButton	 *PhysicModelButton;

	CEditBox *PositionXEdit;
	CEditBox *PositionYEdit;
	CEditBox *PositionZEdit;
	CButton	 *PhysicsButton;

	CEditBox *YawEdit;
	CEditBox *PitchEdit;
	CEditBox *RollEdit;
	CEditBox *NameEdit;
	CEditBox *DestinationNameEdit;
	CEditBox *MaxRenderDistance;
	CEditBox *SoundName;
	CEditBox* SoundDistance;
	//CButton *SoundBrowse;
	CCheckBox *LightOccluder;


	CComboBox *MaterialPreset;
	CComboBox *PhysicProperties;


	CEditBox *LifeTime;
	CCheckBox *Renderable;
	CCheckBox *GhostObject;
	CCheckBox *ReceiveProjector;
	CCheckBox *ReceiveShadow;
	CCheckBox *CastShadow;
	CCheckBox *Teleport;
	CCheckBox *TeleportRelative;
	CEditBox *TeleportRadius;
	CCheckBox *IsTrigger;
	CCheckBox *ChangeDirection;
	CCheckBox *PassFlares;

	CButton *OKButton;

	WeActor *Actor;
	CSmartList <WeActor *> ActorList;


    bool Create(HWND hParent, bool Show = true);
	void Show(WeActor *actor);
	void Show(CSmartList <WeActor *> &actorList);
	void Update(WeActor *actor);
	void UpdateFromList();
};


#endif