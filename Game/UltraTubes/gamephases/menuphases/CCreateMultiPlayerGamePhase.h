/*
   CCreateMultiplayerGamePhase.h

   Autor: Vuk Vranjkovic
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CCreateMultiplayerGamePhase_H
#define _CCreateMultiplayerGamePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>

class CGamePlayerConfig;
class MyListItem;

class CCreateMultiplayerGamePhase: public CAbstractPhase
{
public:
	static CCreateMultiplayerGamePhase*	Instance;

						CCreateMultiplayerGamePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();


private:
	bool				bAdmin;

	void				CreateRootComboboxes();
	void				MakeTableFromSettings();
	static bool			OnTeamChange(const CEGUI::EventArgs &e);
	static bool			OnUserChange(const CEGUI::EventArgs &e);


	// GUI callbacks
	static bool			OnCancel(const CEGUI::EventArgs &);
	static bool			OnStart(const CEGUI::EventArgs &);

};


#endif