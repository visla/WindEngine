/*
   CSinglePlayerPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/

#ifndef _CSinglePlayerPhase_H
#define _CSinglePlayerPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>

class CSinglePlayerPhase: public CAbstractPhase
{
public:
	static CSinglePlayerPhase*	Instance;

						CSinglePlayerPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();


protected:
	bool WNDFadeIn;
	float WNDAlpha;

	// Buttons
	CEGUI::Window* SingleRaceButton;
	CEGUI::Window* TournamentButton;
	CEGUI::Window* InstantRaceButton;

	// Button titles
	CEGUI::Window* SingleRaceTitle;
	CEGUI::Window* TournamentTitle;
	CEGUI::Window* InstantTitle;

	// Window
	CEGUI::FrameWindow* SinglePlayerWnd;

	// Window background
	CEGUI::Window* SinglePlayerBackground;

	// Windows title
	CEGUI::Window* SinglePlayerWindowTitle;

	static bool OnSingleRace(const CEGUI::EventArgs& e);
	static bool OnInstantRace(const CEGUI::EventArgs& e);
	static bool OnTournament(const CEGUI::EventArgs& e);

};


#endif