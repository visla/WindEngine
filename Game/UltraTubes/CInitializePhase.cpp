/*
     File: CInitializePhase.h
	 Author: Tomislav Kukic
	 Date: 12.01.2006
	 Version: 1.0
*/



#include <CUTWindow.h>
#include <CInitializePhase.h>
#include <CUTApplication.h>
#include <WeEngineDesc.h>
#include <WeEngine.h>
#include <COptionsSettings.h>
#include <CGame.h>
#include <CWeaponFabric.h>
#include <WeShaderManager.h>
#include <WeTextureManager.h>
#include <WePostProcess.h>
#include <CLocalizationSettings.h>
#include <CConfig.h>
#include <resource.h>
#include <CMainGameInterface.h>


PHASES CInitializePhase::Start()
{
   	bool InitializeInFullScreen = true;

#ifdef _DEBUG
	FILE *f = fopen("ut_windowed.txt", "rt");
	if (f)
	{
		fclose(f);
		InitializeInFullScreen = false;
	}
#endif

	COptionsSettings::Instance->Load();

	if(!CUTWindow::Instance->Create("Ultra Tubes", "Ultra Tubes", 0, IDI_ICON1, 0, false, false, true, InitializeInFullScreen, false, !InitializeInFullScreen))
	{
		return PHASE_DEINITIALIZE; 
	}

	WeEngineDesc d;
	d.FullScreen	= InitializeInFullScreen;
	d.Dedicated		= false;
	d.ResWidth		= COptionsSettings::Instance->ResWidth;
	d.ResHeight		= COptionsSettings::Instance->ResHeight; 
	d.ImmediatePresent = false;
	d.DefaultShaderName = "Effects.Default.Standard";
	d.AutomaticRendering = false;
	d.DistortionShaderName = "Effects.Default.Refraction";
	d.DistortionMaterialName = "Materials.Distortion.Distortion1";
	d.AntialiasFactor = COptionsSettings::Instance->Antialiasing;


	if (!WeEngine::Instance()->Start(d, GetModuleHandle(NULL), CUTWindow::Instance->GetHWND(), CUTWindow::Instance->GetHWND(),"Ultra Tubes",""))
	{
		MessageBox(0, CLocalizationSettings::Instance()->GetLocalText("ERROR_ENGINE_INIT"), 0, 0);
		return PHASE_DEINITIALIZE;
	}

	float AmbientIntensity = 0.0f;
	WeRenderDevice::Instance()->SetAmbientColor(
		WeColor(AmbientIntensity, AmbientIntensity, AmbientIntensity, AmbientIntensity));

	WeAudioDevice::Instance()->SetMusicVolume(COptionsSettings::Instance->MusicVolume);
	WeAudioDevice::Instance()->SetSoundVolume(COptionsSettings::Instance->SoundVolume);
	WeAudioDevice::Instance()->SetAutoLoadFromPackage(false);

	WeRenderDevice::Instance()->SetRenderCallback(CGame::OnRender);
	WeRenderDevice::Instance()->SetClearColor(D3DCOLOR_ARGB(255,20,20,20));

	if (COptionsSettings::Instance->Anisotropic == 0)
	{
		WeRenderDevice::Instance()->SetAnisotropic(false);
		WeRenderDevice::Instance()->SetAnisotropicFactor(0);
	} else
	{
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(COptionsSettings::Instance->Anisotropic);	
	}

	WeRenderDevice::Instance()->SetPostProcessEffects(COptionsSettings::Instance->PostProcess);
	WeRenderDevice::Instance()->SetAddedPostProcessEffects(true);

	if (COptionsSettings::Instance->ShaderQuality == 0) WeShaderManager::Instance()->SetQuality(SHADER_LOW);
	if (COptionsSettings::Instance->ShaderQuality == 1) WeShaderManager::Instance()->SetQuality(SHADER_LOW);
	if (COptionsSettings::Instance->ShaderQuality == 2) WeShaderManager::Instance()->SetQuality(SHADER_MEDIUM);
	if (COptionsSettings::Instance->ShaderQuality == 3) WeShaderManager::Instance()->SetQuality(SHADER_HIGH);

	if (COptionsSettings::Instance->TextureQuality == 0) WeTextureManager::Instance()->SetQuality(TEXTURES_LOW);
	if (COptionsSettings::Instance->TextureQuality == 1) WeTextureManager::Instance()->SetQuality(TEXTURES_MEDIUM);
	if (COptionsSettings::Instance->TextureQuality == 2) WeTextureManager::Instance()->SetQuality(TEXTURES_HIGH);

	WeRenderDevice::Instance()->SetPixelMotionBlur(COptionsSettings::Instance->MotionBlur);
	WeRenderDevice::Instance()->SetGlow(COptionsSettings::Instance->Glow);
	WeRenderDevice::Instance()->SetRefractive(COptionsSettings::Instance->Refraction);
	WeRenderDevice::Instance()->SetShadows(COptionsSettings::Instance->Shadows);
	WeRenderDevice::Instance()->SetProjectors(COptionsSettings::Instance->Projektors);
	//WeRenderDevice::Instance()->SetFlares(COptionsSettings::Instance->Flares);
	WeRenderDevice::Instance()->SetFlares(true);
	WeRenderDevice::Instance()->SetSpecular(COptionsSettings::Instance->Specular);
	WeRenderDevice::Instance()->SetBump(COptionsSettings::Instance->Bumping);
	//WeRenderDevice::Instance()->SetRenderParticles(COptionsSettings::Instance->Particles);	
	WeRenderDevice::Instance()->SetRenderParticles(true);

	WeInput::Instance()->OnKeydown = CGame::OnKeyDown;
	WeInput::Instance()->OnMouseMove = CGame::OnMouseMove;
	WeInput::Instance()->OnMouseButton = CGame::OnMouseButton;
	WeInput::Instance()->OnJoystick = CGame::OnJoystick;

	// clip mouse cursor to one monitor only
	if (d.FullScreen)
	{
		RECT MouseClip;
		MouseClip.left = 0;
		MouseClip.top = 0;
		MouseClip.right = d.ResWidth;
		MouseClip.bottom = d.ResHeight;
		ClipCursor(&MouseClip);
	}

	// just in case, refresh fonts
	AddFontResource("Xirod.ttf");
	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);

	CMainGameInterface::Instance->InitializeFonts();

	CGame::Instance->Start();


	return PHASE_MAINLOOP;
}

