/*
        File: WeCylinder.cpp
		Author: Tomislav Kukic
		Date: 06.10.2005.
		Version: 1.0
		Platform: Windows
*/




#include <WeSphere.h>
#include <WeCylinder.h>




WeCylinder::WeCylinder(void)
{
	Position = WeVector(0,0,0);
	Height = 0.0f;
	Radius = 0.0f;
}


WeCylinder::WeCylinder(WeVector Position, float Radius, float Height)
{
	this->Position = Position;
	this->Radius = Radius;
	this->Height = Height;
}



bool WeCylinder::Intersect(WeCylinder* Cylinder)
{
	float tmp1 = Cylinder->GetPosition().y;
	float tmp2 = Cylinder->GetPosition().y + Cylinder->GetHeight();

	float tmp3 = this->Position.y;
	float tmp4 = this->Position.y + this->Height;

	if((tmp1 < tmp3 || tmp1 > tmp4) && 
	   (tmp2 < tmp3 || tmp2 > tmp4)) return false;

	float tmpRadius = Cylinder->GetRadius() + this->Radius;
	float tmpDistance = 0.0f;

	WeVector tmpVect = Cylinder->GetPosition() - this->Position;
	tmpDistance = D3DXVec3LengthSq(&tmpVect);

	if(tmpDistance < tmpRadius) return true;

	return false;
}



bool WeCylinder::Intersect(WeSphere* Sphere)
{
	float tmp1 = this->Position.y;
	float tmp2 = this->Position.y + this->Height;

	float tmp3 = Sphere->Position.y + Sphere->Radius;

	if(tmp3 < tmp1 || tmp3 > tmp2) return false;

	float tmpRadius = Sphere->Radius + this->Radius;
	float tmpDistance = 0.0f;

	WeVector tmpVect = Sphere->Position - this->Position;
	tmpDistance = D3DXVec3LengthSq(&tmpVect);

	if(tmpDistance < tmpRadius) return true;

	return false;
}

