/*
		Flie: CTuberPartFabric.cpp
		Author: Tomislav Kukic
		Date: 12.05.2005
		Version: 1.0
*/


#include <CTuberPartFabric.h>
#include <CTuber.h>
#include <CTuberPart.h>
#include <CProfileManager.h>
#include <CProfile.h>
#include <WeWorld.h>
#include <WeActor.h>
#include <WeModelManager.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>
#include <WeParticleSystem.h>
#include <COptionsSettings.h>



CTuberPartFabric* CTuberPartFabric::_instance = NULL;

WeStaticModel* CTuberPartFabric::ModelParts[10];
WeStaticModel* CTuberPartFabric::TuberDestructionModels[10];

CTuberPartFabric* CTuberPartFabric::Instance()
{
	if(_instance) return _instance;

	_instance = new CTuberPartFabric();
	return _instance;
}


CTuberPartFabric::CTuberPartFabric()
{

	for(int i=0; i<10; i++)
	{
		ModelParts[i] = NULL;
	}

	for(int i=0; i<10; i++)
	{
		TuberDestructionModels[i] = NULL;
	}
}




CTuberPartFabric::~CTuberPartFabric()
{
	for(int i=0; i<10; i++)
	{
		delete ModelParts[i];
		ModelParts[i] = NULL;
	}
}



void CTuberPartFabric::ShutDown()
{
	for (int i = 0; i < 10; i++)
	{
		ModelParts[i] = NULL;
		TuberDestructionModels[i] = NULL;
	}
}

void CTuberPartFabric::CreateTuberPart(WeActor* Actor, bool FixedLocation, bool ShowParticles)
{
	int qnt = 0;
	if (COptionsSettings::Instance->PhysicsQuality == 1) qnt = 1;
	if (COptionsSettings::Instance->PhysicsQuality == 2) qnt = 6;
	if (COptionsSettings::Instance->PhysicsQuality == 3) qnt = 20;

	for (int k = 0; k < qnt; k++)
	{
		int i = rand()%10;
		if(!ModelParts[i])
		{
			switch(i)
			{
			case 0:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment00");
					break;
			case 1:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment01");
					break;
			case 2:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment02");
					break;
			case 3:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment03");
					break;
			case 4:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment04");
					break;
			case 5:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment05");
					break;
			case 6:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment06");
					break;
			case 7:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment07");
					break;
			case 8:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment08");
					break;
			case 9:
					ModelParts[i] = WeModelManager::Instance()->Load("Fragments.Models.Fragment09");
					break;
			}
		}


		//CTuberPart* tmpTP = new CTuberPart(ShowParticles ? TuberPartPSTypeMini : TuberPartPSTypeNone);
		CTuberPart* tmpTP = new CTuberPart(TuberPartPSTypeNone);
		WeStaticModel* tmpM = new WeStaticModel();
		tmpM->Clone(ModelParts[i],false);
		tmpTP->Model = tmpM;

		float rX = 0.0f, rY = 0.0f, rZ = 0.0f;

		rX = (float)(rand()%10);
		rY = (float)(rand()%8);
		rZ = (float)(rand()&15);

		if(!FixedLocation)
		{
			tmpTP->Location = Actor->Location + Actor->Direction*10;
			
			tmpTP->Location.x += rX;
			tmpTP->Location.y += rY;
			tmpTP->Location.z += rZ;
		}else
			tmpTP->Location = Actor->Location;

		tmpTP->PhysicHandle = WePhysics::Instance()->CreateBox(tmpTP, tmpTP->Location, WeVector(0.25,0.25,0.25), COLLISION_GROUP_FRAGMENT, 10.0f, false);

		WeVector tmp = Actor->Direction;
		tmp.x += rX;
		tmp.y += rY;
		tmp.z += rZ;

		tmpTP->PhysicHandle->AddVelocity(-tmp);
		Actor->GetWorld()->AddActor(tmpTP);
	}
}




void CTuberPartFabric::CreateTuberDestroyParts(CTuber* tuber)
{
	for (int i = 0; i < 10; i++)
	{
		if (CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.DestructionModel[i].Model != (String)"")
		{
			if (TuberDestructionModels[i] == NULL)
			{
				TuberDestructionModels[i] = WeModelManager::Instance()->Load(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.DestructionModel[i].Model);
			}
		}
	}

	CTuberPart* Part = NULL;

	for(int i = 0; i < 10; ++i)
	{
		if(TuberDestructionModels[i])
		{
			int qnt = 1;
			//if (COptionsSettings::Instance->PhysicsQuality == 1) qnt = 1;
			//if (COptionsSettings::Instance->PhysicsQuality == 2) qnt = tuber->Description.DestructionModel[i].Quantity;
			//if (COptionsSettings::Instance->PhysicsQuality == 3) qnt = tuber->Description.DestructionModel[i].Quantity*6;

			for(int j = 0; j < qnt; j++)
			{
				TuberPartPSType PSType = TuberPartPSTypeBurnBaby;
				if (i == 0)
					PSType = TuberPartPSTypeBurnBabyBURN;

				Part = new CTuberPart(PSType);
				Part->Model = new WeStaticModel();
				Part->Model->Clone(TuberDestructionModels[i],false);
				Part->Location = tuber->Location;
				Part->PhysicHandle = WePhysics::Instance()->CreateBox(Part, Part->Location, WeVector(0.1f, 0.1f, 0.1f), COLLISION_GROUP_FRAGMENT, 0.1f, false);
				Part->Tuber = tuber;
				tuber->GetWorld()->AddActor(Part);

				WeVector StartVelocity;
				StartVelocity = tuber->Direction * tuber->CurrentSpeed * 0.75f;
				StartVelocity.x += -10.0f + (float)(rand() % 20);
				StartVelocity.y += -10.0f + (float)(rand() % 20);
				StartVelocity.z += -10.0f + (float)(rand() % 20);
 				Part->PhysicHandle->AddVelocity(StartVelocity);

				if ((i == 0) && (j == 0))
					tuber->FirstPart = Part;
			}
		}
	}
}



void CTuberPartFabric::CreateShellPart(WeVector &Position, WeVector &_Direction, WeWorld* WorldToBeIn, float fLifeTime, bool HavePS, StringParam ModelName)
{
	CTuberPart* Part = new CTuberPart(HavePS ? TuberPartPSTypeMini : TuberPartPSTypeNone);
	Part->LifeTime = fLifeTime;
	Part->Location = Position;
	Part->Direction = _Direction;
	Part->Model = WeModelManager::Instance()->Load(ModelName);
	//Part->PhysicHandle = WePhysics::Instance()->CreateBox(Part, Part->Location, WeVector(0.2f, 0.2f, 0.2f), COLLISION_GROUP_SHELL, 100.0f, false);
	Part->PhysicHandle = WePhysics::Instance()->CreateSphere(Part, Part->Location, 0.2f, COLLISION_GROUP_SHELL, 100.0f, false);
	Part->PhysicHandle->AddVelocity(_Direction);
	Part->PhysicHandle->SetCollisionGroup(COLLISION_GROUP_SHELL);
	WorldToBeIn->AddActor(Part);
}
