/*
    COptionsAudioPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <COptionsAudioPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <CGUIHelper.h>
#include <WeCEGUI.h>
#include <COptionsSettings.h>
#include <COptionsPhase.h>
#include <CLocalizationSettings.h>
#include <CMainMenuPhase.h>
#include <CPlayGamePhase.h>



COptionsAudioPhase *COptionsAudioPhase::Instance = new COptionsAudioPhase;
bool COptionsAudioPhase::ManualScrollSet = false;




COptionsAudioPhase::COptionsAudioPhase()
{
	Created = false;
}



void COptionsAudioPhase::Start()
{
	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();


	CEGUI::Window* AudioWindow = CGUIHelper::CreateWnd("Options_AudioWindow",CLocalizationSettings::Instance()->GetLocalText("AUDIO"),
		0.0f,0.6f,0.13f,0.4f,0.5f,"RootWindow");
	

	Strings Volumes;
	Volumes.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));
	for (int i = 10; i <= 100; i += 10)
	{
		char tmp[100];
		sprintf(tmp, "%d%%", i);
		Volumes.PushBack((String)tmp);
	}


	CGUIHelper::CreateText("AudioOptions_MusicVolumeTitle", CLocalizationSettings::Instance()->GetLocalText("MUSIC VOLUME"),
		0.1f, 0.3f, 1.0f, 0.05f, "", "", "", "Options_AudioWindow");

	//CEGUI::Combobox* MusicVolumeCombo = CGUIHelper::CreateCombobox("AudioOptions_MusicVolumeCombo", 
		//0.6f, 0.3f, 0.3f, 0.6f, &Volumes, "Options_AudioWindow", &OnMusicVolumeChanged);

	CEGUI::Scrollbar *MusicScrollbar = CGUIHelper::CreateHorzScrollbar("AudioOptions_MusicVolumeScrollbar",
		0.6f, 0.3f, 0.3f, 0.02f, 0.1f, 1.1f, "Options_AudioWindow", &OnMusicVolumeChanged);

	if(COptionsSettings::Instance->MusicVolume == 0.0f)
		//MusicVolumeCombo->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
		MusicScrollbar->setScrollPosition(0.0f);
	else 
	{
		//char tmp[100];
		//sprintf(tmp, "%d%%", (int)(COptionsSettings::Instance->MusicVolume * 100.0f));
		//MusicVolumeCombo->setText(tmp);
		MusicScrollbar->setScrollPosition(COptionsSettings::Instance->MusicVolume);
	}


	CGUIHelper::CreateText("AudioOptions_SoundVolumeTitle", CLocalizationSettings::Instance()->GetLocalText("FX VOLUME"),
		0.1f, 0.4f, 1.0f, 0.05f, "", "", "", "Options_AudioWindow");


	//CEGUI::Combobox* SoundVolumeCombo = CGUIHelper::CreateCombobox("AudioOptions_SoundVolumeCombo", 
		//0.6f, 0.4f, 0.3f, 0.6f, &Volumes, "Options_AudioWindow", &OnSoundVolumeChanged);

	CEGUI::Scrollbar *SoundScrollbar = CGUIHelper::CreateHorzScrollbar("AudioOptions_SoundVolumeScrollbar",
		0.6f, 0.4f, 0.3f, 0.02f, 0.1f, 1.1f, "Options_AudioWindow", &OnSoundVolumeChanged);

	if(COptionsSettings::Instance->SoundVolume == 0.0f)
		//SoundVolumeCombo->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
		SoundScrollbar->setScrollPosition(0.0f);
	else 
	{
		//char tmp[100];
		//sprintf(tmp, "%d%%", (int)(COptionsSettings::Instance->SoundVolume * 100.0f));
		//SoundVolumeCombo->setText(tmp);
		ManualScrollSet = true;
		SoundScrollbar->setScrollPosition(COptionsSettings::Instance->SoundVolume);
	}

	

	CGUIHelper::CreateButton("AudioOptions_BackButton", CLocalizationSettings::Instance()->GetLocalText("BACK"),
		0.05f, 0.8f, 1.0f, 0.12f, "Options_AudioWindow", &COptionsAudioPhase::OnBack);

	winMgr.getWindow("Options_AudioWindow")->setProperty("Visible","true");
}



void COptionsAudioPhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	winMgr.getWindow("Options_AudioWindow")->destroy();

	COptionsPhase::Instance->End();
}



void COptionsAudioPhase::Tick()
{
	COptionsPhase::Instance->Tick();

	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("Options_AudioWindow", WNDAlpha);

	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}



void COptionsAudioPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



bool COptionsAudioPhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	if (COptionsPhase::Instance->FromInGame)
		COptionsAudioPhase::Instance->End();
	else
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	return true;
}



bool COptionsAudioPhase::OnMusicVolumeChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
/*
	CEGUI::Combobox *Combo = (CEGUI::Combobox*)winMgr.getWindow("AudioOptions_MusicVolumeCombo");

	int Selected = (int)Combo->getItemIndex(Combo->getSelectedItem());

	COptionsSettings::Instance->MusicVolume = (float)Selected / 10.0f;
*/
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("AudioOptions_MusicVolumeScrollbar");
	COptionsSettings::Instance->MusicVolume = Scrollbar->getScrollPosition();

	WeAudioDevice::Instance()->SetMusicVolume(COptionsSettings::Instance->MusicVolume);

	COptionsSettings::Instance->Save();

	return true;
}




bool COptionsAudioPhase::OnSoundVolumeChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
/*
	CEGUI::Combobox *Combo = (CEGUI::Combobox*)winMgr.getWindow("AudioOptions_SoundVolumeCombo");
	
	int Selected = (int)Combo->getItemIndex(Combo->getSelectedItem());

	COptionsSettings::Instance->SoundVolume = (float)Selected / 10.0f;
*/
	CEGUI::Scrollbar *Scrollbar = (CEGUI::Scrollbar*)winMgr.getWindow("AudioOptions_SoundVolumeScrollbar");
	COptionsSettings::Instance->SoundVolume = Scrollbar->getScrollPosition();

	WeAudioDevice::Instance()->SetSoundVolume(COptionsSettings::Instance->SoundVolume);

	if (!ManualScrollSet)
		CMenuPhase::Instance->PlayMenuSound(MenuSoundVolumeTest);
	else
		ManualScrollSet = false;

	COptionsSettings::Instance->Save();

	return true;
}
