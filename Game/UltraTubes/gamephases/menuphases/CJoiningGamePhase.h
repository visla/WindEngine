/*
    CJoiningGamePhase.h

	Autor: Vuk Vranjkovic
	Date: 17.3.2006.
	Version: 1.0
    
*/
#ifndef _CJoiningGamePhase_H
#define _CJoiningGamePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>

class WeFont;

class CJoiningGamePhase: public CAbstractPhase
{
public:
	static CJoiningGamePhase*	Instance;

						CJoiningGamePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

private:
	WeFont			   *pFont;

	static bool			OnCancel(const CEGUI::EventArgs &);

	float				Time;
};

#endif
