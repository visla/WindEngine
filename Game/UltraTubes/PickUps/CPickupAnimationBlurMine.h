/*
       File: CPickupAnimationBlurMine.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupAnimationBlurMine_h_
#define _CPickupAnimationBlurMine_h_


#include <CPickUpFactory.h>
#include <CPickUpAnimation.h>
#include <WeActor.h>


class CPickupAnimationBlurMine:public CPickUpAnimation
{
protected:
	bool FirstTime;

	int MinePictureSX;
	int MinePictureSY;
	int MinePictureSW;
	int MinePictureSH;

	int MinePictureEX;
	int MinePictureEY;
	int MinePictureEW;
	int MinePictureEH;

	float SX;
	float SY;
	float SW;
	float SH;
	float SA;
	float SAF;

	float SWF;
	float SHF;
	float MXF;
	float MYF;

	float WOffset;
	float HOffset;

public:
	CPickupAnimationBlurMine();
	~CPickupAnimationBlurMine();

	void	Tick();
	void	Render();
};



#endif
