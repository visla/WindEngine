/*
      File: WePrefab.h
	  Author: Tomislav Kukic
	  Date: 13.09.2005.
	  Version: 1.0


	  Serbian: Voditi racuna da se Prefabi ne brisu rucno. Jel se nalaze slozeni u Octree-u
	  odnosno u statickoj sceni. Ako ih obrisete gubite mesto na koje pokazuje element iz 
	  Octree-a i eto greske.
*/





#ifndef _WEPREFAB
#define _WEPREFAB


#include <WeActor.h>
#include <WindEngine.h>


class WeWorld;
class WeCamera;


class WINDENGINE_API WePrefab: public WeActor
{
protected:
	
	~WePrefab();

public:
	WePrefab();

	void SetWorld(WeWorld* World);
	void Tick();

	
	void PreRender(WeCamera* Cam);
	void PostRender(WeCamera* Cam);
	bool IntersectFrustum(WeFrustum *frustum);


};



#endif
