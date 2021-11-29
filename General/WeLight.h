/*
    WeLight.h

	Autor: Nebojsa Vislavski
	Date: 15.1.2006.
	Version: 1.0
	Platform: Win32/Unix

	this is light class it contains information of light properties.


*/

#ifndef _WELIGHT_H
#define _WELIGHT_H

#include <WeActor.h>
#include <WindEngine.h>
#include <WeLightState.h>
#include <SmartList.h>
#include <WeFlareRenderer.h>

class WeTexture;
class WeMaterial;


enum WeLightType
{
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_PROJECTORONLY, // dont behave as light but only as projector
	LIGHT_STATIC_LINEAR,
	LIGHT_STATIC_AREA,
	LIGHT_STATIC_BEAM
};




class WINDENGINE_API WeLight: public WeActor
{
friend class WeWorld;
friend class WeLightMap;
friend class WeShadowVolumeRenderer;
friend class WeProjectorRenderer;
friend class WeFlareRenderer;

protected:
	WeLight *NextLight;
	WeLight	*PrevLight;

	int							     LightVer;
	WeColor							 CurrentDiffuse;
	float							 CurrentIntensity;
	WeColor							 CurrentSpecular;
	float							 CurrentSpecularIntensity;
	float							 CurrentCoronaAlpha;
	float							 CurrentFlareAlpha[LIGHT_FLARE_TEXTURES];
	WeColor							 NextDiffuse;
	float							 NextIntensity;
	WeColor							 NextSpecular;
	float							 NextSpecularIntensity;
	float							 NextCoronaAlpha;
	float							 NextFlareAlpha[LIGHT_FLARE_TEXTURES];

	float							 InterpolateTime;
	WeLightType						 PrevLightType;
	float							 PrevSizeX;
	float							 PrevSizeY;

	CSmartListIterator<WeLightState> LightStateIterator;
	CSmartListIterator<WeLightState> NextLightStateIterator;

	// DEPRECATED
	float							CoronaAlpha;  // don`t use 
	float							FlareAlpha[LIGHT_FLARE_TEXTURES]; // don`t use 
	// ========


public:
								WeLight();
	virtual					    ~WeLight();

	bool						IsDynamic;		// true for dynamic lights this is default
	WeLightType					LightType;		// type of light default is point
	bool						LoopKeyStates;  // default is true
	bool						InterpolateStates; // interpolate light states
	CSmartList <WeLightState>	LightStates;	// NOTE! don`t use his pushBack. Use AddKeyState for adding keystates

	float						Range;
	float						StartFalloff;
	float						SpotAngle;
	float						SpotAngleFalloff;	    	
	float						AnimationTime;  
	float						AnimationDuration;
	// if it should affect dynamic actors. If it is static light maybe it still wants to affect dynamic. Default is false
	bool						AffectDynamicActors; 	

	bool						ProjectorIsOmni;		// if projector is omni
	bool						Projector;				// if this light is projector	
	WeMaterial*					ProjectorMaterial;		// what material it uses if it is projector
	String						ProjectorMaterialName;	// name of projector material

	// size of light if it is linear or if it is area
	float						SizeX;
	float						SizeY;

	bool						Corona;					// does this light have corona
	bool						Flares;					// does this light have flares
	WeTexture*					CoronaTexture;
	WeTexture*					FlareTexture[LIGHT_FLARE_TEXTURES];
    String						CoronaTextureName;
	String						FlareTextureNames[LIGHT_FLARE_TEXTURES];
	float						CoronaSizeX;
	float						CoronaSizeY;
	float						FlareSizeX;
	float						FlareSizeY;
	float						FadeInTime;
	float						FadeOutTime;
	bool						CoronaFixedSize;
	bool						CoronaRotation;
	bool						AlwaysShowFlares;


	WeMatrix					RenderTM;
	WeVector					RenderLocation;

	
	// Add animation to light. Time is param that tells when to start that state. ex. If it is 0.1 then it means
	// this state will have duration of 0.1 sec
	void						AddKeyState(WeLightState &LightState, float Time, bool LoopKeyStates = true);
	void						AddLightState(WeLightState *ls);
	void						RemoveLightState(WeLightState *ls);
	float						GetLightTime() { return AnimationTime; }	// return time used for this lights animation
	WeLightState*				GetCurrentLightState();
	WeColor						GetDiffuseColor();
	WeColor						GetSpecularColor();
	float						GetIntensity();
	float						GetFlareAlpha(int index);
	float						GetCoronaAlpha();
	float						GetSpecularIntensity();
	void						ResetAnimation() { AnimationTime = 0;  }	// resets time to 0	
	void						Tick();

	virtual String				GetInfo();

	// This will clone Light to its new destination
	virtual void				Clone(WeActor *Destination, bool DeepCopy = false); 
	void						Initialize();
	bool						IntersectFrustum(WeFrustum *frustum);

	void						Serialize(WeFileStream &FS);
	void						Deserialize(WeFileStream &FS);

};

#endif