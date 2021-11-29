/*
       File: CPickUp.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickUp_h_
#define _CPickUp_h_


#include <CPickUpFactory.h>
#include <WeActor.h>
#include <CTuber.h>


class WeSound;

enum PickupMatPreset
{
	PickupMatPresetNormal = 0,
	PickupMatPresetAlpha = 1
};


class CPickUp:public WeActor
{
protected:
	float LevitationParam;

	float Timer;

	WeVector	OriginalPosition;

public:
	CPickUp();
	~CPickUp(){}

	float		LockFactor;
	int			NewPreset;
	bool		Active;   // znaci da Ontrigger moze da funckionise
	float		RespawnTime; // -1 nikada se nevraca, 0 nikada nece nestati
	bool		ShouldTerminate; // default je false
	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
	void		Update(CTuber *tuber); // ovo ce da radi logiku kada je pokupljen za tuber-a
	void		Tick();

	WeSound*	RespawnSound;

	virtual void		Initialize();

	void		Serialize(WeFileStream &FS);
	void		Deserialize(WeFileStream &FS);
};



#endif
