/*
   CProfileTournamentInfo.cpp

   autor: nebojsa Vislavski
   Date: 17.3.2006.
   Version: 1.0

   
*/

#include <CProfileTournamentInfo.h>

CProfileTournamentInfo::CProfileTournamentInfo()
{
	Version = 1;
	TournamentCleared = false;	
}

void CProfileTournamentInfo::Serialize(WeStream &S)
{
	S.WriteInt(Version);
	S.WriteBool(TournamentCleared);
	S.WriteString(Name);
	S.WriteInt(MapsCleared.GetItemsCount());
	for (MapsCleared.First(); !MapsCleared.End(); MapsCleared.Next())
	{
		S.WriteString(MapsCleared.GetItem());
	}
}

void CProfileTournamentInfo::Deserialize(WeStream &S)
{
	Version = S.ReadInt();
	if (Version == 1)
	{
		TournamentCleared = S.ReadBool();
		Name = S.ReadString();
		int count;
		String s;
		count = S.ReadInt();
		for (int i = 0; i < count; i++)
		{
			s = S.ReadString();
			MapsCleared.PushBack(s);
		}
	}
}