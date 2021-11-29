/*
    CDialogMeshEditor.h

	Autor: Tomislav Kukic
	Date: 24.1.2006.
	Version: 1.0


*/

#ifndef _CDIALOGMESHEDITOR_H
#define _CDIALOGMESHEDITOR_H

#include <CDialog.h>
#include <CComponent.h>
#include <WeMeshEditor.h>
#include <WeActor.h>


extern class CDialogMeshEditor* MeshEditor;

class CDialogMeshEditor : public CDialog
{
protected:
	void OnShow();
	void OnClick(DWORD ID);
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(WORD Position);
	void OnRefreshPackage(StringParam PackageName);
	void OnShowOnlySelected();

	float OldMouseX;
	float OldMouseY;
	float OldMouseZ;

	String ClipboardMaterial;
	Strings ClipboardMaterials;

public:
	CDialogMeshEditor(){}

	void OnEveryFrame();
	
	WeStaticModel*	cModel;
	WeMeshEditor*	cMeshEditor;

	CGroupBox*		MeshPreview;
	CButton*		DiffusePreview;
	CTreeView*		TreeModels;
	CTreeView*		TreeMeshes;
	CTreeView*		TreeMeshMaterials;
	CTreeView*		TreeMaterials;
	CButton*		FindButton;
	CButton*		SetButton;
	CComboBox*		MaterialPresetCombo;
	CCheckBox*		UseNormalMap;
	CCheckBox*		CheckWireframe;
	CButton*		SaveSelected;
	CButton*		SaveSelectedAs;
	CButton*		RemoveSelected;
	CButton*		ImportXFile;
	CButton*		RefreshPackage;
	CCheckBox*		CheckOldStyle;
	CCheckBox*		CheckShowOnlySelected;
	CComboBox*		LightMapSize;
	CButton*		ResaveButton;

	WeActor*		Actor;
	WeWorld*		World;
	WeCamera*		Camera;
	WeLight*		Light;

	bool	EnableRotation;
	bool	WireFrame;
	String	RootName;

	float	MouseX;
	float	MouseY;
	float	MouseZ;

	WeMaterial* PreviewMaterial;
	WeTexture* PreviewTexture;

	static void Render();
	void Tick();
	bool Create(HWND hParent, bool Show = true);
	String GetAttributeText(int MaterialPreset, int AttributeIndex);
	void FillMaterialTree(int MaterialPreset);
	String SaveAutoModel(WeMeshEditor* Mesh, StringParam MeshName, bool ShowMsgs, bool OldStyle, bool Silent = false);

};


#endif