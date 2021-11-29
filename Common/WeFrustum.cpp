/*
     WeFrustum.cpp


	 Autor: Nebojsa Vislavski
	 Date: 26.10.2005.
	 Version: 1.0
	 Platform: Linux/Win32

	 This is frustum class. Frustum is used for camera.     

*/

#include <WeFrustum.h>
#include <stdio.h>



bool WeFrustum::Intersection(WeBox &Box)
{
/*
	if (!Sphere.Intersect(&Box)) return false;

	// make sphere of box and call intersecion with sphere
	WeSphere s;
	float HalfSize = Box.GetSize();
	s.Position = Box.Min + (WeVector(1,1,1)*Box.GetSize());
	s.Radius = HalfSize*HalfSize*3; // a*a+a*a+a*a this is squared radius

  WeVector V2, D;
  float E, Dsqr;

  V2 = Cone.V - Cone.A * (sqrtf(s.Radius) * ReciprocalSinHalfFOV);
  D = s.Position - V2;
  Dsqr = D3DXVec3Dot(&D,&D);
  E = D3DXVec3Dot(&Cone.A, &D);
  
  if (E > 0)
  {
    if (E*E >= Dsqr*CosHalfFOV*CosHalfFOV)
	{
	  D = s.Position - Cone.V;
      Dsqr = D3DXVec3Dot(&D,&D);
	  E = -D3DXVec3Dot(&Cone.A, &D);
	  if ((E > 0) && (E*E >= Dsqr * SinHalfFOV*SinHalfFOV))
	  {
		  if (Dsqr <= s.Radius) return true;
		   else return false;
	  } else 
	  {
        return true;
	  } 
	  
	}
  } */

  return false;
	
}


void WeFrustum::MakePlanes(WeMatrix &tmp)
{
	// left plane
	Planes[0].a = tmp._14 + tmp._11;
	Planes[0].b = tmp._24 + tmp._21;
	Planes[0].c = tmp._34 + tmp._31;
	Planes[0].d = tmp._44 + tmp._41;
	D3DXPlaneNormalize(&Planes[0], &Planes[0]);

	
	// right plane
	Planes[1].a = tmp._14 - tmp._11;
	Planes[1].b = tmp._24 - tmp._21;
	Planes[1].c = tmp._34 - tmp._31;
	Planes[1].d = tmp._44 - tmp._41;
	D3DXPlaneNormalize(&Planes[1], &Planes[1]);

	// bottom
	Planes[2].a = tmp._14 + tmp._12;
	Planes[2].b = tmp._24 + tmp._22;
	Planes[2].c = tmp._34 + tmp._32;
	Planes[2].d = tmp._44 + tmp._42;
	D3DXPlaneNormalize(&Planes[2], &Planes[2]);

	// top
	Planes[3].a = tmp._14 - tmp._12;
	Planes[3].b = tmp._24 - tmp._22;
	Planes[3].c = tmp._34 - tmp._32;
	Planes[3].d = tmp._44 - tmp._42;
	D3DXPlaneNormalize(&Planes[3], &Planes[3]);


	// near
	Planes[4].a = tmp._13;
	Planes[4].b = tmp._23;
	Planes[4].c = tmp._33;
	Planes[4].d = tmp._43;
	D3DXPlaneNormalize(&Planes[4], &Planes[4]);


	// far
	Planes[5].a = tmp._14 - tmp._13;
	Planes[5].b = tmp._24 - tmp._23;
	Planes[5].c = tmp._34 - tmp._33;
	Planes[5].d = tmp._44 - tmp._43;	
	D3DXPlaneNormalize(&Planes[5], &Planes[5]);

	PlanesMade = true;
}

WeFrustum::WeFrustum()
{
	PlanesMade = false;
}

bool WeFrustum::Collide(WeSphere &Sphere, bool IgnoreNearFar)
{
	// make translation world matrix.
	WeMatrix trans, tmp;
	WeVector tmpv;
	D3DXVECTOR4 t4;

	//D3DXVec3Transform(&t4, &Sphere.Position, &View);
	//tmpv.x = t4.x; tmpv.y = t4.y; tmpv.z = t4.z;
	tmpv = Sphere.Position;

	//if (!PlanesMade) MakePlanes(Projection);

	// Now check to see if this sphere lies inside sphere
	float m;

	for (int i = 0; i < 6; i++)
	{			
		m = D3DXVec3Dot(&WeVector(Planes[i].a, Planes[i].b, Planes[i].c), &tmpv) + Planes[i].d;		
		if (m < -Sphere.Radius)
		{
			return false; // if it is on back side and more distant than sphere radius discar this
		}
	}
	return true;
}

bool WeFrustum::Collide(WeVector &Point, bool IgnoreNearFar)
{
	// make translation world matrix.
	WeMatrix trans, tmp;
	WeVector tmpv;
	D3DXVECTOR4 t4;

	//D3DXVec3TransformNormal(&Point, &Point, &View);
	
	//if (!PlanesMade) MakePlanes(Projection);

	for (int i = 0; i < 6; i++)
	{			
		if (D3DXPlaneDotCoord(&Planes[i], &Point) < -0.001)
		{
			return false;  // if it is some epsilon value in front of plane return true
		}
	}
	return true;
}


void WeFrustum::UpdateFrustrum(WeVector &cameraLocation, WeMatrix &view, WeMatrix &proj)
{	
	CameraLocation = cameraLocation;
	View = view;
	ViewProj = view*proj;

	if (Projection != proj) 
	{
		PlanesMade = false;
		Projection = proj;	
	}
	MakePlanes(ViewProj);
}