/*
    CEditorWindow.cpp

	Author: Nebojsa Vislavski
	Date: 6.6.2005.
	Version: 1.1

	This class is used as main window in editors
  
    Look in .h for history

*/


#ifndef DEBUG
#ifdef _DEBUG
#undef _DEBUG
#endif
#endif

#include <CEditorWindow.h>
#include <windows.h>
#include <commctrl.h>
#include <CString.h>
#include <CEditorResourceManager.h>
#include <WeCEGUI.h>
#include <WeLoger.h>

#define WM_GRAPHNOTIFY WM_APP + 1


CEditorWindow::CEditorWindow()
{
	bActive = false;
	bMinimized = false;
	hWnd = NULL;
	hInstance = NULL;
	Windows.PushBack(this);
	CEGUIInitialized = false;
}


CEditorWindow::~CEditorWindow()
{
	Terminate();
	for (Windows.First(); !Windows.End(); Windows.Next())
	{
		if (Windows.GetItem() == this)
		{
			Windows.Delete();
		}
	}
}


void CEditorWindow::Terminate()
{
	if (!GetActive()) return;
   
	if (GetHWND()) DestroyWindow(GetHWND());
	UnregisterClass(ClassName, NULL);
	SetActive(false);
}


bool CEditorWindow::IsEditor = false;
bool CEditorWindow::CEGUIInitialized = false;
bool CEditorWindow::Playing = false;


bool CEditorWindow::Create(StringParam ClassName, StringParam WindowName, int MenuID, 
						   int IconID, int IconIDSmall, bool VScroll, bool HScroll,
						   bool bShow, bool bFullScreen, bool bEditor, bool bMaximized)
{
	IsEditor = bEditor;

	EditorResourceManager.Start();

	hInstance = (HINSTANCE)EditorResourceManager.GetResourceInstance();//(HINSTANCE)GetModuleHandle(NULL);

    //HMODULE hDllResource;
	//hDllResource = LoadLibrary("editors.dll");
	//if (hDllResource == NULL) return false;

	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //(HBRUSH)COLOR_ACTIVECAPTION;
	if(!bFullScreen)
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	else
		wndclass.hCursor = NULL;//LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, *((LPCSTR*)&IconID));
	wndclass.hIconSm =  LoadIcon(hInstance, *((LPCSTR*)&IconIDSmall));
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = MsgProc;
	wndclass.lpszClassName = ClassName;
	wndclass.lpszMenuName =  "MAPMENU"; 
	wndclass.style = CS_OWNDC | CS_DBLCLKS;
	


	if (RegisterClassEx(&wndclass) == 0) 
	{   
		//FreeLibrary(hDllResource);
        return false;
	}

	if (bFullScreen)
	{
		hWnd = CreateWindowEx(0, ClassName, WindowName, WS_POPUPWINDOW,
							  0, 0, 800, 600, 
							  NULL, NULL,//LoadMenu(hInstance, (LPCSTR)MenuID), 
							  hInstance, 
							  0);
	}
	else
	{
		HMENU menu = LoadMenu(hInstance, *((LPCSTR*)&MenuID));
		hWnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW,
							  0, 0, 800, 600, 
							  NULL, menu,
							  hInstance, 
							  0);
	}


	if (hWnd == NULL) 
	{  	
		Terminate();
		return false;
	}
   	

	/*SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLBARINFO);
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMax = 100;
	ScrollInfo.nMin = 0;
	ScrollInfo.nPage = 20;
	ScrollInfo.nPos = 0;


	SetScrollInfo(hWnd, SB_VERT, &ScrollInfo, false);*/

	SetActive(true);
	SetMinimized(false);

	bShowMaximized = bMaximized;

	if (bShow)
		Show();
	InitCommonControls();
	
    //Sleep(500); // wait just to visually make things better

	if(!bFullScreen && !IsEditor)
		ShowCursor(false);

	return true;
}



void CEditorWindow::Show()
{
	if (!GetActive()) return;

	if (bShowMaximized)
	{
		ShowWindow(GetHWND(), SW_SHOWMAXIMIZED);
	}
	else
	{
		ShowWindow(GetHWND(), SW_SHOW);
	}
}



void CEditorWindow::Hide()
{
	if (!GetActive()) return;

	ShowWindow(GetHWND(), SW_HIDE);
}



bool CEditorWindow::Update()
{
	if (!GetActive()) return false;

	OnEveryFrame();

	bool bGotMsg;
	bGotMsg = ( PeekMessage( &Message, NULL, 0U, 0U, PM_REMOVE ) != 0 );

	if (bGotMsg)
	{
		if (Message.message == WM_QUIT)
		{
			Terminate();
			return true;
		}
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		return true;
	}
	else
	{
		return false;
	}
}




int CEditorWindow::GetClientWidth()
{
  if (!GetActive()) return 0;  

  RECT Rect;
  GetClientRect(GetHWND(), &Rect);
  return Rect.right - Rect.left;
}


int CEditorWindow::GetClientHeight()
{
	if (!GetActive()) return 0;  

	RECT Rect;
	GetClientRect(GetHWND(), &Rect);
	return Rect.bottom - Rect.top;

}





LRESULT CALLBACK CEditorWindow::MsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	bool tmp;

	switch(Msg)
	{
	case WM_PAINT:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnPaint();
				break;
			}
		}		
		break;

	case WM_CLOSE:
		tmp = true;
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnBeforeClose(tmp);
				if (tmp) DestroyWindow(hWnd);
				return 0;
			}
		}		
		return 0;


	case WM_DESTROY:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnDestroy();
				//return 0;
				break;
			}
		}
		PostQuitMessage(0);
		return 0;


	case WM_CREATE:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnCreate();
				return 0;
			}
		}
		return TRUE;

	

    case WM_SIZE:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnResize(LOWORD(lParam), HIWORD(lParam), (DWORD)wParam);
				return 0;
			}
		}		  
	  return 0;


	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if ((Msg == WM_SYSKEYDOWN) && (wParam == VK_F4))
			break;

		if ((Msg == WM_SYSKEYDOWN) && (wParam == VK_SPACE))
			return 0;

		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{				
				if ((lParam & 0x40000000) == 0)
					Windows.GetItem()->OnKeyDown((DWORD)wParam);
				return 0;
			}
		}		  		  		
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{				
				//if ((lParam & 0x40000000) == 0)
				  Windows.GetItem()->OnKeyUp((DWORD)wParam);
				return 0;
			}
		}		  		  		
		return 0;

	case WM_CHAR:
	case WM_SYSCHAR:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{				
				if ((lParam & 0x40000000) == 0)
					Windows.GetItem()->OnChar((DWORD)wParam);
				break;
			}
		}		  		  		
        break;

	case WM_HSCROLL:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnHScroll(LOWORD(wParam), HIWORD(wParam));
				return 0;
			}
		}		  		  		
		return 0;

	case WM_VSCROLL:		  
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnVScroll(LOWORD(wParam), HIWORD(wParam));
				return 0;
			}
		}		  		  		
	  return 0;

	case WM_LBUTTONDBLCLK:

		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnDblClk(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		  				
		return 0;


	case WM_MOUSEWHEEL:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnMouseWheel(HIWORD(wParam));
				return 0;
			}
		}		  		  		
	  	return 0;

	case WM_LBUTTONDOWN:		  
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
	  return 0;

	case WM_RBUTTONDOWN:		  
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
	  return 0;

	case WM_MBUTTONDOWN:		  
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnMButtonDown(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
	  return 0;

	case WM_LBUTTONUP:
			
		for (Windows.First(); !Windows.End(); Windows.Next())
		{			
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
        return 0;

	case WM_RBUTTONUP:
			
		for (Windows.First(); !Windows.End(); Windows.Next())
		{			
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
        return 0;

	case WM_MBUTTONUP:
			
		for (Windows.First(); !Windows.End(); Windows.Next())
		{			
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnMButtonUp(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
        return 0;

	case WM_MOUSEMOVE:
		if(IsEditor == false)
			if(CEGUIInitialized && !Playing){
				CEGUI::System::getSingleton().injectMousePosition((float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
			}

		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->MouseX = LOWORD(lParam);
				Windows.GetItem()->MouseY = HIWORD(lParam);
				Windows.GetItem()->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}		  		
		return 0;

	case WM_COMMAND: 
		for (Windows.First(); !Windows.End(); Windows.Next())
		{
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				// from menu
				if (HIWORD(wParam) == 0) 
				{
					Windows.GetItem()->OnClick(LOWORD(wParam));
					break;
				}
				if (HIWORD(wParam) == EN_CHANGE)
				{
					Windows.GetItem()->OnChange(LOWORD(wParam));
					break;
				}
				if (HIWORD(wParam) == CBN_SELCHANGE)
				{
					Windows.GetItem()->OnClick(LOWORD(wParam));
					break;                    
				}				
				if (HIWORD(wParam) == BN_CLICKED)
				{
					Windows.GetItem()->OnClick(LOWORD(wParam));
					break;                    
				}				
				if (HIWORD(wParam) == BN_DBLCLK)
				{
					Windows.GetItem()->OnDblClick(LOWORD(wParam));
					break;                    
				}				
			}
		}
	  return FALSE;		  	  

	case WM_SYSCOMMAND:
		WPARAM cmd;
		cmd = wParam & 0xFFF0;
		if ((cmd == SC_SCREENSAVE) || (cmd == SC_MONITORPOWER))
			return 0;
		break;


	case WM_GRAPHNOTIFY:
		for (Windows.First(); !Windows.End(); Windows.Next())
		{			
			if (hWnd == Windows.GetItem()->GetHWND())
			{
				Windows.GetItem()->OnGraphNotify();
				return 0;
			}
		}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

CSmartList <CEditorWindow *> CEditorWindow::Windows;

