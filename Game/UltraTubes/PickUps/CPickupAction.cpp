/*
	Filename:	CPickupAction.h
	Author:		Goran Bogdanovic
*/


#include <CPickupAction.h>
#include <CTuber.h>
#include <WeTimer.h>


CPickupAction::CPickupAction()
{
	Timer = 0.0f;
	ValidityTime = 60.0f;
	Validity = true;
	OwnerTuber = NULL;
}


void CPickupAction::Update(CTuber *Owner)
{
	OwnerTuber = Owner;
	Timer += WeTimer::Instance()->GetDeltaTimeF();
	if (Timer >= ValidityTime)
		Validity = false;
}


bool CPickupAction::Valid()
{
	return Validity;
}


PickupActionType CPickupAction::GetActionType()
{
	return ActionType;
}

void CPickupAction::SetValidityTime(float Time)
{
	ValidityTime = Time;
}

float CPickupAction::GetValidityTime()
{
	return ValidityTime;
}

float CPickupAction::GetTimeRemaining()
{
	float Remaining = ValidityTime - Timer;
	if (Remaining < 0.0f) Remaining = 0.0f;

	return Remaining;
}
