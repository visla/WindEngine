/*
    CTournametMapSettings.h

	autor: Vuk Vranjkovic
	Date: 21.3.2006.
	Version: 1.0

	setting for tournament maps
*/
#ifndef _CTournamentMapSettings_h_
#define _CTournamentMapSettings_h_

#include <CString.h>
#include <SmartList.h>
#include <CGameSettings.h>
#include <CTournamentOpenCloseState.h>

class WeStream;

class CTournamentMapSettings
{
public:
	int							Version;
	String						StateName;
	String						MapName;  // ovo je ime mape bez ekstenzije
	bool						Opened;
	String						LoadingMusic;
	String						MapDescription;
	String						LoadingImage;
	String						LoadingText;
	String						MapDescriptionImage;
	RACETYPE					RaceType;    	
	RACEENDCONDITION			EndCondition;
	bool						RespawnPlayers;
	bool						Weapons;
	float						TimeForRace; // vreme za koje je potrebno preci trku- ako je 0 onda je beskonacno
	int							TrialsCount;
	int							MaxTrialsCount;
	bool						MapReverse;
	int							LapCount;
	int							Bonus1stCash;
	int							Bonus2ndCash;
	int							Bonus3rdCash;
	bool						MarksEndOfGame;
	String						EndGameImage;
	String						EndGameText;
	String						EndGameMusic;

	CSmartList <CTournamentOpenCloseState> ToOpen;
	CSmartList <CTournamentOpenCloseState> ToClose;
	CSmartList <CProfile*>				   AIProfiles;


								CTournamentMapSettings();
	void						Serialize(WeStream &);
	void						Deserialize(WeStream &);
	void						DoOpenClose(CProfile *p);  // ovo ce da namesti turnire za profile. Neke da otvori neke da zatvori
};

#endif