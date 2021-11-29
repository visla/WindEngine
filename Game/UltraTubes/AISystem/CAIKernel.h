/*
	Filename:	CAIKernel.h
	Author:		Goran Bogdanovic
*/

#ifndef _CAIKERNEL_H
#define _CAIKERNEL_H


#include <SmartList.h>

class CAgent;
class CTuber;


class CAIKernel
{
protected:
	bool						ArmoryStatus;
	bool						MotoricsStatus;


public:
	CAIKernel();
	~CAIKernel();

	CSmartList<CAgent*>			Agents;

	void						Initialize();
	void						Tick();

	bool						HasAnAgent(CTuber *Who);
	CAgent*						GetAgent(CTuber *Who);

	bool						GetArmoryStatus();
	void						SetArmoryStatus(bool Status);

	bool						GetMotoricsStatus();
	void						SetMotoricsStatus(bool Status);
};


#endif