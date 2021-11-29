/*
       File: CPickupAnimationStandardRocket.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickupAnimationStandardRocket.h>
#include <WeTexture.h>
#include <WeTextureManager.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <CMainGameInterface.h>
#include <CWeaponDescriptionManager.h>
#include <WeTimer.h>
#include <CLocalizationSettings.h>
#include <WeSound.h>
#include <WeAudioDevice.h>


CPickupAnimationStandardRocket::CPickupAnimationStandardRocket()
{
	Weapon = GetAnimTexture(WEAPONTYPE_STANDARDROCKET);
	Mine = NULL;
	PickupAnimations.PushBack(this);
	FirstTime = true;
	AnimatingMine = false;
	AnimatingWeapon = true;

	WOffset = WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	HOffset = WeRenderDevice::Instance()->GetResHeight() / 768.0f;

	WeaponPictureSX = (int)((1024*WOffset/2) - (1024*WOffset/4));
	WeaponPictureSY = (int)((768*WOffset/2) - (768*WOffset/4));
	WeaponPictureSW = (int)(1024*WOffset/2);
	WeaponPictureSH = (int)(768*WOffset/2);

	WeaponPictureEX = CMainGameInterface::Instance->WeaponPictureEX;
	WeaponPictureEY = CMainGameInterface::Instance->WeaponPictureEY;
	WeaponPictureEW = (int)(100*WOffset);
	WeaponPictureEH = (int)(100*HOffset);

	char uber[100];
	sprintf(uber, "%s (%d)", CLocalizationSettings::Instance()->GetLocalText("StandardRocket").GetString(), CWeaponDescriptionManager::Instance->GetWeaponDescription(WEAPONTYPE_STANDARDROCKET)->PickUpAmmo);
	UberText = String(uber);
	UberTextColor = COLOR_PRIMARY;
	OutputUberText();

	WeSound *PickupAudio = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.PickupStandardRocket", false);
	PickupAudio->Play(false);
	PickupAudio->DieOnStop();
}




void CPickupAnimationStandardRocket::Tick()
{
	if(FirstTime)
	{
		WSX = (float)WeaponPictureSX;
		WSY = (float)WeaponPictureSY;
		WSW = (float)WeaponPictureSW;
		WSH = (float)WeaponPictureSH;
		SA = 0.0f;
		SAF = 255.0f / 25.0f;

		float tmpTime = WeTimer::Instance()->GetDeltaTimeF() * 100.0f / 0.18f;
		WSWF = (WeaponPictureEW - WeaponPictureSW) * tmpTime / 100.0f;
		WSHF = (WeaponPictureEH - WeaponPictureSH) * tmpTime / 100.0f;
		MXF = (WeaponPictureEX - WeaponPictureSX) * tmpTime / 100.0f;
		MYF = (WeaponPictureEY - WeaponPictureSY) * tmpTime / 100.0f;

		FirstTime = false;
	}

	if(AnimatingWeapon)
	{
		WSX += MXF;
		WSY += MYF;
		WSW += WSWF;
		WSH += WSHF;
		SA += SAF;
		if(SA > 255.0f) SA = 255.0f;

		if(WSX <= WeaponPictureEX && WSY >= WeaponPictureEY && WSW <= WeaponPictureEW && WSH <= WeaponPictureEH)
		{
			AnimatingWeapon = false;
			FirstTime = true;
		}
	}
}



void CPickupAnimationStandardRocket::Render()
{
	if(Weapon)
	{
		WeCanvas::Instance()->DrawRectangle((int)(WSX+30*WOffset), (int)(WSY+30*HOffset), (int)WSW, (int)WSH, D3DCOLOR_RGBA(255,255,255,(int)SA-50), Weapon);
		WeCanvas::Instance()->DrawRectangle((int)(WSX+15*WOffset), (int)(WSY+15*HOffset), (int)WSW, (int)WSH, D3DCOLOR_RGBA(255,255,255,(int)SA-20), Weapon);
		WeCanvas::Instance()->DrawRectangle((int)WSX, (int)WSY, (int)WSW, (int)WSH, D3DCOLOR_RGBA(255,255,255,(int)SA), Weapon);
	}
}

