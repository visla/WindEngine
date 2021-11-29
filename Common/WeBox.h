/*

     WeBox.h

	 Autor: Nebojsa Vislavski
	 Date: 24.10.2005.
	 Version: 1.0
	 Platform: Linux/Win32


	 Serbian: Ovo je klasa koja predstavlja box


*/

#ifndef _WEBOX_H
#define _WEBOX_H

#include <WeVector.h>
#include <WindEngine.h>

class WeSphere;


class WINDENGINE_API WeBox
{
protected:
    
public:
    WeVector    Min;  // lower-left
	WeVector	Max;  // upper-right

	WeBox();
	WeBox(WeVector &location, float size);
	WeBox(WeVector &Min, WeVector &Max);

	bool		PointInBox(WeVector &Point);
	bool		Intersect(WeSphere *Sphere);
	bool		Intersect(WeVector *Point);
	bool		Intersect(WeBox    *Box);
	bool		Contains(WeBox *Box);

	void		Move(WeVector &DeltaMove);

	float		GetSize() { return Max.x - Min.x; }
	
};



#endif