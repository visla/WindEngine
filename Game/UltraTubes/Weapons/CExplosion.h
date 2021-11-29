/*
       File: CExplosion.h
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/




#ifndef _CExplosion_h_
#define _CExplosion_h_


#include <WeActor.h>
#include <CWeaponDescription.h>

class CTuber;

class CExplosion: public WeActor
{
protected:
	float			Damage;
	CTuber			*Pyromaniac;
	WEAPONTYPE		Creator;

	void			AddExplosiveRefraction(float Size, float Life = 0.0f);

	void			LogicCalc(CTuber *Victim, float DamageTaken);

	void			AddDistantExplosion();

public:
	CExplosion();
	~CExplosion();

	void			Tick();

	virtual void	Affect(CTuber *Victim);
	virtual void	Affect(CTuber *Victim, float SpecificDamage);
	virtual void	OnNotify(WeActor *Victim) {};
};



#endif
