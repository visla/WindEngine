/*
      File:  WeEffectEditor.h
	  Author: Tomislav Kukic
	  Date: 14.12.2005.
	  Version: 1.0
*/


#ifndef _WEEFFECTEDITOR
#define _WEEFFECTEDITOR


#include <d3dx9.h>
#include <WindEngine.h>
#include <WeShader.h>



class WINDENGINE_API WeEffectEditor : public WeShader
{
public:
	WeEffectEditor();
	~WeEffectEditor();

	ID3DXEffectCompiler* EffectCompiler;

	bool Save(StringParam SaveName, bool Replace);
	void Destroy();
};


#endif