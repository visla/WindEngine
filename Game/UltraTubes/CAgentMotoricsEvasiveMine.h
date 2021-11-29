/*
	Filename:	CAgentMotoricsEvasiveMine.h
	Author:		Goran Bogdanovic
*/


#ifndef _CAGENTMOTORICSEVASIVEMINE_H
#define _CAGENTMOTORICSEVASIVEMINE_H


#include <CAgentMotoricsStandardTube.h>

class CAgent;
class CInputMessage;
class CWeapon;


class CAgentMotoricsEvasiveMine : public CAgentMotoricsStandardTube
{
protected:
	WeVector							MineLocation;

public:
	CAgentMotoricsEvasiveMine(CAgent* Owner);
	~CAgentMotoricsEvasiveMine();

	void								Tick(CInputMessage *Msg);

	void								SetMineLocation(WeVector &MineLoc);
	WeVector&							GetMineLocation();

};


#endif