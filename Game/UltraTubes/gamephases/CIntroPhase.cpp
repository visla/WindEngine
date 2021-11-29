/*
    CIntroPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CIntroPhase.h>
#include <CGame.h>
#include <CLoadMenuPhase.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <WeTimer.h>
#include <CAVPlayback.h>
#include <dinput.h>
#include <CMainGamePhase.h>


#ifdef  _DEBUG
	#define WALLSPLASH_WAIT		0.0f
#else
	#define WALLSPLASH_WAIT		5.0f
#endif

CIntroPhase* CIntroPhase::Instance = new CIntroPhase;

CIntroPhase::CIntroPhase()
{
	Background = NULL;
}


void CIntroPhase::Start()
{
	CurrentState = INTRO_STATE_NONE;
	//CurrentState = INTRO_STATE_MOVIE_LOGO;		// use this state for no intro movie
}


void CIntroPhase::End()
{
	//WeTextureManager::Instance()->Free(Background);
	//Background = NULL;
}

void CIntroPhase::Tick()
{
	static float Time = 0.0f;
	Time += WeTimer::Instance()->GetDeltaTimeF();

	switch(CurrentState)
	{
	case INTRO_STATE_NONE:
		WeRenderDevice::Instance()->SetRendering(false);
		if (CAVPlayback::Instance->Play("load.seq"))
			CurrentState = INTRO_STATE_MOVIE_LOAD;
		break;

	case INTRO_STATE_MOVIE_LOAD:
		if ( (!CAVPlayback::Instance->Playing) )
		{
			CAVPlayback::Instance->Stop();
			Time = 0.0f;
			if (CAVPlayback::Instance->Play("intro.avi"))
			{
				CMainGamePhase::Instance->PlaySong(SongMenu);
				CurrentState = INTRO_STATE_MOVIE_LOGO;
			}
		}
		break;

	case INTRO_STATE_MOVIE_LOGO:
		if ( (!CAVPlayback::Instance->Playing) || (CGame::Instance->InputMessage.AnyKey) )
		{
			CAVPlayback::Instance->Stop();
			WeRenderDevice::Instance()->SetRendering(true);
			Time = 0.0f;
			CurrentState = INTRO_STATE_WALLSPLASH;
		}
		break;

	case INTRO_STATE_WALLSPLASH:
		if(Time >= WALLSPLASH_WAIT)
			CGame::Instance->SetPhase(CLoadMenuPhase::Instance);
		break;
	}
}

void CIntroPhase::Render()
{
	//switch(CurrentState)
	//{
	//case INTRO_STATE_WALLSPLASH:
		if (!Background)
			Background = WeTextureManager::Instance()->Load("Intro.jpg");

		WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),
												WeRenderDevice::Instance()->GetResHeight(),
												D3DCOLOR_XRGB(255,255,255),
												Background, bmNormal);

	//	break;
	//}
}
