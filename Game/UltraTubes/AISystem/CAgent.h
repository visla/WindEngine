/*
	Filename:	CAgent.h
	Author:		Goran Bogdanovic
*/


#ifndef _CAGENT_H
#define _CAGENT_H

#include <WeVector.h>
#include <WePhysics.h>


class CTuber;
class CGamePlayerConfig;
class CAgentMotorics;
class CAgentMotoricsStandardTube;
class CAgentMotoricsNodeDirection;
class CAgentMotoricsEvasiveMine;
class CAgentArmory;
class CAgentArmoryBolts;
class CAgentArmoryClassicMines;
class CAgentArmoryHomingMissiles;
class CAgentArmoryPositionMissiles;
class CAgentArmoryPositionMines;
class CAgentArmoryNukeMine;
class CWeapon;


enum AgentDifficulty
{
	AgentDifficultyEasy,
	AgentDifficultyMedium,
	AgentDifficultyGuru
};


class CAgent
{
protected:
	CGamePlayerConfig					*AvatarConfig;
	CTuber								*Avatar;

	CAgentMotorics						*Motorics;
	CAgentMotoricsStandardTube			*MotoricsStandardTube;
	CAgentMotoricsNodeDirection			*MotoricsNodeDirection;
	CAgentMotoricsEvasiveMine			*MotoricsEvasiveMine;
	CAgentArmory						*ArmoryPrimary;
	CAgentArmory						*ArmorySecondary;
	CAgentArmoryBolts					*ArmoryBolts;
	CAgentArmoryClassicMines			*ArmoryClassicMines;
	CAgentArmoryHomingMissiles			*ArmoryHomingMissiles;
	CAgentArmoryPositionMissiles		*ArmoryPositionMissiles;
	CAgentArmoryPositionMines			*ArmoryPositionMines;
	CAgentArmoryNukeMine				*ArmoryNukeMine;

	float								MotoricsSelectedTimer;
	float								MotoricsSelectedTime;
	CAgentMotorics						*MotoricsSelected;

	CAgentMotorics						*ForcedMotorics;

	bool								ArmoryEnabled;

	float								BumpTimer;
	WeVector							PreviousBumpLocation;

	void								SelectMotorics(CAgentMotorics *FixedTimeMotorics, float Duration);

	bool								TestRightDirection();
	bool								TestWallBump();
	CWeapon*							TestMineProximity();
	bool								TestNodeForcedMotorics();

	int									GetNodeDistance(int FirstNodeIndex, int SecondNodeIndex);

	bool								DeathOccurred;


public:
	CAgent(CGamePlayerConfig* OwnerConfig);
	~CAgent();

	static float						QuickRay(WeVector &From, WeVector &Dir, float Dist, WeShapesType TType = WE_SHAPE_TYPE_STATIC);

	CGamePlayerConfig*					GetAvatarConfig() { return AvatarConfig; }

	void								ChooseMotorics();
	void								ChooseArmories();
	void								Tick();
	void								TickEmpty();

	void								ForceNodeMotorics(float SpeedLimit = 75.0f, float AngularCoef = 0.8f, float StrictAngle = -10.0f);
	void								ForcedMotoricsOff();

	void								SetArmoryEnabled(bool Value);
	bool								GetArmoryEnabled();

	AgentDifficulty						GetDifficulty();
};


#endif