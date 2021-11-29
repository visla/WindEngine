/*
       File: CPickUpFactory.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <Windows.h>
#include <CPickUpFactory.h>
#include <CPickupHealth.h>
#include <CPickupRandomWeapon.h>
#include <CPickupNukeRocket.h>
#include <CPickupQuantumLaser.h>
#include <CPickupMiniRockets.h>
#include <CPickupFrontFieldGenerator.h>
#include <CPickupSlowingRocket.h>
#include <CPickupTranslocator.h>
#include <CPickupMinigun.h>
#include <CPickupSuperNukeMine.h>
#include <CPickupBlurMine.h>
#include <CPickupMovingMine.h>
#include <CPickupInvisibleMine.h>
#include <CPickupForceFieldMine.h>
#include <CPickupElectricMine.h>
#include <CPickupGravityMine.h>
#include <CPickupHomingMine.h>
#include <CPickupProximityMine.h>
#include <CPickupSlowingMine.h>
#include <CPickupSmokeMine.h>
#include <CPickupSmokeMine.h>
#include <CPickupStandardMine.h>
#include <CPickupStandardRocket.h>
#include <CPickupStandardLaser.h>
#include <CPickupDoubleLaser.h>
#include <CPickupQuadLaser.h>
#include <CPickupCoin.h>
#include <CPickupInvisibility.h>
#include <CPickupInvulnerability.h>
#include <CPickupDoubleDamage.h>
#include <CPickupQuadDamage.h>
#include <CPickUp.h>



CPickUpFactory* CPickUpFactory::Instance = new CPickUpFactory();


CPickUpFactory::CPickUpFactory()
{
	WeaponPickupsActive = true;
}

CPickUpFactory::~CPickUpFactory()
{
}


CPickUp* CPickUpFactory::MakePickUp(PICKUPTYPE ID)
{
	CPickUp* tmp = NULL;

	if (!WeaponPickupsActive)
	{
		switch (ID)
		{
		case PICKUPTYPE_INVISIBILITY:
		case PICKUPTYPE_COINSMALL:
		case PICKUPTYPE_COINMEDIUM:
		case PICKUPTYPE_COINLARGE:
			break;
		default:
			return NULL;
		}
	}

	switch(ID)
	{
	case PICKUPTYPE_HEALTH:
		tmp = new CPickupHealth();
		break;
	case PICKUPTYPE_RANDOMWEAPON:
		tmp = new CPickupRandomWeapon();
		break;
	case PICKUPTYPE_STANDARDLASER:
		tmp = new CPickupStandardLaser();
		break;
	case PICKUPTYPE_DOUBLELASER:
		tmp = new CPickupDoubleLaser();
		break;
	case PICKUPTYPE_QUADLASER:
		tmp = new CPickupQuadLaser();
		break;
	case PICKUPTYPE_NUKEROCKET:
		tmp = new CPickupNukeRocket();
		break;
	case PICKUPTYPE_QUANTUMLASER:
		tmp = new CPickupQuantumLaser();
		break;
	case PICKUPTYPE_MINIROCKETS:
		tmp = new CPickupMiniRockets();
		break;
	case PICKUPTYPE_FRONTFIELDGENERATOR:
		tmp = new CPickupFrontFieldGenerator();
		break;
	case PICKUPTYPE_SLOWINGROCKET:
		tmp = new CPickupSlowingRocket();
		break;
	case PICKUPTYPE_TRANSLOCATOR:
		tmp = new CPickupTranslocator();
		break;
	case PICKUPTYPE_MINIGUN:
		tmp = new CPickupMinigun();
		break;
	case PICKUPTYPE_SUPERNUKEMINE:
		tmp = new CPickupSuperNukeMine();
		break;
	case PICKUPTYPE_BLURMINE:
		tmp = new CPickupBlurMine();
		break;
	case PICKUPTYPE_MOVINGMINE:
		tmp = new CPickupMovingMine();
		break;
	case PICKUPTYPE_INVISIBLEMINE:
		tmp = new CPickupInvisibleMine();
		break;
	case PICKUPTYPE_FORCEFIELDMINE:
		tmp = new CPickupForceFieldMine();
		break;
	case PICKUPTYPE_ELECTRICMINE:
		tmp = new CPickupElectricMine();
		break;
	case PICKUPTYPE_GRAVITYMINE:
		tmp = new CPickupGravityMine();
		break;
	case PICKUPTYPE_HOMINGMINE:
		tmp = new CPickupHomingMine();
		break;
	case PICKUPTYPE_PROXIMITYMINE:
		tmp = new CPickupProximityMine();
		break;
	case PICKUPTYPE_SLOWINGMINE:
		tmp = new CPickupSlowingMine();
		break;
	case PICKUPTYPE_SMOKEMINE:
		tmp = new CPickupSmokeMine();
		break;
	case PICKUPTYPE_STANDARDMINE:
		tmp = new CPickupStandardMine();
		break;
	case PICKUPTYPE_STANDARDROCKET:
		tmp = new CPickupStandardRocket();
		break;
	case PICKUPTYPE_COINSMALL:
	case PICKUPTYPE_COINMEDIUM:
	case PICKUPTYPE_COINLARGE:
		tmp = new CPickupCoin();
		tmp->Type = ID;
		((CPickupCoin*)tmp)->Initialize();
		break;
	case PICKUPTYPE_INVISIBILITY:
		tmp = new CPickupInvisibility();
		break;
	case PICKUPTYPE_INVULNERABILITY:
		tmp = new CPickupInvulnerability();
        break;
	case PICKUPTYPE_DOUBLEDAMAGE:
		tmp = new CPickupDoubleDamage();
		break;
	case PICKUPTYPE_QUADDAMAGE:
		tmp = new CPickupQuadDamage();
		break;
	}

	return tmp;
}



void CPickUpFactory::SetWeaponPickupsActive(bool BooleanVariableThatActivatesWeaponPickups)
{
	WeaponPickupsActive = BooleanVariableThatActivatesWeaponPickups;
}


bool CPickUpFactory::AreWeaponPickupsActive()
{
	return WeaponPickupsActive;
}
