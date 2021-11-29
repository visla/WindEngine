/*
    CProfileManager.cpp

	Autor: Nebojsa Vislavski
	Date: 17.3.2006.
	Version: 1.0

     
*/

#include <CProfileManager.h>
#include <WeFileStream.h>
#include <CTournamentManager.h>
#include <COptionsSettings.h>


CProfileManager *CProfileManager::_Instance = NULL;

CProfileManager *CProfileManager::Instance()
{
	if (!_Instance)
		_Instance = new CProfileManager;
	return _Instance;
}


CProfileManager::CProfileManager()
{
	CurrentProfile = NULL;
}

CProfile* CProfileManager::GenerateAIProfile()
{
	CProfile *tmpP;
	tmpP = new CProfile;
	tmpP->Name = "AI player";
	Profiles.PushBack(tmpP);
	return tmpP;
}



void CProfileManager::LoadAllProfiles()
{
	WeFileStream FS;
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		SAFE_DELETE(Profiles.GetItem());
	}
	Profiles.Clear();
	
	if (!FS.Exists("profiles.utp")) return;
	if (!FS.Open("profiles.utp")) return;

	int ver = 0;

	int Count;
	Count = FS.ReadInt();

	if (Count < 0) // versioned
	{
		ver = -Count;
		Count = FS.ReadInt();
	}

	for (int i = 0; i < Count; i++)
	{
		CProfile *tmpP;
		tmpP = new CProfile;
		tmpP->Deserialize(FS);
		Profiles.PushBack(tmpP);
	}

	if (ver == 1)
	{
		String LastProfileName;
		LastProfileName = FS.ReadString();
		SetCurrentProfile(GetProfileByName(LastProfileName), true);
	}

	FS.Close();
}




void CProfileManager::RemoveAIProfiles()
{
	for (Profiles.First(); !Profiles.End(); )
	{
		if (Profiles.GetItem()->AIProfile) 
		{
			SAFE_DELETE(Profiles.GetItem());
			Profiles.Delete();
		}
		else
			Profiles.Next();
	}
}


void CProfileManager::SaveAllProfiles()
{
	WeFileStream FS;

	if (!FS.OpenNew("profiles.utp")) return;

	FS.WriteInt(-1);
	FS.WriteInt(Profiles.GetItemsCount());

	int Count = 0;  // kako bismo izbrojali koliko ima onih profila bez AI-a
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (!Profiles.GetItem()->AIProfile)
		{
			Profiles.GetItem()->Serialize(FS);
			Count++;
		}
	}

	//FS.Seek(0);
	//FS.WriteInt(Count);

	if (CurrentProfile)
		FS.WriteString(CurrentProfile->Name);
	else
		FS.WriteString("");

	FS.Close();
}


CProfile* CProfileManager::NewProfile(StringParam Name)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem()->Name == Name) 
		{
			return NULL;
		}
	}

	CProfile *tmpP;
	tmpP = new CProfile;
	tmpP->Name = Name;
	Profiles.PushBack(tmpP);
	CurrentProfile = tmpP;
	CTournamentManager::Instance->CopyToProfile(tmpP);
	SaveAllProfiles();  
	return tmpP;
}

void CProfileManager::RemoveProfile(CProfile *Profile)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem() == Profile)
		{
			SAFE_DELETE(Profiles.GetItem());
			Profiles.Delete();
			return;
		}
	}
}

void CProfileManager::RemoveProfile(StringParam Name)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (Profiles.GetItem()->Name == Name)
		{
			SAFE_DELETE(Profiles.GetItem());
			Profiles.Delete();
			return;
		}
	}
}

void CProfileManager::SetProfileControls(CProfile *p)
{
	if (!p) return;
	
	COptionsSettings::Instance->ControllerSettings = *p->ControllerSettings;
}

void CProfileManager::SetCurrentProfile(CProfile *p, bool AsLoadedProfile)
{
	CurrentProfile = p;
	if (AsLoadedProfile)
		LoadedProfile = p;	
}


void CProfileManager::RestoreToLoadedProfile()
{
	CurrentProfile = LoadedProfile;
}


CProfile* CProfileManager::GetProfileByName(StringParam Name)
{
	for (Profiles.First(); !Profiles.End(); Profiles.Next())
	{
		if (!Profiles.GetItem()->AIProfile)
		{
			if (Profiles.GetItem()->Name == Name)
			{
				return Profiles.GetItem();
			}
		}
	}

	return NULL;
}
