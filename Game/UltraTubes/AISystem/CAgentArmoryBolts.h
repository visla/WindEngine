/*
	Filename:	CAgentArmoryBolts.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYBOLTS
#define C_AGENTARMORYBOLTS


#include <CAgentArmory.h>

class CAgent;


class CAgentArmoryBolts : public CAgentArmory
{
public:
	CAgentArmoryBolts(CAgent *Owner) { Parent = Owner; }
	~CAgentArmoryBolts();

	void					Tick(CInputMessage *Msg);
};


#endif