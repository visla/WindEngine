/*
 
     WeLightState.h

	 Autor: Nebojsa Vislavski
	 Date: 15.1.2006.
	 Version: 1.0
	 Platform: Win32/Linux


	 This is light state for one light. 

*/

#ifndef _WELIGHTSTATE_H
#define _WELIGHTSTATE_H

#include <WindEngine.h>
#include <WeColor.h>
#include <WeVector.h>
#include <WeFileStream.h>


#define LIGHT_FLARE_TEXTURES 5


class WINDENGINE_API WeLightState
{
public:
	WeLightState();


	int			Version;
	// deprecated ------------------
	WeVector	DeltaLocation;
	WeVector	Direction;
	float		Range;
	float		StartFalloffRange;
	// -----------------------------
	
	WeColor		Diffuse;
	WeColor		Specular;	
	float		Duration;			// duration time of this state
	float		Intensity;
	float		SpecularIntensity;

	float		CoronaAlpha;
	float		FlareAlpha[LIGHT_FLARE_TEXTURES];

	void		Serialize(WeFileStream &FS);
	void		Deserialize(WeFileStream &FS);

};



#endif