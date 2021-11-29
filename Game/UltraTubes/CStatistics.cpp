/*
     CStatistics.cpp

	 Autor: Nebojsa Vislavski
	 Date: 1.6.2006.
	 Version: 1.0

*/


#include <CStatistics.h>
#include <CProfile.h>
#include <CTuber.h>

CStatistics *CStatistics::Instance = new CStatistics;


void CStatistics::Init(CGameSettings &Settings)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Table[i].Valid = false;
		Table[i].Position = -1;

		if (Settings.tmpPlayers[i].User)
		{
			Table[i].Valid = true;
			Table[i].Winner = false;
			Table[i].PlayerName = Settings.tmpPlayers[i].User->Name;
			Table[i].MyProfile = false;
			if (Settings.tmpPlayers[i].User->Tuber)
			{
				Table[i].TuberName = Settings.tmpPlayers[i].User->Tuber->TuberName;
				Table[i].Position = Settings.tmpPlayers[i].User->Tuber->Position;
				Table[i].TeamNumber = Settings.tmpPlayers[i].User->TeamNumber;
				Table[i].Score  = Settings.tmpPlayers[i].User->Tuber->Score;
				Table[i].Kills = Settings.tmpPlayers[i].User->Tuber->Kills;
				Table[i].RaceTime = Settings.tmpPlayers[i].User->Tuber->RaceTime;
				Table[i].FastestLap = Settings.tmpPlayers[i].User->Tuber->BestLapTime;
				Table[i].Cash = 0;
				if (Settings.tmpPlayers[i].MyProfile) Table[i].MyProfile = true;
			}
		}
	}
}

void CStatistics::Update(CGameSettings &Settings)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (Settings.tmpPlayers[i].User)
		{
			Table[i].PlayerName = Settings.tmpPlayers[i].User->Name;
			if (Settings.tmpPlayers[i].User->Tuber)
			{				
				Table[i].TuberName = Settings.tmpPlayers[i].User->Tuber->TuberName;
				Table[i].Position = Settings.tmpPlayers[i].User->Tuber->Position;
				Table[i].TeamNumber = Settings.tmpPlayers[i].User->TeamNumber;
				Table[i].Score  = Settings.tmpPlayers[i].User->Tuber->Score;
				Table[i].Kills = Settings.tmpPlayers[i].User->Tuber->Kills;
				Table[i].RaceTime = Settings.tmpPlayers[i].User->Tuber->RaceTime;
				Table[i].FastestLap = Settings.tmpPlayers[i].User->Tuber->BestLapTime;
				Table[i].Winner = Settings.tmpPlayers[i].User->Tuber->Win;
				Table[i].Cash = Settings.tmpPlayers[i].User->Tuber->CashCollected;
				if (Settings.tmpPlayers[i].MyProfile) Table[i].MyProfile = true;				
			}
		}
	}
}

void CStatistics::Sort()
{
	// sorting will be according to position
	SStatisticsTable tmp;

	for (int j = MAX_PLAYERS-1; j > 1; j--)
	{
		for (int i = 0; i < j-1; i++)
		{
			if (Table[i].Position < Table[i+1].Position)
			{
				tmp = Table[i];
				Table[i] = Table[i+1];
				Table[i+1] = tmp;
			}
		}
	}
}