/*
		File: CUpgradeItem.h
		Author: Tomislav Kukic
		Date: 06.04.2006
		Version: 1.0
*/




#ifndef _CUpgradeItem_h_
#define _CUpgradeItem_h_


#include <d3d9.h>


class WeTexture;
class WeFont;


#define UPGRADE_ITEMTYPE_SPEED				0
#define UPGRADE_ITEMTYPE_ACCELERATION		1
#define UPGRADE_ITEMTYPE_HP					2
#define UPGRADE_ITEMTYPE_MANOUVERABILITY	3
#define UPGRADE_ITEMTYPE_WEAPON				4
#define UPGRADE_ITEMTYPE_MINE				5
#define UPGRADE_ITEMTYPE_TURBO				6




class CUpgradeItem
{
protected:
	WeTexture* UPGPicture;
	WeTexture* SmallClearedPicture;
	WeTexture* SmallFilledPicture;

	WeFont* Font;

	D3DCOLOR Color;

	D3DCOLOR ClearColor;
	D3DCOLOR FillColor;

	float WResOffset;
	float HResOffset;

public:
	CUpgradeItem(int _Type, int _SubType = 0);
	~CUpgradeItem();

	int x,y,w,h;
	int Type;
	int UpgradeLevel;
	int MaxUpgrades;
	int Cost;


	bool Selected;
    bool Select(int,int);
	void Render();
	bool UpdateUpgradeLevel();
};



#endif

