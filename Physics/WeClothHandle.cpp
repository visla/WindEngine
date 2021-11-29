/*
    WeClothHandle.cpp

	Autor: Nebojsa Vislavski
	Date: 23.6.2006.
	Version: 1.0

*/

#include <WeClothHandle.h>
#include <WePhysics.h>

WeClothHandle::WeClothHandle(WeActor *parent)
{
	Parent = parent;
}


WeClothHandle::~WeClothHandle()
{
	WePhysics::Instance()->GetScene()->releaseCloth(*Cloth);
}

void WeClothHandle::SetStatic(bool Static)
{
	
	NxU32 Flag;
	//NxU32 Mask;

	
  	Flag = Cloth->getFlags();
	if (Static) Flag |= NX_CLF_STATIC;
	else Flag &= 0xfffffffd;

	Cloth->setFlags(Flag);
}


void WeClothHandle::SetBendingStiffness(float Stiffness)
{
	Cloth->setBendingStiffness(Stiffness);
}

float WeClothHandle::GetBendingStiffness()
{
	return Cloth->getBendingStiffness();
}


void WeClothHandle::SetStretchingStiffness(float Stiffness)
{
	Cloth->setStretchingStiffness(Stiffness);
}

float WeClothHandle::GetStretchingStiffness()
{
	return Cloth->getBendingStiffness();
}


void WeClothHandle::SetDampingCoefficient(float DampingCoefficient)
{
	Cloth->setDampingCoefficient(DampingCoefficient);
}

float WeClothHandle::GetDampingCoefficient()
{
	return Cloth->getDampingCoefficient();
}


void WeClothHandle::SetFriction(float Friction)
{
	Cloth->setFriction(Friction);
}

float WeClothHandle::GetFriction()
{
	return Cloth->getFriction();
}


void WeClothHandle::SetPressure(float Pressure)
{
	Cloth->setPressure(Pressure);
}

float WeClothHandle::GetPressure()
{
	return Cloth->getPressure();
}


void WeClothHandle::SetTearFactor(float Factor)
{
	Cloth->setTearFactor(Factor);
}

float WeClothHandle::GetTearFactor()
{
	return Cloth->getTearFactor();
}


void WeClothHandle::SetThickness(float Thickness)
{
	Cloth->setThickness(Thickness);
}

float WeClothHandle::GetThickness()
{
	return Cloth->getThickness();
}


float WeClothHandle::GetDensity()
{
	return Cloth->getDensity();
}


void WeClothHandle::SetSolverIterations(DWORD Iterations)
{
	Cloth->setSolverIterations(Iterations);
}

DWORD WeClothHandle::GetSolverIterations()
{
	return Cloth->getSolverIterations();
}


void WeClothHandle::SetGroup(WORD CollisionGroup)
{
	Cloth->setGroup(CollisionGroup);
}

WORD WeClothHandle::GetGroup()
{
	return Cloth->getGroup();
}


void WeClothHandle::SetCollisionResponseCoefficient(float Coefficient)
{
	Cloth->setCollisionResponseCoefficient(Coefficient);
}

float WeClothHandle::GetCollisionResponseCoefficient()
{
	return Cloth->getCollisionResponseCoefficient();
}


void WeClothHandle::SetAttachmentResponseCoefficient(float Coefficient)
{
	Cloth->setAttachmentResponseCoefficient(Coefficient);
}

float WeClothHandle::GetAttachmentResponseCoefficient()
{
	return Cloth->getAttachmentResponseCoefficient();
}

