/*
		Flie: CTuberPartFabric.h
		Author: Tomislav Kukic
		Date: 12.05.2005
		Version: 1.0
*/





#ifndef _CTUBERPARTFABRIC_H_
#define _CTUBERPARTFABRIC_H_


#include <WeStaticModel.h>
#include <WeVector.h>
#include <WeParticleSystem.h>

class CTuber;
class WeWorld;


class CTuberPartFabric
{
private:
	static CTuberPartFabric* _instance;

public:
	CTuberPartFabric();
	~CTuberPartFabric();

	static CTuberPartFabric* Instance();
	static WeStaticModel* ModelParts[10];
	static WeStaticModel* TuberDestructionModels[10];

	void CreateTuberPart(WeActor* Actor, bool FixedLocation = true, bool ShowParticles = false);
	void CreateTuberDestroyParts(CTuber* tuber);
	void CreateShellPart(WeVector &Position, WeVector &_Direction, WeWorld* WorldToBeIn, float fLifeTime, bool HavePS, StringParam ModelName);
	void ShutDown();
};



#endif
