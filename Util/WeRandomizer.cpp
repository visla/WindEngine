/*
    WeRandomizer.cpp

	Autor: Nebojsa Vislavski
	Date: 15.11.2005.
	Version: 1.0
	Platform: Win32/Linux

	This is random number generator. It is used to hold array of random numbers


*/


#include <WeRandomizer.h>
#include <time.h>
#include <stdlib.h>


int WeRandomizer::RandA[MAX_RAND_ITEMS];
int WeRandomizer::curr = -1;
bool WeRandomizer::Initialized = false;


WeRandomizer *WeRandomizer::_instance = NULL;

WeRandomizer *WeRandomizer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeRandomizer;
	return _instance;
}


int WeRandomizer::Rand()
{	
	if (!Initialized) Initialize();

	if (curr >= MAX_RAND_ITEMS) curr = -1;
	curr++;
	return RandA[curr];    
}

void WeRandomizer::Initialize()
{	
	curr = -1;
	srand( (unsigned)time( NULL ) );

	for (int i = 0; i < MAX_RAND_ITEMS; i++)
	{
		RandA[i] = rand();
	}
	Initialized = true;
}