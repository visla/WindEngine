/*
	Filename:	CPickupAnimationQuadDamage.cpp
	Author:		Goran Bogdanovic
*/

#include <CPickupAnimationQuadDamage.h>
#include <CLocalizationSettings.h>
#include <WeAudioDevice.h>
#include <WeSound.h>


CPickupAnimationQuadDamage::CPickupAnimationQuadDamage(CTuber* ThePicker)
{
	Owner = ThePicker;

	char uber[100];
	sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("QuadDamage"));
	UberText = String(uber);
	UberTextColor = COLOR_UTILITY;
	OutputUberText();

	WeSound *PickupAudio = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.PickupQuadDamage", false);
	PickupAudio->Play(false);
	PickupAudio->DieOnStop();

	//do sth to owner

	delete this;
}


void CPickupAnimationQuadDamage::Tick()
{
}

void CPickupAnimationQuadDamage::Render()
{
}
