/*
       File: CTrigger.cpp
	   Author: Goran Bogdanovic
*/


#include <CTrigger.h>
#include <ActorTags.h>


CTrigger::CTrigger(WeActor *MyParent)
{
	Type = ACTORTYPE_TRIGGER;
	Parent = MyParent;
}


CTrigger::~CTrigger()
{
	Parent = NULL;
}
