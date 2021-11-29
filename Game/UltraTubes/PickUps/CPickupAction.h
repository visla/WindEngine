/*
	Filename:	CPickupAction.h
	Author:		Goran Bogdanovic
*/


#ifndef _CPICKUP_ACTION_H
#define _CPICKUP_ACTION_H


enum PickupActionType
{
	PickupActionInvisibility,
	PickupActionInvulnerability,
	PickupActionDoubleDamage,
	PickupActionQuadDamage
};


class CTuber;


class CPickupAction
{
protected:
	PickupActionType		ActionType;

	float					Timer;
	float					ValidityTime;
	bool					Validity;

	CTuber*					OwnerTuber;


public:
	CPickupAction();
	virtual ~CPickupAction() {}

	virtual void			Update(CTuber* Owner);

	bool					Valid();

	void					SetValidityTime(float Time);
	float					GetValidityTime();

	float					GetTimeRemaining();

	PickupActionType		GetActionType();

};

#endif
