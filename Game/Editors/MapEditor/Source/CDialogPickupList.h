/*
		File: CDialogPickupList.h
		Author: Tomislav Kukic
		Date: 24.03.2006
		Version: 1.0
*/




#ifndef _CDialogPickupList_h_
#define _CDialogPickupList_h_


#include <CDialog.h>


class CDialogPickupList:public CDialog
{
protected:
	void OnClick(DWORD ID);
	
public:
	CDialogPickupList();
	~CDialogPickupList(){}

	static CDialogPickupList* Instance;

	 bool Create(HWND hParent, bool Show = true);

	 CListBox* PickupList;
	 int Type;

	 void OnOk();
	 void OnCancel();
	 void OnShow();
};



#endif
