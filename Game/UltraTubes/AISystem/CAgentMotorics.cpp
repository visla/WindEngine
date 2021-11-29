/*
	Filename:	CAgentMotorics.cpp
	Author:		Goran Bogdanovic
*/


#include <CAgentMotorics.h>
#include <CAgent.h>
#include <CInputMessage.h>


CAgentMotorics::~CAgentMotorics()
{
	ForcedFactor = 0.0f;
	Parent = NULL;
}


void CAgentMotorics::Tick(CInputMessage *Msg)
{
	float RXFactor = 1.0f;
	float RYFactor = 1.0f;

	if (ForcedFactor > 0.0f)
	{
		RXFactor = ForcedFactor;
		RYFactor = ForcedFactor;
	}
	else
	{
		switch (Parent->GetDifficulty())
		{
		case AgentDifficultyEasy:
			RXFactor = 0.2f;
			RYFactor = 0.2f;
			break;

		case AgentDifficultyMedium:
			RXFactor = 0.25f;
			RYFactor = 0.25f;
			break;

		case AgentDifficultyGuru:
			RXFactor = 0.9f;
			RYFactor = 0.9f;
			break;
		}
	}

	Msg->rx = (int)(Msg->rx * RXFactor);
	Msg->ry = (int)(Msg->ry * RYFactor);
}


void CAgentMotorics::SetForcedFactor(float Value)
{
	ForcedFactor = Value;
}


float CAgentMotorics::GetForcedFactor()
{
	return ForcedFactor;
}
