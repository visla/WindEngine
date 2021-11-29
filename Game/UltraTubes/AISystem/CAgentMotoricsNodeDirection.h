/*
	Filename:	CAgentMotoricsNodeDirection.h
	Author:		Goran Bogdanovic
*/


#ifndef _CAGENTMOTORICSNODEDIRECTION_H
#define _CAGENTMOTORICSNODEDIRECTION_H


#include <CAgentMotorics.h>

class CAgent;
class CInputMessage;


class CAgentMotoricsNodeDirection : public CAgentMotorics
{
protected:
	float								SpeedLimit;
	float								AngularCoef;
	float								StrictAngle;

public:
	CAgentMotoricsNodeDirection(CAgent* Owner);
	~CAgentMotoricsNodeDirection();

	void								SetSpeedLimit(float Value);
	float								GetSpeedLimit();

	void								SetAngularCoef(float Value);
	float								GetAngularCoef();

	void								SetStrictAngle(float Value);
	float								GetStrictAngle();

	void								SetDefaultSettings();

	void								Tick(CInputMessage *Msg);

};


#endif