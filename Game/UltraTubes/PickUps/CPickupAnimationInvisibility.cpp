/*
	Filename:	CPickupAnimationInvisibility.cpp
	Author:		Goran Bogdanovic
*/

#include <CPickupAnimationInvisibility.h>
#include <CLocalizationSettings.h>
#include <WeAudioDevice.h>
#include <WeSound.h>


CPickupAnimationInvisibility::CPickupAnimationInvisibility(CTuber* ThePicker)
{
	Owner = ThePicker;

	char uber[100];
	sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("Invisibility"));
	UberText = String(uber);
	UberTextColor = COLOR_UTILITY;
	OutputUberText();

	WeSound *PickupAudio = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Voice.PickupInvisibility", false);
	PickupAudio->Play(false);
	PickupAudio->DieOnStop();

	//do sth to owner

	delete this;
}


void CPickupAnimationInvisibility::Tick()
{
}

void CPickupAnimationInvisibility::Render()
{
}
