/*
	Filename:	CAgentArmoryClassicMines.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgentArmoryClassicMines.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CTuber.h>
#include <CWeapon.h>


CAgentArmoryClassicMines::~CAgentArmoryClassicMines()
{
}


void CAgentArmoryClassicMines::Tick(CInputMessage *Msg)
{

	Msg->FireMine = false;

    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;


	CGameSettings *Settings = NULL;
	Settings = CGameSettings::ClientInstance;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!Settings->tmpPlayers[i].User) continue;

		CTuber* Target = Settings->tmpPlayers[i].User->Tuber;
		if (!Avatar->ValidateTarget(Target)) continue;

		int NodeDistance = Avatar->DistanceInPathNodes(Target);
		if ( (NodeDistance < 2) || (NodeDistance > 10) ) continue;

		if (Parent->GetDifficulty() != AgentDifficultyEasy)
		{
			float Distance = D3DXVec3Length(&(Avatar->Location - Target->Location));
			float Time = Distance / Target->CurrentSpeed;

			if (Parent->GetDifficulty() == AgentDifficultyMedium)
				Time *= 2.0f;

			WEAPONTYPE ID;
			if (Avatar->SecondaryWeaponAddonID != WEAPONTYPE_NONE)
				ID = Avatar->SecondaryWeaponAddonID;
			else
				ID = Avatar->SecondaryWeaponID;

			if (Time < CWeapon::GetTimeToActivate(ID)) continue;
		}

		
		Msg->FireMine = true;
		//Avatar->FireWeapon(2);
		return;
	}
}
