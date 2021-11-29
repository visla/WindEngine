/*
   File    : CHostGamePhase.h
   Autor   : Vuk Vranjkovic
   Date    : 27.3.2006.
   Version : 1.0
*/
#ifndef _CHostGamePhase_H
#define _CHostGamePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>

class WeFont;

class CHostGamePhase: public CAbstractPhase
{
public:
	static CHostGamePhase*	Instance;

						CHostGamePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();




private:
	WeFont			   *pFont;

	static bool			OnCancel(const CEGUI::EventArgs &);
};

#endif