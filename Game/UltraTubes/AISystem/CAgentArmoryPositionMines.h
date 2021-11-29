/*
	Filename:	CAgentArmoryPositionMines.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYPositionMINES
#define C_AGENTARMORYPositionMINES


#include <CAgentArmory.h>

class CAgent;


class CAgentArmoryPositionMines : public CAgentArmory
{
public:
	CAgentArmoryPositionMines(CAgent *Owner) { Parent = Owner; }
	~CAgentArmoryPositionMines();

	void					Tick(CInputMessage *Msg);
};


#endif