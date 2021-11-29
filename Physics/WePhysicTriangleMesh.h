/*
     WePhysicTriangleMesh.h

	 Autor: nebojsa Vislavski
	 Date: 8.5.2006.
	 Version: 1.0

     

*/


#ifndef _WEPHYSICTRIANGLEMESH_H
#define _WEPHYSICTRIANGLEMESH_H

#include <CString.h>
#include <NxPhysics.h>

class WeStaticModel;

class WePhysicTriangleMesh
{
friend class WePhysics;

protected:
	NxActorDesc					Desc;
	NxTriangleMeshShapeDesc		ShapeDesc;

public:
	String			ModelName;		// this is static model name whose mesh is this

	bool			MakeTriangleMesh(WeStaticModel *);
	NxActorDesc*	GetActorDesc() { return &Desc; }


};


#endif