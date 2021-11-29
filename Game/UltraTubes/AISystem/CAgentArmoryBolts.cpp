/*
	Filename:	CAgentArmoryBolts.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgentArmoryBolts.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CTuber.h>
#include <CWeapon.h>


CAgentArmoryBolts::~CAgentArmoryBolts()
{
}


void CAgentArmoryBolts::Tick(CInputMessage *Msg)
{

	Msg->FireWeapon = false;
    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	float LockDistance = BOLT_LOCK_DISTANCE;
	float LockMinDot = BOLT_LOCK_MINDOT;

	switch (Parent->GetDifficulty())
	{
	case AgentDifficultyEasy:
		LockDistance *= 2.0f;
		LockMinDot *= 0.5f;
		break;
	case AgentDifficultyMedium:
		LockDistance *= 1.5f;
		LockMinDot *= 0.8f;
		break;
	}

	CTuber *Target = Avatar->GetBestEnemyTargetLock(LockDistance, LockMinDot);
	if (Target)
	{
		if (Parent->GetDifficulty() == AgentDifficultyGuru)
		{
			WeVector Direction = Target->Location - Avatar->Location;
			float Distance = D3DXVec3Length(&Direction);
			D3DXVec3Normalize(&Direction, &Direction);
			if (Parent->QuickRay(Avatar->Location, Direction, Distance) == -1.0f)
			{
				Msg->FireWeapon = true;
				//Avatar->FireWeapon(1);
				return;
			}
		}
		else
		{
			//stupid trigger-happy bot
			Msg->FireWeapon = true;
			//Avatar->FireWeapon(1);
			return;
		}
	}
	else if (Parent->GetDifficulty() == AgentDifficultyGuru)	//try to snipe over the auto-aiming distance
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (!CGameSettings::ServerInstance->tmpPlayers[i].User) continue;
			if (!Avatar->ValidateTarget(CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber)) continue;

            CTuber* Target = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;

			WeVector VecDistance = Target->Location - Avatar->Location;
			WeVector Direction;
			D3DXVec3Normalize(&Direction, &VecDistance);
			if (D3DXVec3Dot(&Direction, &Avatar->Direction) < 0.998f) return;
			float Distance = D3DXVec3Length(&VecDistance);
			if (Distance > BOLT_LOCK_DISTANCE * 4.0f) return;
			if (Parent->QuickRay(Avatar->Location, Direction, Distance) == -1.0f)
			{
				Msg->FireWeapon = true;
				//Avatar->FireWeapon(1);
				return;
			}
		}
	}
}
