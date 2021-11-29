/*
	Filename:	CAgentArmoryNukeMine.h
	Author:		Goran Bogdanovic
*/


#ifndef C_AGENTARMORYNukeMine
#define C_AGENTARMORYNukeMine


#include <CAgentArmory.h>
#include <WeVector.h>

class CAgent;
class CTuber;


class CAgentArmoryNukeMine : public CAgentArmory
{
protected:
	float					Timer;

	float					GetEstLapTime();
	WeVector*				GetEstPosition(CTuber* Gari, float ForecastTime);


public:
	CAgentArmoryNukeMine(CAgent *Owner);
	~CAgentArmoryNukeMine();

	void					Tick(CInputMessage *Msg);
};


#endif