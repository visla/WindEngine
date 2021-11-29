#ifndef _DIALOG_PARTICLESYSTEMEDITOR
#define _DIALOG_PARTICLESYSTEMEDITOR


#include <CDialog.h>
#include <CComponent.h>
#include <CDialogParticleSystem.h>
#include <WeParticleSystem.h>


extern class CDialogParticleSystemEditor* ParticleSystemEditor;

class CDialogParticleSystemEditor : public CDialog
{
protected:
	void OnClick(DWORD ID);
	void OnMouseMove(int x, int y);
	void OnChange(DWORD ID);

	CButton* Close;
	CButton* Browse;
	CButton* ColorFromChose;
	CButton* ColorToChose;

	CEditBox* MaxParticles;
	CEditBox* EmitRate;
	CEditBox* SpownVolumeSize;
	CEditBox* AlphaFrom;
	CEditBox* AlphaTo;
	CEditBox* GrowFrom;
	CEditBox* GrowTo;
	CEditBox* GravityX;
	CEditBox* GravityY;
	CEditBox* GravityZ;
	CEditBox* StartVelociy;
	CEditBox* Spread;
	CEditBox* LifeTime;
	CEditBox* Material;
	CButton* ShowHideButton;
	CComboBox* SpawingType;
	CComboBox* MovingType;
	CEditBox* PlaneWidth;
	CEditBox* PlaneHeight;
	CEditBox* LineX;
	CEditBox* LineY;
	CEditBox* LineZ;
	CEditBox* DirectionX;
	CEditBox* DirectionY;
	CEditBox* DirectionZ;
	CLabel* ColorFromLabel;
	CLabel* ColorToLabel;

	bool Created;
	bool MouseMove;

	int PreY;

	WeParticleSystem* ParticleSystem;
	
	void OnCloseClick();
	void OnBrowse();
	void OnLoad();
	void OnSave();
	void OnNew();
	void OnShowHide();
	void OnPreviewClick();
	void RefreshItems();

public:
	CButton* Preview;

	WeWorld* PSEditorWorld;
	WeCamera* PSEditorCamera;

	CDialogParticleSystemEditor();

	CParticleSystemPresset* ParticlePresset;

	bool Create(HWND hParent, bool Shown = true);
	void DoRender();
	void OnColorFromChose();
	void OnColorToChose();
};


#endif
