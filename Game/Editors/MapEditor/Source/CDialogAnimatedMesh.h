/*
		File: CDialogAnimatedMesh.h
		Authoe: Tomislav kukic
		Date: 11.07.2006
		Version: 1.0
*/




#ifndef _CDIALOGANIMATEDMESH_H_
#define _CDIALOGANIMATEDMESH_H_


#include <CDialog.h>
#include <WeModel.h>
#include <WeWorld.h>
#include <WeActor.h>
#include <WeCamera.h>
#include <CComponent.h>



class CDialogAnimatedMesh: public CDialog
{
protected:
	static CDialogAnimatedMesh* _instance;

	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnChooseMesh();
	WeMesh* FindMesh(WeModelFrame* pFrame, StringParam Name);
	void FillMaterialTree(WeMesh *mesh, int MaterialPreset);
	void OnChangeButton();
	void OnShow();

public:
	static CDialogAnimatedMesh* Instance();

	CDialogAnimatedMesh();
	~CDialogAnimatedMesh(){}

	
	WeModel*  Model;
	WeActor*  Actor;
	WeWorld*  PreviewWorld;
	WeCamera* PreviewCamera;

	CTreeView* MeshList;
	CTreeView* FrameTree;
	CTreeView* MaterialList;
	CButton*   PreviewArea;
	CComboBox* MaterialPresetCombo;
	CListBox*  AnimationsList;
	CEditBox*  AnimSpeed;
	CEditBox*  EditTime;



	bool Create(HWND hParent, bool Shown = true);
	void LoadMeshFromX(StringParam name);
	void FillFrames(WeModelFrame* pFrame, CTreeViewItem* Parent);
	void FillAnimations();
};





#endif

