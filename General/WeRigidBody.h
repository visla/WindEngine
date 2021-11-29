/*
   WeRigidBody.h


   Autor: Nebojsa Vislavski
   Date: 5.12.2005.
   Version: 1.0

   Platform: Win32/Linux
  

*/


#ifndef _WERIGIDBODY_H
#define _WERIGIDBODY_H

#include <WeMatrix.h>
#include <WindEngine.h>
#include <CString.h>
#include <WeVector.h>

class WeStaticModel;
class WePhysicHandle;
class WeWorld;
class WeCamera;
class WeActor;



class WINDENGINE_API WeRigidBody
{
protected:
public:
	WeRigidBody(): Model(NULL), PhysicHandle(NULL) { }
	~WeRigidBody();
	
	WeStaticModel *Model;
	WePhysicHandle *PhysicHandle;
	String			Name; 

	void Draw(WeWorld *World, WeCamera *Cam, WeActor *Owner);
	void UpdateTransformations(WeMatrix &parentTM);
	void MoveTo(WeVector &newLocation);
};

#endif