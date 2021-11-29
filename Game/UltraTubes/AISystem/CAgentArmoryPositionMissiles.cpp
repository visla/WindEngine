/*
	Filename:	CAgentArmoryPositionMissiles.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgentArmoryPositionMissiles.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CTuber.h>
#include <CWeapon.h>


CAgentArmoryPositionMissiles::~CAgentArmoryPositionMissiles()
{
}


void CAgentArmoryPositionMissiles::Tick(CInputMessage *Msg)
{
    CTuber *Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;
	if (Avatar->Position == 1) return;

	CAgentArmoryHomingMissiles::Tick(Msg);
}
