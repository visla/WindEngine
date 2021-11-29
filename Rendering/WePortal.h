/*
		File: WePortal.h
		Author: Tomislav Kukic
		Date: 05.07.2006
		Version: 1.0

		This is used for portal rendering. Portal will only be quad shaped. Custom
		shaped portals are not implemented in this class.		
*/




#ifndef _WEPORTAL_H_
#define _WEPORTAL_H_


#include <SmartList.h>
#include <WeVector.h>
#include <WeActor.h>
#include <WeFileStream.h>
#include <WindEngine.h>

class WeFrustum;
class WeCamera;


class WINDENGINE_API WePortal: public WeActor
{
protected:
	int						PortalVer;
	WeVector				PrevScale;


public:
							WePortal();
							~WePortal();

	WeZone*					Zone1;
	WeZone*					Zone2;

	String					Zone1Name;
	String					Zone2Name;

	float					SizeX;
	float					SizeY;
			
	int						Zone1ID;
	int						Zone2ID;

	bool					TwoWayPortal;

	CSmartList<WeVector>	PortalVertices;			// dynamicly created when initalizing
	WeVector				Vertices[4];
	CSmartList<WeVector>	ClippedPortalVertices;     // dynamicly created when needed

	void					Initialize();
	void					Clone(WeActor* Destination, bool DeepCopy = false);
	bool					ClipPortal(WeFrustum* frustum, WeFrustum *clippedFrustum); // makes new frustum. returns false if there is no intersection

	void					Serialize(WeFileStream &FS);
	void					Deserialize(WeFileStream &FS);
	
	void					Tick();
};




#endif

