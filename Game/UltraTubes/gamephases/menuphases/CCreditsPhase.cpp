/*
    File    : CCreditsPhase.cpp
	Autor   : Vuk Vranjkovic
	Date    : 10.3.2006.
	Version : 1.0
*/

#include <CCreditsPhase.h>
#include <CGame.h>
#include <CMainMenuPhase.h>
#include <WeFont.h>
#include <CString.h>
#include <WeCanvas.h>
#include <CInputMessage.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <WeTimer.h>
#include <WeRenderDevice.h>


const String CreditsText1("ULTRA TUBES CREDITS:\n\nOriginal idea by:\nMirko Topalski\nNebojsa Vislavski");
const String CreditsText2("\n\nPROGAMERS:\nTomislav Kukic\nGoran Bogdanovic\nAleksa Todorovic");
const String CreditsText3("\n\nADDITIONALL PROGRAMMING:\nVuk Vranjkovic");
const String CreditsText4("Art:\nUros Banjesevic\nNenad Tomic\nSandor Goli");

CCreditsPhase *CCreditsPhase::Instance = new CCreditsPhase;


CCreditsPhase::CCreditsPhase()
{
	TextX = 200;
}



void CCreditsPhase::Start()
{
	pFont = WeCanvas::Instance()->MakeFont("Xirod", 28);
	Time = 0.0f;
	YCoorText = (float)WeRenderDevice::Instance()->GetResHeight();

	Text1A = 0.0f;
	Text2A = 0.0f;
	Text3A = 0.0f;
	Text4A = 0.0f;

	WOffset = WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	HOffset = WeRenderDevice::Instance()->GetResHeight() / 768.0f;
}


void CCreditsPhase::End()
{
}



void CCreditsPhase::Tick()
{
	Time = WeTimer::Instance()->GetDeltaTimeF();
	YCoorText -= Time * 30.0f;
	float AlphaStep = Time * 10.0f;


	if(CGame::Instance->InputMessage.Exit)
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);


	Text1A = UpdateAlphaStep(0.0f, Text1A, AlphaStep);
	Text2A = UpdateAlphaStep(150.0f, Text2A, AlphaStep);
	Text3A = UpdateAlphaStep(300.0f, Text3A, AlphaStep);
	Text4A = UpdateAlphaStep(450.0f, Text4A, AlphaStep);
}



float CCreditsPhase::UpdateAlphaStep(float Offset, float Alpha, float AlphaStep)
{
	float tmpF = Alpha;
	if(((YCoorText + Offset) < WeRenderDevice::Instance()->GetResHeight()) && ((YCoorText + Offset) > (WeRenderDevice::Instance()->GetResHeight() / 2)))
	{
		tmpF += AlphaStep;
		if(tmpF >= 255.0f)
			tmpF = 255.0f;
	}
	
	if(((YCoorText + Offset) < WeRenderDevice::Instance()->GetResHeight()) && ((YCoorText + Offset) < (WeRenderDevice::Instance()->GetResHeight() / 2)))
	{
		tmpF -= AlphaStep;
		if(tmpF <= 0.0f)
			tmpF = 0.0f;
	}

	return tmpF;
}





void CCreditsPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();

	WeCanvas::Instance()->Draw2DText(pFont,CreditsText1, (int)(300*WOffset), (int)(YCoorText*HOffset),		1000, 1000, D3DCOLOR_RGBA(255,255,255,(int)Text1A));
	WeCanvas::Instance()->Draw2DText(pFont,CreditsText2, (int)(300*WOffset), (int)(YCoorText*HOffset + 150*HOffset), 1000, 1000, D3DCOLOR_RGBA(255,255,255,(int)Text2A));
	WeCanvas::Instance()->Draw2DText(pFont,CreditsText3, (int)(300*WOffset), (int)(YCoorText*HOffset + 300*HOffset), 1000, 1000, D3DCOLOR_RGBA(255,255,255,(int)Text3A));
	WeCanvas::Instance()->Draw2DText(pFont,CreditsText4, (int)(300*WOffset), (int)(YCoorText*HOffset + 450*HOffset), 1000, 1000, D3DCOLOR_RGBA(255,255,255,(int)Text4A));
}


