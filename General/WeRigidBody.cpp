/*
	  WeRigidBody.cpp


	Autor: Nebojsa Vislavski
	Date: 6.12.2005.
	Version: 1.0

	Platform: Win32/Linux
  
    
*/


#include <WeRigidBody.h>
#include <WePhysicHandle.h>
#include <WeStaticModel.h>

WeRigidBody::~WeRigidBody()
{
	SAFE_DELETE(PhysicHandle);
}

void WeRigidBody::Draw(WeWorld *World, WeCamera *Cam, WeActor *Owner)
{
	if (Model) Model->Draw(World, Cam, Owner);
}

void WeRigidBody::UpdateTransformations(WeMatrix &parentTM)
{
	// give transformation from physic handle to model
	WeMatrix M;
	PhysicHandle->GetTransformation(M);
	if (Model) Model->UpdateTransformations(M, M);	
}

void WeRigidBody::MoveTo(WeVector &newLocation)
{

}