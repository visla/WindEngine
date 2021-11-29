/*
	Filename:	CPickupAnimationDoubleDamage.cpp
	Author:		Goran Bogdanovic
*/


#include <CPickupAnimationDoubleDamage.h>
#include <CLocalizationSettings.h>


CPickupAnimationDoubleDamage::CPickupAnimationDoubleDamage(CTuber* ThePicker)
{
	Owner = ThePicker;

	char uber[100];
	sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("DoubleDamage"));
	UberText = String(uber);
	UberTextColor = COLOR_UTILITY;
	OutputUberText();

	//do sth to owner

	delete this;
}


void CPickupAnimationDoubleDamage::Tick()
{
}

void CPickupAnimationDoubleDamage::Render()
{
}