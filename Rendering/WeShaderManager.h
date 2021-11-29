/*
       File: WeShaderManager.h
	   Author: Tomislav Kukic
	   Date: 12.09.2005.
	   Version: 1.0
*/




#ifndef _WESHADERMANAGER
#define _WESHADERMANAGER


#include <d3d9.h>
#include <d3dx9.h>
#include <SmartList.h>
#include <CString.h>
#include <WeMatrix.h>
#include <WeVector.h>
#include <WindEngine.h>
#include <WeColor.h>
#include <WeLight.h>
#include <WeShader.h>



typedef D3DXHANDLE SHADERHANDLE;





class WINDENGINE_API WeShaderManager
{
protected:
    // Universal Handles used to talk with shaders
	SHADERHANDLE	ViewMatrix;
	SHADERHANDLE	ProjectionMatrix;
	SHADERHANDLE	WorldMatrix;
	SHADERHANDLE	PrevWorldMatrix;
	SHADERHANDLE    ViewProjection;
	SHADERHANDLE    PrevViewProjection;
	SHADERHANDLE	DeltaTime;
	SHADERHANDLE	ElapsedTime;
	SHADERHANDLE	EyePosition;
	SHADERHANDLE	ResWidth;
	SHADERHANDLE	ResHeight;
	SHADERHANDLE	MaterialColor;	
	SHADERHANDLE	LightEnabled;

	bool			bInitialized;
	WeShaderQuality Quality;


	WeShaderManager(void);
	virtual ~WeShaderManager(void);

	CSmartList<WeShader*> Shaders;

private:
	static WeShaderManager* _instance;

public:
	WeShader			*DefaultShader;
	String				 DefaultShaderName;
	LPD3DXEFFECTPOOL	 EffectPool;



	static WeShaderManager* Instance();

	virtual void Free(WeShader* Shader);
	virtual void FreeAll();
	virtual void Initialize(StringParam DefaultShaderName);

	bool			SetQuality(WeShaderQuality Quality);
	WeShaderQuality GetQuality() { return Quality; }

	// ------------------------------------------------------------------
	// Use this methods to set global parameters to all shaders that have them
	// All this methods are called automaticly already.

	void	SetWorldMatrix(WeMatrix &World);
	void	SetPrevWorldMatrix(WeMatrix &PrevWorld);

	void	SetViewProjection(WeMatrix &ViewProjection);
	void	SetPrevViewProjection(WeMatrix &PrevViewProjection);

	void	SetProjectionMatrix(WeMatrix &Projection);
	void	SetViewMatrix(WeMatrix &View);
	void	SetDeltaTime(float DeltaTime);
	void	SetElapsedTime(float ElapsedTime);
	void	SetEyePosition(WeVector &EyePosition);	
	void	SetResWidth(float ResWidth);
	void	SetResHeight(float ResHeight);
	void	SetMaterialColor(WeColor &MatCol);
	void	SetLights(bool LightEnable);
	// ------------------------------------------------------------------

	//WeMatrix&		GetWorldMatrix();
	//WeMatrix&		GetViewProjection();
	//WeMatrix&		GetViewMatrix();
	//WeMatrix&		GetProjectionMatrix();
	//WeVector&		GetEyePosition();

	virtual WeShader* Load(StringParam name, bool ForceReload = false, bool FromPackage = false);

	void	OnLostDevice();
	void	OnResetDevice();
};




#endif