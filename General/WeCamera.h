/*
	      File: WeCamera.h
		  Author: Tomislav Kukic
		  Date: 12.09.2005.
		  Version: 1.0
*/




#ifndef _WECAMERA
#define _WECAMERA



#include <WeMatrix.h>
#include <WeVector.h>
#include <WindEngine.h>
#include <WeFrustum.h>



class WeBox;
class WeSphere;



class WINDENGINE_API WeCamera
{
friend class WeZone;
friend class WeWorld;

protected:
	//WeCamera(void);
	virtual ~WeCamera(void){}

	float		Fov;
	float		NearClip;
	float		FarClip;
	float		Aspect;
	WeZone		*LastZone;  // this is last active zone camera was. dont modify this 


public:
	WeCamera(void);//Public for testing

	// You should erase this frustum when you find better method for frustum culling
	WeFrustum   Frustum; // Frustum for this camera.
	WeVector	Location;
	WeVector	Direction;
	WeVector	LookAt;
	WeVector	Up;
	WeVector	Velocity;

	void		SetFov(float f);
	void		SetNearClip(float nc);
	void		SetFarClip(float fc);
	void		SetAspect(float a);

	float		GetFov() { return Fov;}
	float		GetNearClip() { return NearClip; }
	float		GetFarClip() { return FarClip; }
	float		GetAspect() {return Aspect; }


	float		OldFov;
	float		OldNearClip;
	float		OldFarClip;

	WeMatrix	View;
	WeMatrix	Projection;
	WeMatrix	ViewProj;
	WeMatrix	PrevViewProj;

	bool		UpdateProjection;

	// Orthogonal settings for camera. 
	// This is used in editors 
	bool		Orthogonal;
	float		OrthoW;
	float		OrthoH;
	// --------------------------

	virtual void Update();			

	virtual void CreateClipPlanes();
			void SetOrthogonal(bool o);

	virtual bool BoxInFrustum(WeBox &Box, bool IgnoreNearFar = false);
	virtual bool SphereInFrustum(WeSphere &Sphere, bool IgnoreNearFar = false);

};



#endif
