/*
     CStatistics.h

	 Autor: Nebojsa Vislavski
	 Date: 1.6.2006.
	 Version: 1.0

*/

#ifndef _CSTATISTICS_H
#define _CSTATISTICS_H

#include <CString.h>
#include <CGameSettings.h>

class CStatistics
{
protected:
	struct	SStatisticsTable
	{
		String			PlayerName;
		String			TuberName;
		int				Position;
		int				Kills;
		int				Score;
		int				TeamNumber;
		int				Cash;
		float			FastestLap;
		float			RaceTime;
		bool			Winner;
		bool			Valid;
		bool			MyProfile;
	};

public:
	static CStatistics *Instance;
	SStatisticsTable	Table[MAX_PLAYERS];


	void				Init(CGameSettings &Settings);
	void				Update(CGameSettings &Settings);	
	void				Sort();
};



#endif
