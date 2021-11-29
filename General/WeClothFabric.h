/*
    WeClothFabric.h

	Autor: Nebojsa Vislavski
	Date: 30.11.2005.
	Version: 1.0


	This is cloth fabric class used to create clothes.


*/


#ifndef _WECLOTHFABRIC_H
#define _WECLOTHFABRIC_H

#include <WindEngine.h>
#include <WeMaterial.h>
#include <SmartList.h>
#include <WeCloth.h>
#include <WeVector.h>

class WeWorld;



class WINDENGINE_API WeClothFabric
{
private:
	static WeClothFabric *_instance;
    
protected:

public:
	static WeClothFabric *Instance();


	// Adding cloth method. This creates only simple cloth like plane, oriented
	// z = 0. 
	// User is responsible for deleting clothes
	
	WeCloth		*CreateCloth(WeWorld *WorldToBeIn, WeVector &Location, WeMaterial *clothMaterial,
						  int Width, int Height, 
						  float TextureTileW, float TextureTileH, 
						  int VerticesW, int VerticesH);


};


#endif