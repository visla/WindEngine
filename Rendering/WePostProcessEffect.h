/*
     WePostProcessEffect.h

	 Autor: Nebojsa Vislavski
	 Date: 2.11.2005.
	 Version: 1.0

	 This is post process effect. User is responsible of 
	 creating and deleting this objects


*/

#ifndef _WEPOSTPROCESSEFFECT_H
#define _WEPOSTPROCESSEFFECT_H

#include <WindEngine.h>
#include <CString.h>

class WeShader;

class WINDENGINE_API WePostProcessEffect
{
friend class WePostProcess;

protected:
	float ScaleXBy;
	float ScaleYBy;
	bool  Initialized;
	WeShader *Shader;
	bool  Combine;

public:
	bool  Active;

    float Param1;
	float Param2;
	float Param3;
	float Param4;


	WePostProcessEffect(): Param1(0), Param2(0), Param3(0), Param4(0), Initialized(false), Shader(NULL), Combine(false), Active(true) {} 
	~WePostProcessEffect() { ShutDown(); }

	// Initilizes effect with shader name and size (1, 1) 
	// combine effect indicates that this shader will combine old scene with scene 
	// rendered in previous post process effect
	virtual bool	Initialize(StringParam ShaderName, float ScaleXBy, float ScaleYBy, bool FromPackage = false); 
	virtual void	ShutDown();

	

};


#endif