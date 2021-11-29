/*
	Author  : Vuk Vranjkovic
	File    : CGameServer.h
	Date    : 21.03.2006.
	Version : 1.00
*/
#ifndef _CGameServer_h_
#define _CGameServer_h_

#include <WeServer.h>
#include <CServerInfo.h>
#include <WeNetworkClient.h>

#define GS_HOSTING				0
#define GS_IDLE					1
#define GS_LOADING_MAP			2
#define GS_WAITING_FOR_PLAYERS	3
#define GS_GAME_STARTED			4
#define GS_GAME_PLAYING			5

class CProfile;
class CGamePlayerConfig;

class CGameServer
{
public:
	static CGameServer *Instance;
	~CGameServer();

	bool                Initialize();
	void				StartHosting(bool bIsMultiplayer=true);
	void				StopHosting();
	bool                ShutDown();
	void				SpawnAIPlayers();

	void                Tick();

	bool				GetInit() { return bInit; }
	CServerInfo		   &GetInfo();
	int					GetStatus() const;

private:
	CGameServer();
	int					Status;	
	bool				SinglePlayer;
	bool				SpawnedAIPlayers;

	bool				bInit;

	static void			MsgProc(WeNetworkClient *, WeNetworkClients *, 
								WeNetworkMessageType Type, WeNetworkMessage *);
	static void			UnknownMsgProc(WeNetworkMessageType, WeNetworkMessage *, 
									   StringParam FromIP, unsigned short FromPort);
	static void			OnRecquestGame(WeNetworkMessage *);
	static void			OnClientDisconnect(WeNetworkClient *);
	static WeNetworkActor *OnRequestSpawn(int Type, WeNetworkClient *tmp);


	WeServer           *pServer;

	CServerInfo		   *pServerInfo;

	void				SendPlayersInfo(WeNetworkClient *ToClient, WeNetworkClients *AllClients, CGamePlayerConfig *pNew);
	void				SendPlayersTableToAll(WeNetworkClients *AllClients);

	bool				CheckAllLoadedMap() const;
};



inline void CGameServer::Tick() {pServer->Tick();}
inline CServerInfo &CGameServer::GetInfo() {return *pServerInfo;}
inline int CGameServer::GetStatus() const {return Status;}

#endif