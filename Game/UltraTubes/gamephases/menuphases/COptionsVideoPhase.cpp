/*
    COptionsVideoPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <COptionsVideoPhase.h>
#include <CGame.h>
#include <WeCEGUI.h>
#include <CGUIHelper.h>
#include <CMenuPhase.h>
#include <COptionsSettings.h>
#include <COptionsPhase.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeTextureManager.h>
#include <WePhysics.h>
#include <WePostProcess.h>
#include <CLocalizationSettings.h>
#include <CLoadMenuPhase.h>
#include <CEGUIPropertyHelper.h>
#include <CMainMenuPhase.h>
#include <CPlayGamePhase.h>



COptionsVideoPhase *COptionsVideoPhase::Instance = new COptionsVideoPhase;


COptionsVideoPhase::COptionsVideoPhase()
{
	Created = false;
}



void COptionsVideoPhase::Start()
{
	WNDFadeIn = true;
	WNDAlpha = 0.0f;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	Strings rows;
	CEGUI::Window *wnd;


	CEGUI::Window* VideoWindow = CGUIHelper::CreateWnd("Options_VideoWindow",CLocalizationSettings::Instance()->GetLocalText("VIDEO"),
		0.0f, 0.6f, 0.01f, 0.4f, 0.9f, "RootWindow");


	wnd = CGUIHelper::CreateText("VideoOptions_ResolutionTitle", CLocalizationSettings::Instance()->GetLocalText("RESOLUTION"), 0.1f, 0.3f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");
	if (COptionsPhase::Instance->FromInGame)
		wnd->setAlpha(0.5f);

	rows.Clear();
	rows.PushBack("640x480");
	rows.PushBack("800x600");
	rows.PushBack("1024x768");
	rows.PushBack("1280x960");
	rows.PushBack("1280x1024");
	CEGUI::Combobox* ResolutionCombo = CGUIHelper::CreateCombobox("VideoOptions_ResolutionCombo", 0.5f, 0.3f, 0.4f, 0.15f, &rows, "Options_VideoWindow", &OnResolutionChanged);
	if (COptionsPhase::Instance->FromInGame)
	{
		ResolutionCombo->setAlpha(0.5f);
		ResolutionCombo->setEnabled(false);
	}

	if (COptionsSettings::Instance->ResHeight == 480)
		ResolutionCombo->setText("640x480");
	else if (COptionsSettings::Instance->ResHeight == 600)
		ResolutionCombo->setText("800x600");
	else if (COptionsSettings::Instance->ResHeight == 768)
		ResolutionCombo->setText("1024x768");
	else if (COptionsSettings::Instance->ResHeight == 960)
		ResolutionCombo->setText("1280x960");
	else if (COptionsSettings::Instance->ResHeight == 1024)
		ResolutionCombo->setText("1280x1024");
	else
		ResolutionCombo->setText("800x600");


	wnd = CGUIHelper::CreateText("VideoOptions_EffectQualityTitle", CLocalizationSettings::Instance()->GetLocalText("EFFECTS"), 0.1f, 0.345f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");
	if (COptionsPhase::Instance->FromInGame)
		wnd->setAlpha(0.5f);
	wnd->setAlpha(0.5f);

	rows.Clear();
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("LOW"));
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("HIGH"));
	CEGUI::Combobox* EffectQualityCombo = CGUIHelper::CreateCombobox("VideoOptions_EffectQualityCombo", 0.5f, 0.345f, 0.4f, 0.1f, &rows, "Options_VideoWindow", &OnEffectChanged);
	if (COptionsPhase::Instance->FromInGame)
	{
		EffectQualityCombo->setAlpha(0.5f);
		EffectQualityCombo->setEnabled(false);
	}

	if ((COptionsSettings::Instance->ShaderQuality == 3) && WeShaderManager::Instance()->SetQuality(SHADER_HIGH))
	{
		EffectQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("HIGH").GetString());
	}
	else 
	{
		COptionsSettings::Instance->ShaderQuality = 1;
		EffectQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());
	}

	COptionsSettings::Instance->ShaderQuality = 1;
	EffectQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());
	EffectQualityCombo->setAlpha(0.5f);
	EffectQualityCombo->setEnabled(false);


	wnd = CGUIHelper::CreateText("VideoOptions_TextureQualityTitle", CLocalizationSettings::Instance()->GetLocalText("TEXTURES"), 0.1f, 0.39f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");

	rows.Clear();
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("LOW"));
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("MEDIUM"));
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("HIGH"));
	CEGUI::Combobox* TextureQualityCombo = CGUIHelper::CreateCombobox("VideoOptions_TextureQualityCombo", 0.5f, 0.39f, 0.4f, 0.12f, &rows, "Options_VideoWindow", &OnTextureChanged);

	if (COptionsSettings::Instance->TextureQuality == 0)
		TextureQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());
	else if (COptionsSettings::Instance->TextureQuality == 1)
		TextureQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("MEDIUM").GetString());
	else if (COptionsSettings::Instance->TextureQuality == 2)
		TextureQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("HIGH").GetString());
	else
		TextureQualityCombo->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());


	wnd = CGUIHelper::CreateText("VideoOptions_PhysicsQualityTitle", CLocalizationSettings::Instance()->GetLocalText("PHYSICS"), 0.1f, 0.435f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");

	rows.Clear();
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("LOW"));
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("MEDIUM"));
	if (WePhysics::Instance()->IsHardwareAvailable())
		rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("HIGH"));
	CEGUI::Combobox* PhysicsQuality = CGUIHelper::CreateCombobox("VideoOptions_PhysicsQualityCombo", 0.5f, 0.435f, 0.4f, 0.12f, &rows, "Options_VideoWindow", &OnPhysicsChanged);

	if (COptionsSettings::Instance->PhysicsQuality == 0)
		PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	else if (COptionsSettings::Instance->PhysicsQuality == 1)
		PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());
	else if (COptionsSettings::Instance->PhysicsQuality == 2)
		PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("MEDIUM").GetString());
	else if (COptionsSettings::Instance->PhysicsQuality == 3)
	{
		if (WePhysics::Instance()->IsHardwareAvailable())
			PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("HIGH").GetString());
		else
			PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("MEDIUM").GetString());
	}
	else
		PhysicsQuality->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());



	wnd = CGUIHelper::CreateText("VideoOptions_AntialiasingTitle", CLocalizationSettings::Instance()->GetLocalText("ANTIALIASING"), 0.1f, 0.48f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");
	if (COptionsPhase::Instance->FromInGame)
		wnd->setAlpha(0.5f);

	rows.Clear();
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));
	rows.PushBack("2X");
	rows.PushBack("4X");
	rows.PushBack("8X");
	rows.PushBack("16X");
	CEGUI::Combobox* Antialiasing = CGUIHelper::CreateCombobox("VideoOptions_AntialiasingCombo", 0.5f, 0.48f, 0.4f, 0.17f, &rows, "Options_VideoWindow", &OnAntialiasingChanged);
	if (COptionsPhase::Instance->FromInGame)
	{
		Antialiasing->setAlpha(0.5f);
		Antialiasing->setEnabled(false);
	}

	if (COptionsSettings::Instance->Antialiasing == 0)
		Antialiasing->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	else if (COptionsSettings::Instance->Antialiasing == 2)
		Antialiasing->setText("2X");
	else if (COptionsSettings::Instance->Antialiasing == 4)
		Antialiasing->setText("4X");
	else if (COptionsSettings::Instance->Antialiasing == 8)
		Antialiasing->setText("8X");
	else if (COptionsSettings::Instance->Antialiasing == 16)
		Antialiasing->setText("16X");
	else
		Antialiasing->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());



	wnd = CGUIHelper::CreateText("VideoOptions_AnisotropicTitle", CLocalizationSettings::Instance()->GetLocalText("ANISOTROPIC"), 0.1f, 0.525f, 1.0f, 0.05f, "", "", "", "Options_VideoWindow");

	rows.Clear();
	rows.PushBack(CLocalizationSettings::Instance()->GetLocalText("OFF"));
	rows.PushBack("2X");
	rows.PushBack("4X");
	rows.PushBack("8X");
	rows.PushBack("16X");
	CEGUI::Combobox* Anisotropic = CGUIHelper::CreateCombobox("VideoOptions_AnisotropicCombo", 0.5f, 0.525f, 0.4f, 0.17f, &rows, "Options_VideoWindow", &OnAnisotropicChanged);

	if (COptionsSettings::Instance->Anisotropic == 0)
		Anisotropic->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());
	else if (COptionsSettings::Instance->Anisotropic == 2)
		Anisotropic->setText("2X");
	else if (COptionsSettings::Instance->Anisotropic == 4)
		Anisotropic->setText("4X");
	else if (COptionsSettings::Instance->Anisotropic == 8)
		Anisotropic->setText("8X");
	else if (COptionsSettings::Instance->Anisotropic == 16)
		Anisotropic->setText("16X");
	else
		Anisotropic->setText(CLocalizationSettings::Instance()->GetLocalText("OFF").GetString());


	//wnd = CGUIHelper::CreateText("VideoOptions_ParticleCheckTitle", CLocalizationSettings::Instance()->GetLocalText("PARTICLES"), 0.1f, 0.57f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	//CEGUI::Checkbox* ParticleCheck = CGUIHelper::CreateCheckbox("VideoOptions_ParticleCheck", 0.6f, 0.57f, 0.05f, 0.05f, "Options_VideoWindow", &OnParticlesChanged);
	//ParticleCheck->setSelected(COptionsSettings::Instance->Particles);


	CGUIHelper::CreateText("VideoOptions_PostProcessTitle", CLocalizationSettings::Instance()->GetLocalText("POST PROCESS"), 0.1f, 0.615f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* PostProcessCheck = CGUIHelper::CreateCheckbox("VideoOptions_PostProcessCheck", 0.6f, 0.615f, 0.05f, 0.05f, "Options_VideoWindow", &OnPostProcessChanged);
	PostProcessCheck->setSelected(COptionsSettings::Instance->PostProcess);


	CGUIHelper::CreateText("VideoOptions_MotionBlurTitle", CLocalizationSettings::Instance()->GetLocalText("MOTION BLUR"), 0.1f, 0.66f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* MotionBlurCheck = CGUIHelper::CreateCheckbox("VideoOptions_MotionBlurCheck", 0.6f, 0.66f, 0.05f, 0.05f, "Options_VideoWindow", &OnMotionBlurChanged);
	MotionBlurCheck->setSelected(COptionsSettings::Instance->MotionBlur);


	CGUIHelper::CreateText("VideoOptions_GlowTitle", CLocalizationSettings::Instance()->GetLocalText("GLOW"), 0.1f, 0.705f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* GlowCheck = CGUIHelper::CreateCheckbox("VideoOptions_GlowCheck", 0.6f, 0.705f, 0.05f, 0.05f, "Options_VideoWindow", &OnGlowChanged);
	GlowCheck->setSelected(COptionsSettings::Instance->Glow);


	wnd = CGUIHelper::CreateText("VideoOptions_RefractionTitle", CLocalizationSettings::Instance()->GetLocalText("REFRACTION"), 0.1f, 0.93f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* RefractionCheck = CGUIHelper::CreateCheckbox("VideoOptions_RefractionCheck", 0.6f, 0.95f, 0.05f, 0.05f, "Options_VideoWindow", &OnRefractionChanged);
	RefractionCheck->setSelected(COptionsSettings::Instance->Refraction);


	wnd = CGUIHelper::CreateText("VideoOptions_ShadowsTitle", CLocalizationSettings::Instance()->GetLocalText("SHADOWS"), 0.1f, 0.75f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	if (true || COptionsPhase::Instance->FromInGame)
		wnd->setAlpha(0.5f);

	CEGUI::Checkbox* ShadowsCheck = CGUIHelper::CreateCheckbox("VideoOptions_ShadowsCheck", 0.6f, 0.75f, 0.05f, 0.05f, "Options_VideoWindow", &OnShadowsChanged);
	if (true || COptionsPhase::Instance->FromInGame)
	{
		ShadowsCheck->setAlpha(0.5f);
		ShadowsCheck->setEnabled(false);
	}
	ShadowsCheck->setSelected(COptionsSettings::Instance->Shadows);
	ShadowsCheck->setSelected(false);

	
	wnd = CGUIHelper::CreateText("VideoOptions_ProjectorsTitle", CLocalizationSettings::Instance()->GetLocalText("PROJEKTORS"), 0.1f, 0.795f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* ProjectorsCheck = CGUIHelper::CreateCheckbox("VideoOptions_ProjectorsCheck", 0.6f, 0.795f, 0.05f, 0.05f, "Options_VideoWindow", &OnProjektorsChanged);
	ProjectorsCheck->setSelected(COptionsSettings::Instance->Projektors);


	//wnd = CGUIHelper::CreateText("VideoOptions_FlaresTitle", CLocalizationSettings::Instance()->GetLocalText("FLARES"), 0.1f, 0.84f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	//CEGUI::Checkbox* FlaresCheck = CGUIHelper::CreateCheckbox("VideoOptions_FlaresCheck", 0.6f, 0.84f, 0.05f, 0.05f, "Options_VideoWindow", &OnFlaresChanged);
	//FlaresCheck->setSelected(COptionsSettings::Instance->Flares);


	wnd = CGUIHelper::CreateText("VideoOptions_SpecularTitle", CLocalizationSettings::Instance()->GetLocalText("SPECULAR"), 0.1f, 0.885f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* SpecularCheck = CGUIHelper::CreateCheckbox("VideoOptions_SpecularCheck", 0.6f, 0.885f, 0.05f, 0.05f, "Options_VideoWindow", &OnSpecularChanged);
	SpecularCheck->setSelected(COptionsSettings::Instance->Specular);


	wnd = CGUIHelper::CreateText("VideoOptions_BumpingTitle", CLocalizationSettings::Instance()->GetLocalText("BUMPING"), 0.1f, 0.93f, 0.4f, 0.05f, "", "", "", "Options_VideoWindow");
	CEGUI::Checkbox* BumpingCheck = CGUIHelper::CreateCheckbox("VideoOptions_BumpingCheck", 0.6f, 0.93f, 0.05f, 0.05f, "Options_VideoWindow", &OnBumpingChanged);
	BumpingCheck->setSelected(COptionsSettings::Instance->Bumping);

	CGUIHelper::CreateButton("VideoOptions_BackButton", CLocalizationSettings::Instance()->GetLocalText("BACK"),
		0.05f, 0.8f, 1.0f, 0.12f, "Options_VideoWindow", &COptionsVideoPhase::OnBack);

	UpdateOptions();
}



void COptionsVideoPhase::End()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	winMgr.getWindow("Options_VideoWindow")->destroy();

	COptionsPhase::Instance->End();
}



void COptionsVideoPhase::Tick()
{
	COptionsPhase::Instance->Tick();

	if(WNDFadeIn) WNDFadeIn = CGUIHelper::WindowFade("Options_VideoWindow", WNDAlpha);

	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}





void COptionsVideoPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



bool COptionsVideoPhase::OnBack(const CEGUI::EventArgs& e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	if (COptionsPhase::Instance->FromInGame)
		COptionsVideoPhase::Instance->End();
	else
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	return true;
}




bool COptionsVideoPhase::OnResolutionChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_ResolutionCombo")->getText().c_str();

	COptionsSettings::Instance->ResWidth = Res.Extract('x', 0).AsInt();
	COptionsSettings::Instance->ResHeight = Res.Extract('x', 1).AsInt();

	CMainMenuPhase::Instance->SomethingChanged = true;
	CMainMenuPhase::Instance->MustRestartText->setVisible(true);

	COptionsSettings::Instance->Save();

	//WeRenderDevice::Instance()->SetDisplayMode(COptionsSettings::Instance->ResWidth, COptionsSettings::Instance->ResHeight, 32, WeRenderDevice::Instance()->IsFullScreen(), true);
	//CLoadMenuPhase::Instance->CreateFonts(COptionsSettings::Instance->ResWidth);

	return true;
}



bool COptionsVideoPhase::OnEffectChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_EffectQualityCombo")->getText().c_str();

	if (Res == CLocalizationSettings::Instance()->GetLocalText("LOW"))
	{
		COptionsSettings::Instance->ShaderQuality = 1;
		WeShaderManager::Instance()->SetQuality(SHADER_LOW);
	} 
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("HIGH"))
	{
		COptionsSettings::Instance->ShaderQuality = 3;
		if (!WeShaderManager::Instance()->SetQuality(SHADER_HIGH))
		{
			winMgr.getWindow("VideoOptions_EffectQualityCombo")->setText(CLocalizationSettings::Instance()->GetLocalText("LOW").GetString());
		}
	}

	CMainMenuPhase::Instance->SomethingChanged = true;
	CMainMenuPhase::Instance->MustRestartText->setVisible(true);

	COptionsSettings::Instance->Save();
	return true;
}



bool COptionsVideoPhase::OnTextureChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_TextureQualityCombo")->getText().c_str();

	if (Res == CLocalizationSettings::Instance()->GetLocalText("LOW"))
	{
		COptionsSettings::Instance->TextureQuality = 0;
		WeTextureManager::Instance()->SetQuality(TEXTURES_LOW);
	}
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("MEDIUM"))
	{
		COptionsSettings::Instance->TextureQuality = 1;
		WeTextureManager::Instance()->SetQuality(TEXTURES_MEDIUM);
	}
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("HIGH"))
	{
		COptionsSettings::Instance->TextureQuality = 2;
		WeTextureManager::Instance()->SetQuality(TEXTURES_HIGH);
	}
	
	CMainMenuPhase::Instance->SomethingChanged = true;
	CMainMenuPhase::Instance->MustRestartText->setVisible(true);

	COptionsSettings::Instance->Save();
	return true;
}




bool COptionsVideoPhase::OnPhysicsChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_PhysicsQualityCombo")->getText().c_str();

	if (Res == CLocalizationSettings::Instance()->GetLocalText("OFF"))
		COptionsSettings::Instance->PhysicsQuality = 0;
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("LOW"))
		COptionsSettings::Instance->PhysicsQuality = 1;
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("MEDIUM"))
		COptionsSettings::Instance->PhysicsQuality = 2;
	else if (Res == CLocalizationSettings::Instance()->GetLocalText("HIGH"))
	{
		if (WePhysics::Instance()->IsHardwareAvailable())
			COptionsSettings::Instance->PhysicsQuality = 3;
		else
			COptionsSettings::Instance->PhysicsQuality = 2;
	}
	
	COptionsSettings::Instance->Save();
	return true;
}







bool COptionsVideoPhase::OnPostProcessChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_PostProcessCheck"));
	
	COptionsSettings::Instance->PostProcess = tmp->isSelected();
	WeRenderDevice::Instance()->SetPostProcessEffects(COptionsSettings::Instance->PostProcess);

	//WePostProcess::Instance()->ShutDown();
	//WePostProcess::Instance()->Initialize();
	//WeRenderDevice::Instance()->SetAddedPostProcessEffects(tmp->isSelected());

	COptionsSettings::Instance->Save();

	UpdateOptions();

	if (COptionsPhase::Instance->FromInGame)
		WePostProcess::Instance()->UpdateEffects();

	return true;
}



bool COptionsVideoPhase::OnMotionBlurChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_MotionBlurCheck"));

	COptionsSettings::Instance->MotionBlur = tmp->isSelected();
	WeRenderDevice::Instance()->SetPixelMotionBlur(COptionsSettings::Instance->MotionBlur);

	COptionsSettings::Instance->Save();

	if (COptionsPhase::Instance->FromInGame)
		WePostProcess::Instance()->UpdateEffects();

	return false;
}



bool COptionsVideoPhase::OnGlowChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_GlowCheck"));

	COptionsSettings::Instance->Glow = tmp->isSelected();
	WeRenderDevice::Instance()->SetGlow(COptionsSettings::Instance->Glow);

	COptionsSettings::Instance->Save();

	if (COptionsPhase::Instance->FromInGame)
		WePostProcess::Instance()->UpdateEffects();


	return true;
}



bool COptionsVideoPhase::OnRefractionChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_RefractionCheck"));

	COptionsSettings::Instance->Refraction = tmp->isSelected();
	WeRenderDevice::Instance()->SetRefractive(COptionsSettings::Instance->Refraction);

	COptionsSettings::Instance->Save();

	if (COptionsPhase::Instance->FromInGame)
		WePostProcess::Instance()->UpdateEffects();

	return false;
}


bool COptionsVideoPhase::OnShadowsChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_ShadowsCheck"));

	COptionsSettings::Instance->Shadows = tmp->isSelected();
	WeRenderDevice::Instance()->SetShadows(COptionsSettings::Instance->Shadows);

	COptionsSettings::Instance->Save();

	if (COptionsPhase::Instance->FromInGame)
		WePostProcess::Instance()->UpdateEffects();

	return false;
}




bool COptionsVideoPhase::OnParticlesChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_ParticleCheck"));

	COptionsSettings::Instance->Particles = tmp->isSelected();
	WeRenderDevice::Instance()->SetRenderParticles(tmp->isSelected());

	COptionsSettings::Instance->Save();

	return true;
}




//bool COptionsVideoPhase::OnAntiAliasingChanged(const CEGUI::EventArgs& e)
//{
//	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
//
//	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_AntiAliasingCheck"));
//	
//	COptionsSettings::Instance->AntiAliasing = tmp->isSelected();
//
//	//SomethingChanged = true;
//	COptionsSettings::Instance->Save();
//
//	return true;
//}



bool COptionsVideoPhase::OnProjektorsChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_ProjectorsCheck"));
	
	COptionsSettings::Instance->Projektors = tmp->isSelected();
	WeRenderDevice::Instance()->SetProjectors(COptionsSettings::Instance->Projektors);

	COptionsSettings::Instance->Save();

	return false;
}




bool COptionsVideoPhase::OnFlaresChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_FlaresCheck"));
	
	COptionsSettings::Instance->Flares = tmp->isSelected();
	WeRenderDevice::Instance()->SetFlares(COptionsSettings::Instance->Flares);

	COptionsSettings::Instance->Save();

	return false;
}




bool COptionsVideoPhase::OnSpecularChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_SpecularCheck"));
	
	COptionsSettings::Instance->Specular = tmp->isSelected();
	WeRenderDevice::Instance()->SetSpecular(COptionsSettings::Instance->Specular);

	COptionsSettings::Instance->Save();

	return false;
}


bool COptionsVideoPhase::OnBumpingChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* tmp = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("VideoOptions_BumpingCheck"));
	
	COptionsSettings::Instance->Bumping = tmp->isSelected();
	WeRenderDevice::Instance()->SetBump(COptionsSettings::Instance->Bumping);

	COptionsSettings::Instance->Save();

	return false;
}


bool COptionsVideoPhase::OnAntialiasingChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_AntialiasingCombo")->getText().c_str();

	if(Res == CLocalizationSettings::Instance()->GetLocalText("OFF"))
	{
		COptionsSettings::Instance->Antialiasing = 0;
	}
	else if(Res == (String)"2X")
	{
		COptionsSettings::Instance->Antialiasing = 2;
	}
	else if(Res == (String)"4X")
	{
		COptionsSettings::Instance->Antialiasing = 4;
	}
	else if(Res == (String)"8X")
	{
		COptionsSettings::Instance->Antialiasing = 8;
	}
	else if(Res == (String)"16X")
	{
		COptionsSettings::Instance->Antialiasing = 16;
	}
	
	//WeRenderDevice::Instance()->SetAntialiasFactor(COptionsSettings::Instance->Antialiasing);
	CMainMenuPhase::Instance->SomethingChanged = true;
	CMainMenuPhase::Instance->MustRestartText->setVisible(true);

	COptionsSettings::Instance->Save();

	return true;
}


bool COptionsVideoPhase::OnAnisotropicChanged(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String Res = winMgr.getWindow("VideoOptions_AnisotropicCombo")->getText().c_str();

	if(Res == CLocalizationSettings::Instance()->GetLocalText("OFF"))
	{
		COptionsSettings::Instance->Anisotropic = 0;
	}
	else if(Res == (String)"2X")
	{
		COptionsSettings::Instance->Anisotropic = 2;
	}
	else if(Res == (String)"4X")
	{
		COptionsSettings::Instance->Anisotropic = 4;
	}
	else if(Res == (String)"8X")
	{
		COptionsSettings::Instance->Anisotropic = 8;
	}
	else if(Res == (String)"16X")
	{
		COptionsSettings::Instance->Anisotropic = 16;
	}
	
	if (COptionsSettings::Instance->Anisotropic == 0)
	{
		WeRenderDevice::Instance()->SetAnisotropic(false);
		WeRenderDevice::Instance()->SetAnisotropicFactor(0);
	} else
	{
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(COptionsSettings::Instance->Anisotropic);
	}

	COptionsSettings::Instance->Save();

	return true;
}



void COptionsVideoPhase::UpdateOptions()
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	if (COptionsSettings::Instance->PostProcess)
	{
		CEGUI::Window* wnd;

		wnd = winMgr.getWindow("VideoOptions_MotionBlurTitle");
		wnd->setAlpha(1.0f);

		wnd = winMgr.getWindow("VideoOptions_MotionBlurCheck");
		wnd->setAlpha(1.0f);
		wnd->setEnabled(true);

		wnd = winMgr.getWindow("VideoOptions_GlowTitle");
		wnd->setAlpha(1.0f);

		wnd = winMgr.getWindow("VideoOptions_GlowCheck");
		wnd->setAlpha(1.0f);
		wnd->setEnabled(true);

		wnd = winMgr.getWindow("VideoOptions_RefractionTitle");
		wnd->setAlpha(1.0f);

		wnd = winMgr.getWindow("VideoOptions_RefractionCheck");
		wnd->setAlpha(1.0f);
		wnd->setEnabled(true);
	}
	else
	{
		CEGUI::Window* wnd;

		wnd = winMgr.getWindow("VideoOptions_MotionBlurTitle");
		wnd->setAlpha(0.5f);

		wnd = winMgr.getWindow("VideoOptions_MotionBlurCheck");
		wnd->setAlpha(0.5f);
		wnd->setEnabled(false);

		wnd = winMgr.getWindow("VideoOptions_GlowTitle");
		wnd->setAlpha(0.5f);

		wnd = winMgr.getWindow("VideoOptions_GlowCheck");
		wnd->setAlpha(0.5f);
		wnd->setEnabled(false);

		wnd = winMgr.getWindow("VideoOptions_RefractionTitle");
		wnd->setAlpha(0.5f);

		wnd = winMgr.getWindow("VideoOptions_RefractionCheck");
		wnd->setAlpha(0.5f);
		wnd->setEnabled(false);
	}
}
