/*
   CProgressBar.h
   Author: Goran Bogdanovic
*/


#ifndef _CProgressBar_H
#define _CProgressBar_H


#include <CString.h>


enum ProgressBarVisualStyles
{
	ProgressBarVisualStyle1
};


class WeTexture;


class CProgressBar
{
protected:
	WeTexture					*LeftEdge;
	WeTexture					*RightEdge;
	WeTexture					*Middle;
	WeTexture					*FilledLeftEdge;
	WeTexture					*FilledRightEdge;
	WeTexture					*FilledMiddle;
	WeTexture					*Glow;

	void						LoadStyle();

	bool						WithText;

public:
	CProgressBar(ProgressBarVisualStyles MyStyle, bool LoadingText = true);
	~CProgressBar();

	ProgressBarVisualStyles		Style;

	float						PosX;
	float						PosY;
	float						Width;
	float						Height;

	float						Min;
	float						Max;
	float						Value;

	bool						ReplaceFinalText;
	String						FinalText;

	void						Render();

};


#endif