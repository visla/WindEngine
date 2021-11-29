/*
	Filename:	CAgentArmoryHomingMissiles.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgentArmoryHomingMissiles.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CTuber.h>
#include <CWeapon.h>


CAgentArmoryHomingMissiles::~CAgentArmoryHomingMissiles()
{
}


void CAgentArmoryHomingMissiles::Tick(CInputMessage *Msg)
{
	Msg->FireWeapon = false;

    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	switch (Parent->GetDifficulty())
	{
	case AgentDifficultyEasy:
		if (Avatar->EnemyTarget)
		{
			Msg->FireWeapon = true;
			//Avatar->FireWeapon(1);
			return;
		}
		break;

	case AgentDifficultyMedium:
		if (Avatar->EnemyTarget && (Avatar->EnemyLockFactor == 1.0f) )
		{
			Msg->FireWeapon = true;
			//Avatar->FireWeapon(1);
			return;
		}
		break;

	case AgentDifficultyGuru:
		if ( Avatar->EnemyTarget && (Avatar->EnemyLockFactor == 1.0f) )
		{

			WeVector AX, AY, AZ;
			Avatar->GetLocalAxis(AX, AY, AZ);
			WeVector VecDistance = Avatar->EnemyTarget->Location - Avatar->Location;
			float Distance = D3DXVec3Length(&VecDistance);
			D3DXVec3Normalize(&VecDistance, &VecDistance);
			float Dot = D3DXVec3Dot(&VecDistance, &AZ);

			float HitDistance = Parent->QuickRay(Avatar->Location, VecDistance, Distance);

			if ( (Distance <= 100.0f) && (Dot > 0.99f) )
			{
				Msg->FireWeapon = true;
				//Avatar->FireWeapon(1);
				return;
			}
			else if ( Distance > 100.0f && (HitDistance == -1.0f || HitDistance > 100.0f) )
			{
				Msg->FireWeapon = true;
				//Avatar->FireWeapon(1);
				return;
			}
		}
		break;
	}
}
