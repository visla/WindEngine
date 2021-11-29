/*
    File: CInitializePhase.cpp
	Author: Tomislav Kukic
	Date: 04.11.2005.
	Version: 1.0
*/



#include <CMapEditorWindow.h>
#include <CInitializePhase.h>
#include <WeEngine.h>
#include <WeShaderManager.h>
#include <WePostProcess.h>
#include <WeTimer.h>



PHASES CInitializePhase::Start()
{
	srand( (unsigned)WeTimer::Instance()->GetSystemTime() );

	EditorWindow = new CMapEditorWindow;
	EditorWindow->Create();

	WeEngineDesc d;
	d.FullScreen	= false;
	d.Dedicated		= false;
	d.ResWidth		= 1024;
	d.ResHeight		= 768;
	d.ItemQuering	= false;
	d.ImmediatePresent = true;
	d.DefaultMaterialName = "MapEditor.Materials.DefaultGray";
	d.DefaultShaderName = "Effects.Default.Standard";	
	d.DefaultTextureName = "MapEditor.Textures.Default";
	d.DistortionShaderName = "Effects.Default.Refraction";
	d.DistortionMaterialName = "Materials.Distortion.Distortion1";	
	d.AntialiasFactor = 0;
	d.MaxDynamicLights = 10;

		
	WeEngine::Instance()->Start(d,EditorWindow->GetInstance(),EditorWindow->GetHWND(),EditorWindow->GetHWND(),"Eipix UltraTubes","");
	WeAudioDevice::Instance()->SetAutoLoadFromPackage(false);
	WeInput::Instance()->ShutdownJoystick();
	WeShaderManager::Instance()->SetQuality(SHADER_LOW);

	float AmbientIntensity = 0.0f;
	WeRenderDevice::Instance()->SetAmbientColor(
		WeColor(AmbientIntensity, AmbientIntensity, AmbientIntensity, AmbientIntensity));

	WePostProcess::Instance()->Initialize();

	return PHASE_CREATEDIALOGS;
}
