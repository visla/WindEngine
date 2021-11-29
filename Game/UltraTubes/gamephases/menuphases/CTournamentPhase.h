/*
   CTournamentPhase.h

   Autor: Vuk Vranjkovic
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CTournamentPhase_H
#define _CTournamentPhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <SmartList.h>
#include <CString.h>

class CTournament;
class WeTexture;

class CTournamentPhase: public CAbstractPhase
{
public:
	static CTournamentPhase*	Instance;

						CTournamentPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	bool				GameStarted;
	bool				CheckMaps(CTournament* chkTournament);

	WeTexture*			MapDescImage;
protected:

	//bool BackEnters;
	//bool BackLeaves;
	//bool TEnters;
	//bool TLeaves;

	float WOffset;
	float HOffset;

	CSmartList<WeTexture*> MapImagesList;
	CSmartList<String>	MapNames;
	CSmartList<String>	RaceDesc;
	WeTexture*			ScrollerPictures[5];

	int					SelectedMapIndex;
	int					IndexChange;
	void				UpdateScroller();

    static bool			OnBack(const CEGUI::EventArgs &e);
	static bool			OnTournament(const CEGUI::EventArgs &e);
	static bool			OnTournamentChange(const CEGUI::EventArgs &e);

	bool				OnLButtonMouseMove(const CEGUI::EventArgs &e);
	bool				OnLButtonMouseLeave(const CEGUI::EventArgs &e);
	bool				OnMoveLeft(const CEGUI::EventArgs &e);

	bool				OnRButtonMouseMove(const CEGUI::EventArgs &e);
	bool				OnRButtonMouseLeave(const CEGUI::EventArgs &e);
	bool				OnMoveRight(const CEGUI::EventArgs &e);

	bool				OnScrollerClick(const CEGUI::EventArgs &e);

	//bool				OnBackEnters(const CEGUI::EventArgs &e);
	//bool				OnBackLeaves(const CEGUI::EventArgs &e);
	//bool				OnTEnters(const CEGUI::EventArgs &e);
	//bool				OnTLeaves(const CEGUI::EventArgs &e);
};


#endif