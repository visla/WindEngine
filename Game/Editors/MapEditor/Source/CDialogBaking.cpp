

#include <CDialogBaking.h>
#include <CEditorResourceManager.h>
#include <WeModelManager.h>
#include <CMapEditorWindow.h>
#include <WePathNode.h>
#include <CEditorMap.h>
//#include <CDialogMaterialPresets.h>
#include <CDialogInsert.h>
#include <CDialogSave.h>

CProgressBar *CDialogBaking::LoadProgress = NULL;
CLabel *CDialogBaking::BakingLabel = NULL;
bool CDialogBaking::Canceled = false;

CDialogBaking *CDialogBaking::_instance = NULL;
CDialogBaking *CDialogBaking::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogBaking();
	return _instance;
}

bool CDialogBaking::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_BAKINGDIALOG, hParent, Show))
	{
		return false;
	}

	CancelButton = new CButton(this, IDCANCEL);
	LoadProgress = new CProgressBar(this, IDC_PROGRESS);
	BakingLabel = new CLabel(this, IDC_BAKINGSTATIC);

	Canceled = false;

	LoadProgress->SetRange(0, 100);
	return true;
}

void CDialogBaking::OnLoadProgress(int FacesLit, float Progress, bool &Stop)
{
	if (Canceled) 
	{
		Canceled = false;
		Stop = true;
		return;
	}

	if (Progress < 1.0f)
	{
		if (!CDialogBaking::Instance()->GetVisible())
		{
			CDialogBaking::Instance()->Show();	
		}
		bool bGotMsg = true;
		MSG Message;
		while (bGotMsg)
		{
			bGotMsg = ( PeekMessage( &Message, NULL, 0U, 0U, PM_REMOVE ) != 0 );

			if (bGotMsg) 
			{      
				TranslateMessage(&Message);
				DispatchMessage(&Message);     			
			}							
		}
	}
	
	char tmp[100];
	sprintf(tmp, "Baking face: %d - %3.3f percent", FacesLit, Progress*100.0f);
	BakingLabel->SetText(tmp);
	LoadProgress->SetPosition((int)(Progress*100.0f));	
	if (Progress >= 1.0f) CDialogBaking::Instance()->Hide();
}


void CDialogBaking::OnMapLoadProgress(float Progress, bool &Stop)
{
	if (Canceled) 
	{
		Canceled = false;
		Stop = true;
		return;
	}

	if (Progress < 1.0f)
	{
		if (!CDialogBaking::Instance()->GetVisible())
		{
			CDialogBaking::Instance()->Show();	
		}
		bool bGotMsg = true;
		MSG Message;
		while (bGotMsg)
		{
			bGotMsg = ( PeekMessage( &Message, NULL, 0U, 0U, PM_REMOVE ) != 0 );

			if (bGotMsg) 
			{      
				TranslateMessage(&Message);
				DispatchMessage(&Message);     			
			}							
		}
	}


	char tmp[100];
	sprintf(tmp, "Loading: %3.3f percent", Progress*100.0f);
	BakingLabel->SetText(tmp);
	LoadProgress->SetPosition((int)(Progress*100.0f));	
	if (Progress >= 1.0f) CDialogBaking::Instance()->Hide();
}



void CDialogBaking::OnResaveProgress(StringParam MeshName, float Progress, bool &Stop)
{
	// common code pre
	if (Canceled) 
	{
		Canceled = false;
		Stop = true;
		return;
	}

	if (Progress < 1.0f)
	{
		if (!CDialogBaking::Instance()->GetVisible())
		{
			CDialogBaking::Instance()->Show();	
		}
		bool bGotMsg = true;
		MSG Message;
		while (bGotMsg)
		{
			bGotMsg = ( PeekMessage( &Message, NULL, 0U, 0U, PM_REMOVE ) != 0 );

			if (bGotMsg) 
			{      
				TranslateMessage(&Message);
				DispatchMessage(&Message);     			
			}							
		}
	}


	// specific code
	char tmp[100];
	sprintf(tmp, "Saved %s (%3.3f%%)", MeshName.GetString(), Progress*100.0f);
	BakingLabel->SetText(tmp);


	// common code post
	LoadProgress->SetPosition((int)(Progress*100.0f));	
	if (Progress >= 1.0f) CDialogBaking::Instance()->Hide();
}



void CDialogBaking::OnClick(DWORD ID)
{
	if (ID == CancelButton->GetID()) 
	{
		Canceled = true;	
		Hide();
	}
}

void CDialogBaking::OnShow()
{
	Canceled = false;
}