/*
      File: CDemoWindow.h
	  Author: Tomislav Kukic
	  Date: 12.01.2006.
	  Version: 1.0
*/



#ifndef _CUTWINDOW_H
#define _CUTWINDOW_H


#include <CEditorWindow.h>


class CUTWindow : public CEditorWindow
{
protected:


public:

	static CUTWindow *Instance;


	void	OnChar(DWORD c);
	void	OnGraphNotify();
	void	OnDblClick(DWORD ID);

	CUTWindow();
	~CUTWindow();
	
};


#endif
