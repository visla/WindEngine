/*
    WeControllerHandle.h

	Autor: Nebojsa Vislavski
	Date: 11.10.2005.
	Version: 1.0

	This is controller handle

*/


#ifndef _WECONTROLLERHANDLE_H
#define _WECONTROLLERHANDLE_H

#include <WePhysicHandle.h>
#include <NxController.h>

class WeActor;


class WeControllerHandle;

class WINDENGINE_API WePhysicControllerReport: public NxUserControllerHitReport
{
public:
	WeControllerHandle *Owner;

	virtual NxControllerAction onShapeHit(const NxControllerShapeHit &hit);
	virtual NxControllerAction onControllerHit(const NxControllersHit &hit);
};





class WINDENGINE_API WeControllerHandle: public WePhysicHandle
{
friend class WePhysics;

protected:
	WePhysicControllerReport ControllerReport;
	NxController *nxController; // for controllers like box or sphere

	WeControllerHandle() {} 

public:
	WeControllerHandle(WeActor *parent);

	virtual void	GetLocation(WeVector &outLocation);
	virtual void	SetLocation(const WeVector &Location);
	virtual void	OnContactNotify(WePhysicHandle *other, WeVector &NormalForce) {};

};


#endif