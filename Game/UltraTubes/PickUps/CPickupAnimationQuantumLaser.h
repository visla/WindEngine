/*
       File: CPickupAnimationQuantumLaser.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickupAnimationQuantumLaser_h_
#define _CPickupAnimationQuantumLaser_h_


#include <CPickUpFactory.h>
#include <CPickUpAnimation.h>
#include <WeActor.h>


class CPickupAnimationQuantumLaser:public CPickUpAnimation
{
protected:
	bool FirstTime;

	int WeaponPictureSX;
	int WeaponPictureSY;
	int WeaponPictureSW;
	int WeaponPictureSH;

	int WeaponPictureEX;
	int WeaponPictureEY;
	int WeaponPictureEW;
	int WeaponPictureEH;

	float WSX;
	float WSY;
	float WSW;
	float WSH;
	float SA;
	float SAF;

	float WSWF;
	float WSHF;
	float MXF;
	float MYF;

	float WOffset;
	float HOffset;

public:
	CPickupAnimationQuantumLaser();
	~CPickupAnimationQuantumLaser(){}

	void	Tick();
	void	Render();
};



#endif
