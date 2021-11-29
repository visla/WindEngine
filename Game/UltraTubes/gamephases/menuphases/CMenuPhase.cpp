/*
    CMenuPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 13.3.2006.
	Version: 1.0
    
*/

#include <CMenuPhase.h>
#include <CGame.h>
#include <WeCEGUI.h>
#include <CChooseProfilePhase.h>
#include <WeTimer.h>
#include <WeAudioDevice.h>
#include <WeWorld.h>
#include <WeActor.h>
#include <WeInput.h>
#include <WeRenderDevice.h>
#include <WeSound.h>
#include <CChooseProfilePhase.h>
#include <CShoppingPhase.h>
#include <stdio.h>
#include <COptionsSettings.h>
#include <WePostProcess.h>
#include <CNodeCamera.h>
#include <CLANServerBrowsePhase.h>
#include <CMainGamePhase.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <CGUIHelper.h>
#include <CPlayGamePhase.h>
#include <CMainGameInterface.h>
#include <CProfileManager.h>
#include <CMainMenuPhase.h>



CMenuPhase *CMenuPhase::Instance = new CMenuPhase;


CMenuPhase::CMenuPhase()
{
	ppSB = NULL;
}

void CMenuPhase::Start()
{
	MenuCamera = new CMenuCamera();

	//while(!MenuWorld.IsLoaded())
	//{
	//	MenuWorld.LoadInTick("MenuBackground.mbg");
	//}


	/*WeActor *tmpA;
	tmpA = new WeActor();
	tmpA->ModelName = "Models.temp.ship";	
	tmpA->Location = WeVector(0, 0, 10);
	tmpA->Initialize();

	MenuWorld->AddActor(tmpA);    */
	static bool FirstTimeInit = false;
	if (!FirstTimeInit)
	{
		CProfileManager::Instance()->LoadAllProfiles();

		if (CProfileManager::Instance()->GetCurrentProfile())
		{
			CProfileManager::Instance()->SetProfileControls(CProfileManager::Instance()->GetCurrentProfile());

			CTuberDescription tmpDSC;
			CProfileManager::Instance()->GetCurrentProfile()->TuberDescription = tmpDSC;
			CProfileManager::Instance()->GetCurrentProfile()->TuberBougth = false;

			CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
			CMenuPhase::Instance->Tick();
		}
		else
			SetPhase(CChooseProfilePhase::Instance);

		FirstTimeInit = true;
	}
	if (CMainGamePhase::Instance->GetCurrentSong() != SongMenu)
		CMainGamePhase::Instance->PlaySong(SongMenu);

	/*if(COptionsSettings::Instance->MusicVolume == 0)
	{
		WeAudioDevice::Instance()->SetMusicEnabled(false);
	}*/

	//set ppfx stuff
	BlurFactor = 0;
	WeRenderDevice::Instance()->SetBlurFactor(BlurFactor);
	if(COptionsSettings::Instance->Glow)
	{
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);	
		WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorDownFilter4", 0.25, 0.25, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBrightPass", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true); 
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomH", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBloomV", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBright", 1, 1, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorUpFilter4", 4, 4, true);
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorCombine4", 4, 4, true);	
		
		if (COptionsSettings::Instance->MotionBlur)
			WePostProcess::Instance()->AddEffect("Effects.PostProcess.MotionBlur", 1, 1, true);	
	}
	else
	{
		//WePostProcess::Instance()->AddEffect("Effects.PostProcess.ColorBright", 1, 1, true);
		if (COptionsSettings::Instance->MotionBlur)
			WePostProcess::Instance()->AddEffect("Effects.PostProcess.MotionBlur", 1, 1, true);	
	}
	
	RECT r;
	r.top = 200;
	r.left = 200;
	r.right = 800;
	r.bottom = 600;

	//WeRenderDevice::Instance()->SetScissorRect(&r);



	//--------------
}

void CMenuPhase::End()
{
	CAbstractPhase::End();
	//if (CurrentPhase) CurrentPhase->End(); // to ensure we end some subphase.

	MenuWorld.Unload();

	SAFE_DELETE(MenuCamera);
}

void CMenuPhase::Tick()
{
	if (!CPlayGamePhase::Instance->ShowInGameMenu)
	{
		BlurFactor += WeTimer::Instance()->GetDeltaTimeF()*0.1f;

		if (BlurFactor > 0.75f) BlurFactor = 0.75f;
		WeRenderDevice::Instance()->SetBlurFactor(BlurFactor);

		MenuWorld.Tick();
		MenuCamera->Update();
	}

	//CEGUI::System::getSingleton().injectMouseMove(CGame::Instance->InputMessage.rx, CGame::Instance->InputMessage.ry);	
	CEGUI::System::getSingleton().injectTimePulse(WeTimer::Instance()->GetDeltaTimeF());
	CAbstractPhase::Tick();





	// ==========================================================

	/*DIDEVICEOBJECTDATA devDat;
	DWORD itemCount;

	HRESULT res = WeInput::Instance()->GetKeyboardDevice()->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &devDat, &itemCount, 0);

    if (SUCCEEDED(res))
    {
        if (itemCount > 0)
        {
            if (LOBYTE(devDat.dwData) & 0x80)
            {
               CEGUI::System::getSingleton().injectKeyDown(devDat.dwOfs);
            }
            else
            {
                CEGUI::System::getSingleton().injectKeyUp(devDat.dwOfs);
            }

        }
	}*/
	
	// ==========================================================
}


void CMenuPhase::Render()
{
	CurrentPhase->Render();
}


void CMenuPhase::SaveStates(bool Before)
{	
	if(Before)
	{
		/*WeMatrix I;
		D3DXMatrixIdentity(&I);*/
		if (!ppSB)
			WeRenderDevice::Instance()->Device->CreateStateBlock(D3DSBT_ALL, &ppSB);
		else
			ppSB->Apply();
	}else{
		ppSB->Capture();
	}
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//// setup alpha calculations
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	//WeRenderDevice::Instance()->Device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//WeRenderDevice::Instance()->Device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


}


void CMenuPhase::RenderStdScreen()
{
	SaveStates();

	WeTexture *Tex = WeTextureManager::Instance()->Load("Ingame.Misc.MenuBackground", true);
	if (Tex)
	{
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), Tex);
	}

	if (MenuCamera)
	{
		//WePostProcess::Instance()->BeginScreenCapture();
		//MenuWorld.Render(MenuCamera);
		//WePostProcess::Instance()->EndScreenCapture();
		//WePostProcess::Instance()->PostRender();
	}

	SaveStates(false);
	static WeFont *Font = NULL;
	if (!Font) Font = WeCanvas::Instance()->MakeFont("Arial", 10);

	CMainGameInterface::Instance->DrawShadowedText(GAME_VERSION, Font, 0.9f, 0.9f);
	CEGUI::System::getSingleton().renderGUI();
}


void CMenuPhase::PlayMenuSound(MenuSoundType TypeOfTehSoundToPlayForMenu)
{
	switch (TypeOfTehSoundToPlayForMenu)
	{
	case MenuSoundStdButton:
		if (SoundButton)
		{
			SoundButton->Play(false, true);
		}
		break;

	case MenuSoundAltButton:
		if (SoundButtonAlt)
		{
			SoundButtonAlt->Play(false, true);
		}
		break;

	case MenuSoundDenied:
		if (SoundDenied)
		{
			SoundDenied->Play(false, true);
		}
		break;

	case MenuSoundKeyPress:
		if (SoundKeyPress)
		{
			SoundKeyPress->Play(false, true);
		}
		break;

	case MenuSoundVolumeTest:
		if (SoundVolumeTest)
		{
			SoundVolumeTest->Play(false, true);
		}
		break;
	}
}
