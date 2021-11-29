/*
       File: CWeaponNukeRocket.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponNukeRocket_h_
#define _CWeaponNukeRocket_h_

#include <CWeapon.h>

#define NUKE_ROCKET_RADIUS		300.0f

class WeParticleSystem;
class WeSound;

class CWeaponNukeRocket: public CWeapon
{
protected:
	WeParticleSystem	*PS;
	WeMatrix			OrientMatrix;
	WeLight				*Light;
	WeSound				*Sound;

	void				InitializeWeapon(CTuber *Owner, WeVector &RelativeSpawnLoc, WeVector &WeapLocation, WeVector &WeapDirection, WeMatrix &WeapOrientation);

public:
	static WeParticleSystem *StaticPS;

						CWeaponNukeRocket(CTuber* Owner, WeVector &RelativeSpawnLoc, WeMatrix &WeapOwnerMatrix, CWeaponDescription *Desc);
						~CWeaponNukeRocket();

	void				Tick();
	void				OnHit(WeActor *Victim);

};

#endif