/*
     WeTrail.h

	 Autor: Nebojsa Vislavski
	 Date: 28.4.2006.
	 Version: 1.0
       
*/


#ifndef _WETRAIL_H
#define _WETRAIL_H


#include <WeActor.h>
#include <WindEngine.h>
#include <WeMAterial.h>

class WINDENGINE_API WeTrail: public WeActor
{
friend class WeTrailRenderer;

protected:

	struct STrailVertex
	{
		WeVector Position;
		DWORD	 Color;
		float	 u,v;
	};


    STrailVertex		*VertexData;
	WORD				*IndexData;
	int					VertexDataCount;
	int					IndexDataCount;
	int					InsertPlace;
	int					InsertIndexPlace;
	float				Counter;



public:
						WeTrail();
						~WeTrail();

	WeMaterial			*Material;
	String				MaterialName;
	int					MaxSegments;			// maximum number of segments
	float				SegmentWidth;			// width of one segment in meters
	float				StartFadeFactor;		// 0 - means start fading immediately and 1.0 means never fade. 0.5 means start fade on hlaf segment
	int					SegmentsCount;			// how many segments
	WeColor				SegmentColor;			// color of segment
	float				SegmentCreationTime;	// how many seconds to new segment
	

	void				Initialize();			// must be called when params are set
	void				Tick();
	void				Reset(); 
	void				Serialize(WeFileStream &FS);
	void				Deserialize(WeFileStream &FS);
	bool				IntersectFrustum(WeFrustum *frustum);

	void				Clone(WeActor *Destination, bool DeepCopy = false); 

	void				OnRender(WeCamera* Cam);
	virtual int			GetIndexSize();
	virtual	int			GetFacesCount();

};


#endif
