/*
		File: CDialogAnimTextureEditor.h
		Author: Tomislav Kukic\
		Date: 31.05.2006.
		Version: 1.0
*/



#ifndef _CDIALOGANIMTEXTUREEDITOR_H_
#define _CDIALOGANIMTEXTUREEDITOR_H_



#include <CComponent.h>
#include <CDialog.h>
#include <WeAnimTexture.h>



class CDialogAnimTextureEditor:public CDialog
{
protected:
	static CDialogAnimTextureEditor* _instance;

	void OnClick(DWORD ID);
	void OnChange(DWORD ID);

	static WeAnimTexture* Texture;

	bool AnimTimeLastChanged;
	bool FPSLastChanged;

	static int DrawX;
	static int DrawY;
	static int DrawW;
	static int DrawH;

public:
	static CDialogAnimTextureEditor* Instance();

	 bool Create(HWND hParent, bool Show = true);

	CDialogAnimTextureEditor();
	~CDialogAnimTextureEditor(){}

	void Tick();
	static void DoRender();

	void UpdateParams();

	CButton* PlayButton;
	CButton* StopButton;
	CButton* PauseButton;
	CButton* StepUpButton;
	CButton* StepDownButton;
	CButton* PickColorButton;
	CButton* PreviewButton;

	CEditBox* FramesPerSecEdit;
	CEditBox* AnimTimeEdit;
	CEditBox* AlphaEdit;

	CCheckBox* RealSizeCheck;
	CCheckBox* LoopCheck;

	CLabel* TextureNameStatic;
	CLabel* ColorValueStatic;
	CLabel* CurrentFrame;
	CLabel* TextureSize;
	CLabel* TotalFrames;


	void OnNew();
	void OnChangeFPS();
	void OnChangeAnimTime();
	void OnLoopChanged();
	void OnPlay();
	void OnStop();
	void OnPause();
	void OnStepUp();
	void OnStepDown();
	void OnInsertAllFrames();
	void OnRealSize();
	void OnSave();
	void OnLoad();
};





#endif
