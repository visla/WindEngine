/*

     WeBox.cpp

	 Autor: Nebojsa Vislavski
	 Date: 24.10.2005.
	 Version: 1.0
	 Platform: Linux/Win32


	 Sebian: Ovo je klasa koja predstavlja box


*/


#include <WeBox.h>
#include <WeSphere.h>
#include <WeVector.h>



WeBox::WeBox()
{
	
    Min = WeVector(0,0,0);
	Max = WeVector(0,0,0);
}

WeBox::WeBox(WeVector &Min_, WeVector &Max_)
{
	Max = Max_;
	Min = Min_;
}

WeBox::WeBox(WeVector &location, float size)
{
	Min.x = location.x - size;
	Min.y = location.y - size;
	Min.z = location.z - size;

	Max.x = location.x + size;
	Max.y = location.y + size;
	Max.z = location.z + size;
}






bool WeBox::Intersect(WeBox *Box)
{
	if (Max.x < Box->Min.x) return false;
	if (Min.x > Box->Max.x) return false;

	if (Max.y < Box->Min.y) return false;
	if (Min.y > Box->Max.y) return false;

	if (Max.z < Box->Min.z) return false;	
	if (Min.z > Box->Max.z) return false;

	return true;
}

bool WeBox::Contains(WeBox *Box)
{
	if (Min.x <= Box->Min.x && Max.x >= Box->Max.x &&
		Min.y <= Box->Min.y && Max.y >= Box->Max.y &&
		Min.z <= Box->Min.z && Max.z >= Box->Max.z) return true;

	return false;

}


bool WeBox::Intersect(WeSphere *Sphere)
{	
	return Sphere->Intersect(this);
}



bool WeBox::Intersect(WeVector *Point)
{
	if (Point->x >= Min.x && Point->x <= Max.x &&
		Point->y >= Min.y && Point->y <= Max.y &&
		Point->z >= Min.z && Point->z <= Max.z)
	{
		return true;
	}
	return false;
}

bool WeBox::PointInBox(WeVector &Point)
{
	if (Point.x >= Min.x && Point.x <= Max.x &&
		Point.y >= Min.y && Point.y <= Max.y &&
		Point.z >= Min.z && Point.z <= Max.z)
	{
		return true;
	}
	return false;
}


void WeBox::Move(WeVector &DeltaMove)
{
	Min += DeltaMove;
	Max += DeltaMove;
}

