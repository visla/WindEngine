/*
    Author: Vuk Vranjkovic

	File: CProfile.h
	Date: 15.03.2006.
	Version: 1.00
*/

#ifndef _CProfile_h_
#define _CProfile_h_

#include <CString.h>
#include <CProfileTournamentInfo.h>
#include <CTuberDescription.h>
#include <SmartList.h>
#include <WeStream.h>
#include <CTournament.h>
#include <CGameSettings.h>

class CTuber;
class WeNetworkMessage;
class CControllerSettings;

class CProfile
{
public:
	int									Version;
    String								Name;
	int									Score;
	//TODO: something with this bool AIProfile
	bool								AIProfile;  // da li je ovo AI. Ovi se ne snimaju u profiles.cfg	
	int									Cash;
	int									CashCategory;
	int									TeamNumber;
	

	CTuber*								Tuber;	
	CSmartList <CTournament>			Tournaments;   // podaci o turnirima koji postoje. Ovo su svi podaci
	CTuberDescription					TuberDescription; // podaci o vozilu koje koristi igrac
	CTuberDescription					SPTournamentTuberDescription; // podaci o vozilu koje koristi igrac za vreme sp turnira
	bool								TuberBougth;     // da li imamo kupljen tuber tj. da li je tuber description validan
	bool								TournamentTuberBougth; // da li je kupljen tuber u turniru
	CControllerSettings*				ControllerSettings;

										CProfile();
										CProfile(const CProfile &);
										~CProfile();

	void								Deserialize(WeStream &);
	void								Serialize(WeStream &);

	void								NetDeserialize(WeNetworkMessage &);
	void								NetSerialize(WeNetworkMessage &);

	void								InitForMapType(CGameSettings *Settings);
};

#endif