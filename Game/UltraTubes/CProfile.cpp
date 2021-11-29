/*
    Author: Vuk Vranjkovic

	File: CProfile.cpp
	Date: 15.03.2006.
	Version: 1.00
*/

#include <CProfile.h>
#include <WeNetworkMessage.h>
#include <CControllerSettings.h>


CProfile::CProfile()
{
	Version = 4;	
	Name = "Player";
	Score = 0;
	AIProfile = false;
	Tuber = NULL;
	CashCategory = 0;
	Cash = 0;
	TeamNumber = 1;
	TuberBougth = false;
	TournamentTuberBougth = false;
	ControllerSettings = new CControllerSettings();

}

CProfile::CProfile(const CProfile &P)
{
	Version = P.Version;
	Name = P.Name;
	Score = P.Score;
	AIProfile = P.AIProfile; 
	Cash = P.Cash;
	CashCategory = P.CashCategory;
	TeamNumber = P.TeamNumber;
	TuberBougth = P.TuberBougth;
	Tuber = P.Tuber;	
	Tournaments = P.Tournaments;   
	TuberDescription = P.TuberDescription; 
	SPTournamentTuberDescription = P.SPTournamentTuberDescription;	
	TournamentTuberBougth = P.TournamentTuberBougth;
	ControllerSettings = P.ControllerSettings->Clone();
}

CProfile::~CProfile()
{
	SAFE_DELETE(ControllerSettings);
}


void CProfile::Deserialize(WeStream &S)
{
	CTournament ti;

	Version = S.ReadInt();	
	if (Version == 3)
	{
		Name = S.ReadString();
		Score = S.ReadInt();
		TeamNumber = S.ReadInt();
		Cash = S.ReadInt();
		CashCategory = S.ReadInt();

		TournamentTuberBougth = S.ReadBool();
		TuberBougth = TournamentTuberBougth;

		SPTournamentTuberDescription.Deserialize(S);
		TuberDescription = SPTournamentTuberDescription;
		int Count;
		Count = S.ReadInt();
		for (int i = 0; i < Count; i++)
		{			
			ti.Deserialize(S);
			Tournaments.PushBack(ti);
		}	
	}

	else if (Version == 4)
	{
		Name = S.ReadString();
		Score = S.ReadInt();
		TeamNumber = S.ReadInt();
		Cash = S.ReadInt();
		CashCategory = S.ReadInt();

		TournamentTuberBougth = S.ReadBool();
		TuberBougth = TournamentTuberBougth;

		SPTournamentTuberDescription.Deserialize(S);
		TuberDescription = SPTournamentTuberDescription;
		int Count;
		Count = S.ReadInt();
		for (int i = 0; i < Count; i++)
		{			
			ti.Deserialize(S);
			Tournaments.PushBack(ti);
		}
		ControllerSettings->Deserialize(S);
	}

}

void CProfile::Serialize(WeStream &S)
{
	S.WriteInt(4);
	S.WriteString(Name);
	S.WriteInt(Score);
	S.WriteInt(TeamNumber);
	S.WriteInt(Cash);
	S.WriteInt(CashCategory);
	S.WriteBool(TuberBougth);


	TuberDescription.Serialize(S);
	S.WriteInt(Tournaments.GetItemsCount());
	for (Tournaments.First(); !Tournaments.End(); Tournaments.Next())
	{
		Tournaments.GetItem().Serialize(S);
	}
	ControllerSettings->Serialize(S);
}


void CProfile::InitForMapType(CGameSettings *Settings)
{
	if (Settings->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
	{
		TuberDescription = SPTournamentTuberDescription;
		TuberBougth = TournamentTuberBougth;
	} else
	{
		Cash = Settings->StartingMoney;
	}
}

void CProfile::NetSerialize(WeNetworkMessage &Msg)
{
	Msg.WriteString(Name);
	Msg.WriteInt(Score);
	Msg.WriteInt(TeamNumber);
	Msg.WriteInt(Cash);
	Msg.WriteInt(CashCategory);
	Msg.WriteBool(TuberBougth);
	Msg.WriteBool(AIProfile);
	TuberDescription.Serialize(Msg);
}

void CProfile::NetDeserialize(WeNetworkMessage &Msg)
{
	Name = Msg.ReadString();
	Score = Msg.ReadInt();
	TeamNumber = Msg.ReadInt();
	Cash = Msg.ReadInt();
	CashCategory = Msg.ReadInt();
	TuberBougth = Msg.ReadBool();
	AIProfile = Msg.ReadBool();
	TuberDescription.Deserialize(Msg);
}
