/*
       File: CTrigger.h
	   Author: Goran Bogdanovic
*/




#ifndef _CTrigger_h_
#define _CTrigger_h_


#include <WeActor.h>


class CTrigger : public WeActor
{
public:
				CTrigger(WeActor *MyParent);
				~CTrigger();

	WeActor		*Parent;
};


#endif