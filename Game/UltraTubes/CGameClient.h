/*
	Author  : Vuk Vranjkovic
	File    : CGameClient.h
	Date    : 21.03.2006.
	Version : 1.00
*/
#ifndef _CGameClient_h_
#define _CGameClient_h_

#include <WeClient.h>
#include <NetworkDefines.h>

class CServerInfo;
class CGameSettings;
class CTuberDescription;
class CGamePlayerConfig;

#define DT_NONE					0
#define DT_MULTPLAYER_TABLE		1
#define DT_SERVER				2

class CGameClient
{
public:
	static CGameClient *Instance;

	bool			Initialize();
	bool			ShutDown();

	static bool		GameFinished;

	void			AddAI(int index, int AiType);
	void			JoinGame(CServerInfo *pServer,StringParam password);
	void			CreateGame(CServerInfo *ServerInfo, StringParam password, bool SinglePlayer);
	void			SendNewAI(CGamePlayerConfig *);

	void			SendTableInfo(int index);
	void			SendGameSettings(CGameSettings *settings);
	void			SendTuberDesc(CTuberDescription *);
	void			SendKick(int UID);
	void			ReadyForRace();
	void			StartGame();
	void			SendChangeTeam(int team, int UID);

	void			OnRefuseJoin();
	void			OnJoined();

	void			Disconnect();

	void			FindLANServers();

	void			Tick();
	int				GetStatus() const;

	void			SetAdmin(); // Sets that this client is administrator --> His game is local
	bool			IsAdmin();

	bool			IsRecive() {return bRecieve;}
	void			SetRecieve(bool rec){bRecieve=rec;}
	bool			IsGameCreator() { return CreatingGame; }

	void			ResetRefuse();

	unsigned char	GetDataType() const {return DataType;}
	void			ResetDataType() {DataType = DT_NONE;}
	const int	   *GetData() {return Data;}
	bool			GetSinglePlayer() { return SinglePlayer; }

	String			LastPlayerKicked;
	
	
private:
	CGameClient();

	bool			CreatingGame;
	bool			SinglePlayer;

	bool			bInit;
	bool			bAdmin;
	bool			bRecieve;
	String			Password;
	int				Status;
	bool			RequestedSpawn;

	unsigned char	DataType;
	int				Data[100];

	static void		OutputRaceFinishedMessage(CGamePlayerConfig* Player);

	static void		OnConnected();
	static void		OnDisconnect();
	static void		MsgProc(WeNetworkMessageType Type, WeNetworkMessage *pMsg);
	static void		OnServersRecall(WeNetworkMessage *Msg);
	static WeNetworkActor *OnSpawnNewPlayer(int Type);
	WeClient	   *pClient;
};


#define GC_REFUSED				-1
#define GC_IDLE					0
#define GC_JOINING				1
#define GC_JOINED				2
#define GC_ERROR_CONNECTING		3
#define GC_PLAYING				4
#define GC_WAITING_FOR_PLAYERS	5
#define GC_GAME_STARTED			6
#define GC_LOADING_MAP			7


inline void CGameClient::Tick() {pClient->Tick();}
inline int CGameClient::GetStatus() const {return Status;}
inline void CGameClient::FindLANServers() {pClient->FindLANServers(WESERVER_PORT);}
inline void	CGameClient::SetAdmin(){bAdmin=true;}
inline bool	CGameClient::IsAdmin(){return bAdmin;}
inline void	CGameClient::ResetRefuse() {if(Status==GC_REFUSED) Status=GC_IDLE;}

#endif