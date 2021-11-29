/*
    CEditorWindow.h

	Author: Nebojsa Vislavski
	Date: 6.6.2005.
	Version: 2.0

	This class is used as main window in editors

	History:
	  1.1 - Added GetClientWidth, GetClientHeight functions
	  2.0 - this is new concpet. Window has same ability like CDialog

  

*/

#pragma once

#include <windows.h>
#include <CString.h>
#include <SmartList.h>

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A // (not always defined)
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120 // (not always defined)
#endif



class CEditorWindow
{
protected:

	
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static CSmartList <CEditorWindow *> Windows;


    HWND hWnd;
    HINSTANCE hInstance;
	MSG Message;

	bool bMinimized;
	bool bShowMaximized;
	bool bActive;

	String ClassName;

	virtual void	OnPaint() { };
	virtual void	OnBeforeClose(bool &AcceptClose) { } ;
	virtual void	OnDestroy() { } ;	
	virtual void	OnDblClk(int x, int y) { };
	virtual void	OnCreate() { };		
	virtual void	OnResize(int neww, int newh, DWORD ResizeType) { };
	virtual void	OnHScroll(DWORD Type, DWORD TrackPos) { };
	virtual void	OnVScroll(DWORD Type, DWORD TrackPos) { };
	virtual void	OnMouseWheel(WORD Position) { };
	virtual void	OnLButtonDown(int x, int y) { };
	virtual void	OnLButtonUp(int x, int y) { };
	virtual void	OnRButtonDown(int x, int y) { };
	virtual void	OnRButtonUp(int x, int y) { };
	virtual void	OnMButtonDown(int x, int y) { };
	virtual void	OnMButtonUp(int x, int y) { };
	
	virtual void	OnMouseMove(int x, int y) { };
	virtual void	OnClick(DWORD ID) { };
	virtual void	OnChange(DWORD ID) { };	
	virtual void	OnDblClick(DWORD ID) { };
	virtual void	OnKeyDown(DWORD KeyCode) { };
	virtual void	OnKeyUp(DWORD KeyCode) { };
	virtual void	OnEveryFrame() { };
	virtual void	OnChar(DWORD c) { };
	virtual void	OnGraphNotify() { };

	int MouseX;
	int MouseY;

	static bool IsEditor;


public:
    CEditorWindow();
	~CEditorWindow();

	static bool CEGUIInitialized;
	static bool Playing;

	void SetMinimized(bool Minimized) { bMinimized = Minimized; }
	bool GetMinimized() { return bMinimized; }

	void SetActive(bool Active) { bActive = Active; }
	bool GetActive() { return bActive; }

	HWND GetHWND() { return hWnd; }
	HINSTANCE GetInstance() { return hInstance; }
	String &GetClassName() { return ClassName; }
	MSG GetMessage() { return Message; }

    void Terminate();
	bool Create(StringParam ClassName, StringParam WindowName, int MenuID, 
		        int IconID, int IconIDSmall, bool VScroll = false, bool HScroll = false,
				bool bShow = true, bool bFullScreen = false, bool bEditor = true, bool bMaximized = false);

    void Show();
	void Hide();
	int		GetMouseX() { return MouseX; }
	int		GetMouseY() { return MouseY; }

	int		GetWidth() { RECT Rect; GetClientRect(hWnd, &Rect); return Rect.right-Rect.left; }
	int		GetHeight() { RECT Rect; GetClientRect(hWnd, &Rect); return Rect.bottom-Rect.top;}
	int		GetX() { RECT Rect; GetWindowRect(hWnd, &Rect); return Rect.left;}
	int		GetY() { RECT Rect; GetWindowRect(hWnd, &Rect); return Rect.top;}

	bool Update(); // returns false if no message is processed
	bool Running() { return bActive; };

	int GetClientWidth(); // returns width of the client area of window
	int GetClientHeight(); // returns height of the client area of window
	RECT Rect;
	void SetClientRect(LPRECT lpRect) { Rect = *lpRect;} 

};