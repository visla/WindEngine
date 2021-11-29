/*
      File: WePrefab.cpp
	  Author: Tomislav Kukic
	  Date: 13.09.2005.
	  Version: 1.0
*/




#include <WePrefab.h>
#include <WeStaticModel.h>
#include <WeActor.h>
#include <Wetimer.h>
#include <WeFrustum.h>

WePrefab::WePrefab()
{
	IsPrefab = true;
	Type = WETYPE_PREFAB;
	PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
}



WePrefab::~WePrefab()
{	    
}


void WePrefab::SetWorld(WeWorld* World)
{
}



void WePrefab::Tick()
{
	//if (!HasTicked || InEditor)
	/*if (InEditor) 
	{
		if (PhysicHandle)
			PhysicHandle->nxActor->putToSleep();
	}*/
	// if object needs new translation matrix than make it
	PrevTM = TM;
	
	if (PhysicHandle)
		PhysicHandle->GetTransformation(TM);		
	Location.x = TM._41;
	Location.y = TM._42;
	Location.z = TM._43;

	TMWithScale = TM;
	if (Model) Model->UpdateTransformations(TM, PrevTM);			
	HasTicked = true;	 
	ObjectAge += WeTimer::Instance()->GetDeltaTimeF();
	OccludedTime += WeTimer::Instance()->GetDeltaTimeF(); 

	//// suppose this actor is occluded. If it is not BatchRenderer will reset this to 0	
	if (LifeTime != 0.0f)
	{
		LifeTime -= WeTimer::Instance()->GetDeltaTimeF();
		if (LifeTime <= 0.0f)
		{
			delete this;
			return;
		}
	}

}


bool WePrefab::IntersectFrustum(WeFrustum *frustum)
{
	if (World)
	{
		if (!World->ShowActorTypes[-1 - WETYPE_PREFAB])
			return false;
	}

	return frustum->Collide(BSphere);
}

void WePrefab::PreRender(WeCamera* Cam)
{
}


void WePrefab::PostRender(WeCamera* Cam)
{
}

