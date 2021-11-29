/*
       WeRaycastHit.h

	   Autor: Nebojsa Vislavski
	   Date: 13.10.2005.
	   Version: 1.0
	   Platform: Any


       This is slightly modified and improved NxRacastHit structure from NovodeX 
	   physics

*/


#ifndef _WERAYCASTHIT_H
#define _WERATCASTHIT_H


#include <WeVector.h>

class WeActor;

class WeRaycastHit
{
protected:
public:
	WeActor *ActorHit;
	WeVector WorldImpact;
	WeVector WorldNormal;

	int		 FaceID; // faceID that was hit
	int		 MaterialIndex; // index of material that is hit

	float	 Distance;  // distance from object hit
	float	 u;   // barycentric coords
	float    v;  

};


#endif