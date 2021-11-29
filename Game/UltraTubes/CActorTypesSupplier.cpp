/*
   CActorTypesSupplier.

*/

#include <CActorTypesSupplier.h>
#include <ActorTags.h>
#include <WeActor.h>

CActorTypesSupplier *CActorTypesSupplier::_instance = NULL;

CActorTypesSupplier *CActorTypesSupplier::Instance()
{
	if (_instance) return _instance;
	_instance = new CActorTypesSupplier;
	return _instance;
}


CActorTypesSupplier::CActorTypesSupplier()
{
	Types[0].Type = WETYPE_ACTOR;
	Types[0].Description = "Object";
	Types[1].Type = WETYPE_ACTOR;
	Types[1].Description = "Dynamic Object";
	Types[2].Type = WETYPE_LIGHT;
	Types[2].Description = "Light";
	Types[3].Type = WETYPE_PARTICLESYSTEM;
	Types[3].Description = "Particle System";
	Types[4].Type = WETYPE_PATHNODE;
	Types[4].Description = "Path Node";
    Types[5].Type = PICKUPTYPE_GLOBAL;
	Types[5].Description = "Pickup";
	Types[6].Description = "Start location";
	Types[6].Type = ACTORTYPE_STARTINGLOCATION;
	Types[7].Description = "Physic model";
	Types[7].Type = ACTORTYPE_PHYSICMODEL;
	Types[8].Description = "Cloth";
	Types[8].Type = WETYPE_CLOTH;
	Types[9].Description = "Zone";
	Types[9].Type = WETYPE_ZONE;
	Types[10].Description = "Portal";
	Types[10].Type = WETYPE_PORTAL;
}
