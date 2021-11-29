/*
   CMainMenuPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CMainMenuPhase_H
#define _CMainMenuPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>

class CMainMenuPhase: public CAbstractPhase
{
public:
	static CMainMenuPhase*	Instance;

						CMainMenuPhase();
						~CMainMenuPhase();

	bool SomethingChanged;
	CEGUI::Window *MustRestartText;

	void				Tick();
	void				Start();
	void				End();
	void				Render();

protected:
	bool WNDFadeIn;
	float WNDAlpha;

	CEGUI::FrameWindow	*MainMenuWnd;

	CAbstractPhase		*SingleMultiPhase;


	// Events
	static bool OnSinglePlayer(const CEGUI::EventArgs& e);
	static bool OnMultiPlayer(const CEGUI::EventArgs& e);
	static bool OnOptions(const CEGUI::EventArgs& e);
	static bool OnProfiles(const CEGUI::EventArgs& e);
	static bool OnHighScore(const CEGUI::EventArgs& e);
	static bool OnCredits(const CEGUI::EventArgs& e);
	static bool OnExit(const CEGUI::EventArgs& e);
};


#endif