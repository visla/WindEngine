/*
       File: CPickupCoin.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupCoin_h_
#define _CPickupCoin_h_


#include <CPickUpFactory.h>
#include <CPickUp.h>
#include <WeActor.h>


class CPickupCoin:public CPickUp
{
public:
	CPickupCoin();
	~CPickupCoin(){}

	void				Initialize();

	void				OnTrigger(WeActor* ByObject); // Ko ga je trigerovao
};



#endif
