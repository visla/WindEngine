/*
   CCreateProfilePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CCreateProfilePhase_H
#define _CCreateProfilePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>

class CCreateProfilePhase: public CAbstractPhase
{
protected:
	bool Created;
	bool Fade;

public:
	static CCreateProfilePhase*	Instance;

						CCreateProfilePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	String				ProfileName;

	static bool			OnOk(const CEGUI::EventArgs& e);
	static bool			OnCancel(const CEGUI::EventArgs& e);
	static bool			OnKeyPress(const CEGUI::EventArgs& e);
};


#endif