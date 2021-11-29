/*
   CAIProfileManager.h

   Autor: Nebojsa Vislavski
   Date: 24.3.2006.
   Version: 1.0

   Svrha ove klase je da pronadje AI neprijatelje protiv kojih ce igrac da igra kada 
   bira SingleRace

*/

#ifndef _CAIPROFILEMANAGER_H
#define _CAIPROFILEMANAGER_H


#include <CProfile.h>
#include <SmartList.h>


class CAIProfileManager
{
public:
	CSmartList <CProfile*> Profiles;

	static CAIProfileManager *Instance;


	void		SaveProfiles();
	void		LoadProfiles();
	bool		RemoveProfile(CProfile *);
	bool		RemoveProfile(StringParam Name);
	CProfile   *GetProfile(StringParam Name);
	CProfile   *GetProfile(int cash);
	bool		AddProfile(CProfile *);


private:
				CAIProfileManager();
};



#endif