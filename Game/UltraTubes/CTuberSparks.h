/*
		File: CTuberSparks.h
		Author: Goran Bogdanovic
*/

#ifndef _CTuberSparks_h_
#define _CTuberSparks_h_


#include <WeActor.h>

class CTuber;
class WeLight;
class WeParticleSystem;
class WeSound;
class WeTrail;
class CTuberSparks;

class CTrailSpark : public WeActor
{
protected:
	CTuberSparks				*Parent;
	WeTrail						*Trail;
	WeVector					FixedDirection;

public:
								CTrailSpark(CTuberSparks *WhereFrom, WeVector &WhereIs, WeVector &WhereTo);
								~CTrailSpark();

	void						Tick();

};


class CTuberSparks : public WeActor
{
friend class CTrailSpark;

protected:
	CTuber						*Owner;
	WeParticleSystem			*Sparks;
	WeLight						*Light;
	WeSound						*SoundScratch;

	bool						Active;
	float						DeactivationTimer;

	WeMatrix					RelativeTransMatrix;
	WeVector					RelativeVector;
	WeVector					HitNormal;

	CSmartList<CTrailSpark*>	TrailSparks;

	static WeLight				*StaticLight;


public:
	static WeParticleSystem		*StaticPS;

								CTuberSparks(CTuber *MyOwner, WeMatrix &RelMatrix, WeVector &HitNormal);
								~CTuberSparks();

	void						Update(WeMatrix &NewMatrix, WeVector &NewNormal);

	void						SetActive(bool Activate);
	bool						IsValid();

	void						Tick();

};

#endif