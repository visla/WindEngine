/*
     CDialog.h

	 Autor: Nebojsa Vislavsi
	 Date: 15.6.2005.
	 Version: 1.0

	 This class is abstract class used for every dialog in editors. 
	 User only have to override DlgProc and Create functions for dialog to work.
	 Making constructors and destructors in child classes is recommended when user 
	 need additional components in dialog to be initialized

	 Create function in child classes should call protected Create method from this 
	 class in order to work correctly


*/


#pragma once

#include <windows.h>
#include <SmartList.h>
//#include <EditorsResource.h>


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A // (not always defined)
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120 // (not always defined)
#endif



#define MR_OK 1
#define MR_CANCEL 2


class CComponent;

class CDialog
{
protected:
    bool bShown;
	bool bShownModal;
	bool bCreated;
	

	UINT DialogTemplate;
    HWND hParent;
	DLGPROC lpDlgProc;
	int ModalResult;
	
    
    HWND hWnd;
	int PosX;
	int PosY;

	static INT_PTR CALLBACK DlgProc(HWND hDlgWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK ModalDlgProc(HWND hDlgWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static CSmartList <CDialog *>	Dialogs;
	static CDialog					*ModalDialog;

	bool Create(UINT DialogTemplate, HWND hParent, bool Show); 
	bool CreateModal(UINT DialogTemplate, HWND hParent);

	CSmartList <CComponent *> Components;

	//void SetModalResult(int MR) { ModalResult = MR; Hide(); }

	// events


	virtual void	OnKnowHWND(HWND hwnd) { hWnd = hwnd; }; // called when dialog knows about its hwnd when creating
	virtual void	OnCreate() { };		 // this parameter is passed from dlgproc
	virtual void	OnResize(int neww, int newh) { };
	virtual void	OnClose() { }; // for modal dialogs called when closing for others when hidding
	virtual void	OnHide() { };
	virtual void	OnShow() { };
	virtual void	OnHScroll(DWORD Type, DWORD TrackPos, HWND Handle) { };
	virtual void	OnVScroll(DWORD Type, DWORD TrackPos, HWND Handle) { };
	virtual void	OnMouseWheel(WORD Position) { };
	virtual void	OnLButtonDown(int x, int y) { };
	virtual void	OnLButtonUp(int x, int y) { };
	virtual void	OnMouseMove(int x, int y) { };
	virtual void	OnKeyDown(DWORD KeyCode) { };
	virtual void	OnPaint() { };


	virtual void	OnNotify(LPNMHDR lpNMHDR) { };
	virtual void	OnClick(DWORD ID) { };
	virtual void	OnChange(DWORD ID) { };	
	virtual void	OnDblClick(DWORD ID) { };
	virtual void	OnInitPopup(DWORD ID) { };
	virtual void	OnContextMenu(HWND hwnd, int x, int y) { };


	void Clear(); // clears all components within this dialog

public:
	CDialog();
	
	
	virtual ~CDialog();

    virtual bool Create(HWND hParent, bool Shown = true) = 0; // every other class must specify this method 
	

	
    void AddComponent(CComponent *);
   
	int		GetWidth() { RECT Rect; GetWindowRect(hWnd, &Rect); return Rect.right-Rect.left; }
	int		GetHeight() { RECT Rect; GetWindowRect(hWnd, &Rect); return Rect.bottom-Rect.top; }


	void SetPosition(int x, int y);
	void SetSize(int Width, int Height);
	void Close();
	void Show();	
	int  ShowModal(HWND hParent);
	void SetModalResult(int ModalResult);
	void Hide();
	virtual void Refresh();

	

	void SetVisible(bool visible) { if (visible) Show(); else Hide(); }
	bool GetVisible() { return bShown; }
	HWND GetHWND() { return hWnd; }

};
