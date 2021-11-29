/*
    WePhysicClothMesh.h

	Autor: Nebojsa Vislavski
	Date: 23.5.2006.
	Version: 1.0
   
*/

#ifndef _WEPHYSICCLOTHMANAGER_H
#define _WEPHYSICCLOTHMANAGER_H

#include <NxPhysics.h>
#include <CString.h>
#include <WeStaticModel.h>

class WePhysicClothMesh
{
friend class WePhysics;
protected:
	NxClothMesh		*ClothMesh;

public:
	String			ModelName;

	NxClothMesh		*GetClothMeshDesc() { return ClothMesh; }
	void			MakeClothMesh(WeStaticModel *);

};



#endif