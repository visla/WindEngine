/*
   File    : CCreditsPhase.h
   Autor   : Vuk Vranjkovic
   Date    : 10.3.2006.
   Version : 1.0
*/




#ifndef _CCreditsPhase_H
#define _CCreditsPhase_H


#include <CAbstractPhase.h>
class WeFont;


class CCreditsPhase: public CAbstractPhase
{
public:
	static CCreditsPhase*	Instance;

						CCreditsPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	float				UpdateAlphaStep(float Offset, float Alpha, float AlphaStep);

	float TextX;
	float Text1A;
	float Text2A;
	float Text3A;
	float Text4A;

	float WOffset;
	float HOffset;


private:
	float               YCoorText;
	float				Time;
	WeFont             *pFont;
};


#endif