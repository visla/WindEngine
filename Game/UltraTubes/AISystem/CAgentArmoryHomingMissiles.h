/*
	Filename:	CAgentArmoryHomingMissiles.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYHOMINGMISSILES
#define C_AGENTARMORYHOMINGMISSILES


#include <CAgentArmory.h>

class CAgent;


class CAgentArmoryHomingMissiles : public CAgentArmory
{
public:
	CAgentArmoryHomingMissiles() {}
	CAgentArmoryHomingMissiles(CAgent *Owner) { Parent = Owner; }
	~CAgentArmoryHomingMissiles();

	virtual void			Tick(CInputMessage *Msg);
};


#endif