/*
    WeRandomizer.h

	Autor: Nebojsa Vislavski
	Date: 15.11.2005.
	Version: 1.0
	Platform: Win32/Linux

	This is random number generator. It is used to hold array of random numbers


*/


#ifndef _WERANDOMIZER_H
#define _WERANDOMIZER_H


#include <WindEngine.h>



const int MAX_RAND_ITEMS = 3000;

class WINDENGINE_API WeRandomizer
{
private:
	static WeRandomizer *_instance;
protected:
	static int curr;
	static bool Initialized;
public:
	
	static WeRandomizer *Instance();
	static void Initialize();

	static int RandA[MAX_RAND_ITEMS];
	static int Rand();



};

#endif