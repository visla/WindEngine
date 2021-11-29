/*
       File: CPickUpAnimation.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CPickUpAnimation_h_
#define _CPickUpAnimation_h_


#include <WeActor.h>
#include <CPickUpFactory.h>
#include <SmartList.h>
#include <CWeaponDescription.h>

#define COLOR_PRIMARY		WeColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_SECONDARY		WeColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_UTILITY		WeColor(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_COIN			WeColor(1.0f, 1.0f, 0.0f, 1.0f)


class WeTexture;


class CPickUpAnimation
{
protected:
	String			UberText;
	WeColor			UberTextColor;

	void			OutputUberText();


public:
	static CSmartList<CPickUpAnimation*> PickupAnimations;

	CPickUpAnimation();
	~CPickUpAnimation(){}

	bool AnimatingMine;
	bool AnimatingWeapon;

	WeTexture* Mine;
	WeTexture* Weapon;

	virtual void	Tick();
	virtual void	Render();

	static WeTexture*		GetAnimTexture(WEAPONTYPE ID);

};




#endif
