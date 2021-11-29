/*
   CMultiPlayerPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CMultiPlayerPhase_H
#define _CMultiPlayerPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>

class CMultiPlayerPhase: public CAbstractPhase
{
public:
	static CMultiPlayerPhase*	Instance;

						CMultiPlayerPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool			OnInternetGame(const CEGUI::EventArgs &e);
    static bool			OnLANGame(const CEGUI::EventArgs &e);


protected:
	bool WNDFadeIn;
	float WNDAlpha;
};


#endif