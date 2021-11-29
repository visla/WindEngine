/*
    WeJoint.h

	Autor: Nebojsa Vislavski
	Date: 6.12.2005.
	Version: 1.0
	Platform: Win32

	This is joint in physics


*/


#ifndef _WEJOINT_H
#define _WEJOINT_H

#include <WindEngine.h>
#include <NxPhysics.h>

class WINDENGINE_API WeJoint
{
protected:
public:
	~WeJoint();

	NxJoint *nxJoint;

};


#endif