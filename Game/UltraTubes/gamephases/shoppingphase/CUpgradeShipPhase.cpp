/*
    CUpgradeShipPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CUpgradeShipPhase.h>
#include <CGUIHelper.h>
#include <WeCEGUI.h>
#include <CMenuPhase.h>
#include <CGame.h>
#include <CGameClient.h>
#include <CProfile.h>
#include <CProfileManager.h>
#include <CMainGamePhase.h>
#include <CLoadMapPhase.h>
#include <CShoppingPhase.h>
#include <CUpgradeItem.h>
#include <WeWorld.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CLoadMapPhase.h>
#include <CMainGamePhase.h>
#include <CGameClient.h>
#include <CLocalizationSettings.h>
#include <CShipyardPhase.h>



CUpgradeShipPhase *CUpgradeShipPhase::Instance = new CUpgradeShipPhase;


CUpgradeShipPhase::CUpgradeShipPhase()
{
}




void CUpgradeShipPhase::Start()
{
	 //BackEnters = false;
	 //BackLeaves = false;
	 //BuyEnters = false;
	 //BuyLeaves = false;
	 //StartEnters = false;
	 //StartLeaves = false;


	CEGUI::WindowManager &WinMgr = CEGUI::WindowManager::getSingleton();
	CProfile* CurProfile = CProfileManager::Instance()->GetCurrentProfile();

	CGUIHelper::CreateWnd("PlayerItemsInfo",CLocalizationSettings::Instance()->GetLocalText("PLAYER STUFFS"),0.0f,0.01f, 0.01f, 0.35f,0.5f,"RootWindow");
	CEGUI::WindowManager::getSingleton().getWindow("PlayerItemsInfoTitle")->setFont("Font_16");
	CGUIHelper::CreateText("MyShipProperties",CLocalizationSettings::Instance()->GetLocalText("MY SHIP PROPERTIES"),0.1f,0.25f,1.0f,0.1f,"","","","PlayerItemsInfo");

	CGUIHelper::CreateText("PSSpeed", CLocalizationSettings::Instance()->GetLocalText("SPEED"), 0.1f, 0.45f, 0.8f, 0.05f, "Font_8", "", "", "PlayerItemsInfo");
	CGUIHelper::CreateText("PSAcceleration", CLocalizationSettings::Instance()->GetLocalText("ACCELERATION"), 0.1f, 0.5f, 0.8f, 0.05f, "Font_8", "", "", "PlayerItemsInfo");
	CGUIHelper::CreateText("PSSteering", CLocalizationSettings::Instance()->GetLocalText("STEERING"), 0.1f, 0.55f, 0.8f, 0.05f, "Font_8", "", "", "PlayerItemsInfo");
	CGUIHelper::CreateText("PSHP", CLocalizationSettings::Instance()->GetLocalText("HP"), 0.1f, 0.6f, 0.8f, 0.05f, "Font_8", "", "", "PlayerItemsInfo");
	



	CEGUI::ProgressBar* PSSpeedProgress = CGUIHelper::CreateProgressBar("PSSpeedProgress", 0.5f, 0.45f, 0.4f, 0.02f, "PlayerItemsInfo");
	PSSpeedProgress->setProgress(CurProfile->TuberDescription.MaxSpeed / MAX_TUBER_SPEED);
	CEGUI::ProgressBar* PSAccelerationProgress = CGUIHelper::CreateProgressBar("PSAccelProgress", 0.5f, 0.5f, 0.4f, 0.02f, "PlayerItemsInfo");
	PSAccelerationProgress->setProgress(CurProfile->TuberDescription.MaxAcceleration / MAX_TUBER_ACCELERATION);
	CEGUI::ProgressBar* PSManouverabilityProgress = CGUIHelper::CreateProgressBar("PSSteeringProgress", 0.5f, 0.55f, 0.4f, 0.02f, "PlayerItemsInfo");
	PSManouverabilityProgress->setProgress(CurProfile->TuberDescription.Manouverability / MAX_TUBER_MANOUVERABILITY);
	CEGUI::ProgressBar* PSHPProgress = CGUIHelper::CreateProgressBar("PSHPProgress", 0.5f, 0.6f, 0.4f, 0.02f, "PlayerItemsInfo");
	PSHPProgress->setProgress(CurProfile->TuberDescription.HP / MAX_TUBER_HP);



	CEGUI::Window* PSShipName = CGUIHelper::CreateText("PSShipName", "", 0.1f, 0.35f, 0.9f, 0.1f, "", "", "", "PlayerItemsInfo");
	PSShipName->setText(CurProfile->TuberDescription.Name.GetString());
	CEGUI::Window* PSWeaponName = CGUIHelper::CreateText("PSWeaponName", CLocalizationSettings::Instance()->GetLocalText("WEAPON"), 0.1f, 0.70f, 0.8f, 0.08f, "Font_8", "", "", "PlayerItemsInfo");    
	String tmpSTR;
	tmpSTR = CLocalizationSettings::Instance()->GetLocalText("WEAPON") + ":\n ";
	tmpSTR = tmpSTR + CurProfile->TuberDescription.WeaponName;
	PSWeaponName->setText(tmpSTR.GetString());
	CEGUI::Window* PSMineName = CGUIHelper::CreateText("PSMineName", CLocalizationSettings::Instance()->GetLocalText("MINE"), 0.1f, 0.78f, 0.9f, 0.08f, "Font_8", "", "", "PlayerItemsInfo");    
	tmpSTR = CLocalizationSettings::Instance()->GetLocalText("MINE") + ":\n ";
	tmpSTR = tmpSTR + CurProfile->TuberDescription.MineName;
	PSMineName->setText(tmpSTR.GetString());
	Cash = CGUIHelper::CreateText("Cash", "", 0.1f, 0.9f, 0.8f, 0.1f, "", "", "", "PlayerItemsInfo");    
	char tmpC[100];
	sprintf(tmpC,"%s: %d",CLocalizationSettings::Instance()->GetLocalText("CASH").GetString(), CurProfile->Cash);
	Cash->setText(tmpC);


	BuyButton = CGUIHelper::CreateButton("UpgradeBuyButton",CLocalizationSettings::Instance()->GetLocalText("BUY"),0.36f,0.85f,0.3f,0.1f, "RootWindow", &CUpgradeShipPhase::OnBuy);
	BuyButton->setProperty("Visible","false");

	CGUIHelper::CreateButton("UpgradeBackButton",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.01f,0.85f,0.3f,0.1f, "RootWindow", &CUpgradeShipPhase::OnBack);

	CGUIHelper::CreateButton("UpgradeStartRaceButton",CLocalizationSettings::Instance()->GetLocalText("START RACE"),0.71f,0.85f,0.3f,0.1f, "RootWindow", &CUpgradeShipPhase::OnStartRace);
	


	CUpgradeItem* tmpUI;
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_SPEED);
	tmpUI->Cost = 200;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.SpeedUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_ACCELERATION);
	tmpUI->Cost = 250;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.AccelerationUpgradeLevel;	
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_TURBO);
	tmpUI->Cost = 1000;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.TurboUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_HP);
	tmpUI->Cost = 100;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.HPUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	int SubType = 0;
	if(CurProfile->TuberDescription.WeaponName == (String)CLocalizationSettings::Instance()->GetLocalText("STANDARD LASER")) SubType = 1;
	if(CurProfile->TuberDescription.WeaponName == (String)CLocalizationSettings::Instance()->GetLocalText("DOUBLE LASER")) SubType = 2;
	if(CurProfile->TuberDescription.WeaponName == (String)CLocalizationSettings::Instance()->GetLocalText("QUAD LASER")) SubType = 3;
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_WEAPON, SubType);
	tmpUI->Cost = 150;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.WeaponUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_MINE);
	tmpUI->Cost = 100;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.MineUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
	tmpUI = new CUpgradeItem(UPGRADE_ITEMTYPE_MANOUVERABILITY);
	tmpUI->Cost = 300;
	tmpUI->UpgradeLevel = CurProfile->TuberDescription.ManouverabilityUpgradeLevel;
	CUpgradeShipPhase::Instance->UpgradeItems.PushBack(tmpUI);
	
}




void CUpgradeShipPhase::End()
{
	CEGUI::WindowManager &WinMgr = CEGUI::WindowManager::getSingleton();

	WinMgr.destroyWindow("PlayerItemsInfo");
	WinMgr.destroyWindow("UpgradeBuyButton");
	WinMgr.destroyWindow("UpgradeBackButton");
	WinMgr.destroyWindow("UpgradeStartRaceButton");

	for(CUpgradeShipPhase::Instance->UpgradeItems.First(); !CUpgradeShipPhase::Instance->UpgradeItems.End(); CUpgradeShipPhase::Instance->UpgradeItems.Next())
		delete CUpgradeShipPhase::Instance->UpgradeItems.GetItem();

	CUpgradeShipPhase::Instance->UpgradeItems.Clear();
}




void CUpgradeShipPhase::Tick()
{
	//if(BackEnters) BackEnters = CGUIHelper::ButtonFadeIn("UpgradeBackButtonTitle");
	//if(BackLeaves) BackLeaves = CGUIHelper::ButtonFadeOut("UpgradeBackButtonTitle");
	//if(StartEnters) StartEnters = CGUIHelper::ButtonFadeIn("UpgradeStartRaceButtonTitle");
	//if(StartLeaves) StartLeaves = CGUIHelper::ButtonFadeOut("UpgradeStartRaceButtonTitle");
	//if(BuyEnters) BuyEnters = CGUIHelper::ButtonFadeIn("UpgradeBuyButtonTitle");
	//if(BuyLeaves) BuyLeaves = CGUIHelper::ButtonFadeOut("UpgradeBuyButtonTitle");

	if(CGame::Instance->InputMessage.LButton)
	{
		CEGUI::Point P;
		P = CEGUI::MouseCursor::getSingleton().getPosition();
		if(P.d_y < (WeRenderDevice::Instance()->GetResHeight() / 2 + WeRenderDevice::Instance()->GetResHeight() / 4))
			DoSelection((int)P.d_x,(int)P.d_y);
	}

	/*if(CGame::Instance->InputMessage.LDounleClick)
	{
		CEGUI::Point P;
		P = CEGUI::MouseCursor::getSingleton().getPosition();
		if(P.d_y < (WeRenderDevice::Instance()->GetResHeight() / 2 + WeRenderDevice::Instance()->GetResHeight() / 4))
			DoSelection(P.d_x,P.d_y);
		CEGUI::EventArgs e;
		OnBuy(e);
	}*/

	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}




void CUpgradeShipPhase::DoSelection(int x, int y)
{
	bool Res = false;

	for(CUpgradeShipPhase::Instance->UpgradeItems.First(); !CUpgradeShipPhase::Instance->UpgradeItems.End(); CUpgradeShipPhase::Instance->UpgradeItems.Next())
	{
		if(!Res)
		{
			Res = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Select(x,y);
			if(Res)
				if(CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost > CProfileManager::Instance()->GetCurrentProfile()->Cash ||
					CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel == CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->MaxUpgrades)
					Res = false;
		}
		else
			CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Select(x,y);
	}

	if(Res)
		BuyButton->setProperty("Visible","true");
	else
		BuyButton->setProperty("Visible","false");
}




void CUpgradeShipPhase::Render()
{
	CMenuPhase::Instance->SaveStates();
	CMenuPhase::Instance->MenuWorld.Render((WeCamera*)CMenuPhase::Instance->MenuCamera);
	
	for(CUpgradeShipPhase::Instance->UpgradeItems.First(); !CUpgradeShipPhase::Instance->UpgradeItems.End(); CUpgradeShipPhase::Instance->UpgradeItems.Next())
		CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Render();
	
	CMenuPhase::Instance->SaveStates(false);
	CEGUI::System::getSingleton().renderGUI();
}



bool CUpgradeShipPhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CShoppingPhase::Instance->SetPhase(CShipyardPhase::Instance);
	return true;
}


bool CUpgradeShipPhase::OnStartRace(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if(CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	{		
		CGame::Instance->SetPhase(CMainGamePhase::Instance);
		CMainGamePhase::Instance->SetPhase(CLoadMapPhase::Instance);

		CGameClient::Instance->SendTuberDesc(&CProfileManager::Instance()->GetCurrentProfile()->TuberDescription);
	}
	return true;
}



bool CUpgradeShipPhase::OnBuy(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	for(CUpgradeShipPhase::Instance->CUpgradeShipPhase::Instance->UpgradeItems.First(); !CUpgradeShipPhase::Instance->CUpgradeShipPhase::Instance->UpgradeItems.End(); CUpgradeShipPhase::Instance->CUpgradeShipPhase::Instance->UpgradeItems.Next())
	{
		if (CUpgradeShipPhase::Instance->CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Selected) break;
	}

	int tmpType = CUpgradeShipPhase::Instance->CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Type;

	CProfile* tmpProf = CProfileManager::Instance()->GetCurrentProfile();

	switch(tmpType)
	{
	case UPGRADE_ITEMTYPE_ACCELERATION:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.AccelerationUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_HP:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.HPUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_MANOUVERABILITY:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.ManouverabilityUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_MINE:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.MineUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_SPEED:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.SpeedUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_TURBO:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.TurboUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	case UPGRADE_ITEMTYPE_WEAPON:
		if(!CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpdateUpgradeLevel()) break;
		tmpProf->TuberDescription.WeaponUpgradeLevel = CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel;
		tmpProf->Cash -= CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost;
		break;
	}


	if(CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->Cost > tmpProf->Cash || CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->UpgradeLevel == CUpgradeShipPhase::Instance->UpgradeItems.GetItem()->MaxUpgrades) 
		CUpgradeShipPhase::Instance->BuyButton->setProperty("Visible","flase");

	char tmpC[100];
	sprintf(tmpC,"%s: %d",CLocalizationSettings::Instance()->GetLocalText("CASH"), CProfileManager::Instance()->GetCurrentProfile()->Cash);
	CUpgradeShipPhase::Instance->Cash->setText(tmpC);

	return true;
}




//bool CUpgradeShipPhase::OnBuyEnters(const CEGUI::EventArgs& e)
//{
//	BuyEnters = true;
//	BuyLeaves = false;
//	return true;
//}
//
//
//
//bool CUpgradeShipPhase::OnBuyLeaves(const CEGUI::EventArgs& e)
//{
//	BuyLeaves = true;
//	BuyEnters = false;
//	return true;
//}
//
//
//
//bool CUpgradeShipPhase::OnBackEnters(const CEGUI::EventArgs& e)
//{
//	BackEnters = true;
//	BackLeaves = false;
//	return true;
//}
//
//
//
//bool CUpgradeShipPhase::OnBackLeaves(const CEGUI::EventArgs& e)
//{
//	BackLeaves = true;
//	BackEnters = false;
//	return true;
//}
//
//
//
//bool CUpgradeShipPhase::OnStartEnters(const CEGUI::EventArgs& e)
//{
//	StartEnters = true;
//	StartLeaves = false;
//	return true;
//}
//
//
//
//bool CUpgradeShipPhase::OnStartLeaves(const CEGUI::EventArgs& e)
//{
//	StartLeaves = true;
//	StartEnters = false;
//	return true;
//}
