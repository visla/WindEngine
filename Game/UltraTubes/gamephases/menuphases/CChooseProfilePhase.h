/*
   CChooseProfilePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CChooseProfilePhase_H
#define _CChooseProfilePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CProfile.h>

class CChooseProfilePhase: public CAbstractPhase
{
protected:
	bool Created;

public:
	static CChooseProfilePhase*	Instance;

								CChooseProfilePhase();

	void						InitWindows();
	void						Tick();
	void						Start();
	void						End();
	void						Render();

	CEGUI::Window*				ProfileWindow;
	CEGUI::MultiColumnList*		ProfileList;

	static bool					OnOk(const CEGUI::EventArgs& e);
	static bool					OnProfileListDblClick(const CEGUI::EventArgs& e);
	static bool					OnCreateProfile(const CEGUI::EventArgs& e);
	static bool					OnDeleteProfile(const CEGUI::EventArgs& e);
};






#endif