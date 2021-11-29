/*
    WeParticle.cpp

	Autor: Nebojsa Vislavski
	Date: 8.11.2005.
	Version: 1.0

	This is only one particle.


*/


#include <WeParticle.h>
#include <WeParticleSystem.h>
#include <WeTimer.h>

void WeParticle::Update(WeParticleSystem *ps, float fDeltaTime)
{
	if (Particle[0].Alive == 0.0f) 	return;		

	D3DXCOLOR Color;
	Particle[0].Age += fDeltaTime;
	Particle[3].Age = Particle[2].Age = Particle[1].Age = Particle[0].Age;

	if (Particle[0].Age > Particle[0].LifeTime)
	{
		Particle[3].Alive = Particle[2].Alive = Particle[1].Alive = Particle[0].Alive = 0.0f;				 
		ps->Properties.ParticleCount--;
		return;
	}
	float AgeDelta = Particle[0].Age / Particle[0].LifeTime; // get percent of age
	
	// do coloring
	D3DXColorLerp(&Color, &ps->Properties.ColorFrom, &ps->Properties.ColorTo, AgeDelta);		
	Color.a = ps->Properties.AlphaFrom + AgeDelta*( ps->Properties.AlphaTo - ps->Properties.AlphaFrom );	
	Particle[0].Color = Color;
	
	Particle[0].Size = Particle[0].SizeFrom + AgeDelta * ( ps->Properties.SizeTo - Particle[0].SizeFrom);	


	WeVector oldPos;
	if (ps->Properties.RelativePosition)
	{		
		oldPos = Particle[0].PPositionRel;
		Particle[0].PPositionRel += Particle[0].Velocity*fDeltaTime;
		Particle[0].PPositionRel += (ps->Properties.Gravity*Particle[0].Age);	
		Particle[0].PPosition = ps->Location + Particle[0].PPositionRel;
	} else
	{
		oldPos = Particle[0].PPosition;
		Particle[0].PPosition += Particle[0].Velocity*fDeltaTime;
		Particle[0].PPosition += (ps->Properties.Gravity*Particle[0].Age);	
	}


	float angle;	
	if (ps->Properties.SpinningInLife)
	{
		angle = ps->Properties.SpinFrom + AgeDelta * ( Particle[0].SpinTo - ps->Properties.SpinFrom);	
	}
	else
	{
		angle = Particle[0].SpinTo;
	}

	if (ps->Properties.SpinTo != 0 || ps->Properties.SpinFrom != 0)
	{
		WeMatrix M;
		WeVector v;
		D3DXMatrixRotationZ(&M, angle);
		v = WeVector(0, 1, 0);
		D3DXVec3TransformNormal(&v, &v, &M);		
		Particle[0].YAxis = v;
	} else
	{
		if (ps->Properties.AngleType == WEPARTICLE_ANGLE_ALIGN_TO_MOTION)
		{
			Particle[0].YAxis = Particle[0].Velocity;
		} 
		if (ps->Properties.AngleType == WEPARTICLE_ANGLE_RANDOM)
		{
			Particle[0].YAxis = WeVector(0,1,0);
		}
		if (ps->Properties.AngleType == WEPARTICLE_ANGLE_SPECIFY)
		{
			Particle[0].YAxis = WeVector(0,1,0);
		} 
	}
	//Particle[0].YAxis = Particle[0].PPosition - oldPos;

	Particle[3].Color = Particle[2].Color = Particle[1].Color = Particle[0].Color;
	Particle[3].Velocity = Particle[2].Velocity = Particle[1].Velocity = Particle[0].Velocity;
	Particle[3].YAxis = Particle[2].YAxis = Particle[1].YAxis = Particle[0].YAxis;
	Particle[3].Size = Particle[2].Size = Particle[1].Size = Particle[0].Size;
	Particle[3].SizeFrom = Particle[2].SizeFrom = Particle[1].SizeFrom = Particle[0].SizeFrom;
	Particle[3].PPosition = Particle[2].PPosition = Particle[1].PPosition = Particle[0].PPosition;

}