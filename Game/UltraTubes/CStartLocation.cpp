/*
   CStartLocation.cpp

   Autor: Nebojsa Vislavski
   Date: 4.4.2006.
   Version: 1.0

*/

#include <CStartLocation.h>
#include <WeActor.h>
#include <ActorTags.h>


CStartLocation::CStartLocation()
{
	Type = ACTORTYPE_STARTINGLOCATION;
	SpecialModelName = "MapEditor.Geometry.StartLocati";
	TeamNumber = 1;
	Tuber = NULL;
	CanBePrefab = false;
}



void CStartLocation::Initialize()
{
	if  (!InEditor)
	{
		Renderable = false;
	}
	else
	{
		WeActor::Initialize();
		Renderable = true;
	}
}



void CStartLocation::Serialize(WeFileStream &FS)
{
	WeActor::Serialize(FS);

	FS.WriteInt(TeamNumber);

	//char tmp[20];
	//sprintf(tmp,"Team Num:%d",TeamNumber);
	//MessageBox(0,tmp,0,0);
}


void CStartLocation::Clone(WeActor *Dest, bool DeepCopy)
{
	WeActor::Clone(Dest, DeepCopy);

	CStartLocation *sl = (CStartLocation *)Dest;

	sl->TeamNumber = TeamNumber;
}


void CStartLocation::Deserialize(WeFileStream &FS)
{
	WeActor::Deserialize(FS);

	TeamNumber = FS.ReadInt();	
	/*char tmp[20];
	sprintf(tmp,"%d",TeamNumber);
	MessageBox(0,tmp,0,0);*/
}


