/*
    CMapProvider.h

	autor: Vuk Vranjkovic
	Date: 15.3.2006.
	Version: 1.0

	Settings for the game
*/

#ifndef _CMapProvider_h_
#define _CMapProvider_h_

#include <CString.h>

class CMapProvider
{
protected:
	CMapProvider();

public:
    static CMapProvider *Instance;

	void LoadAllMapNames(StringParam Extension);//Example: "*.utm"
    Strings MapNames;
};

#endif