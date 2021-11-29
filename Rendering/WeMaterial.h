/*
      File: WeMaterial.cpp
	  Author: Tomislav Kukic
	  Date: 12.09.2005.
	  Version: 1.0
*/



#ifndef _WEMATERIAL
#define _WEMATERIAL


#include <WeTexture.h>
#include <WeShader.h>
#include <WindEngine.h>
#include <WeColor.h>

#define WEMATERIALTYPE_NORMAL 0
#define WEMATERIALTYPE_TRANSPARENT 1  // uses AlphaRef 
#define WEMATERIALTYPE_ALPHABLENDED 2  // uses alpha blending
#define WEMATERIALTYPE_REFRACTIVE 3  // uses alpha in diffuse for refraction. It is not visible normally
#define WEMATERIALTYPE_FOREGROUND 4  // uses alpha blending, in front of everything

#define WEREFLECTIONTYPE_NONE 0
#define WEREFLECTIONTYPE_REALTIME 1
#define WEREFLECTIONTYPE_SUPPLIED 2   // almost same as none, cause this is in shader


#define MAX_MATERIAL_TEXTURES 16


#define WETEXTUREANIMATION_NONE 0
#define WETEXTUREANIMATION_SCROLL 1
#define WETEXTUREANIMATION_ROTATE 2
#define WETEXTUREANIMATION_ROTATECENTER 3
#define WETEXTUREANIMATION_SLIDESHOW 4
#define WETEXTUREANIMATION_SHAKE 5
#define WETEXTUREANIMATION_ZOOM  6
#define WETEXTUREANIMATION_SLIDESHOWRANDOM 7
#define WETEXTUREANIMATION_UVSCALE 8


#define WESAMPLER_DIFFUSE 0
#define WESAMPLER_NORMALMAP 1
#define WESAMPLER_SPECULAR 2
#define WESAMPLER_GLOW 3
#define WESAMPLER_ENVIRONMENT 4
#define WESAMPLER_LIGHTMAP 5



enum WEMATERIAL_BLEND_TYPE
{
	WEMATERIAL_BLEND_STANDARD,
	WEMATERIAL_BLEND_ADDITIVE,
};

class WINDENGINE_API WeMaterial
{
protected:

	void			TexAnimNormal();
	void			TexAnimScroll();
	void			TexAnimShake();
	void			TexAnimSlideShow();
	void			TexAnimSlideShowRandom();
	void			TexAnimWaveX();
	void			TexAnimWaveY();
	void			TexAnimZoom();
	void			TexAnimRotate();
	void			TexAnimUVScale();

	float			ElapsedTime;
	int				Version;
	

public:
							WeMaterial();
							~WeMaterial();

	void					Initialize();

	WeMatrix				TextureAnimationM;
    int						Type;   // WEMATERIAL_TYPE_NORMAL, ...
	WEMATERIAL_BLEND_TYPE	BlendType;			
	int						TextureAnimation;
	float					TextureAnimParam1;
	float					TextureAnimParam2;
	float					TextureAnimParam3;
	float					TextureAnimParam4;
	float					TextureAnimParam5;
	bool					AcceptLights;     // default true. If false then no light pass will be done for this material
	float					BumpIntensity;
	float					SpecularPow;
	DWORD					AlphaRef;

	int						iParam1;
	int						iParam2;
	int						iParam3;

	float					Param1;
	float					Param2;
	float					Param3;
	float					Param4;

	String					Name;

	bool					Glowing;			// is this material glowing
	bool					Refractive;			// is this material refractive

	// should this be double sided. You could also use setting in shader. 
	// but if you dont you can use this. Default is false
	bool					DoubleSided; 

	// reflection stuff
	int						ReflectionType; // WEREFLECTIONTYPE_REALTIME, NONE
	int						ReflectionUpdateInterval; 		
	int						EnvMapSize; // size of reflection env map
	int						EnvMapSampler; // what is sampler num for env map. default 1

	WeTexture*				Textures[MAX_MATERIAL_TEXTURES];
	WeShader*				Shader;
	WeColor					MaterialColor;

	void					SetParam1(float v) { Param1 = v; }
	void					SetParam2(float v) { Param2 = v; }
	void					SetParam3(float v) { Param3 = v; }
	void					SetParam4(float v) { Param4 = v; }

	float					GetParam1() { return Param1; }
	float					GetParam2() { return Param2; }
	float					GetParam3() { return Param3; }
	float					GetParam4() { return Param4; }

	virtual void			Load(StringParam ShaderName, StringParam TextureName);
	bool					Load(StringParam HoleName); // This loads material from package
	void					SetTextures(float ElapsedTime); // this will set texture use for this material, and also make 
	void					Tick();
};



#endif
