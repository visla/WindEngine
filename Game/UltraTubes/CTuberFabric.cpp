/*
      File: CTuberFabric.cpp
	  Author: Nebojsa Vislavski
	  Date: 23.03.2006
	  Version: 1.0
*/



#include <Windows.h>
#include <CTuberDescription.h>
#include <CTuber.h>
#include <CTuberFabric.h>
#include <CProfile.h>

CTuberFabric *CTuberFabric::Instance = new CTuberFabric;


CTuber* CTuberFabric::CreateTuber(CTuberDescription &TuberDesc, CProfile *Owner)
{
	CTuber* tuber = NULL;

	tuber = new CTuber;

	tuber->Description = TuberDesc;
	tuber->OwnerProfile = Owner;
	tuber->ResetValues();
	Owner->Tuber = tuber;

	return tuber;
}

