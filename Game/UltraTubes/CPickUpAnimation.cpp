/*
       File: CPickUpAnimation.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CPickUpAnimation.h>
#include <CMainGameInterface.h>
#include <CUberText.h>
#include <WeTextureManager.h>


CSmartList<CPickUpAnimation*> CPickUpAnimation::PickupAnimations;


CPickUpAnimation::CPickUpAnimation()
{
	AnimatingMine = false;
	AnimatingWeapon = false;

	UberText = "";
	UberTextColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
}



void CPickUpAnimation::Tick()
{
}


void CPickUpAnimation::Render()
{
}


void CPickUpAnimation::OutputUberText()
{
	CUberText *t = NULL;
	t = new CUberText(UberText);
	t->SetTextColor(UberTextColor);
	CMainGameInterface::Instance->UberLines.PushBack(t);
}

WeTexture* CPickUpAnimation::GetAnimTexture(WEAPONTYPE ID)
{
	String TexName = "";

	switch (ID)
	{
	case WEAPONTYPE_STANDARDLASER:
		TexName = "PickUps.Textures.StandardLaserAnim";
		break;
	case WEAPONTYPE_DOUBLELASER:
		TexName = "PickUps.Textures.DoubleLaserAnim";
		break;
	case WEAPONTYPE_QUADLASER:
		TexName = "PickUps.Textures.QuadLaserAnim";
		break;
	case WEAPONTYPE_STANDARDROCKET:
		TexName = "PickUps.Textures.StandardRocketAnim";
		break;
	case WEAPONTYPE_NUKEROCKET:
		TexName = "PickUps.Textures.NukeRocketAnim";
		break;
	case WEAPONTYPE_QUANTUMLASER:
		TexName = "PickUps.Textures.QuantumLaserAnim";
		break;
	case WEAPONTYPE_MINIROCKETS:
		TexName = "PickUps.Textures.MiniRocketsAnim";
		break;
	case WEAPONTYPE_SLOWINGROCKET:
		TexName = "PickUps.Textures.SlowingRocketAnim";
		break;
	case WEAPONTYPE_TRANSLOCATOR:
		TexName = "PickUps.Textures.TranslocatorAnim";
		break;
	case WEAPONTYPE_MINIGUN:
		TexName = "PickUps.Textures.MinigunAnim";
		break;
	case WEAPONTYPE_STANDARDMINE:
		TexName = "PickUps.Textures.StandardMineAnim";
		break;
	case WEAPONTYPE_SMOKEMINE:
		TexName = "PickUps.Textures.BlurMineAnim";
		//TexName = "PickUps.Textures.SmokeMineAnim";
		break;
	case WEAPONTYPE_PROXIMITYMINE:
		TexName = "PickUps.Textures.ProximityMineAnim";
		break;
	case WEAPONTYPE_SLOWINGMINE:
		TexName = "PickUps.Textures.SlowingMineAnim";
		break;
	case WEAPONTYPE_HOMINGMINE:
		TexName = "PickUps.Textures.HomingMineAnim";
		break;
	case WEAPONTYPE_GRAVITYMINE:
		TexName = "PickUps.Textures.GravityMineAnim";
		break;
	case WEAPONTYPE_SUPERNUKEMINE:
		TexName = "PickUps.Textures.NukeMineAnim";
		break;
	case WEAPONTYPE_BLURMINE:
		TexName = "PickUps.Textures.BlurMineAnim";
		break;
	case WEAPONTYPE_MOVINGMINE:
		TexName = "PickUps.Textures.MovingMineAnim";
		break;
	case WEAPONTYPE_INVISIBLEMINE:
		TexName = "PickUps.Textures.InvisibleMineAnim";
		break;
	case WEAPONTYPE_ELECTRICMINE:
		TexName = "PickUps.Textures.ElectricMineAnim";
		break;
	case WEAPONTYPE_FRONTFIELDGENERATOR:
		TexName = "PickUps.Textures.FrontFieldGeneratorAnim";
		break;
	case WEAPONTYPE_FORCEFIELDMINE:
		TexName = "PickUps.Textures.ForceFieldMineAnim";
		break;
	}

	if (TexName != (String)"")
		return WeTextureManager::Instance()->Load(TexName, true);
	else
		return NULL;
}
