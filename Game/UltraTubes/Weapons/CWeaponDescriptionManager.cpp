/*
     CWEaponDescriptionManager.cpp

	 Autor: Nebojsa Vislavski
	 Date: 11.4.2006.
	 Version: 1.0
*/


#include <CWeaponDescriptionManager.h>


CWeaponDescriptionManager *CWeaponDescriptionManager::Instance = new CWeaponDescriptionManager;


CWeaponDescriptionManager::CWeaponDescriptionManager()
{
}

CWeaponDescriptionManager::~CWeaponDescriptionManager()
{
	ClearDescriptions();
}

void CWeaponDescriptionManager::LoadDescriptions()
{
	if (Descriptions.GetItemsCount() > 0) return;

	LoadHardcoded();
	LoadFromFile();
}

void CWeaponDescriptionManager::LoadHardcoded()
{
	CWeaponDescription *d = NULL;

	d = new CWeaponDescription();
	d->Name = "Standard Bolt";
	d->FireRate = 10.0f;
	d->PickUpAmmo = 100;
	d->AmmoPerShot = 1;
	d->Damage = 30.0f;
	d->Speed = 700.0f;
	d->HomingLockTime = 0.0f;
	d->Type = WEAPONTYPE_STANDARDLASER;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Quantum Bolt";
	d->FireRate = 1.0f;
	d->PickUpAmmo = 10;
	d->AmmoPerShot = 1;
	d->Damage = 500.0f;
	d->Speed = 700.0f;
	d->HomingLockTime = 0.0f;
	d->Type = WEAPONTYPE_QUANTUMLASER;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Standard Rocket";
	d->FireRate = 1.0f;
	d->PickUpAmmo = 20;
	d->AmmoPerShot = 1;
	d->Damage = 150.0f;
	d->Speed = 400.0f;
	d->HomingLockTime = 0.5f;
	d->Type = WEAPONTYPE_STANDARDROCKET;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Slowing Rocket";
	d->FireRate = 1.0f;
	d->PickUpAmmo = 10;
	d->AmmoPerShot = 1;
	d->Damage = 20.0f;
	d->Speed = 450.0f;
	d->HomingLockTime = 1.0f;
	d->Type = WEAPONTYPE_SLOWINGROCKET;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Translocator";
	d->FireRate = 1.0f;
	d->PickUpAmmo = 5;
	d->AmmoPerShot = 1;
	d->Damage = 0.0f;
	d->Speed = 450.0f;
	d->HomingLockTime = 1.5f;
	d->Type = WEAPONTYPE_TRANSLOCATOR;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Smoking Mine";
	d->FireRate = 0.3f;
	d->PickUpAmmo = 2;
	d->AmmoPerShot = 1;
	d->Damage = 0.0f;
	d->Speed = 0.0f;
	d->HomingLockTime = 0.0f;
	d->Type = WEAPONTYPE_SMOKEMINE;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Blur Mine";
	d->FireRate = 0.3f;
	d->PickUpAmmo = 2;
	d->AmmoPerShot = 1;
	d->Damage = 0.0f;
	d->Speed = 0.0f;
	d->HomingLockTime = 0.0f;
	d->Type = WEAPONTYPE_BLURMINE;
	Descriptions.PushBack(d);

	d = new CWeaponDescription();
	d->Name = "Homing Mine";
	d->FireRate = 0.5f;
	d->PickUpAmmo = 2;
	d->AmmoPerShot = 1;
	d->Damage = 150.0f;
	d->Speed = 200.0f;
	d->HomingLockTime = 0.0f;
	d->Type = WEAPONTYPE_HOMINGMINE;
	Descriptions.PushBack(d);
}

void CWeaponDescriptionManager::LoadFromFile()
{
	FILE *f;
	f = fopen("weapons.txt", "rt+");
	if (!f) return;
	CWeaponDescription *d = NULL;

	while (!feof(f))
	{
		d = new CWeaponDescription();
		d->Deserialize(f);

		bool ItemExists = false;
		for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
		{
			if (Descriptions.GetItem()->Type == d->Type)
			{
				ItemExists = true;
				break;
			}
		}
		if (!ItemExists)
		{
			Descriptions.PushBack(d);
		}

		fscanf(f, "\n");
	}

	fclose(f);
}


WEAPONTYPE CWeaponDescriptionManager::GetWeaponID(StringParam Name)
{
	LoadDescriptions();

	for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
	{
		if (Descriptions.GetItem()->Name == Name)
		{
			return Descriptions.GetItem()->Type;
		}
	}
	return WEAPONTYPE_NONE;
}

CWeaponDescription* CWeaponDescriptionManager::GetWeaponDescription(WEAPONTYPE ID)
{
	LoadDescriptions();

	for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
	{
		if (Descriptions.GetItem()->Type == ID)
			return Descriptions.GetItem();
	}
	return NULL;
}

void CWeaponDescriptionManager::ReloadDescriptions()
{
	ClearDescriptions();
	LoadDescriptions();
}


void CWeaponDescriptionManager::ClearDescriptions()
{
	for (Descriptions.First(); !Descriptions.End(); )
	{
		delete Descriptions.GetItem();
		Descriptions.Delete();
	}

	Descriptions.Empty();
}
