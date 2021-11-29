/*
   WeClothHandle.h

   Autor: Nebojsa Vislavski
   Date: 23.6.2006.
   Version: 1.0

*/



#ifndef _WECLOTHHANDLE_H
#define _WECLOTHHANDLE_H


#include <WePhysicHandle.h>
#include <NxPhysics.h>

class String;


class WeClothHandle: public WePhysicHandle
{
protected:
public:
	NxCloth			*Cloth;

	WeClothHandle(WeActor *Parent);
	virtual ~WeClothHandle();

	void	SetTransformation(WeMatrix &TM) {};
	void	SetStatic(bool Static);

	
	void			SetBendingStiffness(float Stiffness);
	float			GetBendingStiffness();

	void			SetStretchingStiffness(float Stiffness);
	float			GetStretchingStiffness();

	void			SetDampingCoefficient(float DampingCoefficient);
	float			GetDampingCoefficient();

	void			SetFriction(float Friction);
	float			GetFriction();

	void			SetPressure(float Pressure);
	float			GetPressure();

	void			SetTearFactor(float Factor);
	float			GetTearFactor();

	void			SetThickness(float Thickness);
	float			GetThickness();

	float			GetDensity();

	void			SetSolverIterations(DWORD Iterations);
	DWORD			GetSolverIterations();

	void			SetGroup(WORD CollisionGroup);
	WORD			GetGroup();

	void			SetCollisionResponseCoefficient(float Coefficient);
	float			GetCollisionResponseCoefficient();

	void			SetAttachmentResponseCoefficient(float Coefficient);
	float			GetAttachmentResponseCoefficient();


};


#endif