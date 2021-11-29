/*
    WePostProcess.h

	Autor: Nebojsa Vislavski
	Date: 2.11.2005.
	Version: 1.0

	This is post process system. It is almost automaticly called by engine.
	You only have to add post process effects.

*/

#ifndef _WEPOSTPROCESS_H
#define _WEPOSTPROCESS_H

#include <d3d9.h>
#include <d3dx9.h>
#include <SmartList.h>
#include <WeShader.h>
#include <WePostProcessEffect.h>
#include <WeMaterial.h>

#include <WeRenderTarget.h>
#include <WindEngine.h>

class WINDENGINE_API WePostProcess
{
friend class WeVelocityRenderer;

private:
	static WePostProcess *_instance;
protected:
	bool Initialized;
	bool  UseMRTBlending;
	float BlurFactor;
	LPDIRECT3DSURFACE9 BackSurface;

	CSmartList <WePostProcessEffect *> Effects;


	// We need buffer for FullScreen quad
	struct SQuadVertex
	{
		float x, y, z, rhw;
		float u,v;	
		float u2, v2;
		#define D3DFVF_QUADVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX2)
	};
	LPDIRECT3DVERTEXBUFFER9 lpQuadBuffer;
	SQuadVertex QuadVertices[4];



	// this is shader used to display to final screen. If 
	// it is null than last effect renders to screen 
	// Maybe will be erased later in development
	WeShader*				FinalScreenShader; 
	WeShader*				RefractiveShader;	
	WeMaterial*				RefractiveMaterial;


	WeRenderTarget*			OldScreenRT[2]; // this is RT for color
	WeRenderTarget*			ColorRT; // this is RT for color
	WeRenderTarget*			ColorFlipChain[2][2]; // first is for 1.0 size and second for 0.25 size

	// THESE ARE NOT USED FOR NOW
	// TODO: Make them useful
	WeRenderTarget*			NormalRT; // this is RT for normal
	WeRenderTarget*			PositionRT; // this is RT for position


	float					ExtentX;  // max uv`s
	float					ExtentY;  
	bool					Capturing;

	void					ResetQuadArray(float z);
	void					DrawQuad(float fScaleByX, float fScaleByY);
	void					DoRefraction();
	void					DoGlowing();

public:
	// these two are for pixel motion blur
	WeRenderTarget*			VelocityRT;
	WeRenderTarget*			OldVelocityRT; 	
	WeRenderTarget*			GlowRT; 	
	WeRenderTarget*			RefractiveRT; 	


	static WePostProcess *Instance();

							WePostProcess(): Initialized(false) { }
							~WePostProcess() { ShutDown(); }

	bool					Initialize();
	void					ShutDown();
		
	void					SetBlurFactor(float blurFactor) { BlurFactor = blurFactor; } 
	float					GetBlurFactor() { return BlurFactor; }
	bool					GetUseMRTBlending() { return UseMRTBlending; }

	void					BeginScreenCapture();
	void					EndScreenCapture();
	void					PostRender(); // called to do postprocess effects
	bool					IsCapturing() {return Capturing; }
	void					ClearOtherMRT(); // this will set rt`s to null except this on 0 element


	WePostProcessEffect*	AddEffect(WePostProcessEffect *);	// Size iz 1, or .25 	
	// Combine effect means that this effect needs Original Scene set in sampler 0
	WePostProcessEffect*	AddEffect(StringParam ShaderName, float ScaleXBy=1.0f, float ScaleYBy=1.0f, bool FromPackage = false);
	// this will not destroy effect just remove it from stack for use. 
	// User is responsible for creation and deletition of PostProcessEffects
	void					RemoveEffect(WePostProcessEffect *); 
	void					ClearEffects();
	void					UpdateEffects(); // u odnosu

	void					TurnOn();
	void					TurnOff();
	bool					Turned();
	void					OnLostDevice();
	void					OnResetDevice();
	void					DrawFullScreenQuad(); 
};

#endif