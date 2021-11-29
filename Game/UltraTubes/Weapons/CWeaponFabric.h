/*
       File: CWeaponFabric.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeaponFabric_h_
#define _CWeaponFabric_h_


#include <CWeaponDescription.h>
#include <CString.h>
#include <WeWorld.h>
#include <SmartList.h>


class CWeapon;
class CTuber;



class CWeaponFabric
{
protected:
	void							BuildWeapon(CTuber* Player, WEAPONTYPE ID, WeMatrix &FullMatrix, bool ServerWeapon);

	bool							LockOnTarget(CWeapon *Weapon);

	bool							Active;

public:
									CWeaponFabric();

	static CWeaponFabric*			Instance;

	WEAPONTYPE						AbleToFire(CTuber *Player, bool AsPrimaryWeapon, bool Reset = false);
	WEAPONTYPE						Server_MakeWeapon(CTuber *Player, bool AsPrimaryWeapon);
	bool							Client_MakeWeapon(CTuber *Owner, WEAPONTYPE ID, WeMatrix &OwnerFullMatrix);

	void							SetActive(bool BooleanVariableThatActivatesWeaponry);
	bool							IsValid();
};



#endif
