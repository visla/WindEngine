/*
     WePostProcessEffect.cpp

	 Autor: Nebojsa Vislavski
	 Date: 2.11.2005.
	 Version: 1.0

	 This is post process effect.


*/

#include <WePostProcessEffect.h>
#include <WeShader.h>
#include <WeShaderManager.h>


bool WePostProcessEffect::Initialize(StringParam ShaderName, float ScaleXBy, float ScaleYBy, bool FromPackage)
{
	if (Initialized) return true;

	this->ScaleXBy = ScaleXBy;
	this->ScaleYBy = ScaleYBy;


	Shader = WeShaderManager::Instance()->Load(ShaderName, false, FromPackage);
	if (!Shader) Initialized = false;
	else Initialized = true;	

	return Initialized;
}

void WePostProcessEffect::ShutDown()
{
	Initialized = false;
}