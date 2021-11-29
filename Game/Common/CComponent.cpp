/*
   CComponent.cpp

   Autor: Nebojsa Vislavski
   Date: 25.5.2005.
   Version: 1.0
 

*/

#include <CComponent.h>
//#include <CEditorResourceManager.h>


CComponent::CComponent()
{
}


void CComboBox::SetSelection(int index)
{	
	SendMessage(hWnd, CB_SETCURSEL, index, 0);
}

void CComboBox::SetSelection(StringParam s)
{
	String tmp; 
	tmp = s;
	SendMessage(hWnd, CB_SELECTSTRING, -1, (LPARAM)(LPCSTR)tmp);
}


void CComboBox::Clear()
{
	SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
}

void CComboBox::AddItem(StringParam Text, DWORD Data)
{
	int i;
	
	String tmp;
	tmp = Text;
	i = (int)SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)tmp);
	SendMessage(hWnd, CB_SETITEMDATA, i, Data);
}

void CComboBox::GetData(String &outS, DWORD &outData)
{
	int i;
	char tmp[1000];
	i = (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (i == CB_ERR) return;
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	outS = tmp;
	outData = (DWORD)SendMessage(hWnd, CB_GETITEMDATA, i, 0);
}

DWORD CComboBox::GetSelectionData()
{
	int i;
	i = (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (i == CB_ERR) return 0;		
	return (DWORD)SendMessage(hWnd, CB_GETITEMDATA, i, 0);
}


String CComboBox::GetSelectionText()
{
	int i;
	String tmpS;
	char tmp[1000];
	i = (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (i == CB_ERR) return tmpS;
//	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	SendMessage(hWnd, CB_GETLBTEXT, i, (LPARAM)tmp);
	tmpS  = tmp;
	return tmpS;
}


String CComboBox::GetText()
{
	String tmpS;
	char tmp[1000];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	tmpS  = tmp;
	return tmpS;
}


void CComboBox::SetText(StringParam Text) 
{
	String tmp;
	tmp = Text;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(LPCSTR)tmp);
}

int CComboBox::GetSelectionIndex()
{
	return (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0);
}



void CLabel::SetText(StringParam Text) 
{
	String tmp;
	tmp = Text;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(LPCSTR)tmp);
}

String CLabel::GetText()
{
	String res;
	char tmp[1000];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	res = tmp;
	return res;
}



void CButton::SetText(StringParam Text)
{
	String tmp;
	tmp = Text;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(LPCSTR)tmp);
}

String CButton::GetText()
{
	String res;
	char tmp[1000];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	res = tmp;
	return res;
}



void CEditBox::Clear()
{
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)L"");
}

void CEditBox::SetText(StringParam tmps)
{
	String tmp;
	tmp = tmps;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(LPCSTR)tmp);
}

String CEditBox::GetText()
{
	String res;
    char tmp[10000];
	SendMessage(hWnd, WM_GETTEXT, 10000, (LPARAM)tmp);
	res = tmp;
	return res;
}

void CEditBox::SetText(int number)
{
    char tmp[100];
	sprintf(tmp, "%d", number);
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)tmp);	
}

void CEditBox::SetText(float number)
{
    char tmp[100];
	sprintf(tmp, "%5.6f", number);
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)tmp);	
}

int CEditBox::GetTextAsInt()
{
	String res;
    char tmp[100];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	res = tmp;
	return res.AsInt();
}



float CEditBox::GetTextAsFloat()
{
	String res;
    char tmp[1000];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	res = tmp;
	return res.AsFloat();
}



bool CCheckBox::GetChecked()
{
	UINT i;
	i = (UINT)SendMessage(hWnd, BM_GETCHECK, 0, 0);
	if (i == BST_CHECKED) return true;
	return false;
}

void CCheckBox::SetChecked(bool c)
{
	if (c) SendMessage(hWnd, BM_SETCHECK, BST_CHECKED, 0);
	else  SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
}



int CScrollBar::GetPosition()
{
	return (int)SendMessage(hWnd, SBM_GETPOS, 0, 0);
}

void CScrollBar::SetPosition(int pos)
{
	SendMessage(hWnd, SBM_SETPOS, pos, TRUE);
}

void CScrollBar::SetRange(int min, int max)
{
	SendMessage(hWnd, SBM_SETRANGE, min, max);
}













String CListBox::GetText()
{
	String tmpS;
	char tmp[1000];
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	tmpS  = tmp;
	return tmpS;
}




void CListBox::SetSelection(int index)
{	
	SendMessage(hWnd, LB_SETCURSEL, index, 0);
}

void CListBox::SetSelection(StringParam s)
{
	String tmp; 
	tmp = s;
	SendMessage(hWnd, LB_SELECTSTRING, -1, (LPARAM)(LPCSTR)tmp);
}


void CListBox::Clear()
{
	SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
}

void CListBox::AddItem(StringParam Text, DWORD Data)
{
	int i;
	
	String tmp;
	tmp = Text;
	i = (int)SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)tmp);
	SendMessage(hWnd, LB_SETITEMDATA, i, Data);
}

void CListBox::GetData(String &outS, DWORD &outData)
{
	int i;
	char tmp[1000];
	i = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	if (i == CB_ERR) return;
	SendMessage(hWnd, WM_GETTEXT, 100, (LPARAM)tmp);
	outS = tmp;
	outData = (DWORD)SendMessage(hWnd, LB_GETITEMDATA, i, 0);
}

DWORD CListBox::GetSelectionData()
{
	int i;
	i = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	if (i == LB_ERR) return 0;		
	return (DWORD)SendMessage(hWnd, LB_GETITEMDATA, i, 0);
}


String CListBox::GetSelectionText()
{
	int i;
	String tmpS;
	char tmp[1000];
	i = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	if (i == LB_ERR) return tmpS;
	SendMessage(hWnd, LB_GETTEXT, i, (LPARAM)tmp);
	tmpS  = tmp;
	return tmpS;
}

int CListBox::GetSelectionIndex()
{
	return (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
}

void CListBox::RemoveItem(StringParam s)
{
	String tmp;
	tmp = s;
	SendMessage(hWnd, LB_DELETESTRING, (WPARAM)(LPCSTR)tmp, 0);
}

void CListBox::RemoveItem(int i)
{
	SetSelection(i);	
	SendMessage(hWnd, LB_DELETESTRING, (WPARAM)i, 0);
}

int CListBox::GetItemCount()
{	
	return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
}



CTreeViewItem* CTreeView::FindChild(CTreeViewItem* Parent, String Name)
{
	CTreeViewItem* Item;

	for (Items.First(); !Items.End(); Items.Next())
	{
		Item = &Items.GetItem();
		if ((Parent == Item->GetParent()) && (Name == Item->GetText()))
			return Item;
	}

	return NULL;
}


CTreeViewItem *CTreeView::Add(CTreeViewItem *Parent, String Text, DWORD Param)
{
	CTreeViewItem Item;
	TVINSERTSTRUCT is;
	HTREEITEM handle;

	if (!Parent)	
	{
		is.hParent =  NULL;
		Item.Level = 0;
	}
	else 
	{
		is.hParent = Parent->hTreeItem;
		Item.Level = Parent->Level+1;
	}

	if (bSorted) is.hInsertAfter = TVI_SORT;
	else is.hInsertAfter = TVI_LAST;

	is.hInsertAfter = TVI_SORT;

	is.itemex.mask = TVIF_TEXT | TVIF_PARAM;
	is.itemex.pszText = strdup(Text.GetString());
	is.itemex.lParam = Param;
	
	handle = TreeView_InsertItem(hWnd, &is);
	if (!handle) return NULL;

	Item.hTreeItem = handle;
	Item.Text = Text;
	Item.Data = Param;	
	Item.tvItemEx = is.itemex;
	Item.Parent = Parent;
	Item.hwndTV = hWnd;

	Items.PushBack(Item);
	return &Items.GetItem();
}


void CTreeView::Clear()
{
	Items.Clear();
	TreeView_DeleteAllItems(hWnd);
}




CTreeViewItem *CTreeView::GetSelected()
{
    HTREEITEM handle;
	
	handle = TreeView_GetSelection(hWnd);
	for (Items.First(); !Items.End(); Items.Next())
	{
		if (handle == Items.GetItem().hTreeItem) return &Items.GetItem();
	}   
	return NULL;
	
}


void CTreeView::Sort()
{
	Items.First();
	TreeView_SortChildren(hWnd, Items.GetItem().hTreeItem, TRUE);
}


bool CTreeView::SelectItem(CTreeViewItem* Item)
{
	//return (TreeView_Select(hWnd, Item->GetTreeItem(), TVGN_CARET | TVGN_DROPHILITE | TVGN_FIRSTVISIBLE) != FALSE);
	return (TreeView_SelectItem(hWnd, Item->GetTreeItem()) != FALSE);
}


void CTreeView::SetStateImages(Strings &ImageStateNames)
{
	if (ImgList)
	{
		ImageList_Destroy(ImgList);
	}

	ImgList = ImageList_Create(8, 8, ILC_COLOR24, 4, 1);
	for (ImageStateNames.First(); !ImageStateNames.End(); ImageStateNames.Next())
	{
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(0, ImageStateNames.GetItem().GetString(), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_VGACOLOR | LR_CREATEDIBSECTION);
		ImageList_Add(ImgList, bmp, NULL);
	}
	TreeView_SetImageList(hWnd, ImgList, TVSIL_STATE);
}


void CTreeViewItem::SetText(StringParam text)
{
	Text = text;
	tvItemEx.pszText = Text; 	
	tvItemEx.hItem = hTreeItem;
	TreeView_SetItem(hwndTV, &tvItemEx); 
}


void CTreeViewItem::SetData(DWORD Data)
{
	this->Data = Data;
	tvItemEx.lParam = Data;
	tvItemEx.hItem = hTreeItem;
	TreeView_SetItem(hwndTV, &tvItemEx); 
}


void CTreeViewItem::SetState(int state)
{
	tvItemEx.mask |= LVIF_STATE;
	tvItemEx.state = (state & 0xF) << 12;
	tvItemEx.stateMask = LVIS_STATEIMAGEMASK;
	tvItemEx.hItem = hTreeItem;
	TreeView_SetItem(hwndTV, &tvItemEx); 
}


bool CTreeView::Delete(CTreeViewItem *Item)
{
	if (!Item) return false;

	HTREEITEM hTreeItem = Item->hTreeItem;

	for (Items.First(); !Items.End(); Items.Next())
	{
		if (Item == &Items.GetItem())
		{
			Items.Delete();
			break;
		}
	}

	return (TreeView_DeleteItem(hWnd, hTreeItem) != FALSE);
}

bool CTreeView::DeleteSelected()
{
	return Delete(GetSelected());
}






void CListView::Clear()
{
	Items.Clear();
	ListView_DeleteAllItems(hWnd);
}

void CListView::SetView(DWORD View)
{
	//ListView_SetView(hWnd, View);
}

CListViewItem *CListView::AddItem(StringParam Caption, DWORD Param)
{
	String tmp;
	CListViewItem Item;
	Item.SetCaption(Caption);
	Item.SetData(Param);
	
	tmp = Caption;
	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_PARAM;
	lvitem.iSubItem = 0;
	lvitem.lParam = Param;
	lvitem.iItem = ItemsCount;
	lvitem.pszText = tmp;
	Item.ItemIndex = ListView_InsertItem(hWnd, &lvitem);
	ItemsCount++;

	Item.hwndLV = hWnd;	
	Items.PushBack(Item);
	return &(Items.GetItem());
}

void CListView::AddColumn(StringParam Caption, DWORD Width)
{
	String tmp;
	tmp = Caption;
	LVCOLUMN lvcolumn;
	lvcolumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.pszText = tmp;
	lvcolumn.cx = Width;
	ListView_InsertColumn(hWnd, ColCount++, &lvcolumn);
}

void CListView::MakeExStyle()
{
	DWORD Style = 0;
	if (GridShown) Style = Style | LVS_EX_GRIDLINES;
	if (RowSelect) Style = Style | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(hWnd, Style);

}


CListViewItem *CListView::GetSelected()
{    
	int i;
	i = ListView_GetSelectionMark(hWnd);
	if (i == -1) return NULL;

	return &(Items[i]);
}





// ===============================================================
// 
//
//
// ===============================================================
CPopupMenu::CPopupMenu(CDialog *Parent, UINT ID)
{
 	/*this->ID = ID;
	if (Parent) Parent->AddComponent(this);
	hMenu = LoadMenu(EditorResourceManager.GetResourceInstance(), (LPWSTR)ID);
	hMenu = GetSubMenu(hMenu, 0);
	hWnd = Parent->GetHWND();*/
}


// ===============================================================
// 
//
//
// ===============================================================
void CPopupMenu::Popup(int x, int y)
{
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, x, y, 0, hWnd, NULL);		
}


