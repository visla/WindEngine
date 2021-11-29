/*
	CTournametMapSettings.cpp

	autor: Vuk Vranjkovic
	Date: 21.3.2006.
	Version: 1.0
*/
#include <CTournamentMapSettings.h>
#include <WeStream.h>
#include <CProfile.h>
#include <CTournament.h>

CTournamentMapSettings::CTournamentMapSettings()
{
	Version = 1;
	TrialsCount = 0;
	Opened = false;
	RaceType = RACETYPE_STANDARD;
	EndCondition = RACEENDCONDITION_123;
	RespawnPlayers = true;
	Weapons = true;
	TimeForRace = 0.0f;
	MaxTrialsCount = 0;
	MapReverse = true;
	LapCount = 3;
	Bonus1stCash = 1000;
	Bonus2ndCash = 500;
	Bonus3rdCash = 300;
	MarksEndOfGame = false;

	
}

void CTournamentMapSettings::Serialize(WeStream &S)
{
	S.WriteInt(Version);
	S.WriteString(StateName);
	S.WriteString(MapName);
	S.WriteBool(Opened);
	S.WriteString(LoadingMusic);
	S.WriteString(MapDescription);
	S.WriteString(MapDescriptionImage);
	S.WriteString(LoadingText);
	S.WriteString(LoadingImage);
	S.WriteInt(RaceType);
	S.WriteInt(EndCondition);
	S.WriteBool(RespawnPlayers);
	S.WriteBool(Weapons);
	S.WriteFloat(TimeForRace);
	S.WriteInt(TrialsCount);
	S.WriteInt(MaxTrialsCount);
	S.WriteBool(MapReverse);
	S.WriteInt(LapCount);
	S.WriteInt(Bonus1stCash);
	S.WriteInt(Bonus2ndCash);
	S.WriteInt(Bonus3rdCash);
	S.WriteBool(MarksEndOfGame);
	S.WriteString(EndGameImage);
	S.WriteString(EndGameMusic);
	S.WriteString(EndGameText);

	S.WriteInt(ToOpen.GetItemsCount());
	for (ToOpen.First(); !ToOpen.End(); ToOpen.Next())
	{
        ToOpen.GetItem().Serialize(S);
	}
	
	S.WriteInt(ToClose.GetItemsCount());
	for (ToClose.First(); !ToClose.End(); ToClose.Next())
	{
        ToClose.GetItem().Serialize(S);
	}

	S.WriteInt(AIProfiles.GetItemsCount());
	for (AIProfiles.First(); !AIProfiles.End(); AIProfiles.Next())
	{
		AIProfiles.GetItem()->Serialize(S);
	}
}

void CTournamentMapSettings::Deserialize(WeStream &S)
{
	Version = S.ReadInt();
	if(Version==1)
	{
		StateName = S.ReadString();
		MapName = S.ReadString();
		Opened = S.ReadBool();
		LoadingMusic = S.ReadString();
		MapDescription = S.ReadString();
		MapDescriptionImage = S.ReadString();
		LoadingText = S.ReadString();
		LoadingImage = S.ReadString();
		RaceType = (RACETYPE)S.ReadInt();
		EndCondition = (RACEENDCONDITION)S.ReadInt();
		RespawnPlayers = S.ReadBool();
		Weapons = S.ReadBool();
		TimeForRace = S.ReadFloat();
		TrialsCount = S.ReadInt();
		MaxTrialsCount = S.ReadInt();
		MapReverse = S.ReadBool();
		LapCount = S.ReadInt();
		Bonus1stCash = S.ReadInt();
		Bonus2ndCash = S.ReadInt();
		Bonus3rdCash = S.ReadInt();
		MarksEndOfGame  = S.ReadBool();
		EndGameImage = S.ReadString();
		EndGameMusic = S.ReadString();
		EndGameText = S.ReadString();

		int Count;
		Count = S.ReadInt();
		CTournamentOpenCloseState tocs;
		ToOpen.Clear();
		for (int i = 0; i < Count; i++)
		{
			tocs.Deserialize(S);
			ToOpen.PushBack(tocs);
		}
		
		Count = S.ReadInt();		
		ToClose.Clear();
		for (int i = 0; i < Count; i++)
		{
			tocs.Deserialize(S);
			ToClose.PushBack(tocs);		
		}
		
		Count = S.ReadInt();
		CProfile *P;
		AIProfiles.Clear();
		for (int i = 0; i < Count; i++)
		{
			P = new CProfile;
			P->Deserialize(S);
			AIProfiles.PushBack(P);
		}
		
	}
}


void CTournamentMapSettings::DoOpenClose(CProfile *p)
{
	if (!p) return;

	for (ToOpen.First(); !ToOpen.End(); ToOpen.Next())
	{
		for (p->Tournaments.First(); !p->Tournaments.End(); p->Tournaments.Next())
		{
			if (p->Tournaments.GetItem().Name == ToOpen.GetItem().TournamentName)
			{
				//MessageBox(0, p->Tournaments.GetItem().Name, 0, 0);
				p->Tournaments.GetItem().Opened = true;
				for (p->Tournaments.GetItem().Maps.First(); !p->Tournaments.GetItem().Maps.End(); p->Tournaments.GetItem().Maps.Next())
				{
					if (p->Tournaments.GetItem().Maps.GetItem().StateName == ToOpen.GetItem().StateName)
					{
						p->Tournaments.GetItem().Maps.GetItem().Opened = true;
					}
				}
			}
		}
	}

	for (ToClose.First(); !ToClose.End(); ToClose.Next())
	{
		for (p->Tournaments.First(); !p->Tournaments.End(); p->Tournaments.Next())
		{
			if (p->Tournaments.GetItem().Name == ToClose.GetItem().TournamentName)
			{
				for (p->Tournaments.GetItem().Maps.First(); !p->Tournaments.GetItem().Maps.End(); p->Tournaments.GetItem().Maps.Next())
				{
					if (p->Tournaments.GetItem().Maps.GetItem().StateName == ToClose.GetItem().StateName)
					{
						p->Tournaments.GetItem().Maps.GetItem().Opened = false;
					}
				}
			}
		}
	}
}