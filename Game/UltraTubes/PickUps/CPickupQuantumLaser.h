/*
       File: CPickupQuantumLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupQuantumLaser_h_
#define _CPickupQuantumLaser_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupQuantumLaser:public CPickUp
{
public:
	CPickupQuantumLaser();
	~CPickupQuantumLaser(){}

	void		OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
