/*  

    CMapEditorCamera.h

	Autor: Nebojsa Vislavski
	Date: 12.12.2005.
	Version: 1.0

	This is camera used in map editor

*/

#ifndef _CMAPEDITORCAMERA_H
#define _CMAPEDITORCAMERA_H


#include <WeCamera.h>


#define CAMERA_FRONT	 1
#define CAMERA_TOP		 2
#define CAMERA_LEFT		 3
#define CAMERA_FREE		 4

#define CAMERA_MOVELEFT		1
#define CAMERA_MOVERIGHT	2
#define CAMERA_MOVEUP		3
#define CAMERA_MOVEDOWN		4


class CMapEditorCamera: public WeCamera
{
protected:
	int ViewMode;
	WeVector OldLocation;
	WeVector OldDirection;
	WeVector OldUp;

	WeVector OldTopLocation;
	WeVector OldTopDirection;
	WeVector OldTopUp;
	float	 OldTopOrthoW;
	float	 OldTopOrthoH;

	WeVector OldFrontLocation;
	WeVector OldFrontDirection;
	WeVector OldFrontUp;
	float	 OldFrontOrthoW;
	float	 OldFrontOrthoH;

	WeVector OldLeftLocation;
	WeVector OldLeftDirection;
	WeVector OldLeftUp;
	float	 OldLeftOrthoW;
	float	 OldLeftOrthoH;


	float	Yaw;
	float	Pitch;

	float	Speed;
	float   FreeCameraSpeed;

	bool	Ref[5]; // x,y,z,yaw,pitch
	float	RefX;
	float	RefY;
	float	RefZ;
	float	RefYaw;
	float	RefPitch;

public:
	CMapEditorCamera();

	void MoveCamera(int dmx, int dmy, int mx, int my, bool Left, bool Right, int DRoll);
	void MoveCamera(int MoveType);

	int GetViewMode() { return ViewMode; }
	void SetViewMode(int mode);

	
	int ScrollSpeed;
	float fScrollSpeed;
	void SetScrollSpeed(int NewScrollSpeed);
};


#endif
