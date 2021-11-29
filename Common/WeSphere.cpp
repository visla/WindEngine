/*
	    File: WeSphere.cpp
		Author: Tomislav Kukic
		Date: 06.10.2005.
		Version: 1.0
		Platform: Windows
*/



#include <WeCylinder.h>
#include <WeSphere.h>
#include <WeBox.h>



WeSphere::WeSphere(void)
{
	Position = WeVector(0,0,0);
	Radius = 0.0f;
}


WeSphere::WeSphere(WeVector Position, float Radius)
{
	this->Position = Position;
	this->Radius = Radius;
}



bool WeSphere::Intersect(WeSphere* Sphere)
{
	float tmpRadius = Sphere->Radius + this->Radius;
	float tmpDistance = 0.0f;

	WeVector tmpVector = Sphere->Position - this->Position;
	tmpDistance = D3DXVec3LengthSq(&tmpVector);

	if(tmpDistance < tmpRadius*tmpRadius) return true;

	return false;
}


bool WeSphere::Intersect(WeCylinder* Cylinder)
{
	float tmp1 = Cylinder->GetPosition().y;
	float tmp2 = Cylinder->GetPosition().y + Cylinder->GetHeight();

	float tmp3 = this->Position.y + this->Radius;

	if(tmp3 < tmp1 || tmp3 > tmp2) return false;

	float tmpRadius = this->Radius + Cylinder->GetRadius();
	float tmpDistance = 0.0f;

	WeVector tmpVect = this->Position - Cylinder->GetPosition();
	tmpDistance = D3DXVec3LengthSq(&tmpVect);

	if(tmpDistance < tmpRadius*tmpRadius) return true;

	return false;
}


bool WeSphere::Intersect(WeVector &Point)
{
	float DistSq;
	WeVector tmp = Position - Point;
	DistSq = D3DXVec3LengthSq(&tmp);

	if (DistSq < Radius*Radius) return true;
	return false;
}

bool WeSphere::Intersect(WeBox *Box)
{
	// if just one point of box is in sphere return true
	bool Res = false;
	WeVector tmpLoc;

	tmpLoc = Box->Min;	
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.x = Box->Max.x; 
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.y = Box->Max.y;
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.x = Box->Max.x; tmpLoc.y = Box->Max.y;
	if (Intersect(tmpLoc)) return true;


	// back line

	tmpLoc = Box->Min;
	tmpLoc.z = Box->Max.z;
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.z = Box->Max.z; tmpLoc.x = Box->Max.x;
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.z = Box->Max.z; tmpLoc.y = Box->Max.y;
	if (Intersect(tmpLoc)) return true;

	tmpLoc = Box->Min;
	tmpLoc.z = Box->Max.z; tmpLoc.x = Box->Max.x; tmpLoc.y = Box->Max.y;
	if (Intersect(tmpLoc)) return true;

	// if sphere is contained in box than check if center is inside box
	if (Box->PointInBox(Position)) return true;

	return false;
}
