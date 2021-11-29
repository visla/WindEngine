/*
     CDialog.cpp

	 Autor: Nebojsa Vislavsi
	 Date: 15.6.2005.
	 Version: 1.0


*/

#include <CDialog.h>
#include <windows.h>
#include <commctrl.h>
#include <CEditorResourceManager.h>
#include <CComponent.h>
#include <WindEngine.h>


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  CDialog
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Constructor
//
//////////////////////////////////////////////////////////////////////////
CDialog::CDialog()
{
  bShown = false;
  bShownModal = false;
  bCreated = false;
  PosX = 0;
  PosY = 0;
  hWnd = NULL;
  Dialogs.PushBack(this);
  ModalDialog = NULL;
  
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ~CDialog
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Destructor
//
//////////////////////////////////////////////////////////////////////////
CDialog::~CDialog()
{
	Clear();
	Close();
	for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
	{
		if (Dialogs.GetItem() == this)
		{
			Dialogs.Delete();
		}
	}	
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  AddComponent
//         Input Parametars :  CComponent*
//         Output Parametars:  N/A
//         Comments         :  Add component to dialog
//
//////////////////////////////////////////////////////////////////////////
void CDialog::AddComponent(CComponent *Comp)
{
  if (!Comp) return;
  Components.PushBack(Comp);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Clear
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Clears All Components
//
//////////////////////////////////////////////////////////////////////////
void CDialog::Clear()
{
	for (Components.First(); !Components.End(); Components.Next())
	{
		SAFE_DELETE(Components.GetItem());
	}
	Components.Clear();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  UINT, HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create And Show Dialog
//
//////////////////////////////////////////////////////////////////////////
bool CDialog::Create(UINT DialogTemplate, HWND hParent, bool Show_)
{
	if (bCreated) return true;

	bShown = false;

	this->DialogTemplate = DialogTemplate;
	this->hParent = hParent;


	hWnd = CreateDialog((HINSTANCE)EditorResourceManager.GetResourceInstance(), *((LPCSTR*)&DialogTemplate), hParent, DlgProc);

	if (hWnd == NULL) return false;
    bCreated = true;

    if (Show_) Show();
	return true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  CreateModal
//         Input Parametars :  UINT, HWND
//         Output Parametars:  bool
//         Comments         :  Create Modal dialog
//
//////////////////////////////////////////////////////////////////////////
bool CDialog::CreateModal(UINT DialogTemplate, HWND hParent)
{
	if (bCreated) return true;

	bShown = false;

	this->DialogTemplate = DialogTemplate;
	this->hParent = hParent;   
	bCreated = true;
    
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShowModal
//         Input Parametars :  HWND
//         Output Parametars:  int
//         Comments         :  Shows modal dialog
//
//////////////////////////////////////////////////////////////////////////
int CDialog::ShowModal(HWND hParent)
{
	if (!bCreated) return 0;
	
	bShownModal = true;
	bShown = true;
	int tmp;
	
	ModalDialog = this;
	tmp = (int)DialogBox((HINSTANCE)EditorResourceManager.GetResourceInstance(), *((LPCSTR*)&DialogTemplate), hParent, DlgProc);
	bShown = false;
	bShownModal = false;
	ModalDialog = NULL;
	return tmp;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  CSetModalResult
//         Input Parametars :  int
//         Output Parametars:  N/A
//         Comments         :  Closing modal dialog
//
//////////////////////////////////////////////////////////////////////////
void CDialog::SetModalResult(int ModalResult)
{
	if (!bShownModal) return;	
	OnClose();
	EndDialog(hWnd, ModalResult);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Show
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Shows dialog
//
//////////////////////////////////////////////////////////////////////////
void CDialog::Show()
{	
	if (bShown) return;
	if (!bCreated) return;

	ShowWindow(hWnd, SW_SHOWNA);
    bShown = true;

	InvalidateRect(hWnd, NULL, false);
	UpdateWindow(hWnd);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Hide
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Hide dialog
//
//////////////////////////////////////////////////////////////////////////
void CDialog::Hide()
{ 
	if (!bShown) return;
    if (!bCreated) return;
  
	OnClose();

	ShowWindow(hWnd, SW_HIDE);
    bShown = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Close
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Close dialog
//
//////////////////////////////////////////////////////////////////////////
void CDialog::Close()
{
	if (!bCreated) return;
    //if (hWnd) PostMessage(hWnd, WM_CLOSE, 0, 0);

	bCreated = false;
	bShown = false;
}


void CDialog::Refresh()
{

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetPosition
//         Input Parametars :  int, int
//         Output Parametars:  N/A
//         Comments         :  Set dialog position;
//
//////////////////////////////////////////////////////////////////////////
void CDialog::SetPosition(int x, int y)
{
  SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);             
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetSize
//         Input Parametars :  int, int
//         Output Parametars:  N/A
//         Comments         :  Set dialog size
//
//////////////////////////////////////////////////////////////////////////
void CDialog::SetSize(int Width, int Height)
{
	SetWindowPos(hWnd, HWND_TOP, 0, 0, Width, Height, SWP_NOMOVE);             
}

CSmartList <CDialog *> CDialog::Dialogs;
CDialog *CDialog::ModalDialog;









// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
// =========================================================================================
INT_PTR CALLBACK CDialog::DlgProc(HWND hDlgWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{

	case WM_INITDIALOG:
		if (ModalDialog) 
		{
			ModalDialog->OnKnowHWND(hDlgWnd);
			ModalDialog->OnCreate();
			return TRUE;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnKnowHWND(hDlgWnd);
				Dialogs.GetItem()->OnCreate();
				return TRUE;
			}
		}
		return TRUE;

	case WM_SHOWWINDOW:
		if (ModalDialog) 
		{
			if (wParam == TRUE) ModalDialog->OnShow();
			else ModalDialog->OnHide();
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{	
				if (wParam == TRUE) Dialogs.GetItem()->OnShow();
				else Dialogs.GetItem()->OnHide();
				return 0;
			}
		}		  
		return 0;


	case WM_PAINT:
		if (ModalDialog) 
		{
			ModalDialog->OnPaint();
			break;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{	
				// all modal dialogs are destroyed by closing, other dialogs are only hidden
				Dialogs.GetItem()->OnPaint();				
				break;
			}
		}		  

		break;

	case WM_CLOSE:
		if (ModalDialog) 
		{
			ModalDialog->SetModalResult(MR_CANCEL);			
			break;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{	
				// all modal dialogs are destroyed by closing, other dialogs are only hidden
				if (Dialogs.GetItem()->bShownModal) Dialogs.GetItem()->SetModalResult(MR_CANCEL);
				else Dialogs.GetItem()->Hide();
				break;
			}
		}		  
		break;


    case WM_SIZE:
		if (ModalDialog) 
		{
			ModalDialog->OnResize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnResize(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  
	  return 0;



	case WM_HSCROLL:
		if (ModalDialog) 
		{
			ModalDialog->OnHScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnHScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
				return 0;
			}
		}		  		  		
		return 0;

	case WM_VSCROLL:
		if (ModalDialog) 
		{
			ModalDialog->OnVScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnVScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
				return 0;
			}
		}		  		  		
	  return 0;



	case WM_MOUSEWHEEL:
		if (ModalDialog) 
		{
			ModalDialog->OnMouseWheel(HIWORD(wParam));
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnMouseWheel(HIWORD(wParam));
				return 0;
			}
		}		  		  		
	  	return 0;


	case WM_LBUTTONDOWN:		  
		if (ModalDialog) 
		{
			ModalDialog->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
	  return 0;

	case WM_LBUTTONUP:
		if (ModalDialog) 
		{
			ModalDialog->OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
        return 0;


	case WM_MOUSEMOVE:
		if (ModalDialog) 
		{
			ModalDialog->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
		return 0;


	case WM_INITMENUPOPUP:
		if (ModalDialog) 
		{
			ModalDialog->OnInitPopup((DWORD)wParam);
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnInitPopup((DWORD)wParam);
				return 0;
			}
		}
		return 0;

	case WM_CONTEXTMENU:
		if (ModalDialog) 
		{
			ModalDialog->OnContextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}


		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				Dialogs.GetItem()->OnContextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
				break;
			}
		}
		break;




	case WM_COMMAND: 
		if (ModalDialog) 
		{				
			
				if (HIWORD(wParam) == EN_CHANGE)
				{
					ModalDialog->OnChange(LOWORD(wParam));
					return FALSE;
				}
				
				if (HIWORD(wParam) == CBN_SELCHANGE)
				{
					ModalDialog->OnClick(LOWORD(wParam));
					return FALSE;
				}			

				if (HIWORD(wParam) == BN_CLICKED)
				{
					ModalDialog->OnClick(LOWORD(wParam));
					return FALSE;             
				}				
				if (HIWORD(wParam) == BN_DBLCLK)
				{
					ModalDialog->OnDblClick(LOWORD(wParam));
					return FALSE;             
				}				
		}


		// for other non modal dialogs
		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				if (HIWORD(wParam) == EN_CHANGE)
				{
					Dialogs.GetItem()->OnChange(LOWORD(wParam));
					break;
				}
				if (HIWORD(wParam) == CBN_SELCHANGE)
				{
					Dialogs.GetItem()->OnClick(LOWORD(wParam));
					break;                    
				}			

				if (HIWORD(wParam) == BN_CLICKED)
				{
					Dialogs.GetItem()->OnClick(LOWORD(wParam));
					break;                    
				}				
				if (HIWORD(wParam) == BN_DBLCLK)
				{
					Dialogs.GetItem()->OnDblClick(LOWORD(wParam));
					break;                    
				}				
			}
		}
	  return FALSE;		 

	case WM_NOTIFY:
		LPNMHDR lphdr;
		lphdr = (LPNMHDR)lParam;
		if (ModalDialog) 
		{
			switch (lphdr->code)
			{
				//ModalDialog->OnClick(lphdr->idFrom);
			case TVN_SELCHANGEDA:
			case TVN_SELCHANGEDW:
			//case TVN_SELCHANGINGA:
			//case TVN_SELCHANGINGW:
			case TVN_KEYDOWN:
			case LVN_ITEMCHANGED:
					ModalDialog->OnClick((DWORD)lphdr->idFrom);
				break;
				
			}			
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				//Dialogs.GetItem()->OnClick(lphdr->idFrom);
				switch (lphdr->code)
				{
					case TVN_SELCHANGEDA:
					case TVN_SELCHANGEDW:
					//case TVN_SELCHANGINGA:
					//case TVN_SELCHANGINGW:
					case TVN_KEYDOWN:
					case LVN_ITEMCHANGED:
						Dialogs.GetItem()->OnClick((DWORD)lphdr->idFrom);
						break;

				//case TB_BOTTOM:
				//case TB_ENDTRACK:
				//case TB_LINEDOWN:
				//case TB_LINEUP:
				//case TB_PAGEDOWN:
				//case TB_PAGEUP:
				//case TB_THUMBPOSITION:
				//case TB_THUMBTRACK:
				//case TB_TOP:
				//case TVN_SELCHANGED:
				//case LVN_ITEMCHANGED:
				//		Dialogs.GetItem()->OnClick(lphdr->idFrom);
				//	break;

				}
				return 0;
			}
		}		  		
		return 0;


	case WM_KEYDOWN:
		if (ModalDialog) 
		{
			if ((lParam & 0x40000000) == 0) 					
			  ModalDialog->OnKeyDown((DWORD)wParam);
			return 0;
		}

		for (Dialogs.First(); !Dialogs.End(); Dialogs.Next())
		{
			if (hDlgWnd == Dialogs.GetItem()->GetHWND())
			{
				if ((lParam & 0x40000000) == 0) 					
				 Dialogs.GetItem()->OnKeyDown((DWORD)wParam);
				return 0;
			}
		}		  		  		
		return 0;


	}

	
	
	return 0;
}

