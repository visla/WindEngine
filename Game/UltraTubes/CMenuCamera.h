/*
File: CMenuCamera.h
Author: Goran Bogdanovic
*/



#ifndef _CMenuCamera_h_
#define _CMenuCamera_h_


#include <WeCamera.h>

class WePathNode;
class WeLight;

class CMenuCamera : public WeCamera
{
public:
	CMenuCamera();
	~CMenuCamera();

	void			Update();
};


#endif
