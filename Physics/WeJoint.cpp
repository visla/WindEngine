/*
    WeJoint.cpp

	Autor: Nebojsa Vislavski
	Date: 8.12.2005.
	Version: 1.0
	Platform: Win32

	This is joint in physics


*/

#include <WeJoint.h>
#include <WePhysics.h>


WeJoint::~WeJoint()
{
	WePhysics::Instance()->DestroyNxJoint(nxJoint);
}

