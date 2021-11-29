/*
       File: WeRenderDevice.h
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/





#ifndef _WERENDERDEVICE
#define _WERENDERDEVICE


#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <CString.h>
#include <WeMatrix.h>
#include <WeVector.h>
#include <WindEngine.h>
#include <WeColor.h>

class WeTexture;
class WeCanvas;
class WeTextureManager;
class WeShaderManager;
class WeMaterialManager;
class WeCamera;
class WeMesh;
class WeRenderTarget;
class WeBaseRenderer;
class WeShader;



#define WE_RENDERING_BATCHITEMS 1
#define WE_RENDERING_QUERY 2
#define WE_RENDERING_PARTICLES 4
#define WE_RENDERING_ALPHATEXTURES 8
#define WE_RENDERING_TEXTURES 16
#define WE_RENDERING_POSTPROCESSING 32





typedef DWORD COLOR;

typedef void (*RENDERCALLBACK)();
typedef void (*LOSTDEVICECALLBACK)();
typedef void (*RESETDEVICECALLBACK)();


class WINDENGINE_API WeRenderDevice
{
	friend class WeEngine;
	friend class WeBatchRenderer;
	friend class WeVelocityRenderer;
	friend class WeGlowRenderer;

protected:
	WeRenderDevice();
	virtual						~WeRenderDevice(void);

	bool						bUsingShaders;
	bool						bMotionBlur;
	bool						bAddedPostProcessEffects;  // switch for bloom, ... and other post process effects
	bool						bPostProcessEffects; // siwtch for all post processing including motion blur
	bool						bDeviceLost;
	bool						bPaused;
	bool						bWireFrame;
	bool						bFullScreen;
	bool						bClearScreen;
	bool						bInitialized;
	bool						bLight;
	bool						bRenderParticles;
	int							RenderingState; // describes what is going to be rendered
	int							ResWidth;
	int							ResHeight;
	float						BloomScale;
	float						Luminance;
	float						Brightness;
	bool						FogEnabled;
	float						FogStart;
	float						FogEnd;
	float						FogDensity;
	WeColor						FogColor;
	WeColor						BrightnessColor;
	WeColor						AmbientColor;
	WeColor						SkyColor;           // ambient sky color
	WeColor						GroundColor;		// ambient ground color
	DWORD						ClearColor;	
	bool						ClearScreen;
	float						BlurFactor; // no blur is default
	RENDERCALLBACK				RenderCallback;
	LOSTDEVICECALLBACK			LostDeviceCallback;
	RESETDEVICECALLBACK			ResetDeviceCallback;
	WeMatrix					ViewMatrix;
	WeMatrix					ProjMatrix;
	WeShaderManager*			ShaderManager;
	WeTextureManager*			TextureManager;
	WeMaterialManager*			MaterialManager;
	D3DPRESENT_PARAMETERS		PresentParams;
	D3DDISPLAYMODE				d3ddm;
	bool						UseShadowVolumes;
	bool						bSupportShader20;
	bool						bSupportShader2a;
	bool						bSupportShader30;
	bool						bSupportMultipleRT;
	bool						bSupportMRTBlending;
	bool						bDoRendering;	
	bool						bPixelMotionBlur;
	bool						bProjectors;
	float						PixelMotionBlurFactor;
	bool						bFlares;
	bool						bShadows;   
	bool						bBumpMapping;
	bool						bSpecular;
	bool						bGlow;
	bool						bAnisotropic; 
	bool						bUseMaxRenderDistance;
	bool						bRefractive;
	int							AnisotropicFactor;	
	int							AntialiasingFactor;
	D3DMULTISAMPLE_TYPE			MultiSampleType;
	D3DFORMAT					SurfaceFormat;
	LPDIRECT3DSURFACE9			lpNotMultisampledDepthSurf;



	void						MakePresentParams();

	virtual LRESULT				MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);		
	virtual void				Update(WeCamera* ActiveCamera);	
	virtual void				LoadConfig();
	virtual void				SaveConfig();
			void				CheckMultiSampleType();  // this will check multisample type and make closest possible

private:
	static WeRenderDevice*		 _instance;

public:
	HWND						hWnd;
	RECT						SourceRect;
	RECT						ScissorRect;	
	bool						ScissorEnable;
	RECT						DestRect;
	bool						BackBufferScale;
	CSmartList<WeBaseRenderer*> Renderers;

	// Some general parameters that affect rendering
	// TODO: Make this to work
	// -------------------------------------------------------------
	void						SetRendering(int renderingState, bool State) { RenderingState |= renderingState; }
	bool						GetRendering(int renderingState) { return (RenderingState & renderingState) == 0; }	
	virtual bool				Initialize(HINSTANCE hInst, HWND hWnd, String CmdLine, int reswidth, int resheight, bool FullScreen = true, StringParam DefaultShaderName = "default.fx");
	virtual void				ShutDown();
	//================================================================

	IDirect3DDevice9*			Device;
	IDirect3D9*					d3d;
	float						Aspect;
	static	WeRenderDevice*		Instance();
	void						BeginScene()				{ Device->BeginScene(); }
	void						EndScene()					{ Device->EndScene(); }
	virtual WeTextureManager*	GetTextureManager()			{ return TextureManager; }
	virtual WeShaderManager*	GetShaderManager()			{ return ShaderManager; }
	virtual WeMaterialManager*	GetMaterialManager()		{ return MaterialManager; }


	bool						SupportsShader20() { return bSupportShader20; }
	bool						SupportsShader2a() { return bSupportShader2a; }
	bool						SupportsShader30() { return bSupportShader30; }
	bool						SupportsMultipleRT() { return bSupportMultipleRT; }
	bool						SupportsMRTBlending()  { return bSupportMRTBlending; }

	void						SetClearColor(COLOR Clear)	{ ClearColor = Clear; }
	COLOR						GetClearColor()				{ return ClearColor; }	
	void						SetScissorRect(RECT *Rect, bool test = true); 
	void						SetScrissorTest(bool test);

	
	void						SetBlurFactor(float Factor) { BlurFactor = Factor; } // factor goes from 0.0 .. 1.0 (dont use 1.0 cause it is max blur)
	float						GetBlurFactor()				{ return BlurFactor; }
	
	void						SetMotionBlur(bool mb)		{ bMotionBlur = mb; }
	bool						GetMotionBlur()				{ return bMotionBlur; }

	void						SetUsingShaders(bool b)		{ bUsingShaders = b;	}
	bool						GetUsingShaders()			{ return bUsingShaders; }

	void						SetUseMaxRenderingDistance(bool b) { bUseMaxRenderDistance = b; }
	bool						GetUseMaxRenderingDistance() { return bUseMaxRenderDistance; }

	void						SetPostProcessEffects(bool pp)		{ bPostProcessEffects = pp; }
	bool						GetPostProcessEffects()			{ return bPostProcessEffects; }

	void						SetPixelMotionBlur(bool p) { bPixelMotionBlur = p; }
	bool						GetPixelMotionBlur() { return bPixelMotionBlur; }
	void						SetPixelMotionBlurFactor(float factor) { PixelMotionBlurFactor = factor; }
	float						GetPixelMotionBlurFactor() { return PixelMotionBlurFactor; }

	void						SetProjectors(bool p) { bProjectors = p; }
	bool						GetProjectors() { return bProjectors; }

	void						SetFlares(bool b) { bFlares = b; }
	bool						GetFlares() { return bFlares; }

	void						SetGlow(bool b) { bGlow = b; }
	bool						GetGlow() { return bGlow; }

	void						SetRefractive(bool b) { bRefractive = b; }
	bool						GetRefractive() { return bRefractive; }

	void						SetShadows(bool s) { bShadows = s; }
	bool						GetShadows() { return bShadows; }

	void						SetAddedPostProcessEffects(bool b) { bAddedPostProcessEffects = b; }
	bool						GetAddedPostProcessEffects()		{ return bAddedPostProcessEffects; }

	void						SetClearScreen(bool ClearIt){ bClearScreen = ClearIt; }
	bool						GetClearScreen()			{ return bClearScreen; }

	void						SetLight(bool Light) { bLight = Light; }
	bool						GetLight() { return bLight; }
	void						SetBump(bool bump) { bBumpMapping = bump; }
	bool						GetBump() { return bBumpMapping; }
	void						SetSpecular(bool Spec) { bSpecular = Spec; }
	bool						GetSpecular() { return bSpecular; }

	void						SetBloomScale(float bloomScale) { BloomScale = bloomScale; }
	float						GetBloomScale() { return BloomScale; }
	// zero means normal light
	void						SetBrightness(float brightness) { Brightness = brightness; }
	float						GetBrightness() { return Brightness; }
	void						SetBrightnessColor(WeColor &brightnesscol) { BrightnessColor = brightnesscol; }
	WeColor						GetBrightnessColor() { return BrightnessColor; }
	void						SetAmbientColor(WeColor &ambientcol) { /*AmbientColor = ambientcol;*/ }
	WeColor						GetAmbientColor() { return AmbientColor; }
	void						SetSkyColor(WeColor &col) { SkyColor = col; }
	WeColor						GetSkyColor() { return SkyColor; }
	void						SetGroundColor(WeColor &col) { GroundColor = col; }
	WeColor						GetGroundColor() { return GroundColor; }


	void						SetAnisotropic(bool a) { bAnisotropic = a; }
	bool						GetAnisotropic() { return bAnisotropic; }
	void						SetAnisotropicFactor(int level) { AnisotropicFactor = level; }
	int							GetAnisotropicFactor() { return AnisotropicFactor; }

	int							GetAntialiasFactor() { return AntialiasingFactor; }
	void						SetAntialiasFactor(int factor) { AntialiasingFactor = factor; }

	void						SetUseShadowVolumes(bool useShadowVolumes) { UseShadowVolumes = useShadowVolumes; }
	bool						GetUseShadowVolumes() { return UseShadowVolumes; }

	void						SetRendering(bool Rendering) { bDoRendering = Rendering; }
	bool						GetRendering() { return bDoRendering; }

	void						SetRenderParticles(bool Render) { bRenderParticles = Render; }
	bool						GetRenderParticles()			{ return bRenderParticles; }	

	bool						IsFullScreen()				{ return bFullScreen; }
	bool						IsDeviceLost()				{ return bDeviceLost; }
	void						SetPause(bool pause)		{ bPaused = pause; }
	bool						GetPause()					{ return bPaused; }
	void						SetRenderCallback(RENDERCALLBACK _RenderCallback){ RenderCallback = _RenderCallback; }
	void						SetLostDeviceCallback(LOSTDEVICECALLBACK _LostDeviceCallback){ LostDeviceCallback = _LostDeviceCallback; }
	void						SetResetDeviceCallback(RESETDEVICECALLBACK _ResetDeviceCallback){ ResetDeviceCallback = _ResetDeviceCallback; }
	void						DoRender();
	void						SaveThumbnail(String PathAndFileName);
	virtual void				SetWireFrame(bool wireframe);
	virtual bool				GetWireFrame()				{ return bWireFrame; }
	virtual bool				SetDisplayMode(int NewX, int NewY, int NewColorDepth, bool NewFullScreen, bool NewVSync, bool Scale = true);
	virtual void				SetBufferScale(bool BufferScale){ BackBufferScale = BufferScale; }

	void						SetFog(bool Enable, WeColor &Color, float Density); 
	void						SetFogStart(float s);
	void						SetFogEnd(float e);
	void						SetTextureFiltering(); // this will turn on texture filtering, linear or anisotropic. This is called from World->Render
	D3DMULTISAMPLE_TYPE			GetMultiSampleType() { return MultiSampleType; }
	D3DFORMAT					GetSurfaceFormat() { return SurfaceFormat; }

	void						TurnFogOn();
	void						TurnFogOff();


	// ----------------------------
	// RenderTarget stuff
	void						SetRenderTarget(int rt, WeRenderTarget *);
	void						SetTexture(int Sampler, WeTexture *Texture = NULL);	
	LPDIRECT3DSURFACE9			SaveBackBuffer(); // saves screen surface 
	void						RestoreBackBuffer(LPDIRECT3DSURFACE9 BackSurface); // restores screen surface
	// ----------------------------		
	void						CreateDeclaration(const D3DVERTEXELEMENT9  *pVertexElements, LPDIRECT3DVERTEXDECLARATION9 *lplpDecl);	
	virtual WeCanvas*			GetCanvas();
	virtual WeMatrix			BuildViewMatrix(WeVector* Pos, WeVector* LookAt, WeVector* Up);
	virtual WeMatrix			BuildProjMatrix(float Fov, float NearClip, float FarClip, float Aspect = 0);
	virtual void				SetRenderState(D3DRENDERSTATETYPE RenderState, DWORD Value);
	void						OnLostDevice();
	void						OnResetDevice();
	int							GetResWidth()	{ return ResWidth; }
	int							GetResHeight()	{ return ResHeight; }
	void						SetAspect(float NewAspect)	{ if(NewAspect) Aspect = NewAspect; }
	void						SetHWND(HWND _hWnd){ hWnd = _hWnd; }
	HWND						GetHWND(){ return hWnd; }
};





#endif
