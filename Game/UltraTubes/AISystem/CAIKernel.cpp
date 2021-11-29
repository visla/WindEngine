/*
	Filename:	CAIKernel.cpp
	Author:		Goran Bogdanovic
*/

#include <CAIKernel.h>
#include <CAgent.h>
#include <CGameSettings.h>
#include <CProfile.h>
#include <CTuber.h>


CAIKernel::CAIKernel()
{
}


CAIKernel::~CAIKernel()
{
}

  
void CAIKernel::Initialize()
{
	MotoricsStatus = true;
	ArmoryStatus = true;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User)
			if (CGameSettings::ServerInstance->tmpPlayers[i].IsAI())
			{
				Agents.PushBack(new CAgent(&CGameSettings::ServerInstance->tmpPlayers[i]));
			}
	}
}


void CAIKernel::Tick()
{
	for (Agents.First(); !Agents.End(); Agents.Next())
	{
		Agents.GetItem()->Tick();
	}
}


bool CAIKernel::HasAnAgent(CTuber *Who)
{
	CSmartListIterator<CAgent*> I;
	for (I = Agents.FirstI(); I != Agents.EndI(); ++I)
	{
		if (I.GetItem()->GetAvatarConfig()->User->Tuber == Who) return true;
	}

	return false;
}


bool CAIKernel::GetArmoryStatus()
{
	return ArmoryStatus;
}

void CAIKernel::SetArmoryStatus(bool Status)
{
	ArmoryStatus = Status;
}


bool CAIKernel::GetMotoricsStatus()
{
	return MotoricsStatus;
}

void CAIKernel::SetMotoricsStatus(bool Status)
{
	MotoricsStatus = Status;
}


CAgent*	CAIKernel::GetAgent(CTuber *Who)
{
	CSmartListIterator<CAgent*> I;
	for (I = Agents.FirstI(); I != Agents.EndI(); ++I)
	{
		if (I.GetItem()->GetAvatarConfig()->User->Tuber == Who) return I.GetItem();
	}

	return NULL;

}
