/*
    CTournamentManager.cpp

	autor: Vuk Vranjkovic
	Date: 16.3.2006.
	Version: 1.0
*/
#include <CTournamentManager.h>
#include <CTournament.h>
#include <WeFileStream.h>
#include <CProfile.h>


CTournamentManager *CTournamentManager::Instance = new CTournamentManager();

CTournamentManager::CTournamentManager()
{
	Load();
}

CTournamentManager::~CTournamentManager()
{
	Tournaments.Clear();
	//while(!Tournaments.End())
	//{
	//	CTournament *pDel = Tournaments.GetItem();
	//	Tournaments.Delete();
	//	delete pDel;
	//}
}


void CTournamentManager::CopyToProfile(CProfile *p)
{
	if (!p) return;
	p->Tournaments = Tournaments;	
}

void CTournamentManager::Load()
{
	//while(!Tournaments.Empty())
	//{
	//	delete Tournaments.GetItem();
	//	Tournaments.Delete();		
	//}
	Tournaments.Clear();

	WeFileStream f;
	if(!f.Exists("Tournament.cfg")) return;
	if(!f.Open("Tournament.cfg")) return;
	
	int num_of_tours = f.ReadInt();
	CTournament t;
	for(int i=0; i < num_of_tours; i++)
	{
		t.Deserialize(f);
		Tournaments.PushBack(t);
	}
	f.Close();
}


bool CTournamentManager::AddTournament(CTournament &tournament)
{
	for (Tournaments.First(); !Tournaments.End(); Tournaments.Next())
	{
		if (Tournaments.GetItem().Name == tournament.Name)
		{
			return false;
		}
	}
	Tournaments.PushBack(tournament);
	Save();
	return true;

}

void CTournamentManager::Save()
{
	WeFileStream f;
	if(!f.OpenNew("Tournament.cfg")) return;
	
	f.WriteInt(Tournaments.GetItemsCount());
	Tournaments.First();
	while(!Tournaments.End())
	{
		Tournaments.GetItem().Serialize(f);
		Tournaments.Next();
	}
	f.Close();
}


CTournament* CTournamentManager::FindTournament(StringParam name)
{
	for (Tournaments.First(); !Tournaments.End(); Tournaments.Next())
	{
		if (Tournaments.GetItem().Name == name)
		{
			return &Tournaments.GetItem();
		}
	}
	return NULL;
}

//CTournament *CTournamentManager::NewTournament(StringParam name)
//{
//	for(Tournaments.First(); !Tournaments.End(); Tournaments.Next())
//	{
//		if(Tournaments.GetItem().Name==name) return NULL;
//	}
//	CTournament pNew;
//	pNew->Name = name;
//	Tournaments.PushBack(pNew);
//	return pNew;
//}

bool CTournamentManager::Remove(StringParam name)
{
	for(Tournaments.First(); !Tournaments.End(); Tournaments.Next())
	{
		if(Tournaments.GetItem().Name==name)
		{
			Tournaments.Delete();
			Save();
			return true;
		}
	}
	return false;
}

//bool CTournamentManager::Remove(CTournament *pTour)
//{
//	for(Tournaments.First(); !Tournaments.End(); Tournaments.Next())
//	{
//		if(Tournaments.GetItem()==pTour)
//		{
//			CTournament *pDel = Tournaments.GetItem();
//			Tournaments.Delete();
//			delete pDel;
//			Save();
//			return true;
//		}	
//	}
//	return false;
//}