/*
	Author  : Vuk Vranjkovic
	File    : CTableManager.h
	Date    : 28.03.2006.
	Version : 1.00

	Purpose: helper class for handling with Profiles tables
*/
#ifndef _CTableManager_h_
#define _CTableManager_h_

#include <Constants.h>
#include <string>
class CProfile;

/******************************************************************************
	Profile | Team  |
			|	    |
******************************************************************************/

class CTableManager
{
public:
					CTableManager();

	void			setProfileAt(CProfile *, unsigned char pos);
	void			reset(unsigned char);
	void			reset(void);

	CProfile	   *getProfileAt(unsigned char);
	void			sort(void);





private:
	CProfile	   *profiles[MAX_PLAYERS];
	std::string		teams[MAX_PLAYERS];

	void			swap(unsigned char, unsigned char);
};






inline void	CTableManager::reset(unsigned char pos){if(pos<MAX_PLAYERS) profiles[pos]=0;}
inline CProfile	*CTableManager::getProfileAt(unsigned char pos) {if(pos<MAX_PLAYERS) return profiles[pos];}

#endif