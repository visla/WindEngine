/*
	    File: WeSphere.h
		Author: Tomislav Kukic
		Date: 06.10.2005.
		Version: 1.0
		Platform: Windows
*/




#ifndef _WESPHERE
#define _WESPHERE


#include <WeVector.h>
#include <WindEngine.h>

class WeCylinder;
class WeBox;



class WINDENGINE_API WeSphere
{
public:
	WeSphere(WeVector Position, float Radius);
	WeSphere();

	~WeSphere(void){}


	WeVector	Position;
	float		Radius;

	//void		SetPosition(WeVector _Position){ Position = _Position; }
	//void		SetRadius(float _Radius){ Radius = _Radius; }

	//WeVector	GetPosition(){ return Position; }
	//float		GetRadius(){ return Radius; }

	bool		Intersect(WeSphere		*Sphere);
	bool		Intersect(WeCylinder	*Cylinder);
	bool		Intersect(WeBox			*Box);
	bool		Intersect(WeVector		&Point);
};




#endif