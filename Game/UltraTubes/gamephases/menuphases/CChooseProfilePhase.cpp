/*
    CChooseProfilePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CChooseProfilePhase.h>
#include <CGame.h>
#include <CMainMenuPhase.h>
#include <CMenuPhase.h>
#include <WeCEGUI.h>
#include <CEGUIPropertyHelper.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <CCreateProfilePhase.h>
#include <WeTimer.h>
#include <WeSound.h>
#include <CSinglePlayerPhase.h>
#include <CSingleRacePhase.h>
#include <CGUIHelper.h>
#include <CProfileManager.h>
#include <CShoppingPhase.h>
#include <CFunnyPhase.h>
#include <CTuberDescription.h>
#include <CLocalizationSettings.h>
#include <CConfig.h>



class ProfileListItem : public MyListItem
{
public:
	ProfileListItem(StringParam text, StringParam ParentName) : MyListItem(text, ParentName)
	{
	}

	CProfile* Profile;
};


CChooseProfilePhase *CChooseProfilePhase::Instance = new CChooseProfilePhase;

CChooseProfilePhase::CChooseProfilePhase()
{
}


void CChooseProfilePhase::InitWindows()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	if(CMenuPhase::Instance->PrevPhase == CCreateProfilePhase::Instance)
	{
		winMgr.getWindow("ChooseProfile_Window")->setProperty("Alpha","1.0");
		winMgr.getWindow("CHOOSEPROFILE_OK_BUTTON")->setProperty("Alpha","1.0");
		winMgr.getWindow("CHOOSEPROFILE_CREATE_PROFILE_BUTTON")->setProperty("Alpha","1.0");
		winMgr.getWindow("CHOOSEPROFILE_DELETE_PROFILE_BUTTON")->setProperty("Alpha","1.0");

		return;
	}

	
	CGUIHelper::CreateWnd("ChooseProfile_Window", CLocalizationSettings::Instance()->GetLocalText("CHOOSE PROFILE"), 0.0f, 0.3f,0.03f,0.4f,0.05f, "", "true");

	CSmartList<String> ColumnNames;
	ColumnNames.PushBack("PROFILES");

	ProfileList = CGUIHelper::CreateMultiList("ChooseProfile_ProfileList", 0.3f, 0.2f, 0.4f, 0.55f, ColumnNames, false, true, "ChooseProfile_Window");
	ProfileList->subscribeEvent(CEGUI::MultiColumnList::EventMouseDoubleClick, CEGUI::Event::Subscriber(&CChooseProfilePhase::OnProfileListDblClick));


	CGUIHelper::CreateButton("CHOOSEPROFILE_OK_BUTTON",CLocalizationSettings::Instance()->GetLocalText("OK"),0.65f,0.85f,0.35f,0.085f,"RootWindow",&CChooseProfilePhase::OnOk);
	
	CGUIHelper::CreateButton("CHOOSEPROFILE_DELETE_PROFILE_BUTTON",CLocalizationSettings::Instance()->GetLocalText("DELETE"),0.01f,0.85f,0.35f,0.085f,"RootWindow",&CChooseProfilePhase::OnDeleteProfile);

	CGUIHelper::CreateButton("CHOOSEPROFILE_CREATE_PROFILE_BUTTON",CLocalizationSettings::Instance()->GetLocalText("CREATE"),0.33f,0.85f,0.35f,0.085f,"RootWindow",&CChooseProfilePhase::OnCreateProfile);


	CGUIHelper::CreateText("CreateProfileFirst", CLocalizationSettings::Instance()->GetLocalText("CHOOSE PROFILE FIRST"), 0.32f,0.75f,0.4f,0.1f,"Font_16", "", "", "");
	CEGUI::WindowManager::getSingleton().getWindow("CreateProfileFirst")->setProperty("Visible", "false");
}



void CChooseProfilePhase::Start()
{
	Created = false;

	InitWindows();

	int i = 0;
	for (CProfileManager::Instance()->Profiles.First(); !CProfileManager::Instance()->Profiles.End(); CProfileManager::Instance()->Profiles.Next())
	{
		ProfileListItem *item;
		item = new ProfileListItem(CProfileManager::Instance()->Profiles.GetItem()->Name, "ChooseProfile_ProfileList");
		item->Profile = CProfileManager::Instance()->Profiles.GetItem();
		ProfileList->addRow();
		ProfileList->setItem(item, 0, i);

		if (CProfileManager::Instance()->GetCurrentProfile() == item->Profile)
			item->setSelected(true);

		i++;
	}    
}



void CChooseProfilePhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	if(CMenuPhase::Instance->NextPhase == CCreateProfilePhase::Instance)
	{
		winMgr.getWindow("ChooseProfile_Window")->setProperty("Alpha","0.5");
		winMgr.getWindow("CHOOSEPROFILE_OK_BUTTON")->setProperty("Alpha","0.5");
		winMgr.getWindow("CHOOSEPROFILE_CREATE_PROFILE_BUTTON")->setProperty("Alpha","0.5");
		winMgr.getWindow("CHOOSEPROFILE_DELETE_PROFILE_BUTTON")->setProperty("Alpha","0.5");
		return;
	}

	winMgr.getWindow("ChooseProfile_Window")->destroy();
	winMgr.getWindow("CHOOSEPROFILE_OK_BUTTON")->destroy();
	winMgr.getWindow("CHOOSEPROFILE_CREATE_PROFILE_BUTTON")->destroy();
	winMgr.getWindow("CHOOSEPROFILE_DELETE_PROFILE_BUTTON")->destroy();
	winMgr.getWindow("CreateProfileFirst")->destroy();
}



void CChooseProfilePhase::Tick()
{
}



void CChooseProfilePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



bool CChooseProfilePhase::OnProfileListDblClick(const CEGUI::EventArgs& e)
{
	if (!CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem()) 
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
		return true;
	}

	return OnOk(e);
}



bool CChooseProfilePhase::OnOk(const CEGUI::EventArgs& e)
{
	if (!CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem()) 
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);

		CEGUI::WindowManager::getSingleton().getWindow("CreateProfileFirst")->setProperty("Visible", "true");
		return true;
	}

	CProfileManager::Instance()->SetCurrentProfile(((ProfileListItem *)CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem())->Profile, true);
	CProfileManager::Instance()->SetProfileControls(((ProfileListItem *)CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem())->Profile);
	
	CTuberDescription tmpDSC;
	CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = tmpDSC;
	CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = false;

	CProfileManager::Instance()->SaveAllProfiles();

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	return true;
}



bool CChooseProfilePhase::OnDeleteProfile(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager::getSingleton().getWindow("CreateProfileFirst")->setProperty("Visible", "false");

	if(!CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem())
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
		return true;
	}

	CProfileManager::Instance()->RemoveProfile(CChooseProfilePhase::Instance->ProfileList->getFirstSelectedItem()->getText().c_str());
	CProfileManager::Instance()->SaveAllProfiles();
	CProfileManager::Instance()->LoadAllProfiles();

	CChooseProfilePhase::Instance->ProfileList->resetList();

	int i = 0;
	for (CProfileManager::Instance()->Profiles.First(); !CProfileManager::Instance()->Profiles.End(); CProfileManager::Instance()->Profiles.Next())
	{
		ProfileListItem *item;
		item = new ProfileListItem(CProfileManager::Instance()->Profiles.GetItem()->Name, "ChooseProfile_ProfileList");
		item->Profile = CProfileManager::Instance()->Profiles.GetItem();
		CChooseProfilePhase::Instance->ProfileList->addRow();
		CChooseProfilePhase::Instance->ProfileList->setItem(item, 0, i);
		i++;
	}

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	return true;
}



bool CChooseProfilePhase::OnCreateProfile(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager::getSingleton().getWindow("CreateProfileFirst")->setProperty("Visible", "false");

	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CCreateProfilePhase::Instance);

	return true;
}

