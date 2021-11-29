/*
       File: WeFont.cpp
	   Author: Tomislav Kukic
	   Date: 10.10.2005.
	   Version: 1.0
	   Platform: Windows
*/




#include <d3dx9.h>
#include <WeRenderDevice.h>
#include <WeFont.h>


void WeFont::Initialize(StringParam FontName, int FontSize)
{
	if (Initialized) return;

	if (FAILED(D3DXCreateFont(WeRenderDevice::Instance()->Device, FontSize, 0,1,1, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FontName, &Font)))
	{
		Initialized = false;
	}
	else
	{
		SavedName = FontName;
		Font->GetDesc(&FontDesc);
		Initialized = true;
	}
}

void WeFont::ShutDown()
{
	if (!Initialized) return;
	SAFE_RELEASE(Font);
	Initialized = false;
}


void WeFont::Draw2DText(StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor)
{
	if (!Initialized) return;
	
	RECT Rect;
	Rect.top = y;
	Rect.left = x;
	Rect.bottom = y+h;
	Rect.right = x+w;

	//WeRenderDevice::Instance()->BeginScene();
	Font->DrawText(NULL, Text, Text.GetLen(), &Rect, DT_LEFT, FontColor);
	//WeRenderDevice::Instance()->EndScene();
}



void WeFont::OnLostDevice()
{
	if (!Initialized) return;
	Font->OnLostDevice();
}



void WeFont::OnResetDevice()
{
	if (!Initialized) return;
	Font->OnResetDevice();
}


