/*
	Filename:	Helper.h
	Author:		Goran Bogdanovic
*/


#ifndef _HELPER_H
#define _HELPER_H


#include <stdlib.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>


inline int AbsX(float x)
{
	return (int)(x * WeRenderDevice::Instance()->GetResWidth());
}

inline int AbsY(float y)
{
	return (int)(y * WeRenderDevice::Instance()->GetResHeight());
}

inline float RelX(int x)
{
	return ((float)x / (float)WeRenderDevice::Instance()->GetResWidth());
}

inline float RelY(int y)
{
	return ((float)y / (float)WeRenderDevice::Instance()->GetResHeight());
}

inline float WOffset()
{
	return (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
}

inline float HOffset()
{
	return (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;
}

inline float OffsetX(int x)
{
	return ((float)x / 1024.0f);
}

inline float OffsetY(int y)
{
	return ((float)y / 768.0f);
}

inline int IntX(int x)
{
	return (int)((float)x * WOffset());
}

inline int IntY(int y)
{
	return (int)((float)y * HOffset());
}

inline void SRnd()
{
	static bool SRanded = false;
	if (SRanded) return;

	srand((unsigned)WeTimer::Instance()->GetSystemTime());
	SRanded = true;
}

inline UINT Rnd(UINT From, UINT To)
{
	SRnd();
	return ((rand() % (To - From + 1)) + From);
}

inline UINT Rnd(UINT To)
{
	SRnd();
	return (rand() % To);
}

inline float RndF(float From, float To)
{
	SRnd();
	return (From + (To - From) * ((float)rand() / RAND_MAX));
}

inline int round(float x)
{
	if (x >= 0.0f)
		return (int)(x + 0.5f);
	else
		return (int)(x - 0.5f);
}

#endif
