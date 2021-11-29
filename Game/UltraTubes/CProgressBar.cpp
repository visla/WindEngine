/*
   CProgressBar.cpp
   Author: Goran Bogdanovic
*/


#include <CProgressBar.h>
#include <WeColor.h>
#include <WeTexture.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <Helper.h>
#include <CLocalizationSettings.h>
#include <CMainGameInterface.h>


CProgressBar::CProgressBar(ProgressBarVisualStyles MyStyle, bool LoadingText)
{
	WithText = LoadingText;

	LeftEdge = NULL;
	RightEdge = NULL;
	Middle = NULL;
	FilledLeftEdge = NULL;
	FilledRightEdge = NULL;
	FilledMiddle = NULL;
	Glow = NULL;

	ReplaceFinalText = false;
	FinalText = "";

	PosX = 0.25f;
	PosY = 0.8f;
	Width = 0.5f;
	Min = 0.0f;
	Max = 100.0f;
	Value = 0.0f;

	Style = MyStyle;
	LoadStyle();
}


CProgressBar::~CProgressBar()
{
	WeTextureManager::Instance()->Free(LeftEdge);
	WeTextureManager::Instance()->Free(RightEdge);
	WeTextureManager::Instance()->Free(Middle);
	WeTextureManager::Instance()->Free(FilledLeftEdge);
	WeTextureManager::Instance()->Free(FilledRightEdge);
	WeTextureManager::Instance()->Free(FilledMiddle);
	WeTextureManager::Instance()->Free(Glow);
}

void CProgressBar::LoadStyle()
{
	switch(Style)
	{
	case ProgressBarVisualStyle1:
		LeftEdge = WeTextureManager::Instance()->Load("Textures.PBar.LeftEdge", true);
		RightEdge = WeTextureManager::Instance()->Load("Textures.PBar.RightEdge", true);
		Middle = WeTextureManager::Instance()->Load("Textures.PBar.Middle", true);
		FilledLeftEdge = WeTextureManager::Instance()->Load("Textures.PBar.FilledLeftEdge", true);
		FilledRightEdge = WeTextureManager::Instance()->Load("Textures.PBar.FilledRightEdge", true);
		FilledMiddle = WeTextureManager::Instance()->Load("Textures.PBar.FilledMiddle", true);
		Glow = WeTextureManager::Instance()->Load("Textures.PBar.Glow", true);

		Height = RelY((int)((float)Middle->Height * HOffset()));
		break;
	}
}


void CProgressBar::Render()
{
	int AbsEdgeWidth = (int)((float)LeftEdge->Width * WOffset());
	int GlowWidth = (int)((float)Glow->Width * WOffset());

	WeColor Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	WeTexture *Tex = NULL;

	int StartX = AbsX(PosX);
	int StartY = AbsY(PosY);
	int AbsHeight = AbsY(Height);
	int TempWidth = 0;

	if (Value < Min) Value = Min;
	if (Value > Max) Value = Max;

	if (Value <= Min)
		Tex = LeftEdge;
	else
		Tex = FilledLeftEdge;
	WeCanvas::Instance()->DrawRectangle(StartX, StartY, AbsEdgeWidth, AbsHeight, Color, Tex);
	StartX += AbsEdgeWidth;

    if (Value > Min)
	{
        Tex = FilledMiddle;
		TempWidth = AbsX((Value / (Max - Min)) * Width) - (2 * AbsEdgeWidth);
		if (TempWidth < 0) TempWidth = 0;
		WeCanvas::Instance()->DrawRectangle(StartX, StartY, TempWidth, AbsHeight, Color, Tex);
		StartX += TempWidth;
	}

	float GlowStartX = (float)StartX;

	if (Value < Max)
	{
		Tex = Middle;
		TempWidth = AbsX(PosX + Width) - AbsEdgeWidth - StartX;
		if (TempWidth < 0) TempWidth = 0;
		WeCanvas::Instance()->DrawRectangle(StartX, StartY, TempWidth, AbsHeight, Color, Tex);
		StartX = TempWidth + StartX;
	}

	if (Value >= Max)
		Tex = FilledRightEdge;
	else
		Tex = RightEdge;
	WeCanvas::Instance()->DrawRectangle(StartX, StartY, AbsEdgeWidth, AbsHeight, Color, Tex);

	if ( (Value > Min) && (Value < Max) )
	{
		Tex = Glow;
		WeCanvas::Instance()->DrawRectangle((int)(GlowStartX - (int)((float)GlowWidth * 0.65f)), (StartY - (int)((float)AbsHeight / 2.0f)), GlowWidth, AbsHeight * 2, Color, Tex);
	}

	if (WithText)
	{
		static WeFont *LoadingFont = WeCanvas::Instance()->MakeFont("Xirod", (int)(12.0f * HOffset()));
		String LoadingText;
		float x, y;

		if (ReplaceFinalText && (Value >= Max) )
		{
			LoadingText = FinalText;
			x = PosX + (Width / 2.0f) - 0.06f;
			y = PosY + (Height / 2.0f) - 0.007f;
		}
		else
		{
			char bzz[100];
			sprintf(bzz, CLocalizationSettings::Instance()->GetLocalText("LOADING_PBAR"), (int)((Value / Max) * 100.0f) );
			LoadingText = String(bzz);
			x = PosX + (Width / 2.0f) - 0.05f;
			y = PosY + (Height / 2.0f) - 0.007f;
		}

		CMainGameInterface::DrawShadowedText(LoadingText, LoadingFont, x, y, Color);
	}
}
