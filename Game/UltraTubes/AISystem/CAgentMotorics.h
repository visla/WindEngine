/*
	Filename:	CAgentMotorics.h
	Author:		Goran Bogdanovic
*/


#ifndef _CAGENTMOTORICS_H
#define _CAGENTMOTORICS_H


#include <WeVector.h>
#include <WePhysics.h>

class CAgent;
class CInputMessage;


class CAgentMotorics
{
protected:
	CAgent								*Parent;

	float								ForcedFactor;


public:
	~CAgentMotorics();

	void								SetForcedFactor(float Value);
	float								GetForcedFactor();

	virtual void						Tick(CInputMessage *Msg);

};


#endif