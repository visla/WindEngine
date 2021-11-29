/*
    CShoppingPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CShoppingPhase.h>
#include <CGame.h>
#include <WeCEGUI.h>
#include <WeTimer.h>
#include <WeTextureManager.h>
#include <CShipyardPhase.h>

//////////////////////////////////////////////////////////////////////////
// delete this after deleting pass-through code
#include <CMainGamePhase.h>
#include <CLoadMapPhase.h>
#include <CGameClient.h>
#include <CProfileManager.h>
#include <CTuberDescriptionManager.h>
//////////////////////////////////////////////////////////////////////////

CShoppingPhase *CShoppingPhase::Instance = new CShoppingPhase;





CShoppingPhase::CShoppingPhase()
{
	ShopBackground = NULL;
}



void CShoppingPhase::RenderStdScreen()
{
	CEGUI::System::getSingleton().renderGUI();        
}




void CShoppingPhase::Start()
{
#ifdef _DEBUG
	//////////////////////////////////////////////////////////////////////////
	// pass through only
	//
	// Automaticly buy tuber
	int team = CProfileManager::Instance()->GetCurrentProfile()->TeamNumber;

	FILE *f = fopen("ut_autobuy.txt", "rt");
	if (f)
	{
		char TuberName[100];
		while (team > 0)
		{
			fscanf(f, "%s\n", TuberName);
			--team;
		}
		fclose(f);

		CSmartList <CTuberDescription> *AllDescs = &CTuberDescriptionManager::Instance()->Descriptions;
		for (AllDescs->First(); !AllDescs->End(); AllDescs->Next())
		{
			CTuberDescription* Desc = &AllDescs->GetItem();
			if (Desc->Name.EqualCaseInsensitive((String)TuberName))
			{
				CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = *Desc;
				CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = true;
				break;
			}
		}

		if (!CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
		{
			AllDescs->First();
			CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = AllDescs->GetItem();
			CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = true;
		}
	}

	CGameClient::Instance->SendTuberDesc(&CProfileManager::Instance()->GetCurrentProfile()->TuberDescription);

	if (CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	{
		CGame::Instance->SetPhase(CMainGamePhase::Instance);
		CMainGamePhase::Instance->SetPhase(CLoadMapPhase::Instance);
		return;
	}
	//CGame::Instance->SetPhase(CMainGamePhase::Instance);
	//CMainGamePhase::Instance->SetPhase(CLoadMapPhase::Instance);

	//return;
	// delete this l8r
	//////////////////////////////////////////////////////////////////////////
#endif

	if (!CShoppingPhase::Instance->ShopBackground)
		CShoppingPhase::Instance->ShopBackground = WeTextureManager::Instance()->Load("Garage.Textures.ShipyardBackground", true);

	SetPhase(CShipyardPhase::Instance);
}



void CShoppingPhase::End()
{
	WeTextureManager::Instance()->Free(ShopBackground);
	ShopBackground = NULL;
	

	CAbstractPhase::End();
	//if (CurrentPhase) CurrentPhase->End();
}



void CShoppingPhase::Tick()
{
	CEGUI::System::getSingleton().injectTimePulse(WeTimer::Instance()->GetDeltaTimeF());
	CAbstractPhase::Tick();		
}



void CShoppingPhase::Render()
{
	if (CurrentPhase)
		CurrentPhase->Render();
}

