/*
    CShipyardPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CShipyardPhase.h>
#include <CGame.h>
#include <constants.h>
#include <CGUIHelper.h>
#include <WeCEGUI.h>
#include <AllPhases.h>
#include <WeActor.h>
#include <WeTimer.h>
#include <WeLight.h>
#include <WeLightState.h>
#include <WeRenderDevice.h>
#include <CTuberDescriptionManager.h>
#include <WeCanvas.h>
#include <CProfileManager.h>
#include <CMenuPhase.h>
#include <CProgressBar.h>
#include <COptionsSettings.h>
#include <WePostProcess.h>
#include <CLocalizationSettings.h>
#include <WeLoger.h>
#include <CConfig.h>
#include <math.h>
#include <WeStaticModel.h>
#include <WeMesh.h>
#include <CGameServer.h>
#include <CGameClient.h>
#include <WeFlareRenderer.h>



CShipyardPhase *CShipyardPhase::Instance = new CShipyardPhase;

const float NUM_PROGRESS_BOXES = 50;

// ship
const float SHIPYARD_Sx = 2.0;
const float SHIPYARD_Sy = -3.5;
const float SHIPYARD_Sz = 19.5;

// camera
const float SHIPYARD_Lx = -5.0f;
const float SHIPYARD_Ly = 0.0f;
const float SHIPYARD_Lz = 12.0f;
const float SHIPYARD_Dx = 1.3f;
const float SHIPYARD_Dy = -0.4f;
const float SHIPYARD_Dz = 1.0f;

// camera light
const float SHIPYARD_CLx = 0.0f;
const float SHIPYARD_CLy = 10.0f;
const float SHIPYARD_CLz = 20.0f;
const float SHIPYARD_CLrange = 25.0f;



void CShipyardPhase::Start()
{
	FirstTime = true;
	FirstTimeRender = false;
	Loaded = false;

	WePostProcess::Instance()->ClearEffects();
	WeRenderDevice::Instance()->SetMotionBlur(false);

	if(COptionsSettings::Instance->Glow)
	{
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorCombineGlow", 1, 1, true);		
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);	
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);		
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBrightPass", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true); 

		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);		
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true); 
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);		
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true);		

		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorUpFilter4", 4, 4, true);
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorCombine4", 4, 4, true);
	}

	WeFlareRenderer::Instance()->RemoveAllLights();

	//WePostProcess::Instance()->AddEffect("Effects.PostProcess.MotionBlur", 1, 1, true);


	ShipyardWorld = new CMap;
	TuberWorld = new CMap;


	//CameraAngle = 0; //D3DX_PI / 2.0f;
	//DesiredAngle = 0 ;//D3DX_PI / 2.0f;
	MovingToNextShip = false;
	MovingToPrevShip = false;	

	Camera = new WeCamera();
	CameraDistance = GetConfigFloat("shipyard.Camera.Distance", 10.0f);
	CameraY = GetConfigFloat("shipyard.Camera.Y", 1.0f);
	CameraSpeed = GetConfigFloat("shipyard.Camera.Speed", 1.0f) / 180.0f * D3DX_PI;
	CameraYSpeed = GetConfigFloat("shipyard.Camera.YSpeed", 1.0f) / 180.0f * D3DX_PI;
	CameraYDelta = GetConfigFloat("shipyard.Camera.YDelta", 1.0f);
	CameraLookAtY = GetConfigFloat("shipyard.Camera.LookAtY", 0.0f);
	CameraFOV = GetConfigFloat("shipyard.Camera.FOV", 1.0f);
	Camera->SetFov(CameraFOV);
	Camera->UpdateProjection = true;

	ProgressBarsSpeed = GetConfigFloat("shipyard.Misc.ProgressBarsSpeed", 1.0f);

	int aFogColor = GetConfigHexInt("shipyard.Misc.FogColor", 0xFFFFFFFF);
	FogColor = WeColor(aFogColor);
	FogStart = GetConfigFloat("shipyard.Misc.FogStart", 0.0f);
	FogEnd = GetConfigFloat("shipyard.Misc.FogEnd", 100.0f);

	//CameraLight = new WeLight;
	//CameraLight->Range = SHIPYARD_CLrange;
	//CameraLight->LightType = LIGHT_POINT;
	//CameraLight->IsDynamic = true;
	//CameraLight->CastShadowVolume = true;
	//CameraLight->AffectDynamicActors = true;	

	//WeLightState ls;	
	//ls.Duration = 1;
	//ls.Diffuse = WeColor(0.9f,0.9f,1.0f,1.0f);
	//CameraLight->AddKeyState(ls, 0);
	//CameraLight->Initialize();
	//ShipyardWorld->AddActor(CameraLight);

	LoadingProgressBar = new CProgressBar(ProgressBarVisualStyle1);
	LoadingProgressBar->PosY = 0.9f;
	LoadingProgressBar->Value = 0.0f;


	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	
	// ------------------- MainShoppingPhase -------------------
	CEGUI::Window *Button;
	Button = CGUIHelper::CreateButton("Shopping_UpgradeShip_Button", CLocalizationSettings::Instance()->GetLocalText("UPGRADE SHIP"), 0.55f, 0.83f, 0.2f, 0.12f, "", &CShipyardPhase::OnUpgradeShip);
	Button->setEnabled(false);
	Button->setAlpha(0.5f);

	CGUIHelper::CreateButton("Shopping_StartRace_Button", CLocalizationSettings::Instance()->GetLocalText("START RACE"), 0.8f, 0.83f, 0.2f, 0.12f, "", &CShipyardPhase::OnStartRace);

	BuyShipText = CGUIHelper::CreateText("BuyShipText",CLocalizationSettings::Instance()->GetLocalText("BUY SHIP FIRST"),0.15f,0.11f,0.8f,0.1f, "Font_24", "", "", "");
	BuyShipText->setProperty("Visible","false");

	ShipBought = false;

	CMainGamePhase::Instance->PlaySong(SongGarage);
	// ------------------- MainShoppingPhase -------------------



	// Napravi one dugmice i prozore i slicno
	CGUIHelper::CreateButton("Shipyard_Back_Button", CLocalizationSettings::Instance()->GetLocalText("BACK"), 0.75f, 0.9f, 0.2f, 0.1f, "", &CShipyardPhase::OnBack);

	CGUIHelper::CreateButton("Shipyard_NextShip_Button", CLocalizationSettings::Instance()->GetLocalText("NEXT"), 0.75f, 0.8f, 0.2f, 0.1f, "", &CShipyardPhase::OnNextShip);

	CGUIHelper::CreateButton("Shipyard_PrevShip_Button", CLocalizationSettings::Instance()->GetLocalText("PREV"), 0.05f, 0.8f, 0.2f, 0.1f, "", &CShipyardPhase::OnPrevShip);


	CGUIHelper::CreateWnd("Shipyard_DescWindow", CLocalizationSettings::Instance()->GetLocalText("BUY TUBER"),0.0f, 0.58f, 0.01f, 0.42f, 0.7f, "");

	CGUIHelper::CreateText("Shipyard_Speed", CLocalizationSettings::Instance()->GetLocalText("SPEED"), 0.1f, 0.65f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_DescWindow");
	CGUIHelper::CreateText("Shipyard_Acceleration", CLocalizationSettings::Instance()->GetLocalText("ACCELERATION"), 0.1f, 0.7f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_DescWindow");
	CGUIHelper::CreateText("Shipyard_Steering", CLocalizationSettings::Instance()->GetLocalText("STEERING"), 0.1f, 0.75f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_DescWindow");
	CGUIHelper::CreateText("Shipyard_HP", CLocalizationSettings::Instance()->GetLocalText("HP"), 0.1f, 0.8f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_DescWindow");

	ShipName = CGUIHelper::CreateText("Shipyard_ShipName", "", 0.1f, 0.17f, 0.8f, 0.1f, "", "", "", "Shipyard_DescWindow");
	DescText = CGUIHelper::CreateText("Shipyard_Description", "", 0.1f, 0.25f, 0.8f, 0.2f, "Font_8", "", "", "Shipyard_DescWindow");
	WeaponName = CGUIHelper::CreateText("Shipyard_WeaponName", "", 0.1f, 0.45f, 0.8f, 0.08f, "Font_8", "", "", "Shipyard_DescWindow");
	MineName = CGUIHelper::CreateText("Shipyard_MineName", "", 0.1f, 0.51f, 0.8f, 0.08f, "Font_8", "", "", "Shipyard_DescWindow");
	PriceText = CGUIHelper::CreateText("Shipyard_PriceName", "", 0.1f, 0.57f, 0.8f, 0.08f, "Font_8", "", "", "Shipyard_DescWindow");

	SpeedProgress = CGUIHelper::CreateProgressBar("Shipyard_SpeedProgress", 0.45f, 0.65f, 0.45f, 0.02f, "Shipyard_DescWindow");
	AccelerationProgress = CGUIHelper::CreateProgressBar("Shipyard_AccelProgress", 0.45f, 0.7f, 0.45f, 0.02f, "Shipyard_DescWindow");
	ManouverabilityProgress = CGUIHelper::CreateProgressBar("Shipyard_SteeringProgress", 0.45f, 0.75f, 0.45f, 0.02f, "Shipyard_DescWindow");
	HPProgress = CGUIHelper::CreateProgressBar("Shipyard_HPProgress", 0.45f, 0.8f, 0.45f, 0.02f, "Shipyard_DescWindow");

	BuyButton = CGUIHelper::CreateButton("Shipyard_Buy_Button",CLocalizationSettings::Instance()->GetLocalText("BUY"),0.1f,0.85f,0.6f,0.1f,"Shipyard_DescWindow", &CShipyardPhase::OnBuyShip);


	CGUIHelper::CreateWnd("Shipyard_PlayerItemsInfo",CLocalizationSettings::Instance()->GetLocalText("PLAYER STUFFS"),0.0f,0.01f, 0.01f, 0.35f,0.5f,"RootWindow");

	CGUIHelper::CreateText("Shipyard_PSSpeed", CLocalizationSettings::Instance()->GetLocalText("SPEED"), 0.1f, 0.45f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");
	CGUIHelper::CreateText("Shipyard_PSAcceleration", CLocalizationSettings::Instance()->GetLocalText("ACCELERATION"), 0.1f, 0.5f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");
	CGUIHelper::CreateText("Shipyard_PSSteering", CLocalizationSettings::Instance()->GetLocalText("STEERING"), 0.1f, 0.55f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");
	CGUIHelper::CreateText("Shipyard_PSHP", CLocalizationSettings::Instance()->GetLocalText("HP"), 0.1f, 0.6f, 0.8f, 0.05f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");

	PSShipName = CGUIHelper::CreateText("Shipyard_PSShipName", "", 0.1f, 0.35f, 0.8f, 0.1f, "", "", "", "Shipyard_PlayerItemsInfo");
	PSDescText = CGUIHelper::CreateText("Shipyard_PSDescription", "", 0.1f, 0.25f, 0.8f, 0.2f, "", "", "", "Shipyard_PlayerItemsInfo");
	PSWeaponName = CGUIHelper::CreateText("Shipyard_PSWeaponName", CLocalizationSettings::Instance()->GetLocalText("WEAPON"), 0.1f, 0.70f, 0.9f, 0.08f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");
	PSMineName = CGUIHelper::CreateText("Shipyard_PSMineName", CLocalizationSettings::Instance()->GetLocalText("MINE"), 0.1f, 0.78f, 0.9f, 0.08f, "Font_8", "", "", "Shipyard_PlayerItemsInfo");
	PSPriceText = CGUIHelper::CreateText("Shipyard_PSPriceName", CLocalizationSettings::Instance()->GetLocalText("CASH"), 0.1f, 0.9f, 0.9f, 0.1f, "", "", "", "Shipyard_PlayerItemsInfo");

	PSSpeedProgress = CGUIHelper::CreateProgressBar("Shipyard_PSSpeedProgress", 0.5f, 0.45f, 0.4f, 0.02f, "Shipyard_PlayerItemsInfo");
	PSAccelerationProgress = CGUIHelper::CreateProgressBar("Shipyard_PSAccelProgress", 0.5f, 0.5f, 0.4f, 0.02f, "Shipyard_PlayerItemsInfo");
	PSManouverabilityProgress = CGUIHelper::CreateProgressBar("Shipyard_PSSteeringProgress", 0.5f, 0.55f, 0.4f, 0.02f, "Shipyard_PlayerItemsInfo");
	PSHPProgress = CGUIHelper::CreateProgressBar("Shipyard_PSHPProgress", 0.5f, 0.6f, 0.4f, 0.02f, "Shipyard_PlayerItemsInfo");



	//CGUIHelper::CreateText("Shipyard_SJLoadingText",CLocalizationSettings::Instance()->GetLocalText("LOADING"),0.2f,0.82f,0.8f,0.1f, "Font_24", "", "", "");


	winMgr.getWindow("Shipyard_Back_Button")->setProperty("Visible","false");
	winMgr.getWindow("Shipyard_NextShip_Button")->setProperty("Visible","false");
	winMgr.getWindow("Shipyard_PrevShip_Button")->setProperty("Visible","false");
	winMgr.getWindow("Shipyard_DescWindow")->setProperty("Visible","false");
	winMgr.getWindow("Shipyard_PlayerItemsInfo")->setProperty("Visible","false");
	//winMgr.getWindow("Shipyard_SJLoadingText")->setProperty("Visible","true");

	winMgr.getWindow("Shopping_StartRace_Button")->setProperty("Visible","false");
	winMgr.getWindow("Shopping_UpgradeShip_Button")->setProperty("Visible","false");

	CEGUI::MouseCursor::getSingleton().hide();

	CTuberDescriptionManager::Instance()->Descriptions.First();


	UpdateCamera();
}



void CShipyardPhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	winMgr.destroyWindow("Shipyard_Back_Button");
	winMgr.destroyWindow("Shipyard_NextShip_Button");
	winMgr.destroyWindow("Shipyard_PrevShip_Button");
	winMgr.destroyWindow("Shipyard_DescWindow");
	winMgr.destroyWindow("Shipyard_PlayerItemsInfo");
	//winMgr.destroyWindow("Shipyard_SJLoadingText");

	winMgr.destroyWindow("Shopping_StartRace_Button");
	winMgr.destroyWindow("Shopping_UpgradeShip_Button");

	if (BuyShipText)
	{
		BuyShipText->destroy();
		BuyShipText = NULL;
	}

	SAFE_DELETE(LoadingProgressBar);

	ShipyardWorld->Unload();
	SAFE_DELETE(ShipyardWorld);	

	TuberWorld->Unload();
	SAFE_DELETE(TuberWorld);

	Tubers.Clear();
}



void CShipyardPhase::CTuber::Tick()
{
  if (Ship)
  {
	  //Ship->SetOrientation(WeTimer::Instance()->GetElapsedTimeF(), 0, 0);
  }

  if (TuberWorld)
	  TuberWorld->Tick();
}




CShipyardPhase::CShipyardPhase()
{	
	FirstTime = false;
	LoadingProgressBar = NULL;
}



bool CShipyardPhase::LoadTuberInTick()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	// Kreiramo sve one tuber-e i dodajemo ih u svet. Tubere kreiramo 
	// koristeci CTuberDescriptionManager kako bismo dolazili do osobina.
	CShipyardPhase::CTuber *Tuber;

	//static float Angle;
	static int ShipCount;


	
	if (FirstTime)
	{
		ShipCount = CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount();
		//AnglePerShip = 2.0f * D3DX_PI / ShipCount;
		//Angle = 0;
		//CameraAngle = Angle;
	}

	//WeVector tmpV;
	
    static int i = 0;
	//for (CTuberDescriptionManager::Instance()->Descriptions.First(); !CTuberDescriptionManager::Instance->Descriptions.End(); CTuberDescriptionManager::Instance->Descriptions.Next())
	//{
		Tuber = new CShipyardPhase::CTuber;
		Tuber->TuberDescription = CTuberDescriptionManager::Instance()->Descriptions.GetItem();		

		//CalculateRotatedVector(&tmpV, Angle, SHIPYARD_Sx, SHIPYARD_Sy, SHIPYARD_Sz);

		//Angle -= AnglePerShip;
		
		// proveri jel moze da se kupi ovaj brod		
		// ako trenutni profil ima u tournaments info-u oznacen predjen turnir
		// koji ship zahteva onda se moze kupiti
		Tuber->CanBeBought = false;

		if (Tuber->TuberDescription.TournamentRequired == (String)"")
			Tuber->CanBeBought = true;
		else
		{
			if (CProfileManager::Instance()->GetCurrentProfile())
			{					
				for (CProfileManager::Instance()->GetCurrentProfile()->Tournaments.First(); !CProfileManager::Instance()->GetCurrentProfile()->Tournaments.End(); CProfileManager::Instance()->GetCurrentProfile()->Tournaments.Next())
				{
					if ((CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem().Name == Tuber->TuberDescription.TournamentRequired) &&
						CProfileManager::Instance()->GetCurrentProfile()->Tournaments.GetItem().Opened == true)
					{
						Tuber->CanBeBought = true;
					}                        
				}		
			}
		}
		
		Tuber->Ship = new WeActor;
		Tuber->Ship->CastShadowVolume = true;
		Tuber->Ship->ModelName = Tuber->TuberDescription.Model;
		
		//Tuber->Ship->Location = tmpV;
		Tuber->Ship->Initialize();
		for (int Preset = 0; Preset < MAX_MATERIAL_PRESETS2; ++Preset)
			Tuber->Ship->Model->LoadMaterials(Preset);

		TuberWorld->AddActor(Tuber->Ship);

		if (Tuber->CanBeBought)
			Tuber->Ship->WantLight = true;
		else
			Tuber->Ship->WantLight = false;

		Tuber->Ship->Renderable = false;
		
/*
		// TODO: Kada bude bilo ground plate-a
		Tuber->GroundPlate = new WeActor;
		Tuber->GroundPlate->ModelName = "Models.Shopping.Podloga"; 
		Tuber->GroundPlate->Location = tmpV;
		Tuber->GroundPlate->Location.y -= 3;
		Tuber->GroundPlate->Initialize();
		ShipyardWorld->AddActor(Tuber->GroundPlate);

*/

		Tuber->TuberWorld = NULL;

		WeFileStream FS;
		char TuberMapName[1000];
		sprintf_s(TuberMapName, "%s.utt", Tuber->TuberDescription.Name);
		if (FS.Exists(TuberMapName))
		{
			Tuber->TuberWorld = new CMap();
			while (!Tuber->TuberWorld->IsLoaded())
				Tuber->TuberWorld->LoadInTick(TuberMapName, false);
		}

		Tubers.PushBack(Tuber);        
		i++;
	//}
	FirstTime = false;

	CTuberDescriptionManager::Instance()->Descriptions.Next();
	if(!CTuberDescriptionManager::Instance()->Descriptions.End())
		return false;
	else
	{
		winMgr.getWindow("Shipyard_Back_Button")->setProperty("Visible","true");
		winMgr.getWindow("Shipyard_NextShip_Button")->setProperty("Visible","true");
		winMgr.getWindow("Shipyard_PrevShip_Button")->setProperty("Visible","true");
		winMgr.getWindow("Shipyard_DescWindow")->setProperty("Visible","true");
		winMgr.getWindow("Shipyard_PlayerItemsInfo")->setProperty("Visible","true");
		//winMgr.getWindow("Shipyard_SJLoadingText")->setProperty("Visible","false");

		winMgr.getWindow("Shopping_StartRace_Button")->setProperty("Visible","true");
		winMgr.getWindow("Shopping_UpgradeShip_Button")->setProperty("Visible","true");

		CurrentShipIndex = 0;
		CurrentTuber = Tubers[CurrentShipIndex];
		CurrentTuber->Ship->Renderable = true;

		DisplayDescriptions();
		SetProgress();		
		CheckBuyButton();
		
		return true;
	}
}



void CShipyardPhase::SetProgress()
{
	CurrentSpeedProgress = MaxSpeedProgress;
	CurrentAccelrationProgress = MaxAccelrationProgress;
	CurrentManouverabilityProgress = MaxManouverabilityProgress;
	CurrentHPProgress = MaxHPProgress;
}



void CShipyardPhase::DisplayDescriptions()
{
	if(Tubers.GetItemsCount() == 0) return;


	ShipName->setText(CurrentTuber->TuberDescription.Name.GetString());
	DescText->setText(CurrentTuber->TuberDescription.Description.GetString());

	char tmp[100];

	sprintf(tmp, "%s: %s",CLocalizationSettings::Instance()->GetLocalText("WEAPON").GetString(), CurrentTuber->TuberDescription.WeaponName.GetString());
	WeaponName->setText(tmp);

	sprintf(tmp, "%s: %s", CLocalizationSettings::Instance()->GetLocalText("MINE").GetString(), CurrentTuber->TuberDescription.MineName.GetString());
	MineName->setText(tmp);

	sprintf(tmp, "%s: %d",CLocalizationSettings::Instance()->GetLocalText("COST").GetString(), CurrentTuber->TuberDescription.Price);
	PriceText->setText(tmp);


	MaxSpeedProgress = CurrentTuber->TuberDescription.MaxSpeed / MAX_TUBER_SPEED;
	MaxAccelrationProgress = CurrentTuber->TuberDescription.MaxAcceleration / MAX_TUBER_ACCELERATION;
	MaxManouverabilityProgress = CurrentTuber->TuberDescription.Manouverability / MAX_TUBER_MANOUVERABILITY;
	MaxHPProgress = CurrentTuber->TuberDescription.HP / MAX_TUBER_HP;


	if (!CurrentTuber->CanBeBought) 
	{
		ShipName->setText("Unavailable");		
		DescText->setText("");
		WeaponName->setText("");
		MineName->setText("");
		PriceText->setText("");
		MaxSpeedProgress = 0.0f;
		MaxAccelrationProgress = 0.0f;
		MaxManouverabilityProgress = 0.0f;
		MaxHPProgress = 0.0f;
	}

}


void CShipyardPhase::CheckBuyButton()
{
	String TName = CurrentTuber->TuberDescription.Name;
	String CPTName = CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Name;
	int TCash = CurrentTuber->TuberDescription.Price;
	int CPTCash = CProfileManager::Instance()->GetCurrentProfile()->Cash;
	bool CBBought = CurrentTuber->CanBeBought;

	if((TName == CPTName && CProfileManager::Instance()->GetCurrentProfile()->TuberBougth) || TCash > CPTCash || !CBBought)
		BuyButton->setProperty("Visible","false");
	else
		BuyButton->setProperty("Visible","true");
}




void CShipyardPhase::Tick()
{
	static int Counter = 1;
	COptionsSettings::Instance->ControllerSettings.MakeInputMessage(&CGame::Instance->InputMessage);

	if (!Loaded)
	{
		if (FirstTimeRender)
		{
			if (!ShipyardWorld->IsLoaded())
			{
				ShipyardWorld->LoadInTick("garage.utg");
				ShipyardWorld->MapHeader.SetupRenderDevice();
				LoadingProgressBar->Value = ShipyardWorld->PercentLoaded * 0.7f;
			}
			else if (!TuberWorld->IsLoaded())
			{
				TuberWorld->LoadInTick("garage-tuber.utg");
				LoadingProgressBar->Value = 70.0f + TuberWorld->PercentLoaded * 0.1f;
			}
			else
			{
				Loaded = LoadTuberInTick();
				Counter++;
				LoadingProgressBar->Value = 80.0f + Counter * 20.0f / CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount();

				if (Loaded)
					CEGUI::MouseCursor::getSingleton().show();
			}
		}

		//sprintf(tmpText,"LOADING TUBER %d / %d",Counter,CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount());
		//sprintf(tmpText,"LOADING TUBERS %d %%",(Counter*100/CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount()));
		//static_cast<CEGUI::StaticText*>(winMgr.getWindow("SJLoadingText"))->setText(tmpText);

		return;
	}

	Counter = 1;

	CEGUI::EventArgs e;

	static bool RotatingLeft = false;
	if (CGame::Instance->InputMessage.RotateLeft)
	{
		if (!RotatingLeft)
		{
			OnPrevShip(e);
			RotatingLeft = true;
		}
	}
	else
		RotatingLeft = false;

	static bool RotatingRight = false;
	if (CGame::Instance->InputMessage.RotateRight)
	{
		if (!RotatingRight)
		{
			OnNextShip(e);
			RotatingRight = true;
		}
	}
	else
		RotatingRight = false;


	for (Tubers.First(); !Tubers.End(); Tubers.Next())
		Tubers.GetItem()->Tick();

/*
	const float DEFAULT_CAM_SPEED = (D3DX_PI / 196.0f);
	static float Speed = DEFAULT_CAM_SPEED;
	if (MovingToNextShip || MovingToPrevShip)
	{
		//Speed = DEFAULT_CAM_SPEED - ((abs(PrevAngle - CameraAngle) / AnglePerShip) * DEFAULT_CAM_SPEED);
		if (Speed < (DEFAULT_CAM_SPEED / 2)) Speed = (DEFAULT_CAM_SPEED / 2);
	}
*/

	//float CameraSpeed = AnglePerShip * WeTimer::Instance()->GetDeltaTimeF() * 2.0f;

	//float k;
	//if (MovingToNextShip)
	//{
		//if (CameraAngle >= DesiredAngle)
		//{
		//	//Speed = DEFAULT_CAM_SPEED;
		//	MovingToNextShip = false;			
		//	CameraAngle = DesiredAngle;
		//	CurrentHPProgress = MaxHPProgress;
		//	CurrentSpeedProgress = MaxSpeedProgress;
		//	CurrentAccelrationProgress = MaxAccelrationProgress;
		//	CurrentManouverabilityProgress = MaxManouverabilityProgress;

		//}
		//else 
		//{
		//	k = abs(CameraAngle - PrevAngle) / abs(PrevAngle - DesiredAngle);
		//	CurrentSpeedProgress = PrevSpeedProgress + k*(MaxSpeedProgress - PrevSpeedProgress);
		//	CurrentAccelrationProgress = PrevAccelrationProgress + k*(MaxAccelrationProgress - PrevAccelrationProgress);
		//	CurrentManouverabilityProgress = PrevManouverabilityProgress + k*(MaxManouverabilityProgress - PrevManouverabilityProgress);
		//	CurrentHPProgress = PrevHPProgress + k*(MaxHPProgress - PrevHPProgress);
		//	CameraAngle += CameraSpeed;
		//	if (CameraAngle > DesiredAngle)
		//		CameraAngle = DesiredAngle;
		//}

		//UpdateCamera(CameraAngle);
	//}

	//if (MovingToPrevShip)
	//{
		//if (CameraAngle <= DesiredAngle)
		//{
		//	//Speed = DEFAULT_CAM_SPEED;
		//	MovingToPrevShip = false;
		//	CameraAngle = DesiredAngle;
		//	CurrentHPProgress = MaxHPProgress;
		//	CurrentSpeedProgress = MaxSpeedProgress;
		//	CurrentAccelrationProgress = MaxAccelrationProgress;
		//	CurrentManouverabilityProgress = MaxManouverabilityProgress;

		//}
		//else 
		//{
		//	k = abs(CameraAngle - PrevAngle) / abs(PrevAngle - DesiredAngle);
		//	CurrentSpeedProgress = PrevSpeedProgress + k*(MaxSpeedProgress - PrevSpeedProgress);
		//	CurrentAccelrationProgress = PrevAccelrationProgress + k*(MaxAccelrationProgress - PrevAccelrationProgress);
		//	CurrentManouverabilityProgress = PrevManouverabilityProgress + k*(MaxManouverabilityProgress - PrevManouverabilityProgress);
		//	CurrentHPProgress = PrevHPProgress + k*(MaxHPProgress - PrevHPProgress);
		//	CameraAngle -= CameraSpeed;
		//	if (CameraAngle < DesiredAngle)
		//		CameraAngle = DesiredAngle;
		//}

		//UpdateCamera(CameraAngle);
	//}

	if (MovingToPrevShip || MovingToNextShip)
	{
		CurrentProgress += WeTimer::Instance()->GetDeltaTimeF() * ProgressBarsSpeed;
		if (CurrentProgress >= 1.0f)
		{
			CurrentProgress = 1.0f;
			MovingToPrevShip = false;
			MovingToNextShip = false;
		}

		CurrentSpeedProgress = PrevSpeedProgress + CurrentProgress * (MaxSpeedProgress - PrevSpeedProgress);
		CurrentAccelrationProgress = PrevAccelrationProgress + CurrentProgress * (MaxAccelrationProgress - PrevAccelrationProgress);
		CurrentManouverabilityProgress = PrevManouverabilityProgress + CurrentProgress * (MaxManouverabilityProgress - PrevManouverabilityProgress);
		CurrentHPProgress = PrevHPProgress + CurrentProgress * (MaxHPProgress - PrevHPProgress);
	}

	UpdateCamera();

	SpeedProgress->setProgress(CurrentSpeedProgress);
	AccelerationProgress->setProgress(CurrentAccelrationProgress);
	ManouverabilityProgress->setProgress(CurrentManouverabilityProgress);
	HPProgress->setProgress(CurrentHPProgress);


	WeVector AX, AY, AZ;
	CurrentTuber->Ship->GetLocalAxis(AX, AY, AZ);


	CurrentTuber->Ship->Roll = 
		GetConfigFloat("shipyard.Tuber.RollDelta", 90.0f) / 180.0f * D3DX_PI *
		sin(GetConfigFloat("shipyard.Tuber.RollFixed", 0.0f) +
			GetConfigFloat("shipyard.Tuber.RollSpeed", 7.0f) * WeTimer::Instance()->GetElapsedTimeF());

	CurrentTuber->Ship->Yaw = D3DX_PI +
		GetConfigFloat("shipyard.Tuber.YawDelta", 90.0f) / 180.0f * D3DX_PI *
		sin(GetConfigFloat("shipyard.Tuber.YawFixed", 0.0f) +
			GetConfigFloat("shipyard.Tuber.YawSpeed", 7.0f) * WeTimer::Instance()->GetElapsedTimeF());

	CurrentTuber->Ship->Pitch = 
		GetConfigFloat("shipyard.Tuber.PitchDelta", 90.0f) / 180.0f * D3DX_PI *
		sin(GetConfigFloat("shipyard.Tuber.PitchFixed", 0.0f) +
			GetConfigFloat("shipyard.Tuber.PitchSpeed", 7.0f) * WeTimer::Instance()->GetElapsedTimeF());

	CurrentTuber->Ship->Location = WeVector(0.0f, GetConfigFloat("shipyard.Tuber.YFixed", 0.5f), 0.0f) +
		GetConfigFloat("shipyard.Tuber.YDelta", 0.1f) *
		sin(GetConfigFloat("shipyard.Tuber.YSpeed", 3.0f) * WeTimer::Instance()->GetElapsedTimeF()) *
			WeVector(0.0f, 1.0f, 0.0f);


	//if ((CPlayGamePhase::Instance->Status != PLAYGAME_RACE) || (CurrentTuber->Ship->AngularVelocity == WeVector(0.0f, 0.0f, 0.0f)))
	//{
		//D3DXVec3Normalize(&AZ, &AZ);
		//D3DXVec3Normalize(&AX, &AX);
		//CurrentTuber->Ship->AngularVelocity += (0.04f * cos(3.0f * WeTimer::Instance()->GetElapsedTimeF())) * AZ;
		//CurrentTuber->Ship->AngularVelocity += (0.02f * sin(3.0f * WeTimer::Instance()->GetElapsedTimeF())) * AX;
	//}

	
	char tmp[100];

	sprintf(tmp, "%s: %s",CLocalizationSettings::Instance()->GetLocalText("WEAPON").GetString(), CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.WeaponName.GetString());
	PSWeaponName->setText(tmp);

	sprintf(tmp, "%s: %s", CLocalizationSettings::Instance()->GetLocalText("MINE").GetString(), CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MineName.GetString());
	PSMineName->setText(tmp);

	sprintf(tmp, "%s: %d",CLocalizationSettings::Instance()->GetLocalText("COST").GetString(), CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Price);
	PSPriceText->setText(tmp);

	PSShipName->setText(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Name.GetString());
	PSDescText->setText(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Description.GetString());
	PSAccelerationProgress->setProgress(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MaxAcceleration / MAX_TUBER_ACCELERATION);
	PSSpeedProgress->setProgress(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.MaxSpeed / MAX_TUBER_SPEED);
	PSHPProgress->setProgress(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.HP / MAX_TUBER_HP);
	PSManouverabilityProgress->setProgress(CProfileManager::Instance()->GetCurrentProfile()->TuberDescription.Manouverability / MAX_TUBER_MANOUVERABILITY);


	Camera->Update();
	ShipyardWorld->Tick();
	TuberWorld->Tick();


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


	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}



bool CShipyardPhase::OnBuyShip(const CEGUI::EventArgs& e)
{
	CTuber* Tuber = CShipyardPhase::Instance->CurrentTuber;
	if (Tuber->TuberDescription.Price <= CProfileManager::Instance()->GetCurrentProfile()->Cash)
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
		CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = Tuber->TuberDescription;
		CProfileManager::Instance()->GetCurrentProfile()->Cash -= Tuber->TuberDescription.Price;
		CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = true;
	}
	else
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	}

	CShipyardPhase::Instance->CheckBuyButton();
	
	return true;
}


bool CShipyardPhase::OnPrevShip(const CEGUI::EventArgs& e)
{
	//if (CShipyardPhase::Instance->MovingToNextShip || CShipyardPhase::Instance->MovingToPrevShip) return true;

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);

	CShipyardPhase::Instance->MovingToNextShip = true;
	CShipyardPhase::Instance->MovingToPrevShip = false;
	CShipyardPhase::Instance->CurrentProgress = 0.0f;

	CShipyardPhase::Instance->CurrentTuber->Ship->Renderable = false;

	CShipyardPhase::Instance->CurrentShipIndex--;
	if (CShipyardPhase::Instance->CurrentShipIndex < 0)
	{
		CShipyardPhase::Instance->CurrentShipIndex = CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount()-1;
	}

	CShipyardPhase::Instance->CurrentTuber = CShipyardPhase::Instance->Tubers[CShipyardPhase::Instance->CurrentShipIndex];
	CShipyardPhase::Instance->CurrentTuber->Ship->Renderable = true;

	CShipyardPhase::Instance->PrevSpeedProgress = CShipyardPhase::Instance->CurrentSpeedProgress;
	CShipyardPhase::Instance->PrevAccelrationProgress = CShipyardPhase::Instance->CurrentAccelrationProgress;
	CShipyardPhase::Instance->PrevManouverabilityProgress = CShipyardPhase::Instance->CurrentManouverabilityProgress;
	CShipyardPhase::Instance->PrevHPProgress = CShipyardPhase::Instance->CurrentHPProgress;

	CShipyardPhase::Instance->DisplayDescriptions();

	//CShipyardPhase::Instance->PrevAngle = CShipyardPhase::Instance->CameraAngle;
	//CShipyardPhase::Instance->DesiredAngle = CShipyardPhase::Instance->CameraAngle + CShipyardPhase::Instance->AnglePerShip;

	CShipyardPhase::Instance->CheckBuyButton();

	return true;
}



bool CShipyardPhase::OnNextShip(const CEGUI::EventArgs& e)
{
	//if (CShipyardPhase::Instance->MovingToPrevShip || CShipyardPhase::Instance->MovingToNextShip) return true;

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);

	CShipyardPhase::Instance->MovingToNextShip = false;
	CShipyardPhase::Instance->MovingToPrevShip = true;
	CShipyardPhase::Instance->CurrentProgress = 0.0f;

	CShipyardPhase::Instance->CurrentTuber->Ship->Renderable = false;

	CShipyardPhase::Instance->CurrentShipIndex++;
	if (CShipyardPhase::Instance->CurrentShipIndex > CTuberDescriptionManager::Instance()->Descriptions.GetItemsCount()-1)
	{
		CShipyardPhase::Instance->CurrentShipIndex = 0;
	}

	CShipyardPhase::Instance->CurrentTuber = CShipyardPhase::Instance->Tubers[CShipyardPhase::Instance->CurrentShipIndex];
	CShipyardPhase::Instance->CurrentTuber->Ship->Renderable = true;

	CShipyardPhase::Instance->PrevSpeedProgress = CShipyardPhase::Instance->CurrentSpeedProgress;
	CShipyardPhase::Instance->PrevAccelrationProgress = CShipyardPhase::Instance->CurrentAccelrationProgress;
	CShipyardPhase::Instance->PrevManouverabilityProgress = CShipyardPhase::Instance->CurrentManouverabilityProgress;
	CShipyardPhase::Instance->PrevHPProgress = CShipyardPhase::Instance->CurrentHPProgress;

	CShipyardPhase::Instance->DisplayDescriptions();

	//CShipyardPhase::Instance->PrevAngle = CShipyardPhase::Instance->CameraAngle;
	//CShipyardPhase::Instance->DesiredAngle = CShipyardPhase::Instance->CameraAngle - CShipyardPhase::Instance->AnglePerShip;

	CShipyardPhase::Instance->CheckBuyButton();

	return true;
}



void CShipyardPhase::Render()
{	
	if (!Loaded)
	{
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), CShoppingPhase::Instance->ShopBackground);
		CEGUI::System::getSingleton().renderGUI();
		LoadingProgressBar->Render();
		FirstTimeRender = true;
		return;
	}

	CMenuPhase::Instance->SaveStates();
	
	//WeRenderDevice::Instance()->SetFog(true, FogColor, 0.0f);
	//WeRenderDevice::Instance()->SetFogStart(FogStart);
	//WeRenderDevice::Instance()->SetFogEnd(FogEnd);
	WePostProcess::Instance()->BeginScreenCapture();

	ShipyardWorld->Render(Camera);
	TuberWorld->Render(Camera);

	CTuber *OtherTuber = CShipyardPhase::Instance->CurrentTuber;

	if (OtherTuber->TuberWorld)
	{
		for (WeActor *Iterator = OtherTuber->TuberWorld->Actors->NextActor;
			Iterator != OtherTuber->TuberWorld->Actors;
			Iterator = Iterator->NextActor)
		{
			WeMatrix M;
			Iterator->PrevTM = Iterator->TM;
			D3DXMatrixMultiply(&M, &Iterator->TM, &OtherTuber->Ship->TM);
			Iterator->SetTransformation(M);
		}

		OtherTuber->TuberWorld->Render(Camera, false);

		for (WeActor *Iterator = OtherTuber->TuberWorld->Actors->NextActor;
			Iterator != OtherTuber->TuberWorld->Actors;
			Iterator = Iterator->NextActor)
		{
			Iterator->SetTransformation(Iterator->PrevTM);
		}
	}

	WePostProcess::Instance()->EndScreenCapture();
	WePostProcess::Instance()->PostRender();
	//WeRenderDevice::Instance()->SetFog(false, FogColor, 0.0f);


	CMenuPhase::Instance->SaveStates(false);

	CEGUI::System::getSingleton().renderGUI();            
}



void CShipyardPhase::UpdateCamera()
{
	WeVector LookAtVector = WeVector(0.0f, CameraLookAtY, 0.0f);

	CalculateRotatedVector(&Camera->Location, WeTimer::Instance()->GetElapsedTimeF() * CameraSpeed, 0.0f, CameraY, CameraDistance);
	Camera->Location.y += CameraYDelta * sin(WeTimer::Instance()->GetElapsedTimeF() * CameraYSpeed);
	D3DXVec3Subtract(&Camera->Direction, &LookAtVector, &Camera->Location);
	D3DXVec3Normalize(&Camera->Direction, &Camera->Direction);

	//CalculateRotatedVector(&Camera->Location, angle, SHIPYARD_Lx, SHIPYARD_Ly, SHIPYARD_Lz);
	//CalculateRotatedVector(&Camera->Direction, angle, SHIPYARD_Dx, SHIPYARD_Dy, SHIPYARD_Dz);
	//D3DXVec3Normalize(&Camera->Direction, &Camera->Direction);
	//CalculateRotatedVector(&CameraLight->Location, angle, SHIPYARD_CLx, SHIPYARD_CLy, SHIPYARD_CLz);
}


void CShipyardPhase::CalculateRotatedVector(WeVector *V, float Angle, float Sx, float Sy, float Sz)
{
	WeMatrix M;
	D3DXMatrixRotationY(&M, Angle);

	V->x = Sx * M._11 + Sy * M._21 + Sz * M._31;
	V->y = Sx * M._12 + Sy * M._22 + Sz * M._32;
	V->z = Sx * M._13 + Sy * M._23 + Sz * M._33;
}




bool CShipyardPhase::OnStartRace(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	if (CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	{		
		CGame::Instance->SetPhase(CMainGamePhase::Instance);
		CMainGamePhase::Instance->SetPhase(CLoadMapPhase::Instance);

		CGameClient::Instance->SendTuberDesc(&CProfileManager::Instance()->GetCurrentProfile()->TuberDescription);
	}
	else
	{
		CShipyardPhase::Instance->ShipBought = true;
		CShipyardPhase::Instance->BuyShipTime = 0.0f;
		return true;
	}

	return true;
}



bool CShipyardPhase::OnUpgradeShip(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	//if(CProfileManager::Instance()->GetCurrentProfile()->TuberBougth)
	//	CShoppingPhase::Instance->SetPhase(CUpgradeShipPhase::Instance);
	//else
	//	ShipBought = true;

	return true;
}



bool CShipyardPhase::OnBack(const CEGUI::EventArgs& e)
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
