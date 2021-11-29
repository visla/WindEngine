/*
	Filename:	CAgentArmoryClassicMines.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYCLASSICMINES
#define C_AGENTARMORYCLASSICMINES


#include <CAgentArmory.h>

class CAgent;


class CAgentArmoryClassicMines : public CAgentArmory
{
public:
	CAgentArmoryClassicMines(CAgent *Owner) { Parent = Owner; }
	~CAgentArmoryClassicMines();

	void					Tick(CInputMessage *Msg);
};


#endif