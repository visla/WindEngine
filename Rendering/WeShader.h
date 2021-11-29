/*
      File: WeShader.h
	  Author: Tomislav Kukic
	  Date: 12.09.2005.
	  Version: 1.0
*/



#ifndef _WESHADER
#define _WESHADER

#include <d3dx9.h>
#include <CString.h>
#include <WindEngine.h>
#include <WeLight.h>
#include <SmartList.h>
#include <WeVector.h>

class WeRenderTarget;
class WeMaterial;

typedef D3DXHANDLE SHADERHANDLE;
const int MAX_SHADER_LIGHTS = 30;


enum WeShaderQuality
{
	SHADER_LOW,
	SHADER_MEDIUM,
	SHADER_HIGH
};


class WINDENGINE_API WeShader
{
	friend class WeShaderManager;


protected:
	WeShader(void);
	virtual ~WeShader(void);

	bool	Loaded;
	UINT	PassesCount;

	// Light Handles for High Shaders
	//SHADERHANDLE	PointLightPosition[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	PointLightColor[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	PointLightSpecular[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	PointLightRange[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	PointLightIntensity[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	PointLightCount; // how mant lights we have


	//SHADERHANDLE	DirectLightDirection[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	DirectLightColor[MAX_SHADER_LIGHTS];
	//SHADERHANDLE	DirectLightSpecular[MAX_SHADER_LIGHTS];	
	//SHADERHANDLE	DirectLightCount; // how mant direct lights we have

	SHADERHANDLE LightPositionA;
	SHADERHANDLE LightColorA;
	SHADERHANDLE LightIntensityA;
	SHADERHANDLE LightSpecularIntensityA;
	SHADERHANDLE LightSpecularColorA;
	SHADERHANDLE LightRangeA;
	SHADERHANDLE LightCountA;


	SHADERHANDLE	ProjectorPosition; // how mant direct lights we have
	SHADERHANDLE	ProjectorMatrix; // how mant direct lights we have
	SHADERHANDLE	ShadowOmniLightPosition; // where is placed light that is casting shadow

	SHADERHANDLE	LightPosition;
	SHADERHANDLE	LightColor;
	SHADERHANDLE	LightSpecular;
	SHADERHANDLE	LightRange;
	SHADERHANDLE	LightIntensity;
	SHADERHANDLE	SpecularIntensity;
	SHADERHANDLE	LightDirection;

	SHADERHANDLE	BumpIntensity;
	SHADERHANDLE	SpecularPow;
	SHADERHANDLE	UsingAlpha;
	
	SHADERHANDLE	SkyColor;
	SHADERHANDLE	GroundColor;
	SHADERHANDLE	Scaling;

	SHADERHANDLE	PixelMotionBlurFactor;

	SHADERHANDLE	TextureAnimation;
	SHADERHANDLE	ResWidth;
	SHADERHANDLE	ResHeight;
	SHADERHANDLE	BloomScale;
	SHADERHANDLE	Brightness;
	SHADERHANDLE	BrightnessColor;
	SHADERHANDLE	Param1;
	SHADERHANDLE	Param2;
	SHADERHANDLE	Param3;
	SHADERHANDLE	Param4;
	SHADERHANDLE	AmbientColor;

	SHADERHANDLE	CurrentTechnique;
	SHADERHANDLE	DefaultTechnique;			// this technique is used if you dont want to write all things
	SHADERHANDLE	AmbientTechnique;			// where is placed light that is casting shadow
	SHADERHANDLE	RefractiveTechnique;			// where is placed light that is casting shadow

	SHADERHANDLE	DiffusePointLowTechnique;   
	SHADERHANDLE	BumpPointLowTechnique;   
	SHADERHANDLE	SpecularPointLowTechnique;
	SHADERHANDLE	BumpSpecularPointLowTechnique;
	
	SHADERHANDLE	DiffusePointHighTechnique;	
	SHADERHANDLE	BumpPointHighTechnique;
	SHADERHANDLE	SpecularPointHighTechnique;
	SHADERHANDLE	BumpSpecularPointHighTechnique;
	SHADERHANDLE	ReferenceShaderTechnique;


	WeShaderQuality Quality;
	LPD3DXEFFECT	Effect;

	CSmartList<WeRenderTarget*> RenderTargets;
	bool				SetDefaultTechnique();

public:

	String				Name;
	int					WantLightsCount;   // how many lights this shader needs. Default is 0. This shader needs no lights
	int					WantDirectLightsCount;
	int					WantSumLightColor; 

	virtual	LPD3DXEFFECT GetEffect(){ return Effect; }
	virtual	UINT	GetPassesCount() { return PassesCount; }
	
	void			Begin();
	void			End();
	void			BeginPass(UINT PassNum);
	void			CommitChanges();
	void			EndPass();
	void			SetLights(CSmartList <WeLight *> &Lights);
	void			SetLight(WeLight *Light);	
	void			SetTextureAnimation(WeMatrix &TexAnimM);
	void			SetParams(WeMaterial *Mat = NULL); // set various params for shader from material and other sources

	void			SetParam1(float Param1);
	void			SetParam2(float Param2);
	void			SetParam3(float Param3);
	void			SetParam4(float Param4);
	void			SetProjectorPosition(WeVector &v);
	void			SetProjectorMatrix(WeMatrix &m);
	void			SetShadowOmniLightPosition(WeVector &v);


	bool			SetQuality(WeShaderQuality Quality);	
	WeShaderQuality	GetQuality(WeShaderQuality Quality) { return Quality; }
	bool			HasAmbientTechnique() { return AmbientTechnique != NULL; } 


			void	SetScaling(WeVector &scale);
	virtual void	SetAlphaBlending();
	virtual void    SetBlendingAdd();
	virtual void	ClearBlending();

			bool	SetAmbientTechnique(); // this will set ambient technique
			bool	SetRefractiveTechnique();
			bool	SetTechnique(); // this will set appropriate technique for rendering
			

	virtual bool	Load(StringParam name, bool IsFxFile = true);

};



#endif
