/*
	Filename:	CAgentArmoryNukeMine.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgentArmoryNukeMine.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CTuber.h>
#include <CWeapon.h>
#include <CWeaponSuperNukeMine.h>
#include <WePathNode.h>
#include <WeTimer.h>


CAgentArmoryNukeMine::CAgentArmoryNukeMine(CAgent *Owner)
{
	Parent = Owner;
	Timer = 0.0f;
}


CAgentArmoryNukeMine::~CAgentArmoryNukeMine()
{
}


void CAgentArmoryNukeMine::Tick(CInputMessage *Msg)
{
	Msg->FireMine = false;

    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	if (Parent->GetDifficulty() == AgentDifficultyEasy) return;

	CGameSettings *Settings = NULL;
	Settings = CGameSettings::ClientInstance;

	int Enemies = 0;
	int Friends = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!Settings->tmpPlayers[i].User) continue;

		CTuber* Target = Settings->tmpPlayers[i].User->Tuber;

		if (Target->RaceFinished) continue;
		WeVector *EstLocation = GetEstPosition(Target, CWeapon::GetTimeToActivate(WEAPONTYPE_SUPERNUKEMINE));
		if (EstLocation)
		{
			WeVector Distance = *EstLocation - Avatar->Location;
			if (D3DXVec3Length(&Distance) < NUKE_MINE_SIZE)
			{
				if (Avatar->IsFriendWith(Target))
					Friends++;
				else
					Enemies++;
			}
		}
	}

	if (Friends >= Enemies) return;

	if ( (Parent->GetDifficulty() == AgentDifficultyMedium) ||
		((Parent->GetDifficulty() == AgentDifficultyGuru) && ((float)Enemies >= 1.5f * (float)Friends)) )
	{
		Msg->FireMine = true;
		//Avatar->FireWeapon(2);
		return;
	}
}


//////////////////////////////////////////////////////////////////////////
// unused
float CAgentArmoryNukeMine::GetEstLapTime()
{
    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;

	if (Avatar->LastLapTime != 0.0f) return Avatar->LastLapTime;

	return ((Avatar->RaceTime / (float)Avatar->RefPathNode->SortedIndex) * (float)Avatar->GetWorld()->TotalPathNodes);
}
//////////////////////////////////////////////////////////////////////////


WeVector* CAgentArmoryNukeMine::GetEstPosition(CTuber *Gari, float ForecastTime)
{
	WePathNode *CurrentNode = Gari->GetWorld()->FindNextDirectionPathNode(Gari->Location);

	int Nodes = (int)(ForecastTime * (Gari->RefPathNode->SortedIndex / (Gari->RaceTime - Gari->StartLapTime)));
	Nodes = (int)(Nodes * 1.2f);
	int CountedNodes = 0;

	while (CountedNodes < Nodes)
	{
		if (CurrentNode)
		{
			CurrentNode = Gari->GetWorld()->FindNextDirectionPathNode(CurrentNode->Location);
			if (!CurrentNode) return NULL;
			CountedNodes++;
		}
	}

	return &CurrentNode->Location;
}
