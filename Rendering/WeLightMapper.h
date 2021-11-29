/*
	WeLightMapper.h

	Autor: Nebojsa Vislavski
	Date: 1.9.2006.

*/

#ifndef _WELIGHTMAPPER_H
#define _WELIGHTMAPPER_H

#include <WeMesh.h>
#include <WeActor.h>
#include <WeWorld.h>


#define LIGHTMAP_SIZE 128.0f

class WeLightMapper
{
private:
	static WeLightMapper*	_instance;
public:
	static WeLightMapper*	Instance();
							WeLightMapper();
							~WeLightMapper();
	

	void					MakeLightMap(WeActor *Actor, WeWorld *World);
	void					MakeUV(WeMesh *Mesh);
	

protected:
	struct SFace
	{
		DWORD V[3];
	};

	// used for lightmapping
	struct SRectangle 
	{
		float top;
		float left;
		float bottom;
		float right;
	};

	struct SPartition
	{
		SPartition *Child[2];		
		float					X;
		float					Y;
		float					Width;
		float					Height;

		bool					Used;
		SPartition*				GetNode(float Width, float Height, float Padding);		
		void					Clear();
		SPartition(): Used(false) { Child[0] = NULL; Child[1] = NULL; }
	};

	SPartition					Root;
	DWORD*						Planes;   // planes for faces
	SFace*						Faces;
	DWORD*						Marked;
	WeVector*					FacesNormals;

	WeNormalMappedMeshVertex*	NMVertices;

	// data for element choosing
	float						Padding;   // in pixels
	float						LightMapFactor; // this is size correction. 1m = 1 pixel in texture. Default is 1/128
	float						LastWidth;
	float						LastHeight;
	float						LastX;
	float						LastY;
	float						MaxWidth;
	float						MaxHeight;
	float						SizeFactor; // default 1.0f
	float						GroupAngle; // angle for grouping similar faces. default is 0.86 or 30degree


	

	float					CalcMeshArea(int NumFaces, int NumVertices);
	void					MakeUVNormalMapped(WeMesh *Mesh);
	int						DeterminePlane(SFace *Face, int index); // returns 0 - xy, 1 - yz plane, 2 - xz 

	// this will return new offset for element with size width and height

	bool					GetNewElement(float Width, float Height, float &OffsetX, float &OffsetY);
	void					ResetElementParams(); // called in MakeUV on start to reset params for element getting


	// this will bake lights for actor
	void					BakeLights(WeActor *Actor, WeWorld *World);

};


#endif
