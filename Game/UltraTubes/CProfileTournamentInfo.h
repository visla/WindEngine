/*
    CProfileTournamentInfo.h

	Autor: Nebojsa Vislavski
	Date: 17.3.2006.
	Version: 1.0.

    
*/

#ifndef _CTOURNAMENTPROFILEINFO_H
#define _CTOURNAMENTPROFILEINFO_H

#include <String.h>
#include <Constants.h>
#include <WeStream.h>

class CProfileTournamentInfo
{
protected:
public:
	int				Version;
	String			Name;				// ime turnira koje se igra
	Strings			MapsCleared;		// lista mapa koje su odigrane
	bool			TournamentCleared;	// da li je turnir ociscen

					CProfileTournamentInfo();
	void			Deserialize(WeStream &);
	void			Serialize(WeStream &);
};



#endif