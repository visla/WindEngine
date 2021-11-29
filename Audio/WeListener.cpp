/*
	File:	WeListener.cpp
	Author:	Goran Bogdanovic
*/

#include <WeListener.h>
#include <WeTimer.h>


#define DEFAULT_DEFERRED_UPDATE_INTERVAL 0.1f


WeListener::WeListener(IDirectSound3DListener *MyListener)
{
	Listener = MyListener;

	Listener->GetDistanceFactor(&DistanceFactor);
	Listener->GetDopplerFactor(&DopplerFactor);
	Listener->GetRolloffFactor(&RolloffFactor);
	Listener->GetPosition(&Position);
	Listener->GetOrientation(&OrientationFront, &OrientationUp);
	Listener->GetVelocity(&Velocity);
	Dirty = false;

	DeferredUpdate = true;
	DeferredUpdateInterval = DEFAULT_DEFERRED_UPDATE_INTERVAL;

	SetRolloffFactor(0.1f);
}

WeListener::~WeListener()
{
	SAFE_RELEASE(Listener);
}


float WeListener::GetDistanceFactor()
{
	return DistanceFactor;
}

float WeListener::GetDopplerFactor()
{
	return DopplerFactor;
}

float WeListener::GetRolloffFactor()
{
	return RolloffFactor;
}

WeVector WeListener::GetPosition()
{
	return Position;
}

WeVector WeListener::GetOrientationFront()
{
	return OrientationFront;
}

WeVector WeListener::GetOrientationUp()
{
	return OrientationUp;
}

WeVector WeListener::GetVelocity()
{
	return Velocity;
}


void WeListener::SetDistanceFactor(float Value)
{
	if (DistanceFactor != Value)
	{
		Dirty = true;
		DistanceFactor = Value;
		Listener->SetDistanceFactor(DistanceFactor, DS3D_DEFERRED);
	}
}

void WeListener::SetDopplerFactor(float Value)
{
	if (DopplerFactor != Value)
	{
		Dirty = true;
		DopplerFactor = Value;
		Listener->SetDopplerFactor(DopplerFactor, DS3D_DEFERRED);
	}
}

void WeListener::SetRolloffFactor(float Value)
{
	if (RolloffFactor != Value)
	{
		Dirty = true;
		RolloffFactor = Value;
		Listener->SetRolloffFactor(RolloffFactor, DS3D_DEFERRED);
	}
}

void WeListener::SetPosition(WeVector &Value)
{
	if (Position != Value)
	{
		Dirty = true;
		Position = Value;
		Listener->SetPosition(Position.x, Position.y, Position.z, DS3D_DEFERRED);
	}
}

void WeListener::SetOrientationFront(WeVector &Value)
{
	if (OrientationFront != Value)
	{
		Dirty = true;
		D3DXVec3Normalize(&Value, &Value);
		OrientationFront = Value;
		Listener->SetOrientation(OrientationFront.x, OrientationFront.y, OrientationFront.z, OrientationUp.x, OrientationUp.y, OrientationUp.z, DS3D_DEFERRED);
	}
}

void WeListener::SetOrientationUp(WeVector &Value)
{
	if (OrientationUp != Value)
	{
		Dirty = true;
		D3DXVec3Normalize(&Value, &Value);
		OrientationUp = Value;
		Listener->SetOrientation(OrientationFront.x, OrientationFront.y, OrientationFront.z, OrientationUp.x, OrientationUp.y, OrientationUp.z, DS3D_DEFERRED);
	}
}

void WeListener::SetVelocity(WeVector &Value)
{
	if (Velocity != Value)
	{
		Dirty = true;
		Velocity = Value;
		Listener->SetVelocity(Velocity.x, Velocity.y, Velocity.z, DS3D_DEFERRED);
	}
}


void WeListener::Update()
{
	if (!Dirty) return;

	static float DeferredTimer = 0.0f;

	if (DeferredUpdate)
	{
		DeferredTimer += WeTimer::Instance()->GetDeltaTimeF();
		if (DeferredTimer >= DeferredUpdateInterval)
		{
			DeferredTimer = 0.0f;
			Listener->CommitDeferredSettings();
			Dirty = false;
		}
	}
	else
	{
		//DS3DLISTENER Params;
		////Listener->GetAllParameters(&Params);
		//Params.vPosition = Position;
		//Params.vOrientFront = OrientationFront;
		//Params.vOrientTop = OrientationUp;
		//Params.vVelocity = Velocity;
		//Params.flRolloffFactor = RolloffFactor;
		//Params.flDopplerFactor = DopplerFactor;
		//Params.flDistanceFactor = DistanceFactor;
		//Params.dwSize = sizeof(Params);
		//Listener->SetAllParameters(&Params, DS3D_IMMEDIATE);

		Listener->CommitDeferredSettings();
		Dirty = false;
	}
}
