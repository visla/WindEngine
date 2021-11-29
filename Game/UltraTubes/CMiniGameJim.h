/*
    CMiniGameJim.h

	Autor: Goran Bogdanovic
	Date: 3.4.2006.
	Version: 1.0
*/

#ifndef _CMiniGameJim_H
#define _CMiniGameJim_H

#define MINI_GAME_JIM_TOTAL_TEXTURES	5
#define MINI_GAME_JIM_TOTAL_SPLASHES	10

#define MINI_GAME_JIM_TARGET_WIDTH		64
#define MINI_GAME_JIM_TARGET_HEIGHT		64
#define MINI_GAME_JIM_POINTER_WIDTH		32
#define MINI_GAME_JIM_POINTER_HEIGHT	32
#define MINI_GAME_JIM_SPLASH_WIDTH		16
#define MINI_GAME_JIM_SPLASH_HEIGHT		16

#define MINI_GAME_JIM_SPLASH_TIME		0.25f	//s


#include <WeTextureManager.h>
#include <WeTexture.h>
#include <CMiniGame.h>



class CPointerJim
{
public:
				CPointerJim(WeTexture* Arrow) { Visual = Arrow; }
				~CPointerJim() { WeTextureManager::Instance()->Free(Visual); }

	int			X;
	int			Y;
	WeTexture*	Visual;
};


class CTargetJim
{
public:
				CTargetJim(WeTexture* MyVisual) { Visual = MyVisual; }

	int			X;
	int			Y;
	int			Speed;
	int			NextX;
	WeTexture*  Visual;
};


class CSplashJim
{
public:
				CSplashJim(WeTexture* MyVisual) { Visual = MyVisual; }
	
	int			X;
	int			Y;
	float		DisplayedMoment;
	WeTexture*	Visual;
};


class CMiniGameJim: public CMiniGame
{
protected:
	WeTexture*					TargetVisuals[MINI_GAME_JIM_TOTAL_TEXTURES - 1];
	WeTexture*					SplashTexture;

	CSmartList<CTargetJim*>		Targets;
	CSmartList<CSplashJim*>		Splashes;
	CPointerJim*				Pointer;

	int							TargetsTotal;
	int							Score;
	int							ShotsFired;
	bool						TimeElapsed;

	float						MinSpeed;
	float						MaxSpeed;
	float						TotalTimeElapsed;

	WeTexture*					GetSplashTexture();
	WeTexture*					GetRandomTexture();

	void						RenderMiniGame();
	void						RenderStatistics();

public:
								CMiniGameJim();
								~CMiniGameJim();

	void						Tick();
	void						Render();
};


#endif