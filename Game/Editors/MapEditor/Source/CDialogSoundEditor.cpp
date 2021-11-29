#include <CDialogSoundEditor.h>
#include <CEditorResourceManager.h>
#include <CDialogInsert.h>
#include <CDialogTools.h>
#include <CDialogSave.h>
#include <CMapEditorWindow.h>
#include <WePackage.h>
#include <WeAudioDevice.h>


//========================================================
//    SoundEditor Class functions
//=======================================================


CDialogSoundEditor* DialogSoundEditor = NULL;


CDialogSoundEditor::CDialogSoundEditor()
{
}


CDialogSoundEditor::~CDialogSoundEditor()
{
	WeAudioDevice::Instance()->ShutDown();
}



bool CDialogSoundEditor::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_SOUNDEDITOR, hParent, Shown))
	{
		return false;
	}

	Close = new CButton(this, IDC_CLOSE);
	Load = new CButton(this, IDC_LOAD);
	Save = new CButton(this, IDC_SAVE);
	Browse = new CButton(this, IDC_BROWSE);
	Play = new CButton(this, IDC_PLAY);
	SoundName = new CEditBox(this, IDC_SOUNDNAME);
	SoundName->SetText("NO SOUND!!!");

	WeAudioDevice::Instance()->Initialize(GetHWND());

	Sound = NULL;

	return true;
}




void CDialogSoundEditor::OnClick(DWORD ID)
{
	if(ID == Close->GetID()) OnCloseClick();
	if(ID == Browse->GetID()) OnBrowse();
	if(ID == Play->GetID()) OnPlay();
	if(ID == Load->GetID()) OnLoad();
	if(ID == Save->GetID()) OnSave();
}




void CDialogSoundEditor::OnLoad()
{
	InsertDialog->Type = PACKAGEITEM_SOUND;
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		Sound = NULL;
		Sound = (WeSoundEditor*)WeAudioDevice::Instance()->LoadSound(InsertDialog->HoleName, false, true);
		if(Sound) SoundName->SetText(Sound->Name);
	}
}




void CDialogSoundEditor::OnSave()
{
	bool Res = false;
	while(!Res)
	{
		SaveDialog->Type = PACKAGEITEM_SOUND;
		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			Sound->Name = FileName;
			if(Sound->Save(SaveDialog->HoleName))
			{
				Res = true;   // Save is OK!!!
			}else{
				if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
				{					
					Sound->Save(SaveDialog->HoleName, true);
					Res = true;		// Save is OK!!!
				}else Res = false;	// Don't wan't to replase so enter new name
			}
		}else Res = true;  //  Cancel was pressed
	}
}




void CDialogSoundEditor::OnCloseClick()
{
	SetVisible(false);
}




void CDialogSoundEditor::OnPlay()
{
	if(!Sound) return;

	if(Play->GetText() == (String)"Play")
	{
		Play->SetText("Stop");
		Sound->Play(true);
		return;
	}

	if(Play->GetText() == (String)"Stop")
	{
		Play->SetText("Play");
		Sound->Stop();
		return;
	}
}



void CDialogSoundEditor::OnBrowse()
{
	GetFile("Sounds *.wav\0*.wav\0\0","");
	if(FileName == (String)"") return;

	Sound = NULL;
	Sound = (WeSoundEditor*)WeAudioDevice::Instance()->LoadSound(FileName, false);
	if(!Sound)
	{
		MessageBox(GetHWND(), "Error loading sound file...\nCommon error:\nThere are two or more chanels!!!\nSound need to be mono!!!","Load",MB_OK | MB_ICONERROR);
		Sound = NULL;
		return;
	}
	SoundName->SetText(FileNameNoPath);
}


