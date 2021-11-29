/*
    CMiniGameJim.cpp

	Autor: Goran Bogdanovic
	Date: 3.4.2006.
	Version: 1.0
*/


#include <CMiniGameJim.h>
#include <CGame.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <WeTexture.h>
#include <WeTimer.h>
#include <SmartList.h>
#include <WeFont.h>


CMiniGameJim::CMiniGameJim()
{
	//set randomize seed
	srand( (unsigned)WeTimer::Instance()->GetSystemTime() );

	//set default variable values
	TimeElapsed = false;
	Score = 0;
	ShotsFired = 0;
	TargetsTotal = 0;
	MinSpeed = 50.0f;
	MaxSpeed = 200.0f;
	TotalTimeElapsed = 0.0f;

	//create pointer
	Pointer = new CPointerJim( WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Pointer", true) );
	Pointer->X = (int)((WeRenderDevice::Instance()->GetResWidth() / 2) - (MINI_GAME_JIM_POINTER_WIDTH / 2));
	Pointer->Y = (int)((WeRenderDevice::Instance()->GetResHeight() / 2) - (MINI_GAME_JIM_POINTER_HEIGHT / 2));

	//load target textures
	TargetVisuals[0] = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Target1", true);
	TargetVisuals[1] = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Target1", true);
	TargetVisuals[2] = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Target1", true);
	TargetVisuals[3] = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Target1", true);
	TargetVisuals[4] = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Target1", true);

	//load splash textures
	SplashTexture = WeTextureManager::Instance()->Load("TexturesFunny.JimClone.Splash", true);
}


CMiniGameJim::~CMiniGameJim()
{
	//clear target textures
    for(int i=0; i<MINI_GAME_JIM_TOTAL_TEXTURES; i++)
	{
		WeTextureManager::Instance()->Free(TargetVisuals[i]);
		TargetVisuals[i] = NULL;
	}
	//clear targets
	for(Targets.First(); !Targets.End(); Targets.Next())
	{
		SAFE_DELETE(Targets.GetItem());
	}
	Targets.Clear();

	//clear splash texture
	WeTextureManager::Instance()->Free(SplashTexture);
	SplashTexture = NULL;
	//clear splashes
	for(Splashes.First(); !Splashes.End(); Splashes.Next())
	{
		SAFE_DELETE(Splashes.GetItem());
	}
	Splashes.Clear();
}


void CMiniGameJim::RenderMiniGame()
{
	//render targets
	for(Targets.First(); !Targets.End(); Targets.Next())
	{
		WeCanvas::Instance()->DrawRectangle(
											Targets.GetItem()->X,
											Targets.GetItem()->Y,
											MINI_GAME_JIM_TARGET_WIDTH,
											MINI_GAME_JIM_TARGET_HEIGHT,
											0xffffffff,
											Targets.GetItem()->Visual );
	}

	//render splashes
	for(Splashes.First(); !Splashes.End(); Splashes.Next())
	{
		WeCanvas::Instance()->DrawRectangle(
											Splashes.GetItem()->X,
											Splashes.GetItem()->Y,
											MINI_GAME_JIM_SPLASH_WIDTH,
											MINI_GAME_JIM_SPLASH_HEIGHT,
											0xffffffff,
											Splashes.GetItem()->Visual );
	}

	//render mouse pointer
	WeCanvas::Instance()->DrawRectangle(
										Pointer->X,
										Pointer->Y,
										MINI_GAME_JIM_POINTER_WIDTH,
										MINI_GAME_JIM_POINTER_HEIGHT,
										0xffffffff,
										Pointer->Visual );

	//render statistics text
	static WeFont* Font = WeCanvas::Instance()->MakeFont("Arial", 20);
	char ScoreText[100];
	sprintf(ScoreText, "Score: %d", Score);
	WeCanvas::Instance()->Draw2DText(Font, ScoreText, 10, 10, 200, 20, D3DCOLOR_XRGB(0, 0, 0));
}

void CMiniGameJim::RenderStatistics()
{
	//make jokes on how the player is n00bish and sucks ass
	//...

	//render statistics text
	static WeFont* Font = WeCanvas::Instance()->MakeFont("Arial", 50);
	char ScoreText[100];
	sprintf(ScoreText, "Score: %d", Score);
	WeCanvas::Instance()->Draw2DText(Font, ScoreText, 10, 10, 200, 50, D3DCOLOR_XRGB(0, 0, 0));
}


void CMiniGameJim::Tick()
{
	const float TotalTime = 60.0f; //s
	const float Acceleration = 2.0f;
	const int MaxTargets = 10;

	int TotalWidth = WeRenderDevice::Instance()->GetResWidth();
	int TotalHeight = WeRenderDevice::Instance()->GetResHeight();

	//base tick
    CMiniGame::Tick();

	//check if minigame time has elapsed
	TotalTimeElapsed += WeTimer::Instance()->GetDeltaTimeF();

	if(TotalTimeElapsed >= TotalTime)
		TimeElapsed = true;

	if(TimeElapsed)
	{
        if(CGame::Instance->InputMessage.AnyKey)
			Finished = true;
		return;
	}

	//move the mouse pointer
	Pointer->X += CGame::InputMessage.rx;
	Pointer->Y += CGame::InputMessage.ry;
	Pointer->X = (Pointer->X < TotalWidth - (MINI_GAME_JIM_POINTER_WIDTH / 2)) ? Pointer->X : TotalWidth - (MINI_GAME_JIM_POINTER_WIDTH / 2);
	Pointer->X = (Pointer->X > -(MINI_GAME_JIM_POINTER_WIDTH / 2)) ? Pointer->X : -(MINI_GAME_JIM_POINTER_WIDTH / 2);
	Pointer->Y = (Pointer->Y < TotalHeight - (MINI_GAME_JIM_POINTER_HEIGHT / 2)) ? Pointer->Y : TotalHeight - (MINI_GAME_JIM_POINTER_HEIGHT / 2);
	Pointer->Y = (Pointer->Y > -(MINI_GAME_JIM_POINTER_HEIGHT / 2)) ? Pointer->Y : -(MINI_GAME_JIM_POINTER_HEIGHT / 2);

	//check for fire
	if(CGame::Instance->InputMessage.LButton)
	{
		int CenterX = Pointer->X + (MINI_GAME_JIM_POINTER_WIDTH / 2);
		int CenterY = Pointer->Y + (MINI_GAME_JIM_POINTER_HEIGHT / 2);

		ShotsFired += 1;
		//create a splash
		if(Splashes.GetItemsCount() < MINI_GAME_JIM_TOTAL_SPLASHES)
		{
			CSplashJim *NewSplash = new CSplashJim(GetSplashTexture());
            NewSplash->DisplayedMoment = WeTimer::Instance()->GetElapsedTimeF();
			NewSplash->X = CenterX - (MINI_GAME_JIM_SPLASH_WIDTH / 2);
			NewSplash->Y = CenterY - (MINI_GAME_JIM_SPLASH_HEIGHT / 2);
			Splashes.PushBack(NewSplash);
		}

		for(Targets.Last(); !Targets.Begin(); )
		{
			CTargetJim *Target = Targets.GetItem();
			
			//a target is hit
			if(	(CenterX <= (Target->X + MINI_GAME_JIM_TARGET_WIDTH)) && (CenterX >= Target->X) &&
				(CenterY <= (Target->Y + MINI_GAME_JIM_TARGET_HEIGHT)) && (CenterY >= Target->Y) )
			{
				delete Target;
				Targets.Delete();
				Score += 1;
				break;
			}
			else
			{
				Targets.Prev();
			}
		}
	}

	//remove old splashes
	for(Splashes.First(); !Splashes.End();)
	{
		CSplashJim *Splash = Splashes.GetItem();

		if((WeTimer::Instance()->GetElapsedTimeF() - Splash->DisplayedMoment) >= MINI_GAME_JIM_SPLASH_TIME)
		{
			delete Splash;
			Splashes.Delete();
		}
		else
		{
			Splashes.Next();
		}
	}

	//add a new target if needed
	if(Targets.GetItemsCount() < MaxTargets)
	{
		TargetsTotal += 1;
		
		CTargetJim *NewTarget = new CTargetJim(GetRandomTexture());
		NewTarget->X = rand() % TotalWidth;
		NewTarget->Y = TotalHeight;
		NewTarget->NextX = rand() % (TotalWidth - MINI_GAME_JIM_TARGET_WIDTH);
		NewTarget->Speed = (int)(MinSpeed + (rand() % (UINT)(MaxSpeed - MinSpeed)));
		MaxSpeed += Acceleration;
		MinSpeed += Acceleration;
		Targets.PushBack(NewTarget);
	}

	//move targets
	for(Targets.First(); !Targets.End();)
	{
		CTargetJim* Target = Targets.GetItem();
		float Time = WeTimer::Instance()->GetDeltaTimeF();

		if(Target->X <= Target->NextX)
		{
			Target->X += (int)(Target->Speed * Time);
			
			if(Target->X >= Target->NextX)
			{
				Target->NextX = rand() % TotalWidth - MINI_GAME_JIM_TARGET_WIDTH;
			}
		}
		else	//X > NextX
		{
			Target->X -= (int)(Target->Speed * Time);
			if(Target->X <= Target->NextX)
			{
				Target->NextX = rand() % TotalWidth - MINI_GAME_JIM_TARGET_WIDTH;
			}
		}

		Target->Y -= (int)(Target->Speed * Time);

		//remove target if missed
		if(Target->Y <= -MINI_GAME_JIM_TARGET_HEIGHT)
		{
			delete Target;
			Targets.Delete();
		}
		else
		{
			Targets.Next();
		}
	}
}


//get random target texture
WeTexture* CMiniGameJim::GetRandomTexture()
{
	return TargetVisuals[rand() % (MINI_GAME_JIM_TOTAL_TEXTURES - 1)];
}


//get splash texture
WeTexture* CMiniGameJim::GetSplashTexture()
{
	//can randomize here also to get different textures
	return SplashTexture;
}


//render redirection
void CMiniGameJim::Render()
{
	CMiniGame::Render();
	
	if(TimeElapsed)
		RenderStatistics();
	else
		RenderMiniGame();
}
