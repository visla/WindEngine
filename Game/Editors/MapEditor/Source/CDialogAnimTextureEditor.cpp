/*
		File: CDialogAnimTextureEditor.h
		Author: Tomislav Kukic\
		Date: 31.05.2006.
		Version: 1.0
*/



#include <CEditorResourceManager.h>
#include <CDialogAnimTextureEditor.h>
#include <WeCanvas.h>
#include <Commdlg.h>
#include <direct.h>
#include <WeRenderDevice.h>
#include <WePackage.h>
#include <CMapEditorWindow.h>
#include <WeTextureManager.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>




String _FileName;
String _FileNameNoPath;
String _FilesPath;
Strings _FileNames;

bool GetFile(char Filter[256], HWND Owner = NULL);
bool GetFile(char Filter[256], HWND Owner)
{
	_FileName = (String)"";
	_FileNameNoPath = (String)"";
	_FilesPath = (String)"";
	_FileNames.Clear();

	char OldDir[MAX_PATH];
	getcwd(OldDir, MAX_PATH);


	OPENFILENAME ofn;       // common dialog box structure
	char szFile[2000];       // buffer for file name
	char szFileName[260];   // buffer for file name without path information
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFileTitle = szFileName;
	ofn.lpstrInitialDir = OldDir;
	
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.nMaxFileTitle = sizeof(szFileName);
	ofn.lpstrFilter = Filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	// Display the Open dialog box. 
	bool PathIsSet = false;
	bool Res = false;
	String tmpFileName;

	if(GetOpenFileName(&ofn))
	{
		int i = sizeof(szFile);
		for(int j=0; j<i; j++)
		{
			if(!PathIsSet && szFile[j] != '\0')
			{
				_FilesPath += szFile[j];
			}else Res = true;
			
			if(PathIsSet && szFile[j] != '\0')
			{
				tmpFileName += szFile[j];
			}else if(PathIsSet && szFile[j] == '\0')
			{
				if(tmpFileName == (String)"") break;

				_FileNames.PushBack(tmpFileName);
				tmpFileName = (String)"";
				if(szFile[j+1] == '\0') break;
			}

			PathIsSet = Res;
		}
		if(_FileNames.GetItemsCount() == 0)
			_FileName = _FilesPath;
		else
			_FileName = szFileName;

		_FileNameNoPath = szFileName;
		return true;
	}
	
	_FileName = "";
	chdir(OldDir);
	return false;
}






CDialogAnimTextureEditor* CDialogAnimTextureEditor::_instance = NULL;


CDialogAnimTextureEditor* CDialogAnimTextureEditor::Instance()
{
	if(_instance) return _instance;

	_instance = new CDialogAnimTextureEditor();
	return _instance;
}



WeAnimTexture* CDialogAnimTextureEditor::Texture = NULL;

int CDialogAnimTextureEditor::DrawX = 0;
int CDialogAnimTextureEditor::DrawY = 0;
int CDialogAnimTextureEditor::DrawW = 0;
int CDialogAnimTextureEditor::DrawH = 0;


CDialogAnimTextureEditor::CDialogAnimTextureEditor()
{
	FPSLastChanged = false;
	AnimTimeLastChanged = false;
}






bool CDialogAnimTextureEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_ANIMTEXTUREEDITOR, hParent, Show))
	{
		return false;
	}
	
	PreviewButton = new CButton(this, ID_TE_PREVIEW);
	PlayButton = new CButton(this, IDC_PLAY_ANIMATION);
	StopButton = new CButton(this, IDC_STOP);
	PauseButton = new CButton(this, IDC_PAUSE);
	StepUpButton = new CButton(this, IDC_STEP_UP);
	StepDownButton = new CButton(this, IDC_STEP_DOWN);

	FramesPerSecEdit = new CEditBox(this, IDC_FRAMES_PER_SECOND);
	AnimTimeEdit = new CEditBox(this, IDC_ANIM_TIME);

	LoopCheck = new CCheckBox(this, IDC_IS_LOOP);
	RealSizeCheck = new CCheckBox(this, IDC_REALSIZETEXTURE);

	TextureNameStatic = new CLabel(this, IDC_ANIMATION_NAME);
	ColorValueStatic = new CLabel(this, IDC_COLOR);
	CurrentFrame = new CLabel(this, IDC_FRAMENUM);
	TextureSize = new CLabel(this, IDC_TEXTUREDIMENSION);
	TotalFrames = new CLabel(this, IDC_TOTALFRAMES);

	return true;
}


void CDialogAnimTextureEditor::OnClick(DWORD ID)
{
	if(ID == ID_FILENEW) OnNew();
	if(ID == ID_INSERT_INSERTALLFRAMES) OnInsertAllFrames();
	if(ID == ID_FILE_SAVETOPACKAGE) OnSave();
	if(ID == ID_FILE_OPENFROMPACKAGE40104) OnLoad();
	if(ID == ID_FILE_EXIT40106) Hide();
	
	if(ID == LoopCheck->GetID()) OnLoopChanged();
	if(ID == PlayButton->GetID()) OnPlay();
	if(ID == StopButton->GetID()) OnStop();
	if(ID == PauseButton->GetID()) OnPause();
	if(ID == StepUpButton->GetID()) OnStepUp();
	if(ID == StepDownButton->GetID()) OnStepDown();
	if(ID == RealSizeCheck->GetID()) OnRealSize();
}




void CDialogAnimTextureEditor::OnChange(DWORD ID)
{
	if(ID == FramesPerSecEdit->GetID()) OnChangeFPS();
	if(ID == AnimTimeEdit->GetID()) OnChangeAnimTime();
}



void CDialogAnimTextureEditor::OnPlay()
{
	if(!Texture) return;

	Texture->Play();
}



void CDialogAnimTextureEditor::OnStop()
{
	if(!Texture) return;

	Texture->Stop();
}



void CDialogAnimTextureEditor::OnPause()
{
	if(!Texture) return;

	Texture->Pause();
}



void CDialogAnimTextureEditor::OnStepUp()
{
	if(!Texture) return;

	Texture->StepUp();
}



void CDialogAnimTextureEditor::OnStepDown()
{
	if(!Texture) return;

	Texture->StepDown();
}





void CDialogAnimTextureEditor::DoRender()
{
	if (Texture)
		WeCanvas::Instance()->DrawRectangle(DrawX, DrawY, DrawW, DrawH, D3DCOLOR_XRGB(255,255,255), Texture->GetCurrentTexture());
}




void CDialogAnimTextureEditor::OnLoopChanged()
{
	if(!Texture) return;

	Texture->SetLooped(LoopCheck->GetChecked());
}




void CDialogAnimTextureEditor::OnNew()
{
	if(Texture) SAFE_DELETE(Texture);

	Texture = new WeAnimTexture();
	
	CurrentFrame->SetText("Frame: 0");
	TextureNameStatic->SetText("Name: NO TEXTURE");
	TextureSize->SetText("Texture size: W = , H = ");
	TotalFrames->SetText("Total frames: 0");
	
	UpdateParams();
}





void CDialogAnimTextureEditor::OnChangeFPS()
{
	if(AnimTimeLastChanged) return;

	float tmpFPS = FramesPerSecEdit->GetTextAsFloat();
	Texture->SetFramesPerSecond(tmpFPS);

	FPSLastChanged = true;
	AnimTimeLastChanged = false;
}




void CDialogAnimTextureEditor::OnChangeAnimTime()
{
	if(FPSLastChanged) return;

	float tmpAnimTime = AnimTimeEdit->GetTextAsFloat() / 1000.0f;
	Texture->SetAnimationTime(tmpAnimTime);

	AnimTimeLastChanged = true;
	FPSLastChanged = false;
}




void CDialogAnimTextureEditor::OnRealSize()
{
	if(!Texture) return;

	if(RealSizeCheck->GetChecked())
	{
		DrawX = 0;
		DrawY = 0;
		DrawW = Texture->GetWidth();
		DrawH = Texture->GetHeight();
	}else
	{
		DrawX = 0;
		DrawY = 0;
		DrawW = WeRenderDevice::Instance()->GetResWidth();
		DrawH = WeRenderDevice::Instance()->GetResHeight();
	}
}





void CDialogAnimTextureEditor::Tick()
{
	if(Texture)
		Texture->Tick();

	if (AnimTimeLastChanged)
	{
		float tmpAnimTime = AnimTimeEdit->GetTextAsFloat();
		int tmpFPS = (int)(1000.0f / tmpAnimTime);
		FramesPerSecEdit->SetText(tmpFPS);
	}
	else if (FPSLastChanged)
	{
		float tmpAnimTime = 1000.0f / FramesPerSecEdit->GetTextAsFloat();
		AnimTimeEdit->SetText((int)tmpAnimTime);
		Texture->SetAnimationTime(tmpAnimTime / 1000.0f);
	}

	AnimTimeLastChanged = false;
	FPSLastChanged = false;
}



void CDialogAnimTextureEditor::UpdateParams()
{
	if(!Texture) return;

	LoopCheck->SetChecked(Texture->GetLooped());
	AnimTimeEdit->SetText((int)(Texture->GetAnimationTime() * 1000.0f));
	FramesPerSecEdit->SetText((int)Texture->GetFramesPerSecon());

	OnRealSize();
}



void CDialogAnimTextureEditor::OnInsertAllFrames()
{
	if(!Texture){ MessageBox(0,"Error","Make new texture first!",0); return; }

	if(!GetFile("Texture files: *.jpg, *.tga, *.dds\0*.jpg;*.tga;*.dds\0\0",GetHWND())) return;

	if (_FileNames.GetItemsCount() > 0)
	{
		String tmpFileName;
		for (_FileNames.First(); !_FileNames.End(); _FileNames.Next())
		{
			tmpFileName = _FilesPath + "\\" + _FileNames.GetItem();
			Texture->InsertTextureAtEnd(tmpFileName);

			char tmp[100];

			sprintf(tmp,"Frame: %d", Texture->GetCurrentFrameNum());
			CurrentFrame->SetText(tmp);

			TextureNameStatic->SetText("Name: TEX FROM FILES...");

			sprintf(tmp,"Texture size: W = %d, H = %d", Texture->GetWidth(), Texture->GetHeight());
			TextureSize->SetText(tmp);

			sprintf(tmp, "Total frames: %d", Texture->GetTexturesCount());
			TotalFrames->SetText(tmp);
		}
	}
	else
	{
		Texture->InsertTextureAtEnd(_FileName);
	}

	OnRealSize();
}




void CDialogAnimTextureEditor::OnLoad()
{
	InsertDialog->Type = PACKAGEITEM_ANIMTEXTURE;
	if(InsertDialog->ShowModal(GetHWND()) == MR_OK)
	{
		OnNew();
		Texture = WeTextureManager::Instance()->LoadAnimTexture(InsertDialog->HoleName);
		UpdateParams();

		TextureNameStatic->SetText((String)"Name: " + Texture->Name);

		char tmp[100];

		sprintf(tmp, "Frame: %d", Texture->GetCurrentFrameNum());
		CurrentFrame->SetText(tmp);

		sprintf(tmp, "Texture size: W = %d, H = %d", Texture->GetWidth(), Texture->GetHeight());
		TextureSize->SetText(tmp);

		sprintf(tmp, "Total frames: %d", Texture->GetTexturesCount());
		TotalFrames->SetText(tmp);
	}
}



void CDialogAnimTextureEditor::OnSave()
{
	bool Res = false;
	while(!Res)
	{
		SaveDialog->Type = PACKAGEITEM_ANIMTEXTURE;
		if(SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			if(WePackage::Instance()->BeginWriting(SaveDialog->HoleName, PACKAGEITEM_ANIMTEXTURE, false))
			{
				Texture->Serialize(WePackage::Instance()->DataStream);
				WePackage::Instance()->EndWriting();
				Texture->Name = SaveDialog->HoleName;
				Res = true;   // Save is OK!!!
			}else{
				if(IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
				{
					//WePackage::Instance()->Remove(SaveDialog->HoleName);
					if(WePackage::Instance()->BeginWriting(SaveDialog->HoleName, PACKAGEITEM_ANIMTEXTURE, true))
					{
						Texture->Serialize(WePackage::Instance()->DataStream);
						WePackage::Instance()->EndWriting();
						Texture->Name = SaveDialog->HoleName;
					}
					Res = true;		// Save is OK!!!
				}else Res = false;	// Don't wan't to replase so enter new name
			}
		}else Res = true;  //  Cancel was pressed
	}
}



