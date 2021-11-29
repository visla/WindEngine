/*
    CMainShoppingPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CMainShoppingPhase.h>
#include <CGame.h>
#include <AllPhases.h>
#include <CGUIHelper.h>
#include <CGameClient.h>
#include <CProfileManager.h>
#include <CProfile.h>
#include <WeTimer.h>
#include <CGameServer.h>
#include <WeSound.h>
#include <CMenuPhase.h>
#include <WeRenderDevice.h>
#include <WeTextureManager.h>
#include <CLocalizationSettings.h>
#include <CTuberDescriptionManager.h>


CMainShoppingPhase *CMainShoppingPhase::Instance = new CMainShoppingPhase;


CMainShoppingPhase::CMainShoppingPhase()
{
	//Created = false;
}


void CMainShoppingPhase::Start()
{
	CEGUI::Window *Button;
	float y = 0.83f;

	CGUIHelper::CreateButton("Shopping_Back_Button", CLocalizationSettings::Instance()->GetLocalText("BACK"), 0.05f, y, 0.2f, 0.12f, "", &CMainShoppingPhase::OnBack);

	CGUIHelper::CreateButton("Shopping_Shipyard_Button", CLocalizationSettings::Instance()->GetLocalText("SHIPYARD"), 0.3f, y, 0.2f, 0.12f, "", &CMainShoppingPhase::OnShipyard);

	Button = CGUIHelper::CreateButton("Shopping_UpgradeShip_Button", CLocalizationSettings::Instance()->GetLocalText("UPGRADE SHIP"), 0.55f, y, 0.2f, 0.12f, "", &CMainShoppingPhase::OnUpgradeShip);
	Button->setEnabled(false);
	Button->setAlpha(0.5f);

	CGUIHelper::CreateButton("Shopping_StartRace_Button", CLocalizationSettings::Instance()->GetLocalText("START RACE"), 0.8f, y, 0.2f, 0.12f, "", &CMainShoppingPhase::OnStartRace);

	BuyShipText = CGUIHelper::CreateText("BuyShipText",CLocalizationSettings::Instance()->GetLocalText("BUY SHIP FIRST"),0.15f,0.11f,0.8f,0.1f, "Font_24", "", "", "");
	BuyShipText->setProperty("Visible","false");


	// =====================================================================
	// Race

	//Created = true;
	ShipBought = false;

	CMainGamePhase::Instance->PlaySong(SongGarage);


#ifdef _DEBUG
	// Automaticly buy tuber
	//FILE *f = fopen("ut_autobuy.txt", "rt");
	//if (f)
	//{
	//	char TuberName[100];
	//	fscanf(f, "%s", TuberName); 
	//	fclose(f);

	//	CSmartList <CTuberDescription> *AllDescs = &CTuberDescriptionManager::Instance()->Descriptions;
	//	for (AllDescs->First(); !AllDescs->End(); AllDescs->Next())
	//	{
	//		CTuberDescription* Desc = &AllDescs->GetItem();
	//		if (Desc->Name.EqualCaseInsensitive((String)TuberName))
	//		{
	//			CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = *Desc;
	//			CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = true;
	//			break;
	//		}
	//	}
	//}
#endif
}




bool CMainShoppingPhase::OnStartRace(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	{		
		CGame::Instance->SetPhase(CMainGamePhase::Instance);
		CMainGamePhase::Instance->SetPhase(CLoadMapPhase::Instance);

		CGameClient::Instance->SendTuberDesc(&CProfileManager::Instance()->GetCurrentProfile()->TuberDescription);
	}
	else
	{
		CMainShoppingPhase::Instance->ShipBought = true;
		CMainShoppingPhase::Instance->BuyShipTime = 0.0f;
		return true;
	}

	
	
	return true;
}




bool CMainShoppingPhase::OnShipyard(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CShoppingPhase::Instance->SetPhase(CShipyardPhase::Instance);
	return true;
}




bool CMainShoppingPhase::OnUpgradeShip(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	//if(CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	//	CShoppingPhase::Instance->SetPhase(CUpgradeShipPhase::Instance);
	//else
	//	ShipBought = true;
	
	return true;
}



bool CMainShoppingPhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);

	CGameClient::Instance->Disconnect();
	CGameClient::Instance->ShutDown();

	CGameServer::Instance->StopHosting();
	CGameServer::Instance->ShutDown();

	CGame::Instance->SetPhase(CMenuPhase::Instance);
	CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	return true;
}



void CMainShoppingPhase::End()
{
	CEGUI::WindowManager::getSingleton().destroyWindow("Shopping_StartRace_Button");
	CEGUI::WindowManager::getSingleton().destroyWindow("Shopping_UpgradeShip_Button");
	CEGUI::WindowManager::getSingleton().destroyWindow("Shopping_Shipyard_Button");
	CEGUI::WindowManager::getSingleton().destroyWindow("Shopping_Back_Button");

	if (BuyShipText)
	{
		BuyShipText->destroy();
		BuyShipText = NULL;
	}
}




void CMainShoppingPhase::Tick()
{
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
		return;
	}
	

	if (ShipBought && (BuyShipTime < 2.0f))
	{
		BuyShipTime += WeTimer::Instance()->GetDeltaTimeF();
		BuyShipText->setVisible(true);
	}
	else
	{
		ShipBought = false;
		BuyShipText->setVisible(false);
	}
}



void CMainShoppingPhase::Render()
{	
	WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), CShoppingPhase::Instance->ShopBackground);
	CShoppingPhase::Instance->RenderStdScreen();
}


