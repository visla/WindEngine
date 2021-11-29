/*
     WeRadialTrail.h

	 Autor: Nebojsa Vislavski
	 Date: 16.8.2006.
	 Version: 1.0
       
*/


#ifndef _WeRadialTrail_H
#define _WeRadialTrail_H


#include <WeTrail.h>
#include <WindEngine.h>
#include <WeMAterial.h>

class WINDENGINE_API WeRadialTrail: public WeTrail
{
friend class WeTrailRenderer;

protected:

	//struct STrailVertex
	//{
	//	WeVector Position;
	//	DWORD	 Color;
	//	float	 u,v;
	//};


    //STrailVertex		*VertexData;
	//WORD				*IndexData;
	WeVector			*SegmentLocation;  // this location of segment
	/*int					VertexDataCount;
	int					IndexDataCount;
	int					InsertPlace;
	int					InsertIndexPlace;
	float				Counter;
	*/
	int					RadialTrailVer;
	int					RingVerticesCount;



public:
						WeRadialTrail();
						~WeRadialTrail();

	//WeMaterial			*Material;
	//String				MaterialName;
	//int					MaxSegments;			// maximum number of segments
	//float				SegmentWidth;			// width of one segment in meters
	//float				StartFadeFactor;		// 0 - means start fading immediately and 1.0 means never fade. 0.5 means start fade on hlaf segment
	//int					SegmentsCount;			// how many segments
	//WeColor				SegmentColor;			// color of segment
	//float				SegmentCreationTime;	// how many seconds to new segment
	float				FromRadius;				// default 1.0f
	float				ToRadius;				// default 0.1f;
	float				FromAlpha;				// default 1.0
	float				ToAlpha;				// default 1.0
	int					RingSegmentCount;		// default 8. How many segments cylinder has

	

	void				Initialize();			// must be called when params are set
	void				Tick();
	void				Reset(); 
	void				Serialize(WeFileStream &FS);
	void				Deserialize(WeFileStream &FS);
	bool				IntersectFrustum(WeFrustum *frustum);

	void				Clone(WeActor *Destination, bool DeepCopy = false); 

	void				OnRender(WeCamera* Cam);
	int					GetIndexSize();
	int					GetFacesCount();

};


#endif
