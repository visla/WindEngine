/*
       File: CWeapon.h
	   Author: Nebojsa Vislavski
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CWeapon_h_
#define _CWeapon_h_


#include <CWeaponFabric.h>
#include <CString.h>
#include <WeActor.h>
#include <CWeaponDescription.h>

#define BOLT_LOCK_DISTANCE		500.0f
#define BOLT_LOCK_MINDOT		0.995f

#define PRIMARY_PHYS_BOX		WeVector(1.0f, 1.0f, 1.0f)
#define PRIMARY_CCD_BOX_MIN		WeVector(-0.8f, -0.8f, -0.8f)
#define PRIMARY_CCD_BOX_MAX		WeVector(0.8f, 0.8f, 0.8f)
#define SECONDARY_PHYS_BOX		WeVector(1.5f, 1.5f, 1.5f)
#define SECONDARY_CCD_BOX_MIN	WeVector(-1.3f, -1.3f, -1.3f)
#define SECONDARY_CCD_BOX_MAX	WeVector(1.3f, 1.3f, 1.3f)

class CTuber;
class WeWorld;


class CWeapon: public WeActor
{
protected:
	bool				Dying;

public:
								CWeapon();
						virtual ~CWeapon();

	WEAPONTYPE			WeaponID;
	CTuber*				WeaponOwner;
	WeWorld*			WeaponParent;
	CWeaponDescription	WeaponDesc;

	float				MineLockFactor;

	CTuber*				HomingTarget;

	static float		GetTimeToActivate(WEAPONTYPE ID);
	static float		GetEffectiveRadius(WEAPONTYPE ID);

	void				Tick();

	virtual void		OnHit(WeActor *Victim) {}
	virtual void		OnNotify(WeActor *Victim) {}

	bool				IsPrimaryWeapon();
	bool				IsSecondaryWeapon();

};





#endif
