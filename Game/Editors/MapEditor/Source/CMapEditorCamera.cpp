/*  

    CMapEditorCamera.cpp

	Autor: Nebojsa Vislavski
	Date: 12.12.2005.
	Version: 1.0

	This is camera used in map editor

*/

#include <CMapEditorCamera.h>
#include <CMapEditorWindow.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeActor.h>



CMapEditorCamera::CMapEditorCamera()
{
	Location = WeVector(0,0,-50.0);
	Direction = WeVector(0,0,1.0);
	Up = WeVector(0,1.0,0);
	FarClip = 5000;

	OldTopLocation = WeVector(0, 25000, 0);
	OldTopDirection = WeVector(0,-1,0);
	OldTopUp = WeVector(0,0,1);
	OldTopOrthoW = 100;
	OldTopOrthoH = 100;

	OldFrontLocation= WeVector(0, 0, -25000);
	OldFrontDirection = WeVector(0, 0, 1);
	OldFrontUp = WeVector(0, 1, 0);
	OldFrontOrthoW = 100;
	OldFrontOrthoH = 100;

	OldLeftLocation= WeVector(-25000, 0, 0);
	OldLeftDirection = WeVector(1, 0, 0);
	OldLeftUp = WeVector(0, 1, 0);
	OldLeftOrthoW = 100;
	OldLeftOrthoH = 100;

	ScrollSpeed = 100;
	fScrollSpeed = 1.0f;

	Speed = 10; // 10 pixels is meter
	FreeCameraSpeed = 1.0f;

	// used to modify direction
	Yaw = 0;
	Pitch = 0; 

	ViewMode = CAMERA_FREE;
}


void CMapEditorCamera::SetViewMode(int mode)
{
	if (ViewMode == mode) return;


	if (ViewMode == CAMERA_FREE)
	{
		OldLocation = Location;
		OldDirection = Direction;
		OldUp = Up;
		NearClip = (1.0f);
		FarClip = (5000.0f);
	}

	else if (ViewMode == CAMERA_TOP)
	{
		OldTopLocation = Location;
		OldTopDirection = Direction;
		OldTopUp = Up;
		OldTopOrthoH = OrthoH;
		OldTopOrthoW = OrthoW;
		NearClip = (1.0f);
		FarClip = (50000.0f);
	}

	else if (ViewMode == CAMERA_FRONT)
	{
		OldFrontLocation = Location;
		OldFrontDirection = Direction;
		OldFrontUp = Up;
		OldFrontOrthoH = OrthoH;
		OldFrontOrthoW = OrthoW;
		NearClip = (1.0f);
		FarClip = (50000.0f);
	}

	else if (ViewMode == CAMERA_LEFT)
	{
		OldLeftLocation = Location;
		OldLeftDirection = Direction;
		OldLeftUp = Up;
		OldLeftOrthoH = OrthoH;
		OldLeftOrthoW = OrthoW;
		NearClip = (1.0f);
		FarClip = (50000.0f);
	}


    ViewMode = mode;

	if (ViewMode == CAMERA_FREE)
	{
		Location = OldLocation;
		Direction = OldDirection;
		SetOrthogonal(false);
		Up = OldUp;
		NearClip = (1.0f);
		FarClip = (5000.0f);
		WeRenderDevice::Instance()->SetUseMaxRenderingDistance(true);
	}	

	else if (ViewMode == CAMERA_TOP)
	{
		Location = OldTopLocation;
		Direction = OldTopDirection;
		Up = OldTopUp;
		OrthoW = OldTopOrthoW;
		OrthoH = OldTopOrthoH;		
		SetOrthogonal(true);
		NearClip = (1.0f);
		FarClip = (50000.0f);
		WeRenderDevice::Instance()->SetUseMaxRenderingDistance(false);
	}	

	else if (ViewMode == CAMERA_FRONT)
	{
		Location = OldFrontLocation;
		Direction = OldFrontDirection;
		Up = OldFrontUp;
		OrthoW = OldFrontOrthoW;
		OrthoH = OldFrontOrthoH;		
		SetOrthogonal(true);
		NearClip = (1.0f);
		FarClip = (50000.0f);
		WeRenderDevice::Instance()->SetUseMaxRenderingDistance(false);
	}	

	else if (ViewMode == CAMERA_LEFT)
	{
		Location = OldLeftLocation;
		Direction = OldLeftDirection;
		Up = OldLeftUp;
		OrthoW = OldLeftOrthoW;
		OrthoH = OldLeftOrthoH;		
		SetOrthogonal(true);
		NearClip = (1.0f);
		FarClip = (50000.0f);
		WeRenderDevice::Instance()->SetUseMaxRenderingDistance(false);
	}	
}



void CMapEditorCamera::MoveCamera(int MoveType)
{
	WeVector temp;
	D3DXVec3Cross(&temp,&Up,&Direction);
	D3DXVec3Normalize(&temp,&temp);

	if(ViewMode == CAMERA_TOP && MoveType != CAMERA_MOVELEFT && MoveType != CAMERA_MOVERIGHT)
		temp = WeVector(0,0,1);
	else if(ViewMode == CAMERA_FRONT && MoveType != CAMERA_MOVELEFT && MoveType != CAMERA_MOVERIGHT)
		temp = WeVector(0,1,0);
	else if(ViewMode == CAMERA_LEFT && MoveType != CAMERA_MOVELEFT && MoveType != CAMERA_MOVERIGHT)
		temp = WeVector(0,1,0);

	float speed = 10.0f;
	if (EditorWindow->Shift)
		speed = 50.0f;
	speed = speed * WeTimer::Instance()->GetDeltaTimeF() * 30.0f;

	switch(MoveType)
	{
	case CAMERA_MOVEUP:
		if(ViewMode == CAMERA_FREE)
			Location = Location + Direction * speed * fScrollSpeed;
		else
			Location = Location + temp * speed * fScrollSpeed;
		break;

	case CAMERA_MOVEDOWN:
		if(ViewMode == CAMERA_FREE)
			Location = Location - Direction * speed * fScrollSpeed;
		else
			Location = Location - temp * speed * fScrollSpeed;
		break;

	case CAMERA_MOVELEFT:
		Location = Location - temp * speed * fScrollSpeed;
		break;

	case CAMERA_MOVERIGHT:
		Location = Location + temp * speed * fScrollSpeed;
		break;
	}
}




void CMapEditorCamera::MoveCamera(int dmx, int dmy, int mx, int my, bool Left, bool Right, int DRoll)
{
	if (!Left && !Right && !DRoll) 
	{
		Ref[0] = Ref[1] = Ref[2] = Ref[3] = Ref[4] = false;
		RefX = 0;
		RefY = 0;
		RefZ = 0;
		RefYaw = 0;
		RefPitch = 0;
	}
	else 
	{
		if (!Ref[0]) { RefX = Location.x; Ref[0] = true; }
		if (!Ref[1]) { RefY = Location.y; Ref[1] = true; }
		if (!Ref[2]) { RefZ = Location.z; Ref[2] = true; }
		if (!Ref[3]) { RefYaw = Yaw; Ref[3] = true; }
		if (!Ref[4]) { RefPitch = Pitch; Ref[4] = true; }
	}
	

	if (ViewMode == CAMERA_FREE)
	{
		// Rotation of camera
		if (Right && !Left)
		{
			Yaw = RefYaw + (float)dmx / 500.0f;
			Pitch = RefPitch + (float)dmy / 500.0f;
			if (Pitch < (-D3DX_PI/2)+0.009f) Pitch = (-D3DX_PI/2)+0.009f;
			if (Pitch > (D3DX_PI/2)-0.009f) Pitch = (D3DX_PI/2)-0.009f;


			WeMatrix m;
			D3DXMatrixRotationYawPitchRoll(&m, Yaw, Pitch, 0);
			D3DXVec3TransformNormal(&Direction, &WeVector(0,0,1), &m);
		}		
	}

	if (ViewMode != CAMERA_FREE)
	{
		int D;
		if (EditorWindow->Shift) D = 15;
		else D = 1;

		if (DRoll > 0)
		{
			OrthoW -= D;
			OrthoH -= D;
			if (OrthoW < 1) OrthoW = 1;
			if (OrthoH < 1) OrthoH = 1;
		}

		if (DRoll < 0)
		{
			OrthoW += D;
			OrthoH += D;
		}

	}

	float tmpSpeed, tmpSpeed2;

	if (EditorWindow->Shift) 
	{
		tmpSpeed = FreeCameraSpeed * 50 * fScrollSpeed;
		tmpSpeed2 = Speed / 5;
	}
	else 
	{
		tmpSpeed = FreeCameraSpeed * 10 * fScrollSpeed;
		tmpSpeed2 = Speed;
	}


	if (ViewMode == CAMERA_FREE)
	{
		static int refmx, refmy;

		if (Left && !Right)
		{			
			WeVector Strafe;

			D3DXVec3Cross(&Strafe, &Direction, &Up);
			Strafe.y = 0.0f;

			//if (refmy > my)	Location += Direction*tmpSpeed * fScrollSpeed;
			//if (refmy < my)	Location -= Direction*tmpSpeed * fScrollSpeed;

			//if (refmx > mx)	Location += Strafe*tmpSpeed  * fScrollSpeed;
			//if (refmx < mx)	Location -= Strafe*tmpSpeed  * fScrollSpeed;

		}
		// Up/ Down moving
		if (Right && Left)
		{
			Location.y = RefY - (float)dmy / tmpSpeed2 * fScrollSpeed;			
		}
		refmx = mx;
		refmy = my;

		if (EditorWindow->OrbitTarget)
		{
			D3DXVec3Subtract(&Direction, &EditorWindow->OrbitTarget->Location, &Location);
			D3DXVec3Normalize(&Direction, &Direction);
		}
	}

	if (ViewMode == CAMERA_TOP)
	{
		if (Left && !Right) 
		{	
			//Location.x = RefX - (float)dmx / tmpSpeed2 * fScrollSpeed;
			//Location.z = RefZ + (float)dmy / tmpSpeed2 * fScrollSpeed;
		}
		// Up/ Down moving
		if (Right && Left)
		{
			Location.y = RefY - (float)dmy / tmpSpeed2 * fScrollSpeed;			
		}
	}
	if (ViewMode == CAMERA_LEFT)
	{
		if (Left && !Right) 
		{	
			//Location.z = RefZ + (float)dmx / tmpSpeed2 * fScrollSpeed;
			//Location.y = RefY + (float)dmy / tmpSpeed2 * fScrollSpeed;
		}
		// Up/ Down moving
		if (Right && Left)
		{
			Location.x = RefX - (float)dmy / tmpSpeed2 * fScrollSpeed;			
		}
	}
	if (ViewMode == CAMERA_FRONT)
	{
		if (Left && !Right) 
		{	
			//Location.x = RefX - (float)dmx / tmpSpeed2 * fScrollSpeed;
			//Location.y = RefY + (float)dmy / tmpSpeed2 * fScrollSpeed;
		}
		// Up/ Down moving
		if (Right && Left)
		{
			Location.z = RefZ - (float)dmy / tmpSpeed2 * fScrollSpeed;
		}
	}

}


void CMapEditorCamera::SetScrollSpeed(int NewScrollSpeed)
{
	if (NewScrollSpeed < 1)
		NewScrollSpeed = 1;
	ScrollSpeed = NewScrollSpeed;
	fScrollSpeed = ScrollSpeed / 100.0f;
}

