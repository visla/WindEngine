/*
		File: CUpgradeItem.cpp
		Author: Tomislav Kukic
		Date: 06.04.2006
		Version: 1.0
*/



#include <CUpgradeItem.h>
#include <WeCanvas.h>
#include <WeRenderDevice.h>
#include <WeFont.h>
#include <WeTextureManager.h>
#include <CLocalizationSettings.h>



CUpgradeItem::CUpgradeItem(int _Type, int _SubType)
{
	UPGPicture = NULL;
	Type = _Type;
	UpgradeLevel = 0;
	MaxUpgrades = 5;
	Selected = false;
	x = y = w = h = 0;

	WResOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
	HResOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;

	ClearColor = D3DCOLOR_XRGB(50,50,50);
	FillColor = D3DCOLOR_XRGB(150,150,0);

	SmallClearedPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Empty",true);
	SmallFilledPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Full",true);

	switch(_Type)
	{
	case UPGRADE_ITEMTYPE_SPEED:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Speed",true);
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 400;
		y = 50;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_ACCELERATION:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Accel",true);
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 800;
		y = 50;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_HP:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.HP",true);
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 400;
		y = 220;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_MANOUVERABILITY:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Maner",true);
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 800;
		y = 220;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_WEAPON:
		if(_SubType != 0)
		{
			switch(_SubType)
			{
			case 1:
				UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.StandardLaserAnim",true);
				break;
			case 2:
				UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.DoubleLaserAnim",true);
				break;
			case 3:
				UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.QuadLaserAnim",true);
				break;
			}
		}
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 400;
		y = 400;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_MINE:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.StandardMineAnim",true);
		Color = D3DCOLOR_XRGB(0,255,255);
		x = 600;
		y = 400;
		w=h = 100;
		break;
	case UPGRADE_ITEMTYPE_TURBO:
		UPGPicture = WeTextureManager::Instance()->Load("PickUps.Textures.Turbo",true);
		Color = D3DCOLOR_XRGB(255,255,255);
		x = 800;
		y = 400;
		w=h = 100;
		break;
	}

	int WRes = WeRenderDevice::Instance()->GetResWidth();
	if(WRes == 640) Font = WeCanvas::Instance()->MakeFont("Xirod",12);
	else if(WRes == 800) Font = WeCanvas::Instance()->MakeFont("Xirod",14);
	else if(WRes == 1024) Font = WeCanvas::Instance()->MakeFont("Xirod",16);
	else if(WRes == 1280) Font = WeCanvas::Instance()->MakeFont("Xirod",18);
}




CUpgradeItem::~CUpgradeItem()
{
}




bool CUpgradeItem::Select(int X, int Y)
{
	if(X > (float)x*WResOffset && X < (float)x*WResOffset+(float)w*WResOffset && 
		Y > (float)y*HResOffset && Y < (float)y*HResOffset+(float)h*HResOffset)
	{
		Selected = true;
		return true;
	}
	else
	{
		Selected = false;
		return false;
	}
}





void CUpgradeItem::Render()
{
	// Crtanje glavnog prozorcica sa slikom
	if(!Selected)
		WeCanvas::Instance()->DrawRectangle((int)(x*WResOffset),(int)(y*HResOffset),(int)(w*WResOffset),(int)(h*HResOffset),Color,UPGPicture);
	else
	{
		WeColor tmpColor = Color;
		tmpColor.b -= 0.7f;
		tmpColor.r += 0.3f;
		tmpColor.g += 0.3f;
		WeCanvas::Instance()->DrawRectangle((int)(x*WResOffset),(int)(y*HResOffset),(int)(w*WResOffset),(int)(h*HResOffset),tmpColor,UPGPicture);
	}



	//Crtanje malih prozorcica za upgrade level
	for(int i = 0; i<MaxUpgrades; i++)
	{
		if(i+1 <= UpgradeLevel)
		{
			WeCanvas::Instance()->DrawRectangle((int)(x*WResOffset+(20.0f*WResOffset*i)),(int)(y*HResOffset+h*HResOffset+2.0f),(int)(18*WResOffset),(int)(18*HResOffset),D3DCOLOR_XRGB(255,255,255),SmallFilledPicture);
		}else{
			WeCanvas::Instance()->DrawRectangle((int)(x*WResOffset+(20.0f*WResOffset*i)),(int)(y*HResOffset+h*HResOffset+2.0f),(int)(18*WResOffset),(int)(18*HResOffset),D3DCOLOR_XRGB(255,255,255),SmallClearedPicture);
		}
	}

	switch(Type)
	{
	case UPGRADE_ITEMTYPE_ACCELERATION:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("ACCELERATION"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_HP:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("HP"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_MANOUVERABILITY:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("MANOUVERABILITY"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_MINE:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("MINE"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_SPEED:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("SPEED"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_TURBO:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("TURBO"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	case UPGRADE_ITEMTYPE_WEAPON:
		WeCanvas::Instance()->Draw2DText(Font,CLocalizationSettings::Instance()->GetLocalText("WEAPON"),(int)(x*WResOffset),(int)(y*HResOffset-20.0f*HResOffset),200,30,D3DCOLOR_XRGB(255,255,255));
		break;
	}

	// ispis cene upgrade-a
	char tmp[100];
	sprintf(tmp,"%s: %d",CLocalizationSettings::Instance()->GetLocalText("PRICE").GetString(), Cost);
	if(Selected)
	{
		WeCanvas::Instance()->Draw2DText(Font,tmp,(int)(x*WResOffset),(int)(y*HResOffset+h*HResOffset+25.0f*HResOffset),100,30,D3DCOLOR_XRGB(255,255,255));
	}
}




bool CUpgradeItem::UpdateUpgradeLevel()
{
	UpgradeLevel++;
	if(UpgradeLevel > MaxUpgrades)
	{
		UpgradeLevel = MaxUpgrades;
		return false;
	}
	return true;
}


