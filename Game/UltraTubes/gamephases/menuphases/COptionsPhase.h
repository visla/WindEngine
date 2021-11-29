/*
   COptionsPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _COptionsPhase_H
#define _COptionsPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>

class COptionsPhase: public CAbstractPhase
{
protected:
	bool WNDFadeIn;
	float WNDAlpha;

public:
	static COptionsPhase*	Instance;

	bool FromInGame;

						COptionsPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool			OnAudio(const CEGUI::EventArgs& e);
	static bool			OnVideo(const CEGUI::EventArgs& e);
	static bool			OnControls(const CEGUI::EventArgs& e);

};


#endif