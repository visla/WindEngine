/*
       File: CNodeCamera.h
	   Author: Goran Bogdanovic
*/



#ifndef _CNodeCamera_h_
#define _CNodeCamera_h_


#include <WeCamera.h>

class WePathNode;
class WeLight;

class CNodeCamera : public WeCamera
{
protected:
	float			LerpS;
	float			LookDirectionSpeed;
	WeVector		MoveDirection;
	
	WeLight			*Light;

public:
    				CNodeCamera();
					~CNodeCamera();

	float			Speed;

	WePathNode		*Node;
	WePathNode		*NextNode;

	void			Update();
};


#endif
