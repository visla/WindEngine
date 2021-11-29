/*
    CTuberDescription.cpp

	Autor: Nebojsa Vislavski
	Date: 15.3.2006.
	Version: 1.0	  

    
*/

#include <CTuberDescription.h>
#include <Constants.h>
#include <WeStream.h>
#include <CLocalizationSettings.h>


CTuberDescription::CTuberDescription()
{
	Version = TUBER_VERSION;
	for (int i = 0; i < 10; i++)
	{
		DestructionModel[i].Quantity = 0;
	}
	Name = CLocalizationSettings::Instance()->GetLocalText("NO SHIP");
	MaxSpeed = 0;//MAX_TUBER_SPEED;
	MaxAcceleration = 0;//MAX_TUBER_ACCELERATION;
	Manouverability = 0;//MAX_TUBER_MANOUVERABILITY;
	HP = 0;//MAX_TUBER_HP;
	Price = 0;
	SpeedUpgradeLevel = 0;
	AccelerationUpgradeLevel = 0;
	ManouverabilityUpgradeLevel = 0;
	WeaponUpgradeLevel = 0;
	MineUpgradeLevel = 0;
	TurboUpgradeLevel = 0;
	HPUpgradeLevel = 0;

}


void CTuberDescription::Serialize(WeStream &S)
{
	S.WriteInt(TUBER_VERSION);
	S.WriteString(Name);
	S.WriteFloat(MaxSpeed);
	S.WriteFloat(MaxAcceleration);
	S.WriteFloat(Manouverability);
	S.WriteInt(HP);
	S.WriteInt(Price);
	S.WriteString(Model); // model koji se koristi
	S.WriteString(PhysicModel);
	S.WriteString(WeaponName);
	S.WriteString(MineName);
	S.WriteString(Description);
	S.WriteString(TournamentRequired);
	for (int i = 0; i < 10; i++)
	{
		S.WriteString(DestructionModel[i].Model);
		S.WriteInt(DestructionModel[i].Quantity);		
	}

	S.WriteInt(SpeedUpgradeLevel);
	S.WriteInt(AccelerationUpgradeLevel);
	S.WriteInt(HPUpgradeLevel);
	S.WriteInt(ManouverabilityUpgradeLevel);
	S.WriteInt(WeaponUpgradeLevel);
	S.WriteInt(MineUpgradeLevel);
	S.WriteInt(TurboUpgradeLevel);
}


void CTuberDescription::Deserialize(WeStream &S)
{	
	int v;
	v = S.ReadInt();
	if (v == 1)
	{
		Name = S.ReadString();
		MaxSpeed = S.ReadFloat();
		MaxAcceleration =  S.ReadFloat();
		Manouverability = S.ReadFloat();
		HP = S.ReadInt();
		Price = S.ReadInt();
		Model = S.ReadString(); // model koji se koristi
		WeaponName =  S.ReadString();
		MineName = S.ReadString();
		Description = S.ReadString();
		TournamentRequired = S.ReadString();
		for (int i = 0; i < 10; i++)
		{
			DestructionModel[i].Model = S.ReadString();
			DestructionModel[i].Quantity = S.ReadInt();
		}

		SpeedUpgradeLevel = S.ReadInt();
		AccelerationUpgradeLevel = S.ReadInt();
		HPUpgradeLevel =  S.ReadInt();
		ManouverabilityUpgradeLevel = S.ReadInt();
		WeaponUpgradeLevel =  S.ReadInt();
		MineUpgradeLevel =  S.ReadInt();
		TurboUpgradeLevel =  S.ReadInt();
	}
	else if (v == 2)
	{
		Name = S.ReadString();
		MaxSpeed = S.ReadFloat();
		MaxAcceleration =  S.ReadFloat();
		Manouverability = S.ReadFloat();
		HP = S.ReadInt();
		Price = S.ReadInt();
		Model = S.ReadString(); // model koji se koristi
		PhysicModel = S.ReadString();
		WeaponName =  S.ReadString();
		MineName = S.ReadString();
		Description = S.ReadString();
		TournamentRequired = S.ReadString();
		for (int i = 0; i < 10; i++)
		{
			DestructionModel[i].Model = S.ReadString();
			DestructionModel[i].Quantity = S.ReadInt();
		}

		SpeedUpgradeLevel = S.ReadInt();
		AccelerationUpgradeLevel = S.ReadInt();
		HPUpgradeLevel =  S.ReadInt();
		ManouverabilityUpgradeLevel = S.ReadInt();
		WeaponUpgradeLevel =  S.ReadInt();
		MineUpgradeLevel =  S.ReadInt();
		TurboUpgradeLevel =  S.ReadInt();
	}

}