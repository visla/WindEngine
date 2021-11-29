/*
		File: WeZone.h
		Author: Tomislav Kukic
		Date: 05.07.2006
		Version: 1.0
*/



#ifndef _WEZONE_H_
#define _WEZONE_H_


#include <WeActor.h>
#include <WeFrustum.h>
#include <WeBox.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeRenderTree.h>


class WeWorld;
class WePortal;


class WINDENGINE_API WeZone: public WeActor
{
friend class WeWorld;

protected:
	int						ZoneVer;
	int						ObjectsCount; // how many object is in this zone. This is only statistical variable you can read
	WeVector				PrevScale;
	WeVector				OrigScale;

	void					MakeRenderTree();  // called by world when preparing zones rendering

public:
							WeZone();
							~WeZone();

	WeBox					WorldBox;  // this is box in world space
	WeBox					Box;
	CSmartList <WeFrustum>	Frustums;  // frustums used to cull geometry. Dynamicly used 
	CSmartList<WePortal*>	Portals;   // portals in this zone. Dynamicly assigned when loading
	CSmartList <WeActor*>	Actors;    // list of actors in this zone

	int						UID;

	WeRenderTree*			RenderTree;
	int						UsedFrame;
	
	


	bool					AddActor(WeActor *actor); // this will add actor to the zone. If it is prefab it will go down render tree else it will not. 
													  // Returns false if actor is not accepted
	int						GetObjectsCount() { return ObjectsCount; }
	void					Initialize();
	void					Clone(WeActor* Destination, bool DeepCopy = false);
	void					Tick();
	void					Serialize(WeFileStream &FS);
	void					Deserialize(WeFileStream &FS);
	bool					IntersectFrustum(WeFrustum *frustum);
};




#endif
