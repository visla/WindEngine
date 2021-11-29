/*
       File: CWeaponDescription.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CWeaponDescription.h>



CWeaponDescription::CWeaponDescription()
{
}


void CWeaponDescription::Serialize(FILE *f)
{
}


void CWeaponDescription::Deserialize(FILE *f)
{
	char tmp[100], tmp2[100];	
	fgets(tmp, 100, f);	
	Name = tmp;
	Name.RemoveChar(Name.GetLen()-1);

	String TypeS;
	fscanf(f, "%s %s\n", tmp2, tmp);
	TypeS = tmp;

	fscanf(f, "%s %s\n", tmp2, tmp);
	InterfaceImage = tmp;

	fscanf(f, "%s %f\n", tmp2, &FireRate);
	fscanf(f, "%s %d\n", tmp2, &PickUpAmmo);
	fscanf(f, "%s %d\n", tmp2, &AmmoPerShot);
	fscanf(f, "%s %f\n", tmp2, &Damage);
	fscanf(f, "%s %f\n", tmp2, &Speed);	  
	fscanf(f, "%s %f\n", tmp2, &HomingLockTime);

	// now find type 
	if (TypeS == (String)"WEAPONTYPE_STANDARDLASER") Type = WEAPONTYPE_STANDARDLASER;
	if (TypeS == (String)"WEAPONTYPE_DOUBLELASER") Type = WEAPONTYPE_DOUBLELASER;
	if (TypeS == (String)"WEAPONTYPE_QUADLASER") Type = WEAPONTYPE_QUADLASER;
	if (TypeS == (String)"WEAPONTYPE_STANDARDROCKET") Type = WEAPONTYPE_STANDARDROCKET;
	if (TypeS == (String)"WEAPONTYPE_NUKEROCKET") Type = WEAPONTYPE_NUKEROCKET;
	if (TypeS == (String)"WEAPONTYPE_QUANTUMLASER") Type = WEAPONTYPE_QUANTUMLASER;
	if (TypeS == (String)"WEAPONTYPE_MINIROCKETS") Type = WEAPONTYPE_MINIROCKETS;
	if (TypeS == (String)"WEAPONTYPE_FRONTFIELDGENERATOR") Type = WEAPONTYPE_FRONTFIELDGENERATOR;
	if (TypeS == (String)"WEAPONTYPE_SLOWINGROCKET") Type = WEAPONTYPE_SLOWINGROCKET;
	if (TypeS == (String)"WEAPONTYPE_TRANSLOCATOR") Type = WEAPONTYPE_TRANSLOCATOR;
	if (TypeS == (String)"WEAPONTYPE_MINIGUN") Type = WEAPONTYPE_MINIGUN;


	// mines 
	if (TypeS == (String)"WEAPONTYPE_STANDARDMINE") Type = WEAPONTYPE_STANDARDMINE;
	if (TypeS == (String)"WEAPONTYPE_SMOKEMINE") Type = WEAPONTYPE_SMOKEMINE;
	if (TypeS == (String)"WEAPONTYPE_PROXIMITYMINE") Type = WEAPONTYPE_PROXIMITYMINE;
	if (TypeS == (String)"WEAPONTYPE_SLOWINGMINE") Type = WEAPONTYPE_SLOWINGMINE;
	if (TypeS == (String)"WEAPONTYPE_HOMINGMINE") Type = WEAPONTYPE_HOMINGMINE;
	if (TypeS == (String)"WEAPONTYPE_GRAVITYMINE") Type = WEAPONTYPE_GRAVITYMINE;
	if (TypeS == (String)"WEAPONTYPE_SUPERNUKEMINE") Type = WEAPONTYPE_SUPERNUKEMINE;
	if (TypeS == (String)"WEAPONTYPE_BLURMINE") Type = WEAPONTYPE_BLURMINE;
	if (TypeS == (String)"WEAPONTYPE_MOVINGMINE") Type = WEAPONTYPE_MOVINGMINE;
	if (TypeS == (String)"WEAPONTYPE_FORCEFIELDMINE") Type = WEAPONTYPE_FORCEFIELDMINE;
	if (TypeS == (String)"WEAPONTYPE_ELECTRICMINE") Type = WEAPONTYPE_ELECTRICMINE;
	if (TypeS == (String)"WEAPONTYPE_INVISIBLEMINE") Type = WEAPONTYPE_INVISIBLEMINE;

}
