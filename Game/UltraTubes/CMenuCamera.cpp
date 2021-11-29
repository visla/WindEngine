/*
File: CMenuCamera.cpp
Author: Goran Bogdanovic
*/


#include <CMenuCamera.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePathNode.h>
#include <CMenuPhase.h>
#include <WeLight.h>

#define	DEFAULT_SPEED	50.0f


CMenuCamera::CMenuCamera()
{
	NearClip = 1.0f;
	FarClip = 3200.0f;

	Location = WeVector(0.0f, 0.0f, 0.0f);
	Direction = WeVector(0.0f, 0.0f, 1.0f);
	Up = WeVector(0.0f, 1.0f, 0.0f);

	UpdateProjection = true;
}


CMenuCamera::~CMenuCamera()
{
}


void CMenuCamera::Update()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();


	Location = WeVector(-295.7f, 13.6f, 3741.689f);
	Location += WeVector(
		1.0f * sin(WeTimer::Instance()->GetElapsedTimeF() * 0.5f),
		1.0f * sin(WeTimer::Instance()->GetElapsedTimeF() * 1.1f),
		1.0f * sin(WeTimer::Instance()->GetElapsedTimeF() * 1.7f));

	Direction = WeVector(-60.9f, 93.1f, 4347.689f) - Location;
	Up = WeVector(0.0f, 1.0f, 0.0f);


	WeCamera::Update();
}
