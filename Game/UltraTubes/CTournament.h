/*
    CTournamet.h

	autor: Vuk Vranjkovic, Nebojsa Vislavski
	Date: 16.3.2006.
	Version: 1.0

	data for tournament in the game
*/
#ifndef _CTournament_h_
#define _CTournament_h_

#include <CString.h>
#include <SmartList.h>
#include <CTournamentMapSettings.h>
#include <WeStream.h>


class CTournament
{
public:
	enum TOURNAMENTTYPE{TT_SINGLEPLAYER, TT_MULTIPLAYER};


	CTournament();
	int										Version;
	TOURNAMENTTYPE							Type;
	String									Name;
    CSmartList	<CTournamentMapSettings>	Maps;
	bool									Opened;


	bool									AddMapSettings(CTournamentMapSettings &ms);
	bool									RemoveMapSettings(StringParam Name);
	CTournamentMapSettings*					FindMapSettings(StringParam Name);

	void									Serialize(WeStream &);
	void									Deserialize(WeStream &);
};

#endif