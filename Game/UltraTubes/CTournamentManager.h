/*
    CTournametManager.h

	autor: Vuk Vranjkovic
	Date: 16.3.2006.
	Version: 1.0

	controls tournaments in the game
*/
#ifndef _CTournament_Manager_h
#define _CTournament_Manager_h

#include <SmartList.h>
#include <CString.h>
#include <CTournament.h>

class CTournament;
class CProfile;

class CTournamentManager
{
private:
    CTournamentManager();

public:
    static CTournamentManager *Instance;
	~CTournamentManager();

	CSmartList<CTournament> Tournaments;

	void			Load();
	void			Save();

	//CTournament		*NewTournament(StringParam name);
	bool			AddTournament(CTournament &tournament);
	CTournament*	FindTournament(StringParam name);
	void			CopyToProfile(CProfile *p); // ovo ce da kopira default stanja turnira u profil

	bool			Remove(StringParam name);
};

#endif