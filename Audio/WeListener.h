/*
	File:	WeListener.h
	Author:	Goran Bogdanovic
*/


#ifndef We_Listener_h_
#define	We_Listener_h_

#include <windengine.h>
#include <windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <WeVector.h>


class WINDENGINE_API WeListener
{
protected:
	IDirectSound3DListener*	Listener;

	bool		Dirty;

	float		DistanceFactor;
	float		DopplerFactor;
	float		RolloffFactor;

	WeVector	Position;
	WeVector	OrientationFront;
	WeVector	OrientationUp;
	WeVector	Velocity;


public:
	WeListener(IDirectSound3DListener* MyListener);
	~WeListener();

	bool		DeferredUpdate;
	float		DeferredUpdateInterval;

	void		SetDistanceFactor(float Value);
	void		SetDopplerFactor(float Value);
	void		SetRolloffFactor(float Value);
	void		SetPosition(WeVector &Value);
	void		SetOrientationFront(WeVector &Value);
	void		SetOrientationUp(WeVector &Value);
	void		SetVelocity(WeVector &Value);

	float		GetDistanceFactor();
	float		GetDopplerFactor();
	float		GetRolloffFactor();
	WeVector	GetPosition();
	WeVector	GetOrientationFront();
	WeVector	GetOrientationUp();
	WeVector	GetVelocity();

	void		Update();

};

#endif