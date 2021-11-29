/*
    CAbstractPhase.cpp

	autor: Vuk Vranjkovic
	Date: 14.3.2006.
	Version: 1.0

	Settings for the game
*/
#include <CGameSettings.h>
#include <WeStream.h>
#include <CTournamentMapSettings.h>
#include <Constants.h>
#include <CProfile.h>
#include <CPickUpFactory.h>


CGameSettings *CGameSettings::Instance			= new CGameSettings();
CGameSettings *CGameSettings::ClientInstance	= new CGameSettings();
CGameSettings *CGameSettings::ServerInstance	= new CGameSettings();


WeColor				CGameSettings::TeamColors[MAX_PLAYERS];
int					CGameSettings::CashCategories[MAX_CASH_CATEGORIES];
int					CGameSettings::LapCountPresets[MAX_LAP_PRESETS];


CGameSettings::CGameSettings()
{
	NeedPasswordLogin=false;
	MapSettings = NULL;
	Funny = false;
	MaxPlayers = MAX_PLAYERS;


	int j = 0;
	CashCategories[j++] = 10000;
	CashCategories[j++] = 20000;
	CashCategories[j++] = 30000;
	CashCategories[j++] = 40000;
	CashCategories[j++] = 50000;
	CashCategories[j++] = 80000;
	CashCategories[j++] = 100000;
	CashCategories[j++] = 150000;
	CashCategories[j++] = 200000;
	CashCategories[j++] = 500000;

	j = 0;
	LapCountPresets[j++] = 1;
	LapCountPresets[j++] = 2;
	LapCountPresets[j++] = 3;
	LapCountPresets[j++] = 4;
	LapCountPresets[j++] = 5;
	LapCountPresets[j++] = 6;
	LapCountPresets[j++] = 7;
	LapCountPresets[j++] = 8;
	LapCountPresets[j++] = 9;
	LapCountPresets[j++] = 10;
	LapCountPresets[j++] = 15;


	int i = 0;
	TeamColors[i].r = 130.0f;
	TeamColors[i].g = 0.0f;
	TeamColors[i].b = 255.0f;
	i++;	//1

	TeamColors[i].r = 0.0f;
	TeamColors[i].g = 50.0f;
	TeamColors[i].b = 255.0f;
	i++;	//2

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 255.0f;
	TeamColors[i].b = 0.0f;
	i++;	//3

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 0.0f;
	TeamColors[i].b = 0.0f;
	i++;	//4

	TeamColors[i].r = 75.0f;
	TeamColors[i].g = 255.0f;
	TeamColors[i].b = 244.0f;
	i++;	//5

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 0.0f;
	TeamColors[i].b = 255.0f;
	i++;	//6

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 100.0f;
	TeamColors[i].b = 100.0f;
	i++;	//7

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 200.0f;
	TeamColors[i].b = 100.0f;
	i++;	//8

	TeamColors[i].r = 200.0f;
	TeamColors[i].g = 150.0f;
	TeamColors[i].b = 255.0f;
	i++;	//9

	TeamColors[i].r = 255.0f;
	TeamColors[i].g = 150.0f;
	TeamColors[i].b = 240.0f;
	i++;	//10

	TeamColors[i].r = 0.0f;
	TeamColors[i].g = 150.0f;
	TeamColors[i].b = 10.0f;
	i++;	//11

	TeamColors[i].r = 220.0f;
	TeamColors[i].g = 220.0f;
	TeamColors[i].b = 220.0f;
	i++;	//12

    TeamColors[i].r = 120.0f;
	TeamColors[i].g = 120.0f;
	TeamColors[i].b = 120.0f;
	i++;	//13

	TeamColors[i].r = 125.0f;
	TeamColors[i].g = 30.0f;
	TeamColors[i].b = 0.0f;
	i++;	//14

	TeamColors[i].r = 150.0f;
	TeamColors[i].g = 180.0f;
	TeamColors[i].b = 180.0f;
	i++;	//15

	TeamColors[i].r = 75.0f;
	TeamColors[i].g = 170.0f;
	TeamColors[i].b = 255.0f;
			//16

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		TeamColors[i] /= 255.0f;
	}
}

void CGameSettings::Serialize(WeStream &S)
{
	S.WriteInt(StartingMoney);
	S.WriteInt(LapCount);
	S.WriteString(MapName);
	S.WriteBool(Crates);
	S.WriteBool(Weapons);
	S.WriteBool(UnlimitedSpeed);
	S.WriteBool(CatchUp);
	S.WriteInt(MapType);	

	if (MapSettings) 
	{
		S.WriteBool(true);
		MapSettings->Serialize(S);
	}
	else 
	{
		S.WriteBool(false);
	}
	
	S.WriteBool(NeedPasswordLogin);
	if(NeedPasswordLogin) S.WriteString(Password);
}



void CGameSettings::Deserialize(WeStream &S)
{
	StartingMoney = S.ReadInt();
	LapCount = S.ReadInt();
	MapName = S.ReadString();
	Crates = S.ReadBool();
	Weapons = S.ReadBool();
	UnlimitedSpeed = S.ReadBool();
	CatchUp = S.ReadBool();
	MapType = (MAPTYPE)S.ReadInt();
	//int Count; 
	//Count = S.ReadInt();

	//CGamePlayerConfig tpc;

	//for(int i=0; i < Count; i++) 
	//{
	//	tpc.Deserialize(S);
	//	Players.PushBack(tpc);
	//}
	bool me;
	me = S.ReadBool();	
	if (me)
	{
		MapSettings = new CTournamentMapSettings;
		MapSettings->Deserialize(S);
	}
	NeedPasswordLogin = S.ReadBool();
	if(NeedPasswordLogin) Password = S.ReadString();
}



void CGameSettings::Initialize()
{
	ClearPlayers();
}

void CGameSettings::InitProfiles()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (tmpPlayers[i].User)
		{
			tmpPlayers[i].User->InitForMapType(this);
		}
	}
}


int CGameSettings::GetFirstEmptyPlayer()
{
	for (int i= 0; i < MAX_PLAYERS; i++)
	{
		if (!tmpPlayers[i].User && tmpPlayers[i].Status != PCFG_CLOSE) return i;
	}
	return -1;
}

bool CGameSettings::GetPlayersFull()
{
	for (int i= 0; i < MAX_PLAYERS; i++)
	{
		if (!tmpPlayers[i].User && tmpPlayers[i].Status != PCFG_CLOSE) return false;
	}
	return true;
}

int CGameSettings::GetPlayersCount()
{
	int c = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (tmpPlayers[i].User) c++;

	}
	return c;
}




void CGameSettings::ClearPlayers()
{
	// ako igramo SP igru onda nemoj brisati 0 profil jer sam to sigurno ja
	// ostale mozes obrisati
	int i = 0;
	if (MapType == MAPTYPE_SP_INSTANTRACE || MapType == MAPTYPE_SP_LASTONE || 
		MapType == MAPTYPE_SP_TIMETRIAL || MapType == MAPTYPE_SP_TOURNAMENT ||
		MapType == MAPTYPE_SP_SINGLERACE)
	{
		tmpPlayers[i].CanChange = true;
		if (tmpPlayers[i].User)
			tmpPlayers[i].User->Tuber = NULL;
		tmpPlayers[i].User = NULL;

		tmpPlayers[i].GameCreator = false;
		tmpPlayers[i].Status = PCFG_OPEN;
		tmpPlayers[i].ID = -1;
		tmpPlayers[i].MapLoaded = false;
		tmpPlayers[i].MyProfile = false;
        i = 1;
	} 

	for (; i < MAX_PLAYERS; i++)
	{
		tmpPlayers[i].CanChange = true;
		if (tmpPlayers[i].User)
			tmpPlayers[i].User->Tuber = NULL;
		SAFE_DELETE(tmpPlayers[i].User);
		tmpPlayers[i].GameCreator = false;
		tmpPlayers[i].Status = PCFG_OPEN;
		tmpPlayers[i].ID = -1;
		tmpPlayers[i].MyProfile = false;
		tmpPlayers[i].MapLoaded = false;
	}
}

bool CGameSettings::AddPlayer(CGamePlayerConfig *p_new)
{
	if(Players.GetItemsCount()==MaxPlayers) return false;
	for(Players.First(); !Players.End(); Players.Next())
	{
		if(Players.GetItem().ID == p_new->ID) return false;
	}
	Players.PushBack(*p_new);
	return true;
}

bool CGameSettings::RemovePlayer(CGamePlayerConfig *p_rem)
{
	for(Players.First(); !Players.End(); Players.Next())
	{
		if(Players.GetItem().ID == p_rem->ID)
		{
			Players.Delete();
			return true;
		}
	}
	return false;
}

bool CGameSettings::RemovePlayer(int UID)
{
	for(Players.First(); !Players.End(); Players.Next())
	{
		if(Players.GetItem().ID == UID)
		{
			Players.Delete();
			return true;
		}
	}
	return false;
}

CGamePlayerConfig *CGameSettings::GetPlayer(int UID)
{
	for(Players.First(); !Players.End(); Players.Next())
	{
		if(Players.GetItem().ID == UID) return &Players.GetItem();
	}
	return NULL;	
}

bool CGameSettings::GetWeapons()
{
	return Weapons;
}

void CGameSettings::SetWeapons(bool WeaponsSettingBooleanVariableWithLongName)
{
	Weapons = WeaponsSettingBooleanVariableWithLongName;
}

bool CGameSettings::GetCrates()
{
	return Crates;
}

void CGameSettings::SetCrates(bool CratesSettingBooleanVariableWithLongName)
{
	Crates = CratesSettingBooleanVariableWithLongName;
}

bool CGameSettings::GetUnlimitedSpeed()
{
	return UnlimitedSpeed;
}

void CGameSettings::SetUnlimitedSpeed(bool UnlimitedSpeedSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense)
{
	UnlimitedSpeed = UnlimitedSpeedSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense;
}

bool CGameSettings::GetCatchUp()
{
	return CatchUp;
}

void CGameSettings::SetCatchUp(bool CatchUpSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense)
{
	CatchUp = CatchUpSettingBooleanVariableWithSuperDuperUltraLongNameWhichMakesNoSense;
}
