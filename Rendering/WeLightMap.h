/*
     WeLightMap.h

	 Autor: Nebojsa Vislavski
	 Date: 30.1.2006.
	 Version: 1.0

  
*/

#ifndef _WELIGHTMAP_H
#define _WELIGHTMAP_H

#include <WeFileStream.h>
#include <d3d9.h>
#include <WeColor.h>
#include <WindEngine.h>
#include <WeTexture.h>

class WeActor;
class WeWorld;
class WeLight;


class WINDENGINE_API WeLightMap
{
protected:

	struct SLightMap
	{
		WeColor Color;
	};

	struct SUV
	{
		float lu, lv;
	};

	struct SLumel
	{
		WeVector Position;
		WeVector Normal;		
		WeColor	 Color;
		bool	 isValid;		
	};



	bool					Initialized;
	SUV*					LUVData;
	SLumel*					Lumels;
	int						 Version;
	int						LightMapSize;
	WeTexture				*LightMapTexture;
	LPDIRECT3DVERTEXBUFFER9 ColorVB;
	static LPDIRECT3DVERTEXBUFFER9 DefaultColorVB; // this is made once and used when starting

	// simulated pixel shader. Inputs are already interpolated
	void		ProcessLight(WeVector V, WeVector N, WeVector T, CSmartList <WeLight*> *Lights, WeColor &outCol, WeVector NFromNormalMap);
	void		BlurLightMap(WORD *pbits, bool BlurBlack);	

public:
	// This will start using light map. It sets VB to stream 1
				WeLightMap();
				~WeLightMap();

	WeActor*	Owner;

	int			NumVertices;
	bool		Stopped;	


	void		Use();
	// This will generate Lightmap for for this actor in this world. Counting all lights that affetct it
	// simple generate - baking will be 32x32 lightmaps
	void		Generate(WeActor *Actor, WeWorld *World, CSmartList <WeLight *> *Lights = NULL, bool SimpleGenerate = false);
	void		GenerateDefault(WeActor *Actor); // this will make colorVB be defaultColorVB
	
	void		Serialize(WeFileStream &FS);
	void		Deserialize(WeFileStream &FS);

	void		Initialize();  // this will set lu, lv into mesh
};


#endif