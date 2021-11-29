/*
	Filename:	CAgentMotoricsStandardTube.h
	Author:		Goran Bogdanovic
*/


#ifndef _CAGENTMOTORICSSTANDARDTUBE_H
#define _CAGENTMOTORICSSTANDARDTUBE_H


#include <CAgentMotorics.h>

class CAgent;
class CInputMessage;


class CAgentMotoricsStandardTube : public CAgentMotorics
{
protected:
	void								GetRicochet(WeVector &Result, WeVector &From, WeVector &Impact, WeVector &Normal);


public:
	CAgentMotoricsStandardTube() {}
	CAgentMotoricsStandardTube(CAgent* Owner) { Parent = Owner; }
	virtual ~CAgentMotoricsStandardTube();

	virtual void						Tick(CInputMessage *Msg);
	void								TickTurbo(CInputMessage *Msg);
	void								TickSpeed(CInputMessage *Msg);
};


#endif