/*
        File: WeCylinder.h
		Author: Tomislav Kukic
		Date: 06.10.2005.
		Version: 1.0
		Platform: Windows
*/




#ifndef _WECYLINDER
#define _WECYLINDER



#include <WeVector.h>
#include <WindEngine.h>



class WeSphere;


class WINDENGINE_API WeCylinder
{
protected:
	WeVector	Position;
	float		Height;
	float		Radius;

public:
	WeCylinder(WeVector Position, float Radius, float Height);
	WeCylinder(void);
	~WeCylinder(void){}


	void		SetPosition(WeVector _Position){ Position = _Position; }
	void		SetHeight(float _Height){ Height = _Height; }
	void		SetRadius(float _Radius){ Radius = _Radius; }

	WeVector	GetPosition(){ return Position; }
	float		GetHeight(){ return Height; }
	float		GetRadius(){ return Radius; }

	bool		Intersect(WeCylinder* Cylinder);
	bool		Intersect(WeSphere* Sphere);
};




#endif
