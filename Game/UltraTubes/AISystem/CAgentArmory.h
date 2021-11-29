/*
	Filename:	CAgentArmory.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORY
#define C_AGENTARMORY


class CAgent;
class CInputMessage;


class CAgentArmory
{
protected:
	CAgent						*Parent;

public:
	~CAgentArmory();

	virtual void				Tick(CInputMessage *Msg) = 0;

};


#endif
