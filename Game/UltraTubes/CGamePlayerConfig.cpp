/*
    CGamePlayerConfig.cpp

	autor: Vuk Vranjkovic
	Date: 14.3.2006.
	Version: 1.0

	Settings for one player
*/
#include <CGamePlayerConfig.h>
#include <WeStream.h>
#include <WeNetworkMessage.h>
#include <CProfile.h>

CGamePlayerConfig::CGamePlayerConfig()
{
	Status = PCFG_OPEN;
	MapLoaded = false;
	ID = -1;
	User = NULL;
	CanChange = true;
}

CGamePlayerConfig::CGamePlayerConfig(const CProfile &pro, int status, int id)
{
	User = new CProfile(pro);
    Status = status;
	ID = id;
	MapLoaded = false;
}

void CGamePlayerConfig::Serialize(WeStream &S)
{
	S.WriteInt(Status);
	S.WriteInt(ID);
	if((Status==PCFG_HUMAN)||(Status==PCFG_AI))
	{
		User->Serialize(S);
	}
}

void CGamePlayerConfig::Deserialize(WeStream &S)
{
	Status = S.ReadInt();
	ID = S.ReadInt();
	User = new CProfile();
	if((Status==PCFG_HUMAN)||(Status==PCFG_AI))
	{
		User->Deserialize(S);
	}
}

void CGamePlayerConfig::NetSerialize(WeNetworkMessage &Msg)
{
	Msg.WriteInt(Status);
	Msg.WriteInt(ID);
	if((Status==PCFG_HUMAN)||(Status==PCFG_AI))
	{
		User->NetSerialize(Msg);
	}
}

void CGamePlayerConfig::NetDeserialize(WeNetworkMessage &Msg)
{
	Status = Msg.ReadInt();
	ID = Msg.ReadInt();
	User = new CProfile();
	if((Status==PCFG_HUMAN)||(Status==PCFG_AI))
	{
		User->NetDeserialize(Msg);
	}
}

bool CGamePlayerConfig::IsAI()
{
	return (Status == PCFG_AIEASY ||
			Status == PCFG_AIMEDIUM ||
			Status == PCFG_AIHARD);

}
