/*
     WeFrustum.h


	 Autor: Nebojsa Vislavski
	 Date: 26.10.2005.
	 Version: 1.0
	 Platform: Linux/Win32

	 This is frustum class. Frustum is used for camera.     

*/

#ifndef _WEFRUSTUM_H
#define _WEFRUSTUM_H

#include <WeMatrix.h>
#include <WeSphere.h>
#include <WeBox.h>
#include <WeVector.h>
#include <WePlane.h>

class WINDENGINE_API WeFrustum
{
protected:
	bool		PlanesMade;

public:	
	WeVector	CameraLocation;
	WeMatrix	View;
	WeMatrix	Projection;
	WeMatrix	ViewProj; // this is viewproj matrix gathered from camera

				WeFrustum();
  	 void		MakePlanes(WeMatrix &Matrix);
	 WePlane	Planes[6];
     bool		Intersection(WeBox &Box);	 
	 // this will check for collision between frustum and sphere.
	 // returns 0 - no collision, 1 - sphere intersects, 2 - sphere completely inside frustum
	 bool		Collide(WeSphere &Sphere, bool IgnoreNearFar = false);      
	 bool		Collide(WeVector &Point, bool IgnoreNearFar = false);      // true if inside frustum. Point is in world space
	 void		UpdateFrustrum(WeVector &cameraLocation, WeMatrix &View, WeMatrix &Projection);  // gathers viewproj matrix 

};


#endif