/*
    CGameSettings.h

	autor: Vuk Vranjkovic
	Date: 14.3.2006.
	Version: 1.0

	Settings for the game
*/

#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_

#include <CString.h>
#include <CGamePlayerConfig.h>
#include <SmartList.h>
#include <WeColor.h>
#include <constants.h>

class WeStream;
class CTournament;
class CTournamentMapSettings;

enum RACETYPE
{
	RACETYPE_STANDARD,
	RACETYPE_TIMETRIAL,
	RACETYPE_LASTOUT
};


enum RACEENDCONDITION
{
	RACEENDCONDITION_1,
	RACEENDCONDITION_12,
	RACEENDCONDITION_123,
	RACEENDCONDITION_2,
	RACEENDCONDITION_23,
	RACEENDCONDITION_3,
	RACEENDCONDITION_TIME
};

class CGameSettings
{
protected:
	bool						Crates;
	bool						Weapons;
	bool						CatchUp;
	bool						UnlimitedSpeed;


public:
	static CGameSettings *Instance;
	static CGameSettings *ClientInstance;
	static CGameSettings *ServerInstance;


	enum MAPTYPE
	{
	MAPTYPE_SP_TOURNAMENT, MAPTYPE_MP_SINGLERACE, 
	MAPTYPE_SP_SINGLERACE, MAPTYPE_SP_INSTANTRACE, MAPTYPE_SP_TIMETRIAL,
	MAPTYPE_MP_TOURNAMENT, MAPTYPE_SP_LASTONE, MAPTYPE_MP_LASTONE, MAPTYPE_SP_DEATHMATCH, MAPTYPE_MP_DEATHMATCH
	};

	static WeColor				TeamColors[MAX_PLAYERS];
	static int					CashCategories[MAX_CASH_CATEGORIES];
	static int					LapCountPresets[MAX_LAP_PRESETS];



	int							StartingMoney;
	int							LapCount;
	String						MapName;
	MAPTYPE						MapType;
	
	CGamePlayerConfig			tmpPlayers[MAX_PLAYERS];

	CSmartList<CGamePlayerConfig> 	Players;
	CTournamentMapSettings		*MapSettings;
	bool						NeedPasswordLogin;	
	String						Password;


	bool						Funny;   // ovo se ukljucuje tokom trke i ne ide u serialize i deserialize

    void						Initialize();
	void						ClearPlayers();	
	int							GetPlayersCount();
	bool						GetPlayersFull();
	int							GetFirstEmptyPlayer();

	void						InitProfiles();
	bool						AddPlayer(CGamePlayerConfig *);
	bool						RemovePlayer(CGamePlayerConfig *);
	bool						RemovePlayer(int UID);
	CGamePlayerConfig		   *GetPlayer(int UID);
	unsigned char				getPlayerCount();

	void						Serialize(WeStream &);
	void						Deserialize(WeStream &);

	int							AdminID;

	void						SetMaxPlayers(unsigned u) {if(u) if (u<17) MaxPlayers=u;}
	unsigned					GetMaxPlayers() const {return MaxPlayers;}

	bool						GetWeapons();
	void						SetWeapons(bool WeaponsSettingBooleanVariableWithLongName);

	bool						GetCrates();
	void						SetCrates(bool CratesSettingBooleanVariableWithLongName);

	bool						GetUnlimitedSpeed();
	void						SetUnlimitedSpeed(bool UnlimitedSpeedSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense);

	bool						GetCatchUp();
	void						SetCatchUp(bool CatchUpSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense);

protected:
						CGameSettings();
	unsigned			MaxPlayers;
};





inline unsigned char	CGameSettings::getPlayerCount() {return Players.GetItemsCount();} 


#endif