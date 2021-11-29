/*
     CProfileManager.h

	 Autor: Nebojsa Vislavski
	 Date: 17.3.2006.
	 Version: 1.0


	 Ovo je manager za profile preko kojeg dolazimo do spiska svih profila i
	 to trenutno izabranog profila.
    

*/

#ifndef _CPROFILEMANAGER_H
#define _CPROFILEMANAGER_H


#include <CProfile.h>
#include <SmartList.h>



class CProfileManager
{
protected:
	static CProfileManager*	_Instance;
	CProfile*				CurrentProfile;
	CProfile*				LoadedProfile; // koji je profil sa kojim je ucitana igra

public:
	CSmartList <CProfile*>	Profiles;
	static CProfileManager*	Instance();
							CProfileManager();

	CProfile*				LastProfile;

	void					LoadAllProfiles();
	void					SaveAllProfiles();

	CProfile*				GenerateAIProfile();
	CProfile*				NewProfile(StringParam Name); // vraca NULL ako postoji vec takav profil. Pravi profil i snima ga u fajl
	void					RemoveProfile(CProfile *Profile); // brise profil iz liste kao i iz fajla
	void					RemoveProfile(StringParam Name); // brise profil iz liste kao i iz fajla
	void					SetCurrentProfile(CProfile *p, bool AsLoadedProfile = false);
	void					SetProfileControls(CProfile *p); // this will take profile controls and write them into options
	CProfile*				GetCurrentProfile() { return CurrentProfile; }
	CProfile*				GetProfileByName(StringParam Name);
	void					RemoveAIProfiles();
	void					RestoreToLoadedProfile();




};


#endif