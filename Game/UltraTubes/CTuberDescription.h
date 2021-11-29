/*
    CTuberDescription.h

	Autor: Nebojsa Vislavski
	Date: 15.3.2006.
	Version: 1.0

	
  

*/

#ifndef _CTUBERDESCRIPTION_H
#define _CTUBERDESCRIPTION_H

#include <CString.h>

#define TUBER_VERSION 2
class WeStream;

class CTuberDestructionModel
{
public:
	CTuberDestructionModel(): Quantity(0) {} 

	String		Model;
	int			Quantity;
};

class CTuberDescription
{
public:
					CTuberDescription(); //: Version(TUBER_VERSION)
	int				Version; 
	String			Name;
	float			MaxSpeed;
	float			MaxAcceleration;
	float			Manouverability;
	String			WeaponName;
	String			MineName;
	String			Description;
	String			PhysicModel;
	int				HP;
	int				Price;
	String			Model; // model koji se koristi
	CTuberDestructionModel DestructionModel[10];

	int				SpeedUpgradeLevel;
	int				AccelerationUpgradeLevel;
	int				HPUpgradeLevel;
	int				ManouverabilityUpgradeLevel;
	int				WeaponUpgradeLevel;
	int				MineUpgradeLevel;
	int				TurboUpgradeLevel;
	String			TournamentRequired;

	void			Serialize(WeStream &);
	void			Deserialize(WeStream &);
};


#endif