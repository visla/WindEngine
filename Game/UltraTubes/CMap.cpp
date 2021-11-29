/*
      File: CMap.cpp
	  Author: Tomislav Kukic
	  Date: 25.11.2005.
	  Version: 1.0
*/


#include <CMap.h>
#include <WeFileStream.h>
#include <CPickupFactory.h>
#include <WeActor.h>
#include <CPickup.h>
#include <ActorTags.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <CollisionGroups.h>




CMap::CMap()
{
	FastesLap = 0.0f;
}

CMap::~CMap()
{
	Pickups.Clear();
}




WeActor* CMap::OnLoadMapItem(int Type, WeFileStream &FS)
{

	if(Type == ACTORTYPE_STARTINGLOCATION)
	{
		CStartLocation *tmp = new CStartLocation();
		//tmp->Deserialize(FS);
		//tmp->Initialize();
		//tmp->SetTransformation(tmp->TM);
		//AddActor(tmp);
		//MapHeader.StartLocations.PushBack(tmp);
		return tmp;
	}

	CPickUp *tmpPickup = NULL;
	tmpPickup = CPickUpFactory::Instance->MakePickUp(Type);

	if (tmpPickup)
		Pickups.PushBack(tmpPickup);

	return tmpPickup;
}




void CMap::OnLoadMapHeader(WeFileStream &FS)
{
	MapHeader.Deserialize(FS);
}



void CMap::OnSaveMapHeader(WeFileStream &FS)
{
	MapHeader.Serialize(FS);
}



