/*
    CAIProfileManager.cpp

	Autor: Nebojsa Vislavski
	Date: 24.3.2006.
	Version: 1.0



*/

#include <CAIProfileManager.h>
#include <WeFileStream.h>

CAIProfileManager *CAIProfileManager::Instance = new CAIProfileManager;

CAIProfileManager::CAIProfileManager()
{
	LoadProfiles();
}

void CAIProfileManager::LoadProfiles()
{
	for (Profiles.First(); !Profiles.End(); Profiles.Delete())
	{
		SAFE_DELETE(Profiles.GetItem());		
	}

	WeFileStream FS;
	if (!FS.Exists("aiagents.cfg")) return;
	if (!FS.Open("aiagents.cfg")) return;

	int count;
	count = FS.ReadInt();
    CProfile *p;        
	for (int i = 0; i < count; i++)
	{
		p = new CProfile;
		p->Deserialize(FS);
		Profiles.PushBack(p);
	}

	FS.Close();
}

bool CAIProfileManager::RemoveProfile(CProfile *p)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem() == p)
		{
			delete p;
			Profiles.Delete();
			SaveProfiles();
			return true;			
		}
	}	
	return false;
}


bool CAIProfileManager::RemoveProfile(StringParam Name)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem()->Name == Name)
		{
			delete Profiles.GetItem();
			Profiles.Delete();
			SaveProfiles();
			return true;
		}
	}
	return false;
}

CProfile *CAIProfileManager::GetProfile(StringParam Name)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem()->Name == Name)
		{
			return Profiles.GetItem();
		}
	}
	return NULL;
}

bool CAIProfileManager::AddProfile(CProfile *p)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem()->Name == p->Name)
		{
			return false;
		}
	}

	Profiles.PushBack(p);
	SaveProfiles();
	return true;
}



void CAIProfileManager::SaveProfiles()
{
	WeFileStream FS;
	if (!FS.OpenNew("aiagents.cfg")) return;

	FS.WriteInt(Profiles.GetItemsCount());
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		Profiles.GetItem()->Serialize(FS);
	}
	FS.Close();
}

CProfile *CAIProfileManager::GetProfile(int cash)
{
	CSmartList <CProfile*> cash_ai;

	for(Profiles.First(); !Profiles.End(); Profiles.Next())
	{		
		CProfile *p = Profiles.GetItem();
		if(p->CashCategory == cash)
		{
			cash_ai.PushBack(p);
		}
	}
	if(cash_ai.Empty()) return NULL;

	CProfile *p;
	if (cash_ai.GetItemsCount() == 0)
	{
		p = NULL;
	} else
	{
		// napravi duplikat i vrati ga nazad
		p = new CProfile(*cash_ai[rand()%(cash_ai.GetItemsCount())]);
	}

	return p;
}