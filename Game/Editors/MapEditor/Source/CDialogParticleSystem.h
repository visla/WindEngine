/*
     CDialogParticleSystem.h


	 Autor: Nebojsa Vislavski
	 Date: 9.12.2005.
	 Version: 1.0
	 Platform: Win32

*/


#ifndef _CDIALOGPARTICLESYSTEM_H
#define _CDIALOGPARTICLESYSTEM_H

#include <CDialog.h>
#include <CComponent.h>
#include <CEditorResourceManager.h>
#include <WeParticleSystem.h>
#include <CMapEditorWindow.h>

class CParticleSystemPresset;

class CDialogParticleSystem: public CDialog
{
protected:
	static CDialogParticleSystem *_instance;

	void OnClick(DWORD ID);
	void OnChange(DWORD ID);
	void OnHide() { EditorWindow->Selections.ShownPropertiesType = 0; }

public:
	static CDialogParticleSystem *Instance();

	CButton* Close;

	CComboBox *SpawnType;
	CComboBox *ParticleMoveType;

	CEditBox*	MaxParticles;
	CEditBox*	EmitRate;
	CEditBox*	EmitRateRandomness;
	CEditBox*	SpawnParam1;
	CEditBox*	SpawnParam2;
	CEditBox*	SpawnParam3;
	CEditBox*	SpawnParam4;

	CEditBox *SpawnDirectionX;
	CEditBox *SpawnDirectionY;
	CEditBox *SpawnDirectionZ;

	CButton *ColorFromButton;
	CButton *ColorToButton;
  
	CEditBox* AlphaFrom;
	CEditBox* AlphaTo;
	CEditBox* SizeFrom;
	CEditBox* SizeTo;
	CEditBox* SizeRandomness;
	CEditBox* GravityX;
	CEditBox* GravityY;
	CEditBox* GravityZ;
	CEditBox* StartVelociy;
	CEditBox* StartVelociyRandomness;
	CEditBox* Spread;
	CEditBox* LifeTime;
	CEditBox* LifeTimeRandomness;
	CEditBox* Material;
	CEditBox* SpinFrom;
	CEditBox* SpinTo;
	CEditBox* SpinRandomness;
	CCheckBox*	RelativeParticles;
	CCheckBox*	SpinningInLife;
	CEditBox*	VisibleSphereRadius;

	CComboBox* ParticleAngle;
	CComboBox* BlendType;
	
	CLabel *ColorFromLabel;
	CLabel *ColorToLabel;
	CLabel*	SpawnParam1Label;
	CLabel*	SpawnParam2Label;
	CLabel*	SpawnParam3Label;
	CLabel*	SpawnParam4Label;

	CButton  *MaterialBrowse;
	CButton  *OKButton;
	CButton	 *Load;
	CButton  *Save;


	WeParticleSystem *PS;
	bool CanChange;


	bool Create(HWND hParent, bool Show = true);
	void Show(WeParticleSystem *ps);
	void Update(WeParticleSystem *ps);
	void OnLoadPresset();
	void OnSavePresset();
};


#endif