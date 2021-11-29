/*
       File: CPickupHealth.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupHealth.h>



CPickupHealth::CPickupHealth()
{
	ModelName = "Models.Editor.Light";
	Type = PICKUPTYPE_HEALTH;
}



void CPickupHealth::OnTrigger(WeActor* ByObject)
{
	CTuber* tmpTuber = (CTuber*)ByObject;

	int tmpHP = (int)(tmpTuber->MaxHP * 0.1f);
	tmpTuber->HP += tmpHP;

	if(tmpTuber->HP >= tmpTuber->MaxHP)
		tmpTuber->HP = tmpTuber->MaxHP;
}

