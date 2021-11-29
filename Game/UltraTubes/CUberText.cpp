/*
	CUberText.cpp
	Author:	Goran Bogdanovic
*/


#include <CUberText.h>
#include <WeCanvas.h>
#include <WeTimer.h>
#include <WeFont.h>
#include <WeTexture.h>
#include <Helper.h>
#include <CMainGameInterface.h>

//=== GDI+ ===//
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#include <gdiplus.h>
#undef min
#undef max
//============//

#define DEFAULT_TIME_SHOW	0.5f
#define	DEFAULT_TIME_HIDE	0.5f
#define DEFAULT_TIME_STAY	2.0f
#define DEFAULT_COLOR		WeColor(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_SHADOW		true

#define CELL_FIRST_Y		0.3f
#define CELL_SPACE			0.02f


bool CUberText::FreeCells[10] = { true, true, true, true, true, true, true, true, true, true } ;


CUberText::CUberText(String DisplayText, float DestX, float DestY, UberTextAnimTypes ShowAnimation, UberTextAnimTypes HideAnimation)
{
	m_GeneralType = UberTextGeneralTypeFont;
	Initialize(DisplayText, DestX, DestY, ShowAnimation, HideAnimation, false);
}

CUberText::CUberText(String DisplayText, UberTextAnimTypes ShowAnimation, UberTextAnimTypes HideAnimation)
{
	m_GeneralType = UberTextGeneralTypeFont;

	float DestX = 0.0f;
	float DestY = 0.0f;
	TextFont = NULL;

	MyCell = -1;
	for (int i = 0; i < 10; i++)
	{
		if (FreeCells[i])
		{
			MyCell = i;
			FreeCells[i] = false;
			DestY = CELL_FIRST_Y + ((i + 1) * CELL_SPACE);
			break;
		}
	}

    if (MyCell != -1)
	{
		Initialize(DisplayText, DestX, DestY, ShowAnimation, HideAnimation, true);
		FinalX = 0.5f - (MeasureTextWidth() / 2);
	}
	else
	{
		Dying = true;
	}
}

CUberText::CUberText(WeTexture *FlatAnim, float DestX, float DestY, float Width, float Height, UberTextAnimTypes ShowAnimation, UberTextAnimTypes HideAnimation)
{
	Initialize("", DestX, DestY, ShowAnimation, HideAnimation, false);

	FinalWidth = Width;
	FinalHeight = Height;
	Texture = FlatAnim;
}


void CUberText::Initialize(	String DisplayText, float DestX, float DestY, 
							UberTextAnimTypes ShowAnimation, UberTextAnimTypes HideAnimation,
							bool AutoPlacement )
{
	Dying = false;
	ShutdownFont = true;

	Texture = NULL;
	if (ShowAnimation == UberTextTypeAnimRandom) ShowAnimation = GetRandomAnim();
	if (HideAnimation == UberTextTypeAnimRandom) HideAnimation = GetRandomAnim();
	m_AnimTypeShow = ShowAnimation;
	m_AnimTypeHide = HideAnimation;

	m_TimeShow = DEFAULT_TIME_SHOW;
	m_TimeHide = DEFAULT_TIME_HIDE;
	m_TimeStay = DEFAULT_TIME_STAY;

	TextFont = WeCanvas::Instance()->MakeFont("Xirod", (int)(0.03f * WeRenderDevice::Instance()->GetResHeight()));
	Color = DEFAULT_COLOR;
	Shadow = DEFAULT_SHADOW;

	FullText = DisplayText;
	Text = FullText;
	FinalX = DestX;
	FinalY = DestY;

	CurrentState = UberTextStateNothing;
	x = 0.0f;
	y = 0.0f;
	Timer = 0.0f;
	LetterCount = 0;
	MyCell = AutoPlacement ? MyCell : -1;
}


CUberText::~CUberText()
{
	if (ShutdownFont)
		WeCanvas::Instance()->FreeFont(TextFont);
	TextFont = NULL;
	if (MyCell != -1) FreeCells[MyCell] = true;
	Texture = NULL;
}

UberTextGeneralType	CUberText::GetGeneralType()
{
	return m_GeneralType;
}

UberTextAnimTypes CUberText::GetAnimTypeShow()
{
	return m_AnimTypeShow;
}

void CUberText::SetAnimTypeShow(UberTextAnimTypes Value)
{
	m_AnimTypeShow = Value;
}


UberTextAnimTypes CUberText::GetAnimTypeHide()
{
	return m_AnimTypeHide;
}

void CUberText::SetAnimTypeHide(UberTextAnimTypes Value)
{
	m_AnimTypeHide = Value;
}


float CUberText::GetTimeShow()
{
	return m_TimeShow;
}

void CUberText::SetTimeShow(float Value)
{
	m_TimeShow = Value;
}


float CUberText::GetTimeHide()
{
	return m_TimeHide;
}

void CUberText::SetTimeHide(float Value)
{
	m_TimeHide = Value;
}


float CUberText::GetTimeStay()
{
	return m_TimeStay;
}

void CUberText::SetTimeStay(float Value)
{
	m_TimeStay = Value;
}


WeFont*	CUberText::GetFont()
{
	return TextFont;
}

void CUberText::SetFont(WeFont* Value)
{
	WeCanvas::Instance()->FreeFont(TextFont);
	TextFont = Value;
	ShutdownFont = false;
}

void CUberText::SetFont(String Name, int Size)
{
	WeCanvas::Instance()->FreeFont(TextFont);
	TextFont = WeCanvas::Instance()->MakeFont(Name, Size);
	ShutdownFont = true;
}


WeColor CUberText::GetTextColor()
{
	return Color;
}

void CUberText::SetTextColor(WeColor Value)
{
	Color = Value;
}


bool CUberText::GetShadow()
{
	return Shadow;
}

void CUberText::SetShadow(bool Value)
{
	Shadow = Value;
}


float CUberText::GetDestX()
{
	return FinalX;
}

float CUberText::GetDestY()
{
	return FinalY;
}

String CUberText::GetText()
{
	return Text;
}


void CUberText::Tick()
{
	if (Dying) return;

	Timer += WeTimer::Instance()->GetDeltaTimeF();

	switch(CurrentState)
	{
	case UberTextStateNothing:
		InitShowing();
		break;

	case UberTextStateShowing:
		if (Timer >= m_TimeShow)
		{
			CurrentState = UberTextStatePassive;
			Timer = 0.0f;
		}
		else
		{
			TickShowing();
		}
		break;

	case UberTextStatePassive:
		if (Timer >= m_TimeStay)
		{
			CurrentState = UberTextStateHiding;
			Timer = 0.0f;
		}
		else
		{
			TickPassive();
		}
		break;

	case UberTextStateHiding:
		if (Timer >= m_TimeHide)
		{
			CurrentState = UberTextStateFinished;
			Timer = 0.0f;
		}
		else
		{
			TickHiding();
		}
		break;

	case UberTextStateFinished:
		Dying = true;
		return;

	}
}

void CUberText::InitShowing()
{
	switch(m_AnimTypeShow)
	{
	case UberTextTypeAnimInstant:
		x = FinalX;
		y = FinalY;
		break;

	case UberTextTypeAnimFade:
		x = FinalX;
		y = FinalY;
		Color.a = 0.0f;
		break;

	case UberTextTypeAnimSlideUp:
		x = FinalX;
		y = 1.0f;
		Color.a = 0.0f;
		break;

	case UberTextTypeAnimSlideDown:
		x = FinalX;
		y = -CELL_SPACE;
		Color.a = 0.0f;
		break;

	case UberTextTypeAnimSlideLeft:
		x = 1.0f;
		y = FinalY;
		Color.a = 0.0f;
		break;

	case UberTextTypeAnimSlideRight:
		x = 0.0f;
		y = FinalY;
		Color.a = 0.0f;
		break;

	case UberTextTypeAnimLetterShowFO:
		x = FinalX;
		y = FinalY;
		Text = "";
        break;

	case UberTextTypeAnimFadeFrontTO:
		Color.a = 0.0f;
		width = 0.0f;
		height = 0.0f;
		x = 5.0f;
		y = 5.0f;
		break;

	case UberTextTypeAnimFadeBackTO:
		Color.a = 0.0f;
		width = 0.0f;
		height = 0.0f;
		x = 5.0f;
		y = 5.0f;
		break;
	}

	CurrentState = UberTextStateShowing;
	Timer = 0.0f;
}

void CUberText::TickShowing()
{
	float Factor = (Timer / m_TimeShow);
	if (Factor > 1.0f) Factor = 1.0f;

	switch(m_AnimTypeShow)
	{
	case UberTextTypeAnimInstant:
		Timer = m_TimeShow;
		break;

	case UberTextTypeAnimFade:
		Color.a = 1.0f * Factor;
		break;

	case UberTextTypeAnimSlideUp:
		Color.a = 1.0f * Factor;
		y = 1.0f - ((1.0f - FinalY) * Factor);
		break;

	case UberTextTypeAnimSlideDown:
		Color.a = 1.0f * Factor;
		y = -CELL_SPACE + ((FinalY + CELL_SPACE) * Factor);
		break;

	case UberTextTypeAnimSlideLeft:
		Color.a = 1.0f * Factor;
		x = 1.0f - ((1.0f - FinalX) * Factor);
		break;

	case UberTextTypeAnimSlideRight:
		Color.a = 1.0f * Factor;
		x = FinalX * Factor;
		break;

	case UberTextTypeAnimLetterShowFO:
		Text = "";
		for (int i = 0; i < (int)((float)FullText.GetLen() * Factor); i++)
			Text.InsertChar(Text.GetLen(), FullText[i]);
		break;

	case UberTextTypeAnimFadeFrontTO:
		Color.a = (1.0f - Factor);
		width = FinalWidth * Factor;
		height = FinalHeight * Factor;
		x = FinalX + ((FinalWidth - width) / 2.0f);
		y = FinalY + ((FinalHeight - height) / 2.0f);
		break;

	case UberTextTypeAnimFadeBackTO:
		Color.a = Factor;
		width = FinalWidth * (4.0f - (3.0f * Factor));
		height = FinalHeight * (4.0f - (3.0f * Factor));
		x = FinalX + ((FinalWidth - width) / 2.0f);
		y = FinalY + ((FinalHeight - height) / 2.0f);
		break;
	}
}

void CUberText::TickPassive()
{
	x = FinalX;
	y = FinalY;
	Text = FullText;
}

void CUberText::TickHiding()
{
	float Factor = (Timer / m_TimeHide);
	if (Factor > 1.0f) Factor = 1.0f;

	switch(m_AnimTypeHide)
	{
	case UberTextTypeAnimInstant:
		Timer = m_TimeHide;
		break;

	case UberTextTypeAnimFade:
		Color.a = 1.0f - (1.0f * Factor);
		break;

	case UberTextTypeAnimSlideUp:
		Color.a = 1.0f - (1.0f * Factor);
		y = FinalY - ((FinalY + CELL_SPACE) * Factor);
		break;

	case UberTextTypeAnimSlideDown:
		Color.a = 1.0f - (1.0f * Factor);
		y = FinalY + ((1.0f + CELL_SPACE - FinalY) * Factor);
		break;

	case UberTextTypeAnimSlideLeft:
		Color.a = 1.0f - (1.0f * Factor);
		x = FinalX - (FinalX * Factor);
		break;

	case UberTextTypeAnimSlideRight:
		Color.a = 1.0f - (1.0f * Factor);
		x = FinalX + ((1.0f - FinalX) * Factor);
		break;

	case UberTextTypeAnimLetterShowFO:
		Text = "";
		for (int i = 0; i < (FullText.GetLen() - (int)((float)FullText.GetLen() * Factor)); i++)
			Text.InsertChar(Text.GetLen(), FullText[i]);
		break;

	case UberTextTypeAnimFadeFrontTO:
		Color.a = Factor;
		width = FinalWidth * (1.0f - Factor);
		height = FinalHeight * (1.0f - Factor);
		x = FinalX + ((FinalWidth - width) / 2.0f);
		y = FinalY + ((FinalHeight - height) / 2.0f);
		break;

	case UberTextTypeAnimFadeBackTO:
		break;
	}
}


void CUberText::Render()
{
	if (Dying) return;
	if (CurrentState == UberTextStateNothing) return;

	if (Texture)
	{
		WeCanvas::Instance()->DrawRectangle(AbsX(x), AbsY(y), AbsX(width), AbsY(height), Color, Texture);
	}
	else
	{
		if (!TextFont) return;

		if (Shadow)
		{
			CMainGameInterface::DrawShadowedText(Text, TextFont, x, y, Color);
		}
		WeCanvas::Instance()->Draw2DText(TextFont, Text, AbsX(x), AbsY(y), WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), Color);
	}
}


using namespace Gdiplus;

float CUberText::MeasureTextWidth()
{
	//HDC hdc = GetDC(WeRenderDevice::Instance()->GetHWND());
	//Graphics graph(hdc);

	//int Len = MultiByteToWideChar(CP_UTF8, 0, Text, -1, NULL, 0);
	//LPWSTR text = new wchar_t[Len + 1];
	//MultiByteToWideChar(CP_UTF8, 0, Text, -1, text, Len);

	//PointF origin;
	//origin.X = 0.0f; origin.Y = 0.0f;

	//Gdiplus::Font font(L"Xirod.ttf", 20, FontStyleRegular, UnitPoint, NULL);

	//RectF rect;
	//graph.MeasureString(text, -1, &font, origin, &rect);

	//return (float)(rect.Width / WeRenderDevice::Instance()->GetResWidth());

	//munched version, above thingy is not working =(
	return ((float)(Text.GetLen() * (TextFont->GetHeight() - 4)) / (float)WeRenderDevice::Instance()->GetResWidth());
}


float CUberText::MeasureTextWidth(String Text, WeFont *TextFont)
{
	return ((float)(Text.GetLen() * (TextFont->GetHeight() - 4)) / (float)WeRenderDevice::Instance()->GetResWidth());
}

UberTextAnimTypes CUberText::GetRandomAnim()
{
	const int BOTH_ANIM_COUNT = 5;
	const int FONT_ANIM_COUNT = 1;
	const int TEXT_ANIM_COUNT = 2;

	int SelectedAnim = (int)UberTextTypeAnimInstant;

	switch (m_GeneralType)
	{
	case UberTextGeneralTypeFont:
		SelectedAnim = Rnd(1, (BOTH_ANIM_COUNT + FONT_ANIM_COUNT));
		if (SelectedAnim > BOTH_ANIM_COUNT) SelectedAnim += ANIM_ADDON_FONT;
		break;

	case UberTextGeneralTypeTexture:
		SelectedAnim = Rnd(1, (BOTH_ANIM_COUNT + TEXT_ANIM_COUNT));
		if (SelectedAnim > BOTH_ANIM_COUNT) SelectedAnim += ANIM_ADDON_TEXTURE;
		break;
	}

	return (UberTextAnimTypes)SelectedAnim;
}
