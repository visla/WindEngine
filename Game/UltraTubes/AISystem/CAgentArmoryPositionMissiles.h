/*
	Filename:	CAgentArmoryPositionMissiles.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYPOSITIONMISSILES
#define C_AGENTARMORYPOSITIONMISSILES


#include <CAgentArmoryHomingMissiles.h>

class CAgent;


class CAgentArmoryPositionMissiles : public CAgentArmoryHomingMissiles
{
public:
	CAgentArmoryPositionMissiles(CAgent *Owner) { Parent = Owner; }
	~CAgentArmoryPositionMissiles();

	void					Tick(CInputMessage *Msg);
};


#endif