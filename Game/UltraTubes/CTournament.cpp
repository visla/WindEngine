/*
    CTournament.cpp

	autor: Vuk Vranjkovic
	Date: 16.3.2006.
	Version: 1.0
*/
#include <CTournament.h>
#include <WeStream.h>

CTournament::CTournament()
{
	Version = 1;
	Type = TT_SINGLEPLAYER;
	Opened = false;
}

void CTournament::Serialize(WeStream &S)
{
	S.WriteInt(Version);
	S.WriteInt(Type);
	S.WriteString(Name);
	S.WriteInt(Maps.GetItemsCount());
	for(Maps.First(); !Maps.End(); Maps.Next())
	{
		Maps.GetItem().Serialize(S);
	}
	S.WriteBool(Opened);
}


bool CTournament::AddMapSettings(CTournamentMapSettings &ms)
{
	for (Maps.First(); !Maps.End(); Maps.Next())
	{
		if (Maps.GetItem().StateName == ms.StateName)
		{
			return false;
		}
	}
	Maps.PushBack(ms);
	return true;
}



bool CTournament::RemoveMapSettings(StringParam Name)
{
	for (Maps.First(); !Maps.End(); Maps.Next())
	{
		if (Maps.GetItem().StateName == Name)
		{
			Maps.Delete();
			return true;
		}
	}
	return false;
}


CTournamentMapSettings *CTournament::FindMapSettings(StringParam Name)
{
	for (Maps.First(); !Maps.End(); Maps.Next())
	{
		if (Maps.GetItem().StateName == Name)
		{						
			return &Maps.GetItem();
		}
	}
	return NULL;
}

void CTournament::Deserialize(WeStream &S)
{
	Version=S.ReadInt();
	if(Version==1)
	{
		Type = (TOURNAMENTTYPE) S.ReadInt();
		Name = S.ReadString();
		int c;
		c = S.ReadInt();
		CTournamentMapSettings mp;
		Maps.Clear();
		for(int i = 0; i < c; i++)
		{
			mp.Deserialize(S);
			Maps.PushBack(mp);			
		}
		Opened =  S.ReadBool();
	}
}