/*
	      File: WeCamera.cpp
		  Author: Tomislav Kukic
		  Date: 12.09.2005.
		  Version: 1.0
*/



#include <d3dx9.h>
#include <WeCamera.h>
#include <WeRenderDevice.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WeBox.h>



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeCamera::WeCamera(void)
{
	Location = WeVector(0,0,0);
	Direction = WeVector(0,0,1);
	Up = WeVector(0,1,0);

	LookAt = Location + Direction;

	Fov =			D3DX_PI/4;
	NearClip =		1;
	FarClip =		1000;

	OldFov		=	D3DX_PI/4;
	OldNearClip =	1;
	OldFarClip	=	1000;
	Aspect		=	1.33f;
	Orthogonal	=	false;
	OrthoW		=	100;
	OrthoH		=	100;
	LastZone	=	NULL;

	Projection = WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip);

	UpdateProjection = false;

}

void WeCamera::SetOrthogonal(bool o)
{
	Orthogonal = o;
	UpdateProjection = true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeCamera::Update()
{
	PrevViewProj = ViewProj;

	LookAt = Location + Direction;
	View = WeRenderDevice::Instance()->BuildViewMatrix(&Location, &LookAt, &Up);

	if (UpdateProjection)
	{
		if (!Orthogonal)
		{
			Projection = WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip, Aspect);
		}
		else
		{
			D3DXMatrixOrthoLH(&Projection,OrthoW,OrthoH, NearClip, FarClip);
		}
	}

	D3DXMatrixMultiply(&ViewProj, &View, &Projection);
	Frustum.UpdateFrustrum(Location, View, Projection);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeCamera::CreateClipPlanes()
{
	Frustum.MakePlanes(ViewProj);
	//Frustum.MakePlanes(Projection);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeCamera::BoxInFrustum(WeBox &Box, bool IgnoreNearFar)
{
	return Frustum.Intersection(Box);
	
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeCamera::SphereInFrustum(WeSphere &Sphere, bool IgnoreNearFar)
{
	return Frustum.Collide(Sphere, IgnoreNearFar);
}



void WeCamera::SetFov(float f)
{
	OldFov = Fov; 
	Fov = f; 
	if (!Orthogonal)
		WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip, Aspect);
}


void WeCamera::SetNearClip(float nc)
{
   OldNearClip = NearClip; NearClip = nc; 
   if (!Orthogonal)
		WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip, Aspect);
   else
   {
	   D3DXMatrixOrthoLH(&Projection,OrthoW,OrthoH, NearClip, FarClip);
   }

}

void WeCamera::SetFarClip(float fc)
{
	OldFarClip = FarClip; FarClip = fc; 
	if (!Orthogonal)
		WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip, Aspect);
	else
	{
		D3DXMatrixOrthoLH(&Projection,OrthoW,OrthoH, NearClip, FarClip);
	}

}

void WeCamera::SetAspect(float a)
{
	Aspect = a; 
	if (!Orthogonal)
		WeRenderDevice::Instance()->BuildProjMatrix(Fov, NearClip, FarClip, a);
}