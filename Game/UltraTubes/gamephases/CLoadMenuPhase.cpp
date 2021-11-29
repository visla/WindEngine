/*
    CLoadMenuPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/



#include <CLoadMenuPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CIntroPhase.h>
#include <WeCEGUI.h>
#include <d3d9renderer.h>
#include <WeTimer.h>
#include <WeRenderDevice.h>
#include <COptionsSettings.h>
#include <WeAudioDevice.h>
#include <CMenuPhase.h>
#include <CMap.h>
#include <WePostProcess.h>
#include <CUTWindow.h>
#include <CGUIHelper.h>
#include <CConfig.h>
#include <WeLoger.h>



CLoadMenuPhase *CLoadMenuPhase::Instance = new CLoadMenuPhase;

CLoadMenuPhase::CLoadMenuPhase()
{
	Initialized = false;
	Renderer = NULL;
}



void CreateFont(int size, int ResHeight)
{
	static float scale = GetConfigFloat("cegui.Texts.FontScale", 1.0f);

	char FontName[100];
	sprintf(FontName, "Font_%d", size);
	CEGUI::Font *fnt = CEGUI::FontManager::getSingleton().createFont("FreeType", FontName, "Xirod.ttf");

	char SizeStr[10];
	sprintf(SizeStr, "%d", (int)(size / 768.0f * ResHeight * scale));
	fnt->setProperty("PointSize", SizeStr);
	fnt->load();

	//WeLoger::DebugLog("Font: %d -> %s\n", size, SizeStr);
}


void CLoadMenuPhase::CreateFonts(int ResHeight)
{
	static bool FontsCreated = false;

	if (!FontsCreated)
	{
		int index;
		String FontSizeList, FontSize;

		FontSizeList = GetConfigString("cegui.Init.Fonts", "10");
		index = 0;
		FontSize = FontSizeList.Extract(',', 0);

		while (FontSize.GetLen() != 0)
		{
			int size = 0;
			sscanf(FontSize, "%d", &size);

			if (size > 0)
			{
				CreateFont(size, ResHeight);
			}

			++index;
			FontSize = FontSizeList.Extract(',', index);
		}

		FontsCreated = true;
	}
}




void CLoadMenuPhase::Start()
{
	if (!CUTWindow::CEGUIInitialized)
	{
		Renderer = new CEGUI::DirectX9Renderer(WeRenderDevice::Instance()->Device,1000);
		new CEGUI::System(Renderer);

		CreateFonts(COptionsSettings::Instance->ResHeight);
		CGUIHelper::Initialize(); // postavlja DefaultFont

		CEGUI::SchemeManager::getSingleton().loadScheme("UTLook.scheme");

		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::DefaultWindow* RootWnd = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow", "RootWindow");
		CEGUI::System::getSingleton().setGUISheet(RootWnd);
		CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");

		RootWnd->setMouseCursor("TaharezLook","MouseArrow");

		Initialized = true;

		CMenuPhase::Instance->SoundButton = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Menu.Click1", false);
		CMenuPhase::Instance->SoundButtonAlt = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Menu.Click2", false);
		CMenuPhase::Instance->SoundDenied = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Menu.Denied", false);
		CMenuPhase::Instance->SoundKeyPress = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Menu.KeyPress", false);
		CMenuPhase::Instance->SoundVolumeTest = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Menu.VolumeTest", false);

		CUTWindow::CEGUIInitialized = true;
	}
}

void CLoadMenuPhase::End()
{
	WePostProcess::Instance()->Initialize();
}


void CLoadMenuPhase::Tick()
{
	CGame::Instance->SetPhase(CMenuPhase::Instance);
	//CGame::Instance->SetPhase(CMainGamePhase::Instance);
	//CMainGamePhase::Instance->SetPhase(CStatisticsPhase::Instance);
}



void CLoadMenuPhase::Render()
{
	CIntroPhase::Instance->Render();
}

