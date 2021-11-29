/*
    CStoryTellerPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 30.3.2006.
	Version: 1.0
    
*/

#include <CStoryTellerPhase.h>
#include <CGame.h>
#include <CGameSettings.h>
#include <CMainMenuPhase.h>
#include <CMenuPhase.h>
#include <WeFont.h>
#include <WeCanvas.h>
#include <CFunnyPhase.h>
#include <WeTextureManager.h>
#include <CTournamentMapSettings.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>

CStoryTellerPhase *CStoryTellerPhase::Instance = new CStoryTellerPhase;


CStoryTellerPhase::CStoryTellerPhase()
{
	Background = NULL;
}

void CStoryTellerPhase::Start()
{
	Background = WeTextureManager::Instance()->Load(CGameSettings::Instance->MapSettings->EndGameImage, true);
}

void CStoryTellerPhase::End()
{
	WeTextureManager::Instance()->Free(Background);
	Background = NULL;
}

void CStoryTellerPhase::Tick()
{
	if (CGame::Instance->InputMessage.AnyKey)
	{
		CGame::Instance->SetPhase(CMenuPhase::Instance);
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
	}
}

void CStoryTellerPhase::Render()
{
		static WeFont *Font = NULL;

		if (!Font)
			Font = WeCanvas::Instance()->MakeFont("Xirod", 26);
        
		// Prikazi info za prva tri mesta
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), Background);

		//sprintf(tmp, "KRAJ IGRE!");
		static float ypos = (float)WeRenderDevice::Instance()->GetResHeight() + 30.0f;

		ypos -= WeTimer::Instance()->GetDeltaTimeF()*40.0f;
		if (ypos > -700.0f)
		{
			WeCanvas::Instance()->Draw2DText(Font, CGameSettings::Instance->MapSettings->EndGameText, 100, (int)ypos, 600, 700, D3DCOLOR_XRGB(255,255,255));
		} else
		{
			WeCanvas::Instance()->Draw2DText(Font, "KRAJ IGRE!", 100, 100, 400, 700, D3DCOLOR_XRGB(255,255,255));
		}

}