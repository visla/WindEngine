/*
     CWEaponDescriptionManager.h


	 Autor: Nebojsa Vislavski
	 Date: 11.4.2006.
	 Version: 1.0
*/



#ifndef _CWeaponDescriptionManager_h
#define _CWeaponDescriptionManager_h


#include <SmartList.h>
#include <CWeaponDescription.h>

class CWeaponDescriptionManager
{

protected:
	void								LoadDescriptions();
	void								ClearDescriptions();

	void								LoadHardcoded();
	void								LoadFromFile();


public:
	static CWeaponDescriptionManager	*Instance;
										CWeaponDescriptionManager();
										~CWeaponDescriptionManager();

	void								ReloadDescriptions();

	CSmartList <CWeaponDescription*>	Descriptions;

	CWeaponDescription*					GetWeaponDescription(WEAPONTYPE ID);
	WEAPONTYPE							GetWeaponID(StringParam Name);

protected:


};

#endif