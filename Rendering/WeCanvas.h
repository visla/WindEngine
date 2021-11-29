/*
       File: WeCanvas.h
	   Author: Tomislav Kukic
	   Date: 10.10.2005.
	   Version: 1.0
	   Platform: Windows
*/



#ifndef _WECANVAS
#define _WECANVAS


#include <d3dx9.h>
#include <d3d9.h>
#include <CString.h>
#include <WindEngine.h>


class WeFont;
class WeTexture;




enum WeBlendingMode
{
	bmNone, // doesn't use alpha blending
	bmNormal, // normal alpha blending
    bmAdditive, // additive alpha blending
	bmModulate, // color modulation
	bmModulateInvert // inverted color modulation
};

struct SimpleRectangle
{
	float x,y,z,w;
	DWORD Color;
	float u,v;
};


const DWORD RECT_FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);




class WINDENGINE_API WeCanvas
{
private:
	static WeCanvas *_instance;

protected:	

	CSmartList<WeFont*> Fonts;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DDEVICE9 Device;

	WeCanvas();
	~WeCanvas();

	void ClearAllFonts();

	void SetupAlphaBlending(WeBlendingMode bmode);
	void MakeVB();
	void ChangeVBParams(int x, int y, int w, int h, DWORD Color);
	void ChangeVBParams(int x, int y, int w, int h, float x1, float y1, float x2, float y2, DWORD Color);
	void ChangeVBParamsRotated(float x, float y, float w, float h, float Angle, DWORD Color);
	void ChangeVBParams(int x, int y, int w, int h, DWORD ColorTL, DWORD ColorTR, DWORD ColorBL, DWORD ColorBR);
	void RenderVertexBuffer();

public:
	static WeCanvas *Instance();

	WeFont *MakeFont(StringParam FontName, int FontSize);
	void	FreeFont(WeFont *Font);

	void Draw2DText(WeFont *Font, StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor);
	void Draw2DTextOutlined(WeFont *Font, StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor, D3DCOLOR OutlineColor);

	void DrawRectangle(int x, int y, int w, int h, DWORD Color, WeTexture* Texture = NULL, WeBlendingMode bmode = bmNormal);
	void DrawRectangle(int x, int y, int w, int h, float x1, float y1, float x2, float y2, DWORD Color, WeTexture* Texture = NULL, WeBlendingMode bmode = bmNormal);
	void DrawRectangleRotated(float x, float y, float w, float h, float Angle, DWORD Color, WeTexture* Texture = NULL, WeBlendingMode bmode = bmNormal);
	void DrawRectangle(int x, int y, int w, int h, DWORD ColorTopLeft, DWORD ColorTopRight, DWORD ColorBottomLeft, DWORD ColorBottomRight, WeTexture* Texture = NULL, WeBlendingMode bmode = bmNormal);
	void DrawRectangle(int x, int y, int w, int h, DWORD Color, LPDIRECT3DTEXTURE9 Texture, WeBlendingMode bmode = bmNormal);

	void ShutDown() { ClearAllFonts(); }
	void OnLostDevice();
	void OnResetDevice();
};




#endif
