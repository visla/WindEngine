/*
    CGamePlayerConfig.cpp

	autor: Vuk Vranjkovic
	Date: 14.3.2006.
	Version: 1.0

	Settings for the game
*/
#ifndef _CGAMEPLAYERCONFIG_H_
#define _CGAMEPLAYERCONFIG_H_

class WeNetworkMessage;
class CProfile;
class WeStream;
class CProfile;

#define PCFG_HUMAN 0
#define PCFG_AI    1
#define PCFG_AIEASY 2
#define PCFG_AIMEDIUM 3
#define PCFG_AIHARD 4
#define PCFG_OPEN  5
#define PCFG_CLOSE 6

class CGamePlayerConfig
{
public:
	// Use this constructor by default
	CGamePlayerConfig(const CProfile &pro, int status, int id);
	CGamePlayerConfig();

	CProfile   *User;
    int			Status;
	int			ID;
	bool		MapLoaded;
	bool		MyProfile;
	bool		GameCreator;
	bool		CanChange;

	void		Serialize(WeStream &);
	void		Deserialize(WeStream &);
	void		NetSerialize(WeNetworkMessage &);
	void		NetDeserialize(WeNetworkMessage &);

	bool		IsAI();
};

#endif