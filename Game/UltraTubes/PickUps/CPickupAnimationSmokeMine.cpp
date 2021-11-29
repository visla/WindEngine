/*
       File: CPickupAnimationSmokeMine.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupAnimationSmokeMine.h>
#include <WeTexture.h>
#include <WeTextureManager.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <CMainGameInterface.h>
#include <CWeaponDescriptionManager.h>
#include <WeTimer.h>
#include <CLocalizationSettings.h>
#include <WeAudioDevice.h>
#include <WeSound.h>



CPickupAnimationSmokeMine::CPickupAnimationSmokeMine()
{
	Mine = GetAnimTexture(WEAPONTYPE_SMOKEMINE);
	PickupAnimations.PushBack(this);
	FirstTime = true;
	AnimatingMine = true;
	AnimatingWeapon = false;

	WOffset = WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	HOffset = WeRenderDevice::Instance()->GetResHeight() / 768.0f;


	MinePictureSX = (int)((1024*WOffset/2) - (1024*WOffset/4));
	MinePictureSY = (int)((768*HOffset/2) - (768*HOffset/4));
	MinePictureSW = (int)(1024*WOffset/2);
	MinePictureSH = (int)(768*HOffset/2);

	MinePictureEX = CMainGameInterface::Instance->MinePictureEX;
	MinePictureEY = CMainGameInterface::Instance->MinePictureEY;
	MinePictureEW = (int)(100*WOffset);
	MinePictureEH = (int)(100*HOffset);

	char uber[100];
	sprintf(uber, "%s (%d)", CLocalizationSettings::Instance()->GetLocalText("SmokeMine").GetString(), CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_SMOKEMINE)->PickUpAmmo);
	UberText = String(uber);
	UberTextColor = COLOR_SECONDARY;
	OutputUberText();

	WeSound *PickupAudio = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.PickupSmokeMine", false);
	PickupAudio->Play(false);
	PickupAudio->DieOnStop();
}




CPickupAnimationSmokeMine::~CPickupAnimationSmokeMine()
{
}




void CPickupAnimationSmokeMine::Tick()
{
	if(FirstTime)
	{
		SX = (float)MinePictureSX;
		SY = (float)MinePictureSY;
		SW = (float)MinePictureSW;
		SH = (float)MinePictureSH;
		SA = 0.0f;
		SAF = 255.0f / 25.0f;

		float tmpTime = WeTimer::Instance()->GetDeltaTimeF() * 100.0f / 0.18f;
		SWF = (MinePictureEW - MinePictureSW)*(tmpTime)/100.0f;
		SHF = (MinePictureEH - MinePictureSH)*(tmpTime)/100.0f;
		MXF = (MinePictureEX - MinePictureSX)*(tmpTime)/100.0f;
		MYF = (MinePictureEY - MinePictureSY)*(tmpTime)/100.0f;

		FirstTime = false;
	}

	if(AnimatingMine)
	{
		SX += MXF;
		SY += MYF;
		SW += SWF;
		SH += SHF;
		SA += SAF;
		if(SA > 255.0f) SA = 255.0f;

		if(SX >= MinePictureEX && SY >= MinePictureEY && SW <= MinePictureEW && SH <= MinePictureEH)
		{
			AnimatingMine = false;
			FirstTime = true;
			CMainGameInterface::Instance->Mine = Mine;
		}
	}
}



void CPickupAnimationSmokeMine::Render()
{
	if(Mine)
	{
		WeCanvas::Instance()->DrawRectangle((int)(SX-30*WOffset), (int)(SY+30*HOffset), (int)SW, (int)SH, D3DCOLOR_RGBA(255,255,255,(int)SA-50), Mine);
		WeCanvas::Instance()->DrawRectangle((int)(SX-15*WOffset), (int)(SY+15*HOffset), (int)SW, (int)SH, D3DCOLOR_RGBA(255,255,255,(int)SA-20), Mine);
		WeCanvas::Instance()->DrawRectangle((int)SX, (int)SY, (int)SW, (int)SH, D3DCOLOR_RGBA(255,255,255,(int)SA), Mine);
	}
}

