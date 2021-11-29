#ifndef _DIALOG_TEXTUREEDITOR
#define _DIALOG_TEXTUREEDITOR


#include <CDialog.h>
#include <CComponent.h>
#include <WeTextureEditor.h>


extern class CDialogTextureEditor* TextureEditor;

class CDialogTextureEditor : public CDialog
{
protected:
	void SetTextureInfo();

	void OnShow();
	void OnClick(DWORD ID);
	void OnExit();

	CLabel* TypeLabel;
	CLabel* WidthLabel;
	CLabel* HeightLabel;
	CLabel* DepthLabel;
	CComboBox* LowestQuality;
	CCheckBox *NoMipmaps;
	
	CButton*	ClearPreview;
	CButton*	SaveSelected;
	CButton*	RemoveSelected;

public:
	CDialogTextureEditor(){}

	WeTextureEditor* Texture;
	CGroupBox* RenderBox;
	CTreeView*	TreeTextures;

	bool Create(HWND hParent, bool Show = true);
	void SaveTexture(WeTextureEditor *Tex, StringParam NewName);

	void OnRefreshPackage(StringParam PackageName);
};


#endif
