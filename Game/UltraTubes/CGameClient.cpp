/*
	Author  : Vuk Vranjkovic
	File    : CGameClient.cpp
	Date    : 21.03.2006.
	Version : 1.00
*/

#include <CGameClient.h>
#include <CServerInfo.h>
#include <CGameSettings.h>
#include <CProfileManager.h>
#include <CProfile.h>
#include <WeNetworkMessage.h>
#include <WeClient.h>
#include <NetworkDefines.h>
#include <CMenuPhase.h>
#include <CShoppingPhase.h>
#include <CSingleRacePhase.h>
#include <CLANServerBrowsePhase.h>
#include <CTuberDescription.h>
#include <CTuber.h>
#include <CMainGamePhase.h>
#include <CGameServer.h>
#include <CUberText.h>
#include <CMainGameInterface.h>
#include <CLocalizationSettings.h>

CGameClient::CGameClient()
{
	bInit = false;
	bAdmin = false;
	bRecieve = false;
	Status = GC_IDLE;
}

bool CGameClient::GameFinished = false;

bool CGameClient::Initialize()
{
	if(bInit) return true;

	pClient = WeClient::Instance();
	pClient->OnConnected=OnConnected;
	pClient->OnProcessMessage=MsgProc;
	pClient->OnServerInfo=OnServersRecall;
	pClient->OnSpawnNewPlayer = OnSpawnNewPlayer;
	pClient->OnDisconnected = OnDisconnect;
	pClient->Initialize();
	RequestedSpawn = false;
	GameFinished = false;

	CGameSettings::ClientInstance->Initialize();
	CreatingGame = false;

	return (bInit=true);
}

bool CGameClient::ShutDown()
{
	if(!bInit) return true;

	WeClient::Instance()->Disconnect();
	WeClient::Instance()->ShutDown();
	CreatingGame = false;
	GameFinished = false;
	RequestedSpawn = false;
	bInit=false;
	Status = GC_IDLE;
	return true;
}

void CGameClient::JoinGame(CServerInfo *pServer,StringParam password)
{
	if(pServer) 
	{		
		WeClient::Instance()->ConnectToHost(pServer->IP);
	}
	else WeClient::Instance()->ConnectToHost("");
	SinglePlayer = false;
	bAdmin = false;
	CreatingGame = false;
	Password = password;
}


void CGameClient::CreateGame(CServerInfo *ServerInfo, StringParam password, bool SinglePlayer_)
{
	if(ServerInfo) WeClient::Instance()->ConnectToHost(ServerInfo->IP);
	else WeClient::Instance()->ConnectToHost("");

	SinglePlayer = SinglePlayer_;
	bAdmin=true;
	CreatingGame =	true;
	Password = password;

}


void CGameClient::AddAI(int index, int AIType)
{
	if (!bInit) return;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(ADD_AI);
	Msg.WriteInt(index);
	Msg.WriteInt(AIType);
	WeClient::Instance()->SendMessage(&Msg);
}


void CGameClient::SendGameSettings(CGameSettings *settings)
{
	if (!bInit) return;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(GAME_SETTINGS);
	CGameSettings::ClientInstance->Serialize(Msg);
	WeClient::Instance()->SendMessage(&Msg);	
}




void CGameClient::ReadyForRace()
{
	if(!bInit) return;
	
	// ocemo da se spawnujemo	
	RequestedSpawn = true;
	WeClient::Instance()->RequestSpawn(0);
}

void CGameClient::SendTuberDesc(CTuberDescription *pTD)
{
	if(!bInit) return;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(TUBER_DESC);
	pTD->Serialize(Msg);
	pClient->SendMessage(&Msg);
	Status = GC_WAITING_FOR_PLAYERS;	
}

void CGameClient::OnRefuseJoin()
{

}

void CGameClient::OnJoined()
{

}


void CGameClient::OnDisconnect()
{
    // diskonektovan je ....
	Instance->Status = GC_IDLE;
	CProfileManager::Instance()->RestoreToLoadedProfile();
}


void CGameClient::OnConnected()
{
	WeNetworkMessage Msg;
	Msg.Init();
	if(Instance->CreatingGame)
	{
		Msg.WriteMessageType(CREATE_GAME);
		Msg.WriteBool(Instance->SinglePlayer);
		Msg.WriteString(CProfileManager::Instance()->GetCurrentProfile()->Name);
		CGameSettings::ClientInstance->Serialize(Msg);
	}
	else
	{
		Msg.WriteMessageType(JOIN_GAME);
		Msg.WriteString(Instance->Password);
		Msg.WriteString(CProfileManager::Instance()->GetCurrentProfile()->Name);
	}
	WeClient::Instance()->SendMessage(&Msg);
	Instance->RequestedSpawn = false;
}

void CGameClient::MsgProc(WeNetworkMessageType Type, WeNetworkMessage *pMsg)
{
	CGamePlayerConfig *pPlayer = NULL;
	CSmartList<CGamePlayerConfig> *pPlayerList = &CGameSettings::Instance->Players;
	WeNetworkMessage Msg;
	int id = 0;
	int tmp;
	Instance->bRecieve = true;
	switch(Type)
	{
		case REFUSE_JOIN:
			Instance->Status=GC_REFUSED;
			break;

		case JOINED:
			Instance->Status=GC_JOINED;
			Instance->bAdmin = pMsg->ReadBool(); // ako je kreator igre			
			Instance->CreatingGame = Instance->bAdmin;

			CGameSettings::ClientInstance->SetWeapons( pMsg->ReadBool() );
			CGameSettings::ClientInstance->SetCrates( pMsg->ReadBool() );
			CGameSettings::ClientInstance->LapCount = pMsg->ReadInt();
			CGameSettings::ClientInstance->MapType = (CGameSettings::MAPTYPE)pMsg->ReadInt();
			CGameSettings::ClientInstance->StartingMoney = pMsg->ReadInt();
			CGameSettings::ClientInstance->MapName = pMsg->ReadString();
			break;

		case PLAYER_INFO:
			int i;
			i = 0;
			if (Instance->SinglePlayer)
			{
				// ako je sp onda je sigurno samo jedan profileInfo stigao i to je prvi i to trenutni profil
				CGameSettings::ClientInstance->tmpPlayers[0].ID = pMsg->ReadInt();
				CGameSettings::ClientInstance->tmpPlayers[0].CanChange = pMsg->ReadBool();
				CGameSettings::ClientInstance->tmpPlayers[0].Status = pMsg->ReadInt();
				CGameSettings::ClientInstance->tmpPlayers[0].MyProfile = pMsg->ReadBool();
				CGameSettings::ClientInstance->tmpPlayers[0].User = CProfileManager::Instance()->GetCurrentProfile();				
				CGameSettings::ClientInstance->tmpPlayers[0].User->Name = pMsg->ReadString();
				CGameSettings::ClientInstance->tmpPlayers[0].User->TeamNumber = pMsg->ReadInt();
				i = 1;
			}

			// prikazi tabelu koja treba da ide.
			int prevstatus, status;

			for (; i < MAX_PLAYERS; i++)
			{				
				prevstatus = CGameSettings::ClientInstance->tmpPlayers[i].Status;

				CGameSettings::ClientInstance->tmpPlayers[i].ID = pMsg->ReadInt();
				CGameSettings::ClientInstance->tmpPlayers[i].CanChange = pMsg->ReadBool();
				status = pMsg->ReadInt();
				CGameSettings::ClientInstance->tmpPlayers[i].Status = status;

				// ako je bio igrac a sada vise nije i to je bas moj profil klijent ulazi u IDLE stanje jer
				// nismo vise u tabeli
				if (prevstatus == PCFG_HUMAN && status != PCFG_HUMAN && CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
				{
					Instance->Status = GC_IDLE;
				}

				CGameSettings::ClientInstance->tmpPlayers[i].MyProfile = pMsg->ReadBool();
				if (CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_OPEN && 
					CGameSettings::ClientInstance->tmpPlayers[i].Status != PCFG_CLOSE)
				{
                    CProfile *profile;
					profile = new CProfile;
					profile->Name = pMsg->ReadString();
					SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[i].User);
					CGameSettings::ClientInstance->tmpPlayers[i].User = profile;
					CGameSettings::ClientInstance->tmpPlayers[i].User->TeamNumber = pMsg->ReadInt();
					// ako je to moj brod onda je to moj profil trenutni
					if (CGameSettings::ClientInstance->tmpPlayers[i].MyProfile)
					{
						CProfileManager::Instance()->SetCurrentProfile(profile);						
					}
				}
				if (CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_OPEN ||
					CGameSettings::ClientInstance->tmpPlayers[i].Status == PCFG_CLOSE)
				{
					SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[i].User);
					CGameSettings::ClientInstance->tmpPlayers[i].MyProfile = false;
				}
			}
			break;

		case KICK_PLAYER:
			if(Instance->Status==GC_IDLE) return;

			id = pMsg->ReadInt();

			if (CGameSettings::ClientInstance->tmpPlayers[id].MyProfile)
			{
				Instance->Status = GC_IDLE;
			} else
			{
				if (CGameSettings::ClientInstance->tmpPlayers[id].User)
				{
					SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[id].User->Tuber);
					Instance->LastPlayerKicked = CGameSettings::ClientInstance->tmpPlayers[id].User->Name;
				}

				SAFE_DELETE(CGameSettings::ClientInstance->tmpPlayers[id].User);
				CGameSettings::ClientInstance->tmpPlayers[id].ID = -1;
				CGameSettings::ClientInstance->tmpPlayers[id].CanChange = false;
				CGameSettings::ClientInstance->tmpPlayers[id].Status = PCFG_OPEN;
				CGameSettings::ClientInstance->tmpPlayers[id].GameCreator = false;                
				CGameSettings::ClientInstance->tmpPlayers[id].MyProfile = false;
				CGameSettings::ClientInstance->tmpPlayers[id].MapLoaded = false;
			}
			break;

		case HOST_LEFT:
			if(Instance->Status==GC_IDLE) return;
			Instance->Status = GC_IDLE;
			break;

		case GAME_STARTED:
			if(Instance->Status==GC_IDLE) return;
			CGameSettings::ClientInstance->InitProfiles();
			Instance->Status = GC_GAME_STARTED;
			break;

		case START_RACE:
			if(Instance->Status==GC_IDLE) return;
			Instance->Status = GC_PLAYING;
			break;

		case TEAM_CHANGED:
			if(Instance->Status==GC_IDLE) return;
			tmp = pMsg->ReadInt();
			id = pMsg->ReadInt();
			for(pPlayerList->First(); !pPlayerList->End(); pPlayerList->Next())
			{
				if(pPlayerList->GetItem().ID == id)
				{
					pPlayerList->GetItem().User->TeamNumber = tmp;
					break;
				}
			}
			break;

		case TABLE_INFO:
			if(Instance->Status==GC_IDLE) return;
			Instance->DataType = DT_MULTPLAYER_TABLE;
			for(int i=0;i!=48;i++) Instance->Data[i] = pMsg->ReadInt();
			break;

		case GAMEFINISHED:
			GameFinished = true;
			break;

		case CLIENT_RACEFINISHED:
			int PLID = pMsg->ReadInt();
			bool Win = pMsg->ReadBool();
			CGameSettings::ClientInstance->tmpPlayers[PLID].User->Tuber->RaceFinished = true;
			CGameSettings::ClientInstance->tmpPlayers[PLID].User->Tuber->Win = Win;
			OutputRaceFinishedMessage(&CGameSettings::ClientInstance->tmpPlayers[PLID]);
			break;
	}
}

void CGameClient::OnServersRecall(WeNetworkMessage *pMsg)
{
	char *d_str=0;
	if(pMsg->ReadBool())
	{
		Instance->DataType = DT_SERVER;
		String tmp = pMsg->ReadString();
		d_str = new char[tmp.GetLen()+2];
		strcpy(d_str,tmp.GetString());
		Instance->Data[0] = *((int*)&d_str);
		tmp = pMsg->ReadString();
		d_str = new char[tmp.GetLen()+2];
		strcpy(d_str,tmp.GetString());
		Instance->Data[1] = *((int*)&d_str);
		tmp = (WeNetworkDevice::Instance()->SockaddrToString(&pMsg->FromAddr)).GetString();
		d_str = new char[tmp.GetLen()+2];
		strcpy(d_str,tmp.GetString());
		Instance->Data[2] = *((int*)&d_str);
	}
}

void CGameClient::Disconnect()
{
	WeClient::Instance()->Disconnect();
	Status = GC_IDLE;
}

void CGameClient::StartGame()
{
	if(!bInit) return;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(START_GAME);
	pClient->SendMessage(&Msg);
}





void CGameClient::SendTableInfo(int index)
{
	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(TABLE_INFO);
	Msg.WriteInt(index);
	Msg.WriteInt(CGameSettings::ClientInstance->tmpPlayers[index].Status);
	if (CGameSettings::ClientInstance->tmpPlayers[index].Status != PCFG_OPEN &&
		CGameSettings::ClientInstance->tmpPlayers[index].Status != PCFG_CLOSE)
	{
		Msg.WriteInt(CGameSettings::ClientInstance->tmpPlayers[index].User->TeamNumber);
	}
	WeClient::Instance()->SendMessage(&Msg);
}




void CGameClient::SendChangeTeam(int team, int UID)
{
	if(!bInit) return;

	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(CHANGE_TEAM);
	Msg.WriteInt(team);
	Msg.WriteInt(UID);
	pClient->SendMessage(&Msg);
}

void CGameClient::SendKick(int UID)
{
	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(KICK_PLAYER);
	Msg.WriteInt(UID);
	pClient->SendMessage(&Msg);
}

void CGameClient::SendNewAI(CGamePlayerConfig *pNew)
{
	WeNetworkMessage Msg;
	Msg.Init();
	Msg.WriteMessageType(PLAYER_INFO);
	Msg.WriteBool(false);
	pNew->NetSerialize(Msg);
	pClient->SendMessage(&Msg);
}


WeNetworkActor *CGameClient::OnSpawnNewPlayer(int Type)
{
	if(Instance->RequestedSpawn)
	{
		CGameServer::Instance->SpawnAIPlayers(); // odmah cim se prvi spawnuje stvori AI
		
		CTuber *tmp;
		tmp = new CTuber;
		CMainGamePhase::Instance->Map.AddActor(tmp);
		//for(int i = 0; i < MAX_PLAYERS; i++)
		//{
		//	if(CGameSettings::ClientInstance->tmpPlayers[i].ID)
		//	{				
		//		tmp->OwnerProfile = CGameSettings::ClientInstance->tmpPlayers[i].User;
		//		break;
		//	}
		//}
		return tmp;
	} else return NULL;
}


void CGameClient::OutputRaceFinishedMessage(CGamePlayerConfig* Player)
{
	char uber[100];
	CUberText *Writing = NULL;
	WeColor TextColor = WeColor(1.0f, 1.0f, 1.0f, 1.0f);

	switch(CGameSettings::ClientInstance->MapType)
	{
	case CGameSettings::MAPTYPE_MP_SINGLERACE:
	case CGameSettings::MAPTYPE_SP_INSTANTRACE:
	case CGameSettings::MAPTYPE_SP_SINGLERACE:
		TextColor = WeColor(0.0f, 1.0f, 1.0f, 1.0f);
		switch (Player->User->Tuber->Position)
		{
		case 1:
			sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("FIRST_PLACE"), Player->User->Name);
			break;
		case 2:
			sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("SECOND_PLACE"), Player->User->Name);
			break;
		case 3:
			sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("THIRD_PLACE"), Player->User->Name);
			break;
		default:
			return;
		}
		break;

	case CGameSettings::MAPTYPE_SP_TOURNAMENT:
	case CGameSettings::MAPTYPE_MP_TOURNAMENT:
		return;

	case CGameSettings::MAPTYPE_SP_TIMETRIAL:
		return;

	case CGameSettings::MAPTYPE_SP_LASTONE:
	case CGameSettings::MAPTYPE_MP_LASTONE:
		if (Player->User->Tuber->Win)
		{
			sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("WINNER_MESSAGE"), Player->User->Name);
			TextColor = WeColor(0.0f, 1.0f, 1.0f, 1.0f);
		}
		else
			sprintf(uber, CLocalizationSettings::Instance()->GetLocalText("OUT_MESSAGE"), Player->User->Name);
		break;
	}

	Writing = new CUberText(String(uber));
	Writing->SetTextColor(TextColor);
	CMainGameInterface::Instance->UberLines.PushBack(Writing);
}
