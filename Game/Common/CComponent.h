/*
     CComponent.h

	 Autor: Nebojsa Vislavski
	 Date: 25.7.2005.
	 Version: 1.0

	 This is main class for presenting comboboxes, edit boxes and ...


*/


#pragma once


#include <windows.h>
#include <CDialog.h>
#include <CString.h>
#include <commctrl.h>



class CDialog;

class CComponent
{
protected:
	HWND hWnd;
	UINT ID;
	CDialog *Parent;
	bool Visible;

public:
	CComponent();
	CComponent(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this); }

	operator DWORD() { return ID; }


	HWND	GetParentHWND() { return Parent->GetHWND(); }
	CDialog *GetParentDialog() { return Parent; }
	UINT	GetID() { return ID; }
	HWND	GetHWND() { return hWnd; }

	void	SetVisible(bool Vis) { Visible = Vis; if (Vis) ShowWindow(hWnd, SW_SHOW); else ShowWindow(hWnd, SW_HIDE);}
	bool	GetVisible() { return Visible; }
	void	SetFocused() { SetFocus(hWnd); }
	
};

class CComboBox: public CComponent
{
protected:
public:
	CComboBox(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}


	void	SetSelection(int index);
	void	SetSelection(StringParam s);
	void	AddItem(StringParam Text, DWORD Data = 0);
	void	RemoveItem(int index);
	void	Clear(); 
	int		GetSelectionIndex();
	String	GetSelectionText();
	DWORD	GetSelectionData();
	String  GetText();
	void	SetText(StringParam Text);
	void	GetData(String &outS, DWORD &outData);	

};
class CEditBox: public CComponent
{
protected:
public:
	CEditBox(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

	void	SetText(StringParam tmpS);
	void	SetText(int number);
	void	SetText(float number);
	int		GetTextAsInt();
	float	GetTextAsFloat();
	String GetText();
	void Clear();
};
class CLabel: public CComponent
{
protected:
public:
	CLabel(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

	void SetText(StringParam Text);
	String GetText();	
};

class CButton: public CComponent
{
protected:
public:
	CButton(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}
	
	void	SetText(StringParam Text);
	String	GetText();

};

class CCheckBox: public CComponent
{
protected:
public:
	CCheckBox(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

	bool GetChecked();
	void SetChecked(bool);
};



class CScrollBar: public CComponent
{
protected:
public:
  CScrollBar(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

  void	SetRange(int min, int max);
  int	GetPosition();
  void	SetPosition(int pos);

};


class CGroupBox: public CComponent
{
protected:
public:
	CGroupBox(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}
};


class CListBox: public CComponent
{
protected:
	
public:
	CListBox(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}



	void	SetSelection(int index);
	void	SetSelection(StringParam s);
	void	AddItem(StringParam Text, DWORD Data = 0);
	void	RemoveItem(int index);
	void	RemoveItem(StringParam);
	void	Clear(); 
	int		GetSelectionIndex();
	String	GetSelectionText();
	DWORD	GetSelectionData();

	void	GetData(String &outS, DWORD &outData);	
	int		GetItemCount();
	String	GetText();
	
};


class CTreeViewItem
{
friend class CTreeView;
protected:

	TVITEMEX  tvItemEx;  // description of item
	HTREEITEM hTreeItem; // handle to tree view item
	HWND	hwndTV;    // handle to tree view component

	String		Text;
	DWORD		Data;
	int			Level;   // 0 - parent
	CTreeViewItem *Parent;
    
public:
	CTreeViewItem() { };
	~CTreeViewItem() { };

	void			SetText(StringParam text);
	void			SetData(DWORD Data);

	String			GetText()	{ return Text; }

	int				GetLevel()	{ return Level; }
	DWORD			GetData()	{ return Data; }

	HTREEITEM		GetTreeItem()	{ return hTreeItem; }

	CTreeViewItem	*GetParent() { return Parent; }

	void			SetState(int state);
};






class CTreeView: public CComponent
{
protected:
	bool bSorted;
	
public:
	CTreeView(CDialog *Parent, UINT ID) { 
		this->ID = ID;
		hWnd = GetDlgItem(Parent->GetHWND(), ID); 
		this->Parent =  Parent;
		Parent->AddComponent(this);
		TreeView_SetUnicodeFormat(hWnd, TRUE);
		bSorted = true;
		ImgList = NULL;
	}

	CSmartList <CTreeViewItem> Items;
	HIMAGELIST ImgList;

	CTreeViewItem	*FindChild(CTreeViewItem* Parent, String Name);
	CTreeViewItem	*Add(CTreeViewItem *Parent, String Text, DWORD Param = 0);
	CTreeViewItem	*AddFirst(CTreeViewItem *Parent, String Text, DWORD Param = 0);
	bool			Delete(CTreeViewItem *);
	bool			DeleteSelected();
	void			Clear();

	CTreeViewItem	*TopSelect() { Items.First(); TreeView_SelectItem(hWnd, Items.GetItem().hTreeItem); return &Items.GetItem(); }
	CTreeViewItem	*GetSelected();
	bool SelectItem(CTreeViewItem* Item);
	void	Sort(); 

	void	SetSorted(bool Sorted) { bSorted = Sorted; }
	bool	GetSorted() { return bSorted; }

	void	SetStateImages(Strings &ImageStateNames);
};















class CListViewItem
{
friend class CListView;
protected:
	HWND hwndLV;
	int ItemIndex;	
	DWORD Data;
	String Caption;
	int SubItemsCount;
public:
	CListViewItem() { SubItemsCount = 0; }
	void	SetCaption(String Text) { Caption = Text; ListView_SetItemText(hwndLV, ItemIndex, 0, Text); }
	void	SetText(int SubIndex, String Text) { ListView_SetItemText(hwndLV, ItemIndex, SubIndex, Text); }
	String	GetText(int SubIndex)  { String Res; char tmp[100]; ListView_GetItemText(hwndLV, ItemIndex, SubIndex, tmp, 100); Res = tmp; return Res; }
	void	AddSubString(String Text) { ListView_SetItemText(hwndLV, ItemIndex, ++SubItemsCount, Text); }

	void	SetData(DWORD Param) { Data = Param; }
	DWORD	GetData() { return Data; }
	
	String GetCaption() { return Caption; }
	

};



class CListView: public CComponent
{
protected:
	int ItemsCount;
	int ColCount;
	bool GridShown;
	bool RowSelect;

	void MakeExStyle();

public:
	CListView(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); 
			this->Parent =  Parent; Parent->AddComponent(this);
			ItemsCount = 0;
			ColCount = 0;
	}

	CSmartList <CListViewItem> Items;

	void	ShowGrid(bool Show) { GridShown = Show; MakeExStyle();}
	void	EnableRowSelect(bool rowselect) { RowSelect = rowselect; MakeExStyle(); }	


	void	Clear();
	CListViewItem *AddItem(StringParam Caption, DWORD Param = 0);
	void	AddColumn(StringParam Caption, DWORD Width = 100);
	void	TopSelect() { ListView_SetSelectionMark(hWnd, 0); }
	CListViewItem *GetSelected();



	void	SetView(DWORD View); // LV_VIEW_DETAILS, LV_VIEW_ICON, LV_VIEW_LIST, ...
};


class CProgressBar: public CComponent
{
protected:
public:
	CProgressBar(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

	void	GetRange(int *Min, int *Max) 
	{ 
		*Min = (int)SendMessage(hWnd, PBM_GETRANGE, (WPARAM)TRUE, NULL); 
		*Max = (int)SendMessage(hWnd, PBM_GETRANGE, (WPARAM)FALSE, NULL); 
	}
	void	SetRange(int Min, int Max) { SendMessage(hWnd, PBM_SETRANGE, 0, MAKELPARAM(Min, Max)); }	
	void	SetPosition(int Position) { SendMessage(hWnd, PBM_SETPOS, (WPARAM)Position, 0); }
	int		GetPosition() { return (int)SendMessage(hWnd, PBM_GETPOS, 0, 0); }
};




class CSlider: public CComponent
{
protected:
public:
	CSlider(CDialog *Parent, UINT ID) { this->ID = ID; hWnd = GetDlgItem(Parent->GetHWND(), ID); this->Parent =  Parent; Parent->AddComponent(this);}

	void	GetRange(int *Min, int *Max) { SendMessage(hWnd, SBM_GETRANGE, (WPARAM)Min, (LPARAM)Max); }
	void	SetRange(int Min, int Max) { SendMessage(hWnd, SBM_SETRANGE, Min, Max); }	
	void	SetPosition(int Position) { SendMessage(hWnd, SBM_SETPOS, (WPARAM)Position, (LPARAM)TRUE); }
	int		GetPosition() { return (int)SendMessage(hWnd, SBM_GETPOS, 0, 0); }
};




// ===================================================================
// 
//
// ===================================================================
class CPopupMenu:public CComponent
{
protected:
	UINT	ID;
	HWND	hWnd;
	HMENU	hMenu;

public:
	CPopupMenu(CDialog *Parent, UINT ID);	

	void Popup(int x, int y);	
};