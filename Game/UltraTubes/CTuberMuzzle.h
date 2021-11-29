/*
		File: CTuberMuzzle.h
		Author: Goran Bogdanovic
*/

#ifndef _CTuberMuzzle_h_
#define _CTuberMuzzle_h_


#include <WeActor.h>

class CTuber;
class WeLight;


class CTuberMuzzle : public WeActor
{
protected:
	int				ID;
	CTuber			*Owner;
	WeLight			*Light;
	
	WeMatrix		RelativeTransMatrix;

public:
					CTuberMuzzle(CTuber *MyOwner, int MyID, WeMatrix &RelMatrix);
					~CTuberMuzzle();

	void			Tick();

};

#endif