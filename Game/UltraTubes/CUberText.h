/*
	CUberText.h
	Author:	Goran Bogdanovic
*/


#ifndef _CUberText_h_
#define _CUberText_h_

#include <WeColor.h>
#include <CString.h>
#include <WeRenderDevice.h>

#define ANIM_ADDON_FONT		100
#define ANIM_ADDON_TEXTURE	200


enum UberTextGeneralType
{
	UberTextGeneralTypeFont,
	UberTextGeneralTypeTexture
};

enum UberTextAnimTypes
{
	UberTextTypeAnimRandom = -1,
	UberTextTypeAnimInstant = 0,
	UberTextTypeAnimFade = 1,
	UberTextTypeAnimSlideUp = 2,
	UberTextTypeAnimSlideDown = 3,
	UberTextTypeAnimSlideLeft = 4,
	UberTextTypeAnimSlideRight = 5,

	UberTextTypeAnimLetterShowFO = ANIM_ADDON_FONT + 6,

	UberTextTypeAnimFadeFrontTO = ANIM_ADDON_TEXTURE + 6,
	UberTextTypeAnimFadeBackTO = ANIM_ADDON_TEXTURE + 7
};

enum UberTextState
{
	UberTextStateNothing,
	UberTextStateShowing,
	UberTextStatePassive,
	UberTextStateHiding,
	UberTextStateFinished
};


class WeFont;


class CUberText
{
protected:
	UberTextGeneralType	m_GeneralType;
	UberTextAnimTypes	m_AnimTypeShow;
	UberTextAnimTypes	m_AnimTypeHide;

	float				m_TimeShow;
	float				m_TimeHide;
	float				m_TimeStay;

	WeTexture			*Texture;

	WeFont				*TextFont;
	WeColor				Color;
	bool				Shadow;
	bool				ShutdownFont;

	float				FinalX;
	float				FinalY;
	String				FullText;

	float				FinalWidth;
	float				FinalHeight;

	UberTextState		CurrentState;
	String				Text;
	float				x;
	float				y;
	float				Timer;
	int					LetterCount;
	float				width;
	float				height;

	void				Initialize( String DisplayText, float DestX, float DestY,
									UberTextAnimTypes ShowAnimation, UberTextAnimTypes HideAnimation,
									bool AutoPlacement );

	void				InitShowing();
	void				TickShowing();
	void				TickPassive();
	void				TickHiding();

	UberTextAnimTypes	GetRandomAnim();
    int					AbsX(float x)	{ return (int)(x * WeRenderDevice::Instance()->GetResWidth()); }
	int					AbsY(float y)	{ return (int)(y * WeRenderDevice::Instance()->GetResHeight()); }
	float				RelX(int x)		{ return ((float)x / (float)WeRenderDevice::Instance()->GetResWidth()); }
	float				RelY(int y)		{ return ((float)y / (float)WeRenderDevice::Instance()->GetResHeight()); }

	int					MyCell;
	bool				WithCell;
	static bool			FreeCells[10];

	float				MeasureTextWidth();


public:
	CUberText(	String DisplayText, float DestX, float DestY, 
				UberTextAnimTypes ShowAnimation = UberTextTypeAnimRandom, UberTextAnimTypes HideAnimation = UberTextTypeAnimRandom);
	CUberText(	String DisplayText, UberTextAnimTypes ShowAnimation = UberTextTypeAnimRandom, UberTextAnimTypes HideAnimation = UberTextTypeAnimRandom);
	CUberText(	WeTexture *FlatAnim, float DestX, float DestY, float Width, float Height,
		UberTextAnimTypes ShowAnimation = UberTextTypeAnimRandom, UberTextAnimTypes HideAnimation = UberTextTypeAnimRandom);
	~CUberText();

	bool				Dying;

	UberTextGeneralType	GetGeneralType();
	UberTextAnimTypes	GetAnimTypeShow();
	void				SetAnimTypeShow(UberTextAnimTypes Value);
	UberTextAnimTypes	GetAnimTypeHide();
	void				SetAnimTypeHide(UberTextAnimTypes Value);

	float				GetTimeShow();
	void				SetTimeShow(float Value);
	float				GetTimeHide();
	void				SetTimeHide(float Value);
	float				GetTimeStay();
	void				SetTimeStay(float Value);

	WeFont*				GetFont();
	void				SetFont(WeFont* Value);
	void				SetFont(String Name, int Size);
	WeColor				GetTextColor();
	void				SetTextColor(WeColor Value);
	bool				GetShadow();
	void				SetShadow(bool Value);

	float				GetDestX();
	float				GetDestY();
	String				GetText();

	float				GetCurrentX();
	float				GetCurrentY();
	UberTextState		GetCurrentState();

	void				Tick();
	void				Render();

	static float		MeasureTextWidth(String Text, WeFont *TextFont);

};


#endif
