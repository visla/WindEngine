#include <CDialogPostProcess.h>
#include <CEditorResourceManager.h>
#include <CDialogTools.h>
#include <WePostProcess.h>



//========================================================
//    PostProcesesDialog Class functions
//=======================================================

CDialogPostProceses* CDialogPostProceses::_instance = NULL;


CDialogPostProceses::CDialogPostProceses()
{
}




CDialogPostProceses::~CDialogPostProceses()
{
}




CDialogPostProceses* CDialogPostProceses::Instance()
{
	if(_instance) return _instance;
	_instance = new CDialogPostProceses();
	return _instance;
}



bool CDialogPostProceses::Create(HWND hParent, bool Shown)
{
	if(!CDialog::Create(IDD_POSTPROCESVIEWER, hParent, Shown))
	{
		return false;
	}

	AddEffect = new CButton(this, IDC_ADD);
	RemoveEffect = new CButton(this, IDC_REMOVE);
	ClearAll = new CButton(this, IDC_CLEAR);
	Preview = new CButton(this, IDC_PREVIEW);
	Close = new CButton(this, IDC_CLOSE);
	ScaleX = new CEditBox(this, IDC_SCREENSCALEX);
	ScaleX->SetText(1.0f);
	ScaleY = new CEditBox(this, IDC_SCREENSCALEY);
	ScaleY->SetText(1.0f);

	EffectList = new CListBox(this, IDC_EFFECTLIST);

	return true;
}




void CDialogPostProceses::OnClick(DWORD ID)
{
	if(ID == Close->GetID()) OnCloseClick();
	if(ID == AddEffect->GetID()) OnAdd();
	if(ID == Preview->GetID()) OnPreview();
	if(ID == RemoveEffect->GetID()) OnRemove();
	if(ID == ClearAll->GetID()) OnClearAll();
}



void CDialogPostProceses::OnAdd()
{
	float SX = ScaleX->GetTextAsFloat();
	float SY = ScaleY->GetTextAsFloat();
	if(SX < 0 || SY < 0)
	{
		MessageBox(NULL,"Scaling can't be negative!","ERROR",MB_OK | MB_ICONERROR);
		return;
	}


	GetFile("Effect *.fx files\0*.fx\0\0","");
	if(FileName == (String)"") return;

	WePostProcessEffect* tmpPostEffect = NULL;

	tmpPostEffect = WePostProcess::Instance()->AddEffect(FileName,SX,SY);
	EffectList->AddItem(FileNameNoPath,*((DWORD*)&tmpPostEffect));

	if(SX != 1 || SY != 1)
	{
		if(IDNO == MessageBox(GetHWND(),"Return scaling to default values?","Scaling",MB_YESNO | MB_ICONQUESTION)) return;

		ScaleX->SetText(1.0f);
		ScaleY->SetText(1.0f);
	}
}



void CDialogPostProceses::OnRemove()
{
	DWORD data = EffectList->GetSelectionData();
	WePostProcess::Instance()->RemoveEffect(*((WePostProcessEffect**)&data));
	
	// TO DO: repair GetSelectionText() in CCommponent
	// this doesn't work, because GetSelectionText returning an empty string!!!
	EffectList->RemoveItem(EffectList->GetSelectionText());
}



void CDialogPostProceses::OnPreview()
{
	WePostProcess::Instance()->TurnOn();
}



void CDialogPostProceses::OnCloseClick()
{
	WePostProcess::Instance()->TurnOff();
	SetVisible(false);
}



void CDialogPostProceses::OnClearAll()
{
	WePostProcess::Instance()->TurnOff();
	WePostProcess::Instance()->ClearEffects();
	EffectList->Clear();
}

