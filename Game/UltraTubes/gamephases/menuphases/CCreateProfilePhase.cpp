/*
    CCreateProfilePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CCreateProfilePhase.h>
#include <CMenuPhase.h>
#include <CGame.h>
#include <CChooseProfilePhase.h>
#include <WeCEGUI.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CGUIHelper.h>
#include <CProfileManager.h>
#include <CLocalizationSettings.h>
#include <CEGUIPropertyHelper.h>



CCreateProfilePhase *CCreateProfilePhase::Instance = new CCreateProfilePhase;


CCreateProfilePhase::CCreateProfilePhase()
{
	Created = false;
	Fade = false;
}

void CCreateProfilePhase::Start()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* CreateProfileWnd = CGUIHelper::CreateWnd("CreateProfile_CreateProfileWnd",CLocalizationSettings::Instance()->GetLocalText("CREATE PROFILE"),
		0.0f,0.2f,0.1f,0.5f,0.7f,"RootWindow");
	CreateProfileWnd->setModalState(true);
		
	

	CGUIHelper::CreateText("CreateProfile_NameTitle",CLocalizationSettings::Instance()->GetLocalText("ENTER YOUR NAME"),0.15f,0.2f,0.8f,0.1f,"Font_16", "", "", "CreateProfile_CreateProfileWnd");
	

	CEGUI::Editbox* NameEdit;
	NameEdit = CGUIHelper::CreateEditbox("CreateProfile_NameEdit", 0.15f, 0.3f, 0.6f, 0.05f,
		"ULTRA RACER", CEGUI::colour(1.0f,0.0f,0.0f), "CreateProfile_CreateProfileWnd");
	NameEdit->subscribeEvent(CEGUI::Editbox::EventTextAccepted,CEGUI::Event::Subscriber(&CCreateProfilePhase::OnOk));
	NameEdit->subscribeEvent(CEGUI::Editbox::EventCharacterKey, CEGUI::Event::Subscriber(&CCreateProfilePhase::OnKeyPress));
	NameEdit->activate();


	CGUIHelper::CreateButton("CREATEPROFILE_OK_BUTTON",CLocalizationSettings::Instance()->GetLocalText("OK"),0.55f,0.6f,0.4f,0.12f,"CreateProfile_CreateProfileWnd", &CCreateProfilePhase::OnOk);
	
	CGUIHelper::CreateButton("CREATEPROFILE_CANCEL_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CANCEL"),0.15f,0.6f,0.4f,0.12f,"CreateProfile_CreateProfileWnd", &CCreateProfilePhase::OnCancel);


	winMgr.getWindow("CreateProfile_CreateProfileWnd")->setModalState(true);
	winMgr.getWindow("CreateProfile_CreateProfileWnd")->setProperty("Visible","true");
	winMgr.getWindow("CreateProfile_CreateProfileWnd")->setProperty("Alpha","0.0");

	Fade = true;
}

void CCreateProfilePhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	winMgr.getWindow("CreateProfile_CreateProfileWnd")->setModalState(false);
	winMgr.getWindow("CreateProfile_CreateProfileWnd")->destroy();
}

void CCreateProfilePhase::Tick()
{
	if(CGame::Instance->InputMessage.Exit)
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
		CMenuPhase::Instance->SetPhase(CChooseProfilePhase::Instance);
		return;
	}

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	static float Alpha = 0.0f;
	
	if(!Fade) { Alpha = 0.0f; return; }
	
	
	Alpha += 0.06f;
    
	if(Alpha >= 1.0f)
	{
		Alpha = 1.0f;
		Fade = false;
	}
	
	char tmpAlpha[100];
	sprintf(tmpAlpha,"%f",Alpha);

	winMgr.getWindow("CreateProfile_CreateProfileWnd")->setProperty("Alpha",tmpAlpha);
}

void CCreateProfilePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}


bool CCreateProfilePhase::OnOk(const CEGUI::EventArgs& e)
{
	CCreateProfilePhase::Instance->ProfileName = CEGUI::WindowManager::getSingleton().getWindow("CreateProfile_NameEdit")->getText().c_str();
	if (CCreateProfilePhase::Instance->ProfileName.GetLen() > 20)
		CCreateProfilePhase::Instance->ProfileName = CCreateProfilePhase::Instance->ProfileName.Sub(0, 20);

	if (CCreateProfilePhase::Instance->ProfileName == (String)"")
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
		return false;
	}

	if (CProfileManager::Instance()->GetProfileByName(CCreateProfilePhase::Instance->ProfileName))
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
		return false;
	}

	CProfileManager::Instance()->NewProfile(CCreateProfilePhase::Instance->ProfileName);

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CChooseProfilePhase::Instance);

	return false;
}


bool CCreateProfilePhase::OnCancel(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	CMenuPhase::Instance->SetPhase(CChooseProfilePhase::Instance);
	return false;
}


bool CCreateProfilePhase::OnKeyPress(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundKeyPress);
	return true;
}
