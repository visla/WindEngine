/*
    WeQueryManager.cpp

	Autor: Nebojsa Vislavski
	Date: 1.11.2005.
	Version: 1.0
	Platform: Win32

	This is query manager. It ensures that all possible queries are made. and no query 
	creation occures in real time	

*/

#include <WeQueryManager.h>


WeQueryManager *WeQueryManager::_instance = NULL;

WeQueryManager *WeQueryManager::Instance()
{
	if (_instance) return _instance;
	_instance = new WeQueryManager;
	return _instance;
}



void WeQueryManager::Initialize(int numQueries)
{
	if (bInitialized) return;

	NumQueries = numQueries;
	Queries = new WeQuery[NumQueries];
	for (int i = 0; i < NumQueries; i++)
	{
		Queries[i].Initialize();
	}
	QueryPos = 0;
	bInitialized = true;
}

void WeQueryManager::ShutDown()
{
	if (!bInitialized) return;
	delete[] Queries;
	bInitialized = false;
}


void WeQueryManager::OnLostDevice()
{
	ShutDown();
}

void WeQueryManager::OnResetDevice()
{
	Initialize(NumQueries);
}



WeQuery *WeQueryManager::GetNextQuery()
{
	QueryPos++;
	if (QueryPos >= NumQueries) QueryPos = 1;
	return &Queries[QueryPos-1];
}
