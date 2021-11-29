/*
       File: WeCanvas.cpp
	   Author: Tomislav Kukic
	   Date: 10.10.2005.
	   Version: 1.0
	   Platform: Windows
*/




#include <WeTexture.h>
#include <WeFont.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>




WeCanvas *WeCanvas::_instance = NULL;



WeCanvas *WeCanvas::Instance()
{
	if (_instance) return _instance;
	_instance = new WeCanvas();
	return _instance;
}


WeCanvas::WeCanvas()
{ 
	VertexBuffer = NULL;

    MakeVB();
}


WeCanvas::~WeCanvas()
{
	//ClearAllFonts();
	SAFE_RELEASE(VertexBuffer);
}



WeFont *WeCanvas::MakeFont(StringParam FontName, int FontSize)
{
	WeFont *font;
	font = new WeFont();
	font->Initialize(FontName, FontSize);

	Fonts.PushBack(font);

	return font;
}


void WeCanvas::MakeVB()
{
	LPDIRECT3DDEVICE9 Device = WeRenderDevice::Instance()->Device;

	Device->CreateVertexBuffer(4*sizeof(SimpleRectangle), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, RECT_FVF, D3DPOOL_DEFAULT, &VertexBuffer, NULL);
}




void WeCanvas::Draw2DText(WeFont *Font, StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor)
{
	if (Font)
		Font->Draw2DText(Text, x, y, w, h, FontColor);
}



void WeCanvas::Draw2DTextOutlined(WeFont *Font, StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor, D3DCOLOR OutlineColor)
{
	if (Font)
	{
		Font->Draw2DText(Text, x - 1, y - 1, w, h, OutlineColor);
		Font->Draw2DText(Text, x + 1, y - 1, w, h, OutlineColor);
		Font->Draw2DText(Text, x - 1, y + 1, w, h, OutlineColor);
		Font->Draw2DText(Text, x + 1, y + 1, w, h, OutlineColor);
		Font->Draw2DText(Text, x, y, w, h, FontColor);
	}
}

void WeCanvas::ChangeVBParams(int x, int y, int w, int h, DWORD Color)
{
	SimpleRectangle R[4];

	R[0].x = (float)x;
	R[0].y = (float)y;
	R[0].z = 0;
	R[0].w = 1;
	R[0].u = 0.0;
	R[0].v = 0.0;
	R[0].Color = Color;

	R[1].x = (float)(x+w);
	R[1].y = (float)y;
	R[1].z = 0;
	R[1].w = 1;
	R[1].u = 1.0;
	R[1].v = 0.0;
	R[1].Color = Color;

	R[2].x = (float)x;
	R[2].y = (float)(y+h);
	R[2].z = 0;
	R[2].w = 1;
	R[2].u = 0.0;
	R[2].v = 1.0;
	R[2].Color = Color;

	R[3].x = (float)(x+w);
	R[3].y = (float)(y+h);
	R[3].z = 0;
	R[3].w = 1;
	R[3].u = 1.0;
	R[3].v = 1.0;
	R[3].Color = Color;

	VOID* temp = NULL;
	VertexBuffer->Lock(0, 0,(void**) &temp, D3DLOCK_DISCARD);

	memcpy(temp, R, sizeof(R));

	VertexBuffer->Unlock();

}


void WeCanvas::ChangeVBParams(int x, int y, int w, int h, float x1, float y1, float x2, float y2, DWORD Color)
{
	SimpleRectangle R[4];

	R[0].x = (float)x;
	R[0].y = (float)y;
	R[0].z = 0;
	R[0].w = 1;
	R[0].u = x1;
	R[0].v = y1;
	R[0].Color = Color;

	R[1].x = (float)(x+w);
	R[1].y = (float)y;
	R[1].z = 0;
	R[1].w = 1;
	R[1].u = x2;
	R[1].v = y1;
	R[1].Color = Color;

	R[2].x = (float)x;
	R[2].y = (float)(y+h);
	R[2].z = 0;
	R[2].w = 1;
	R[2].u = x1;
	R[2].v = y2;
	R[2].Color = Color;

	R[3].x = (float)(x+w);
	R[3].y = (float)(y+h);
	R[3].z = 0;
	R[3].w = 1;
	R[3].u = x2;
	R[3].v = y2;
	R[3].Color = Color;

	VOID* temp = NULL;
	VertexBuffer->Lock(0, 0,(void**) &temp, D3DLOCK_DISCARD);

	memcpy(temp, R, sizeof(R));

	VertexBuffer->Unlock();

}


void WeCanvas::ChangeVBParamsRotated(float x, float y, float w, float h, float Angle, DWORD Color)
{
	SimpleRectangle R[4];
	float cx, cy, xdx, xdy, ydx, ydy;

	cx = x + (w / 2.0f);
	cy = y + (h / 2.0f);

	xdx = (w / 2.0f) * cos(Angle);
	xdy = (w / 2.0f) * sin(Angle);
	ydx = (h / 2.0f) * -sin(Angle);
	ydy = (h / 2.0f) * cos(Angle);

	R[0].x = cx - xdx - ydx;
	R[0].y = cy - xdy - ydy;
	R[0].z = 0;
	R[0].w = 1;
	R[0].u = 0.0;
	R[0].v = 0.0;
	R[0].Color = Color;

	R[1].x = cx + xdx - ydx;
	R[1].y = cy + xdy - ydy;
	R[1].z = 0;
	R[1].w = 1;
	R[1].u = 1.0;
	R[1].v = 0.0;
	R[1].Color = Color;

	R[2].x = cx - xdx + ydx;
	R[2].y = cy - xdy + ydy;
	R[2].z = 0;
	R[2].w = 1;
	R[2].u = 0.0;
	R[2].v = 1.0;
	R[2].Color = Color;

	R[3].x = cx + xdx + ydx;
	R[3].y = cy + xdy + ydy;
	R[3].z = 0;
	R[3].w = 1;
	R[3].u = 1.0;
	R[3].v = 1.0;
	R[3].Color = Color;

	VOID* temp = NULL;
	VertexBuffer->Lock(0, 0,(void**) &temp, D3DLOCK_DISCARD);

	memcpy(temp, R, sizeof(R));

	VertexBuffer->Unlock();

}


void WeCanvas::ChangeVBParams(int x, int y, int w, int h, DWORD ColorTL, DWORD ColorTR, DWORD ColorBL, DWORD ColorBR)
{
	SimpleRectangle R[4];

	R[0].x = (float)x;
	R[0].y = (float)y;
	R[0].z = 0;
	R[0].w = 1;
	R[0].u = 0.0;
	R[0].v = 0.0;
	R[0].Color = ColorTL;

	R[1].x = (float)(x+w);
	R[1].y = (float)y;
	R[1].z = 0;
	R[1].w = 1;
	R[1].u = 1.0;
	R[1].v = 0.0;
	R[1].Color = ColorTR;

	R[2].x = (float)x;
	R[2].y = (float)(y+h);
	R[2].z = 0;
	R[2].w = 1;
	R[2].u = 0.0;
	R[2].v = 1.0;
	R[2].Color = ColorBL;

	R[3].x = (float)(x+w);
	R[3].y = (float)(y+h);
	R[3].z = 0;
	R[3].w = 1;
	R[3].u = 1.0;
	R[3].v = 1.0;
	R[3].Color = ColorBR;

	VOID* temp = NULL;
	VertexBuffer->Lock(0, 0,(void**) &temp, D3DLOCK_DISCARD);

	memcpy(temp, R, sizeof(R));

	VertexBuffer->Unlock();

}



void WeCanvas::SetupAlphaBlending(WeBlendingMode bmode)
{
	if (bmode == bmNone)
		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	else
	{
		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		switch (bmode)
		{
			case bmNormal:
				WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				break;
			case bmAdditive:
				WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				break;
			case bmModulate:
				WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
				break;
			case bmModulateInvert:
				WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
				break;
			default:
				WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		}
	}
}



void WeCanvas::RenderVertexBuffer()
{
	WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	WeRenderDevice::Instance()->Device->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	WeRenderDevice::Instance()->Device->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	WeRenderDevice::Instance()->Device->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	DWORD AddressU, AddressV;
	WeRenderDevice::Instance()->Device->GetSamplerState(0, D3DSAMP_ADDRESSU, &AddressU);
	WeRenderDevice::Instance()->Device->GetSamplerState(0, D3DSAMP_ADDRESSV, &AddressV);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	WeRenderDevice::Instance()->Device->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	WeRenderDevice::Instance()->Device->SetTextureStageState(0,D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
	WeRenderDevice::Instance()->Device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	WeRenderDevice::Instance()->Device->SetFVF(RECT_FVF);
	WeRenderDevice::Instance()->Device->SetStreamSource(0, VertexBuffer, 0, sizeof(SimpleRectangle));
	WeRenderDevice::Instance()->Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSU, AddressU);
	WeRenderDevice::Instance()->Device->SetSamplerState(0, D3DSAMP_ADDRESSV, AddressV);

	WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_ZENABLE, TRUE);
}



void WeCanvas::DrawRectangle(int x, int y, int w, int h, DWORD Color, WeTexture* Texture, WeBlendingMode bmode)
{
	if (VertexBuffer == NULL) return;

	ChangeVBParams(x, y, w, h, Color);

	//WeRenderDevice::Instance()->Device->BeginScene();
	{
		if (Texture)
			WeRenderDevice::Instance()->Device->SetTexture(0, Texture->GetTexture());
		else
			WeRenderDevice::Instance()->Device->SetTexture(0, NULL);

		SetupAlphaBlending(bmode);

		RenderVertexBuffer();

		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
	
		//WeRenderDevice::Instance()->Device->EndScene();
	}
}



void WeCanvas::DrawRectangle(int x, int y, int w, int h, float x1, float y1, float x2, float y2, DWORD Color, WeTexture* Texture, WeBlendingMode bmode)
{
	if (VertexBuffer == NULL) return;

	ChangeVBParams(x, y, w, h, x1, y1, x2, y2, Color);

	//WeRenderDevice::Instance()->Device->BeginScene();
	{
		if (Texture)
			WeRenderDevice::Instance()->Device->SetTexture(0, Texture->GetTexture());
		else
			WeRenderDevice::Instance()->Device->SetTexture(0, NULL);

		SetupAlphaBlending(bmode);

		RenderVertexBuffer();

		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	

		//WeRenderDevice::Instance()->Device->EndScene();
	}
}



void WeCanvas::DrawRectangleRotated(float x, float y, float w, float h, float Angle, DWORD Color, WeTexture* Texture, WeBlendingMode bmode)
{
	if (VertexBuffer == NULL) return;

	ChangeVBParamsRotated(x, y, w, h, Angle, Color);

	//WeRenderDevice::Instance()->Device->BeginScene();
	{
		if (Texture)
			WeRenderDevice::Instance()->Device->SetTexture(0,Texture->GetTexture());
		else
			WeRenderDevice::Instance()->Device->SetTexture(0,NULL);


		SetupAlphaBlending(bmode);

		RenderVertexBuffer();

		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	

		//WeRenderDevice::Instance()->Device->EndScene();
	}
}



void WeCanvas::DrawRectangle(int x, int y, int w, int h, DWORD ColorTopLeft, DWORD ColorTopRight, DWORD ColorBottomLeft, DWORD ColorBottomRight, WeTexture* Texture, WeBlendingMode bmode)
{
	if (VertexBuffer == NULL) return;

	ChangeVBParams(x, y, w, h, ColorTopLeft, ColorTopRight, ColorBottomLeft, ColorBottomRight);

	//WeRenderDevice::Instance()->Device->BeginScene();
	{
		if (Texture)
			WeRenderDevice::Instance()->Device->SetTexture(0, Texture->GetTexture());
		else
			WeRenderDevice::Instance()->Device->SetTexture(0, NULL);

		SetupAlphaBlending(bmode);

		RenderVertexBuffer();

		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	

		//WeRenderDevice::Instance()->Device->EndScene();
	}
}



void WeCanvas::DrawRectangle(int x, int y, int w, int h, DWORD Color, LPDIRECT3DTEXTURE9 Texture, WeBlendingMode bmode)
{
	if (VertexBuffer == NULL) return;

	ChangeVBParams(x, y, w, h, Color);

	//WeRenderDevice::Instance()->Device->BeginScene();
	{
		if (Texture)
			WeRenderDevice::Instance()->Device->SetTexture(0, Texture);
		else
			WeRenderDevice::Instance()->Device->SetTexture(0, NULL);

		SetupAlphaBlending(bmode);

		RenderVertexBuffer();

		WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	

		//WeRenderDevice::Instance()->Device->EndScene();
	}
}




void WeCanvas::OnLostDevice()
{
	SAFE_RELEASE(VertexBuffer);

	for(Fonts.First(); !Fonts.End(); Fonts.Next())
		Fonts.GetItem()->OnLostDevice();

}



void WeCanvas::OnResetDevice()
{
	MakeVB();

	for(Fonts.First(); !Fonts.End(); Fonts.Next())
		Fonts.GetItem()->OnResetDevice();
}




void WeCanvas::ClearAllFonts()
{
	for(Fonts.First(); !Fonts.End(); Fonts.Next())
		Fonts.GetItem()->ShutDown();
	
	Fonts.Clear();
}


void WeCanvas::FreeFont(WeFont *Font)
{
	for (Fonts.First(); !Fonts.End(); Fonts.Next())
	{
		if (Fonts.GetItem() == Font)
		{
			Fonts.GetItem()->ShutDown();
			Fonts.Delete();
			return;
		}
	}
}
