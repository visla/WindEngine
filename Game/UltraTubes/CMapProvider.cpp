/*
    CMapProvider.cpp

	autor: Vuk Vranjkovic
	Date: 15.3.2006.
	Version: 1.0

	Settings for the game
*/
#include <CMapProvider.h>

CMapProvider *CMapProvider::Instance = new CMapProvider();

CMapProvider::CMapProvider()
{
	LoadAllMapNames("*.utm");
}

void CMapProvider::LoadAllMapNames(StringParam Extension)
{
	WIN32_FIND_DATA FindData;
	HANDLE fHandle;
	String tmp;

	MapNames.Clear();
	
	fHandle = FindFirstFile(Extension.GetString(), &FindData);
	if (fHandle != INVALID_HANDLE_VALUE) 
	{
		do 
		{			
			tmp = FindData.cFileName;
			tmp = tmp.Extract('.', 0);
			MapNames.PushBack(tmp);
		} while (FindNextFile(fHandle, &FindData));
			FindClose(fHandle);
	}
}