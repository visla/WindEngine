/*
    CBuyDemoPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CBuyDemoPhase.h>
#include <CGame.h>
#include <WeTextureManager.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <WeTimer.h>
#include <CMenuPhase.h>
#include <CExitMenuPhase.h>


CBuyDemoPhase *CBuyDemoPhase::Instance = new CBuyDemoPhase;


CBuyDemoPhase::CBuyDemoPhase()
{
	pTex=NULL;
}

void CBuyDemoPhase::Start()
{
	pTex = WeTextureManager::Instance()->Load("Intro.jpg");
}

void CBuyDemoPhase::End()
{
	WeTextureManager::Instance()->Free(pTex); pTex=NULL;
}

void CBuyDemoPhase::Tick()
{
	static float Time=0.0f;

#ifndef _DEBUG
	if((Time+=WeTimer::Instance()->GetDeltaTimeF())>5.0f)
#endif
		CMenuPhase::Instance->SetPhase(CExitMenuPhase::Instance);
}

void CBuyDemoPhase::Render()
{
	WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),
											WeRenderDevice::Instance()->GetResHeight(),
											D3DCOLOR_XRGB(255,255,255),
											pTex);
}