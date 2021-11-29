/*
	Filename:	CPickupAnimationCoin.cpp
	Author:		Goran Bogdanovic
*/


#include <CPickupAnimationCoin.h>
#include <CTuber.h>
#include <CLocalizationSettings.h>


CPickupAnimationCoin::CPickupAnimationCoin(DWORD CoinType, CTuber* ThePicker)
{
	Type = CoinType;
	Owner = ThePicker;

	char uber[100];
	switch (Type)
	{
	case PICKUPTYPE_COINSMALL:
		sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("SmallCoin"));
		break;
	case PICKUPTYPE_COINMEDIUM:
		sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("MediumCoin"));
		break;
	case PICKUPTYPE_COINLARGE:
		sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("LargeCoin"));
		break;
	}
	//UberText = String(uber);
	//UberTextColor = COLOR_COIN;
	//OutputUberText();

	Owner->CurrentFlash = TuberFlashCoin;

	delete this;
}


void CPickupAnimationCoin::Tick()
{
}

void CPickupAnimationCoin::Render()
{
}