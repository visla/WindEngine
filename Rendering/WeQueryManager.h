/*
    WeQueryManager.h

	Autor: Nebojsa Vislavski
	Date: 1.11.2005.
	Version: 1.0
	Platform: Win32

	This is query manager. It ensures that all possible queries are made. and no query 
	creation occures in real time	

*/

#ifndef _WEQUERYMANAGER_H
#define _WEQUERYMANAGER_H

#include <WeQuery.h>



class WeQueryManager
{
private:
	static WeQueryManager *_instance;
protected:
	WeQuery *Queries;
	int QueryPos;
	int NumQueries;
	bool bInitialized;

public:
	static WeQueryManager *Instance();

	WeQueryManager(): QueryPos(0), Queries(NULL), bInitialized(false) {} 

	void Initialize(int NumQueries = 10000);
	void ShutDown();

	void OnLostDevice();
	void OnResetDevice();

	WeQuery *GetNextQuery();	

};


#endif