// zamenjeno sa CMultiRacePhase.cpp

/*
    File    : CChooseMapPhase.cpp
	Autor   : Vuk Vranjkovic
	Date    : 27.3.2006.
	Version : 1.0
*/

#include <CChooseMapPhase.h>
#include <CMenuPhase.h>
#include <CGame.h>
#include <CGameSettings.h>
#include <CGUIHelper.h>
#include <CMapProvider.h>
#include <CMap.h>
#include <CHostGamePhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <CLocalizationSettings.h>



CChooseMapPhase *CChooseMapPhase::Instance = new CChooseMapPhase;

CChooseMapPhase::CChooseMapPhase()
{
	MapDescImage = NULL;
}



void CChooseMapPhase::Start()
{	
	CGUIHelper::CreateWnd("ChooseMap_Options_Window",CLocalizationSettings::Instance()->GetLocalText("GAME OPTIONS"),
		0.0f, 0.0f, 0.2f, 0.5f, 0.5f,"RootWindow");

	CGUIHelper::CreateWnd("ChooseMap_Description_Window",CLocalizationSettings::Instance()->GetLocalText("MAP DESCRIPTION"),
		0.0f, 0.50f, 0.2f, 0.5f, 0.5f,"RootWindow");

	CreateOptionsComboboxes();

	CGUIHelper::CreateButton("CHOOSEMAP_BACK_BUTTON",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.02f,0.85f,0.3f,0.1f, "RootWindow", &CChooseMapPhase::OnBack);

	CGUIHelper::CreateButton("CHOOSEMAP_CREATE_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CREATE"),0.7f,0.85f,0.3f,0.1f, "RootWindow", &CChooseMapPhase::OnCreate);

	CMap TmpMap;
	TmpMap.LoadMapHeader(CGameSettings::ClientInstance->MapName);
	CGUIHelper::CreateText("ChooseMap_Description",TmpMap.MapHeader.MapDescription,0.5f,0.27f,0.5f,1.0f,
						   "", "", "","ChooseMap_Description_Window");
	CGUIHelper::CreateText("ChooseMap_MapName",CGameSettings::ClientInstance->MapName.Extract('.',0),0.1f,0.9f,0.5f,0.1f,
						   "", "", "","ChooseMap_Description_Window");

	CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_SINGLERACE;

	CGameSettings::ClientInstance->LapCount = 10;
	CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_LapCount_Combobox")->setText("10");

	CGameSettings::ClientInstance->SetCrates(false);
	CGameSettings::ClientInstance->SetWeapons(true);
	CGameSettings::ClientInstance->StartingMoney = CGameSettings::ClientInstance->CashCategories[0];
	CGameSettings::ClientInstance->Funny = false;

	MapDescImage = WeTextureManager::Instance()->Load(TmpMap.MapHeader.MapDescriptionImage,true);
}



void CChooseMapPhase::End()
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("ChooseMap_Options_Window")->destroy();
	Mng.getWindow("ChooseMap_Description_Window")->destroy();
	Mng.getWindow("CHOOSEMAP_BACK_BUTTON")->destroy();
	Mng.getWindow("CHOOSEMAP_CREATE_BUTTON")->destroy();
}



void CChooseMapPhase::Tick()
{
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}



void CChooseMapPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();

	if(MapDescImage)
	{
		float WOffset = WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
		float HOffset = WeRenderDevice::Instance()->GetResHeight() / 768.0f;
		WeCanvas::Instance()->DrawRectangle((int)(550 * WOffset), (int)(250 * HOffset), (int)(200 * WOffset), (int)(200 * HOffset),D3DCOLOR_XRGB(255,255,255),MapDescImage);
		CEGUI::MouseCursor::getSingleton().draw();
	}
}



void CChooseMapPhase::CreateOptionsComboboxes()
{
	Strings Params;
	CEGUI::Combobox *pCombobox;
	CMapProvider::Instance->LoadAllMapNames("*.utm");

	CGUIHelper::CreateText("ChooseMap_Map_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("MAP"),0.06f, 0.3f, 0.22f, 0.1f,"", "", "", "ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_Map_Combobox",0.28f,0.3f,0.6f,0.4f,&CMapProvider::Instance->MapNames, "ChooseMap_Options_Window", &OnMapNameCombobox);
	CMapProvider::Instance->MapNames.First();
	CGameSettings::ClientInstance->MapName = CMapProvider::Instance->MapNames.GetItem() + ".utm";

	Params.Clear();
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE")); 
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("LASTONE"));
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"));

	CGUIHelper::CreateText("ChooseMap_MapType_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("MAP TYPE"),0.06f, 0.4f, 0.21f, 0.1f,"", "", "","ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_MapType_Combobox",0.28f,0.4f,0.6f,0.45f,&Params,"ChooseMap_Options_Window", &OnMapTypeCombobox);

	Params.Clear();
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("ON"));
	Params.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));

	CGUIHelper::CreateText("ChooseMap_Creates_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("CRATES"),0.06f, 0.7f, 0.21f, 0.1f,"", "", "","ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_Creates_Combobox",0.3f,0.7f,0.18f,0.2f,&Params,"ChooseMap_Options_Window", &OnCreatesCombobox);
	pCombobox->setEnabled(false);
	pCombobox->setText("");
	pCombobox->setProperty("Alpha","0.5");

	CGUIHelper::CreateText("ChooseMap_Weapons_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("WEAPONS"),0.06f,0.6f,0.21f,0.1f,"", "", "","ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_Weapons_Combobox",0.3f,0.6f,0.18f,0.3f,&Params, "ChooseMap_Options_Window", &OnWeaponCombobox);

	Params.Clear();
	for (int i = 0; i < MAX_CASH_CATEGORIES; i++)
	{
		char tmp[10];
		itoa(CGameSettings::CashCategories[i], tmp, 10);
		Params.PushBack(tmp);
	}
	
	CGUIHelper::CreateText("ChooseMap_Money_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("MONEY"),0.48f, 0.7f, 0.21f, 0.1f, "", "", "","ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_Money_Combobox",0.72f,0.7f,0.2f,0.2f,&Params,"ChooseMap_Options_Window", &OnMoneyCombobox);

	Params.Clear();
	for (int i = 0; i < MAX_LAP_PRESETS; i++)
	{
		char tmp[10];
		itoa(CGameSettings::LapCountPresets[i], tmp, 10);
		Params.PushBack(tmp);
	}
	
	
	CGUIHelper::CreateText("ChooseMap_LapCount_Combobox_title",CLocalizationSettings::Instance()->GetLocalText("LAP COUNT"),0.48f,0.6f,0.2f,0.1f,"", "", "","ChooseMap_Options_Window");
	pCombobox = CGUIHelper::CreateCombobox("ChooseMap_LapCount_Combobox",0.72f,0.6f,0.2f,0.2f,&Params,"ChooseMap_Options_Window", &OnLapCountCombobox);
}



bool CChooseMapPhase::OnCreatesCombobox(const CEGUI::EventArgs &)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_Creates_Combobox")->getText();
	CGameSettings::ClientInstance->SetCrates( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	return true;
}



bool CChooseMapPhase::OnWeaponCombobox(const CEGUI::EventArgs &)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_Weapons_Combobox")->getText();
	CGameSettings::ClientInstance->SetWeapons( (tmp == CLocalizationSettings::Instance()->GetLocalText("OFF")) ? false : true );
	return true;
}



bool CChooseMapPhase::OnMoneyCombobox(const CEGUI::EventArgs &)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_Money_Combobox")->getText();
	CGameSettings::ClientInstance->StartingMoney = atoi(tmp.c_str());
	return true;
}



bool CChooseMapPhase::OnLapCountCombobox(const CEGUI::EventArgs &)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_LapCount_Combobox")->getText();
	CGameSettings::ClientInstance->LapCount = atoi(tmp.c_str());
	return true;
}



bool CChooseMapPhase::OnBack(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CMenuPhase::Instance->PrevPhase);
	return true;
}



bool CChooseMapPhase::OnCreate(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CHostGamePhase::Instance);
	return true;
}



bool CChooseMapPhase::OnMapTypeCombobox(const CEGUI::EventArgs &)
{
	CEGUI::String tmp = CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_MapType_Combobox")->getText();
	CGameSettings::MAPTYPE LastMapType = CGameSettings::ClientInstance->MapType;
	String MapMask = "*.utm";
	
	if (tmp == CLocalizationSettings::Instance()->GetLocalText("STANDARD RACE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_SINGLERACE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("TOURNAMENT"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_TOURNAMENT;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("LASTONE"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_LASTONE;
	}
	else if (tmp == CLocalizationSettings::Instance()->GetLocalText("DEATHMATCH"))
	{
		CGameSettings::ClientInstance->MapType = CGameSettings::MAPTYPE_MP_DEATHMATCH;
		MapMask = "*.utd";
	}

	CMapProvider::Instance->LoadAllMapNames(MapMask);
	CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_Map_Combobox")->destroy();
	CEGUI::Combobox* pCombobox = CGUIHelper::CreateCombobox("ChooseMap_Map_Combobox",0.28f,0.3f,0.6f,0.4f,&CMapProvider::Instance->MapNames,"ChooseMap_Options_Window", &OnMapNameCombobox);

	return true;
}



bool CChooseMapPhase::OnMapNameCombobox(const CEGUI::EventArgs &)
{
	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	CGameSettings::ClientInstance->MapName = String(CEGUI::WindowManager::getSingleton().getWindow("ChooseMap_Map_Combobox")->getText().c_str())+String(".utm");
	
	CMap TmpMap; 
	TmpMap.LoadMapHeader(CGameSettings::ClientInstance->MapName);
	(Mng.getWindow("ChooseMap_Description"))->setText(TmpMap.MapHeader.MapDescription.GetString());
	(Mng.getWindow("ChooseMap_MapName"))->setText(CGameSettings::ClientInstance->MapName.Extract('.',0).GetString());

	CChooseMapPhase::Instance->MapDescImage = WeTextureManager::Instance()->Load(TmpMap.MapHeader.MapDescriptionImage,true);
	return true;
}
