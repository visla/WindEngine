/*
	Author  : Vuk Vranjkovic
	File    : CGameServer.cpp
	Date    : 21.03.2006.
	Version : 1.00
*/
#include <CGameServer.h>
#include <CGameSettings.h>
#include <NetworkDefines.h>
#include <CProfileManager.h>
#include <CProfile.h>
#include <CProfileManager.h>
#include <CGamePlayerConfig.h>
#include <WeServer.h>
#include <CTuber.h>
#include <CTuberFabric.h>
#include <CMainGamePhase.h>
#include <CTuberDescriptionManager.h>
#include <CAIProfileManager.h>
#include <CEditorWindow.h>

CGameServer *CGameServer::Instance = new CGameServer();

CGameServer::CGameServer()
{
	bInit=false;
	Status = GS_IDLE;
}

CGameServer::~CGameServer()
{
	delete pServerInfo;
}

void Connected(WeNetworkClient *)
{

}

void CGameServer::OnClientDisconnect(WeNetworkClient *p)
{	
	WeNetworkMessage Msg;

	// pronadji ko je disconektovan. Ako je to GameCreator onda posalji ostalima da je HOST LEFT
	// inace posalji ostalima stanje tabele
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (p->GetUID() == CGameSettings::ServerInstance->tmpPlayers[i].ID)
		{
			if (CGameSettings::ServerInstance->tmpPlayers[i].GameCreator)
			{				
				CGameSettings::ServerInstance->ClearPlayers();
				Msg.Init();
				Msg.WriteMessageType(HOST_LEFT);                
				WeServer::Instance()->Send(&Msg);				
			} else
			{	
				// diskonektovao se player tokom igre
				if (CEditorWindow::Playing)
				{
					if (CGameSettings::ServerInstance->tmpPlayers[i].User)
					{
						SAFE_DELETE(CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber);
						// reci da kod klijenta ne postoji 
						if (CGameSettings::ClientInstance->tmpPlayers[i].User)
						{
							CGameSettings::ClientInstance->tmpPlayers[i].User->Tuber = NULL;
						}
					}
				}

				// obrisi player-a i javi svima da ga nema vise i da ga izbace
				SAFE_DELETE(CGameSettings::ServerInstance->tmpPlayers[i].User);
				CGameSettings::ServerInstance->tmpPlayers[i].Status = PCFG_OPEN;
				CGameSettings::ServerInstance->tmpPlayers[i].ID = -1;
				CGameSettings::ServerInstance->tmpPlayers[i].GameCreator = false;
				CGameSettings::ServerInstance->tmpPlayers[i].CanChange = false;
				CGameSettings::ServerInstance->tmpPlayers[i].MyProfile = false;
				CGameSettings::ServerInstance->tmpPlayers[i].MapLoaded = false;

				Msg.Init();
				Msg.WriteMessageType(KICK_PLAYER);
				Msg.WriteInt(i);
				WeServer::Instance()->Send(&Msg);				
			}
		}
	}    
}



bool CGameServer::Initialize()
{
	if(bInit) return true;
	pServer = WeServer::Instance();
	if(!pServer->Initialize()) return false;
	pServer->TickInterval = 1;	

	CGameSettings::ServerInstance->Initialize();
	SpawnedAIPlayers = false;

	return (bInit=true);
}

void CGameServer::StartHosting(bool bIsMultiplayer)
{
	if(!bInit) return;

	pServer->OnClientConnected=Connected;
	pServer->OnProcessClientMessage=MsgProc;
	pServer->OnMakeServerInfo=OnRecquestGame;
	pServer->OnClientDisconnected = OnClientDisconnect;
	pServer->OnRequestSpawn = OnRequestSpawn;

	pServerInfo = new CServerInfo;

	pServerInfo->Playing=false;
	pServerInfo->FreeForHosting=false;
	pServerInfo->CurrentPlayers=1;
	pServerInfo->Map = CGameSettings::ClientInstance->MapName;
	pServerInfo->Name = CProfileManager::Instance()->GetCurrentProfile()->Name;
	pServerInfo->IP = "";
	pServer->StartHosting("",WESERVER_PORT,bIsMultiplayer,false);

	Status = GS_HOSTING;	
}

void CGameServer::StopHosting()
{
	if(!bInit) return;
	delete pServerInfo; pServerInfo=NULL;
	pServer->StopHosting();
	Status = GS_IDLE;
}

bool CGameServer::ShutDown()
{
	if(!bInit) return true;

	StopHosting();
	
	pServer->ShutDown();
	Status = GS_IDLE;
	bInit=false;
	SpawnedAIPlayers = false;
	return true;
}

void CGameServer::MsgProc(WeNetworkClient *pClient, WeNetworkClients *pClients, 
						  WeNetworkMessageType Type, WeNetworkMessage *pMsg)
{
	WeNetworkMessage Msg;
	int tmp;
	CSmartList<CGamePlayerConfig> *pPlayerList  = &CGameSettings::Instance->Players;
	String Tmp;
	CGamePlayerConfig *pPlayer = NULL;

	switch(Type)
	{
		case CREATE_GAME :
			if (Instance->Status == GS_HOSTING)
			{
				Instance->SinglePlayer = pMsg->ReadBool();

				// Parse message
				String Name;
				Name = pMsg->ReadString();
				CGameSettings::ServerInstance->Deserialize(*pMsg);				
				
				// ----------- 
				if (CGameSettings::ServerInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
				{
					for (CGameSettings::ServerInstance->MapSettings->AIProfiles.First(); !CGameSettings::ServerInstance->MapSettings->AIProfiles.End(); CGameSettings::ServerInstance->MapSettings->AIProfiles.Next())
					{
						MessageBox(0, CGameSettings::ServerInstance->MapSettings->AIProfiles.GetItem()->Name, 0, 0);
						MessageBox(0, CGameSettings::ServerInstance->MapSettings->AIProfiles.GetItem()->TuberDescription.Model, 0, 0);
					}
				}
				// -----------


				// napravi profil na mestu 0, ako je singleplayer onda uzmi current player
				if (Instance->SinglePlayer)
				{
					CGameSettings::ServerInstance->tmpPlayers[0].User = CProfileManager::Instance()->GetCurrentProfile();
					CGameSettings::ServerInstance->tmpPlayers[0].CanChange = false;
					CGameSettings::ServerInstance->tmpPlayers[0].Status = PCFG_HUMAN;
					CGameSettings::ServerInstance->tmpPlayers[0].GameCreator = true;
					CGameSettings::ServerInstance->tmpPlayers[0].ID = pClient->GetUID();
				} else
				{
					// napravi novi profil i podesi mu parametre
					CProfile *profile;
					profile = new CProfile();
					profile->Name = Name;										
					CGameSettings::ServerInstance->tmpPlayers[0].User = profile;
					CGameSettings::ServerInstance->tmpPlayers[0].CanChange = false;
					CGameSettings::ServerInstance->tmpPlayers[0].Status = PCFG_HUMAN;
					CGameSettings::ServerInstance->tmpPlayers[0].GameCreator = true;
					CGameSettings::ServerInstance->tmpPlayers[0].ID = pClient->GetUID();
				}

				// next send info on game
				Msg.Init();
				Msg.WriteMessageType(JOINED);
				Msg.WriteBool(true);  // game creator
				Msg.WriteBool(CGameSettings::ServerInstance->GetWeapons());
				Msg.WriteBool(CGameSettings::ServerInstance->GetCrates());
				Msg.WriteInt(CGameSettings::ServerInstance->LapCount);
				Msg.WriteInt(CGameSettings::ServerInstance->MapType);
				Msg.WriteInt(CGameSettings::ServerInstance->StartingMoney);
				Msg.WriteString(CGameSettings::ServerInstance->MapName);
				pClient->SendMessage(&Msg);
				Instance->SendPlayersTableToAll(pClients);
			}
			break;

		case JOIN_GAME :
			if (Instance->Status == GS_HOSTING)
			{
				Tmp=pMsg->ReadString();
				if((CGameSettings::ServerInstance->NeedPasswordLogin)&&(CGameSettings::ServerInstance->Password!=Tmp))
				{
					Msg.Init();
					Msg.WriteMessageType(REFUSE_JOIN);
					pClient->SendMessage(&Msg);
				} else 	if (CGameSettings::ServerInstance->GetPlayersFull())
				{
					Msg.Init();
					Msg.WriteMessageType(REFUSE_JOIN);
					pClient->SendMessage(&Msg);
				} else
				{					
					Msg.Init();
					Msg.WriteMessageType(JOINED);
					Msg.WriteBool(false); // nije game creator

					int index; 
					index = CGameSettings::ServerInstance->GetFirstEmptyPlayer();

					CProfile *profile;
					profile = new CProfile();
					profile->Name = pMsg->ReadString();		
								
					SAFE_DELETE(CGameSettings::ServerInstance->tmpPlayers[index].User);
					CGameSettings::ServerInstance->tmpPlayers[index].User = profile;
					CGameSettings::ServerInstance->tmpPlayers[index].CanChange = false;
					CGameSettings::ServerInstance->tmpPlayers[index].Status = PCFG_HUMAN;
					CGameSettings::ServerInstance->tmpPlayers[index].GameCreator = false;
					CGameSettings::ServerInstance->tmpPlayers[index].ID = pClient->GetUID();
					CGameSettings::ServerInstance->tmpPlayers[index].User->TeamNumber = index+1;

					// napravi novi profil i stavi ga u gamesettings
					Msg.WriteBool(CGameSettings::ServerInstance->GetWeapons());
					Msg.WriteBool(CGameSettings::ServerInstance->GetCrates());
					Msg.WriteInt(CGameSettings::ServerInstance->LapCount);
					Msg.WriteInt(CGameSettings::ServerInstance->MapType);
					Msg.WriteInt(CGameSettings::ServerInstance->StartingMoney);
					Msg.WriteString(CGameSettings::ServerInstance->MapName);
					pClient->SendMessage(&Msg);
					Instance->SendPlayersTableToAll(pClients);										
				}				
			}
			break;

		case PLAYER_INFO:
			Instance->pServer->Send(pMsg);
			break;


		case ADD_AI:
			int index, AIType; 
			index = pMsg->ReadInt();
			AIType = pMsg->ReadInt();

			SAFE_DELETE(CGameSettings::ServerInstance->tmpPlayers[index].User);
			CGameSettings::ServerInstance->tmpPlayers[index].User = CAIProfileManager::Instance->GetProfile(CGameSettings::ServerInstance->StartingMoney);
			if (CGameSettings::ServerInstance->tmpPlayers[index].User)
			{
				CGameSettings::ServerInstance->tmpPlayers[index].Status = AIType;
				CGameSettings::ServerInstance->tmpPlayers[index].ID = -1;
				CGameSettings::ServerInstance->tmpPlayers[index].User->TeamNumber = index+1;
			} else 
			{
				CGameSettings::ServerInstance->tmpPlayers[index].Status = PCFG_OPEN;
				CGameSettings::ServerInstance->tmpPlayers[index].ID = -1;				
			}

			CGameSettings::ServerInstance->tmpPlayers[index].CanChange = false;			
			Instance->SendPlayersTableToAll(pClients);
			break;


		case GAME_SETTINGS:
			CGameSettings::ServerInstance->Deserialize(*pMsg);					            
			break;


		case START_GAME:
			if (Instance->Status == GS_HOSTING)
			{
				CGameSettings::ServerInstance->InitProfiles();
				Msg.Init();
				Msg.WriteMessageType(GAME_STARTED);							
				WeServer::Instance()->Send(&Msg);
				Instance->Status = GS_GAME_STARTED;
			}
			break;

		case TUBER_DESC:		
			if (Instance->Status == GS_GAME_STARTED || Instance->Status == GS_LOADING_MAP)
			{
				tmp = pClient->GetUID();
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (CGameSettings::ServerInstance->tmpPlayers[i].ID == tmp)
					{
						CGameSettings::ServerInstance->tmpPlayers[i].User->TuberDescription.Deserialize(*pMsg);
					}
				}
				Instance->Status = GS_LOADING_MAP;
			}
			break;

		/*case CHANGE_TEAM:
			tmp = pMsg->ReadInt();
			id = pMsg->ReadInt();
			for(pPlayerList->First(); !pPlayerList->End(); pPlayerList->Next())
			{
				if (pPlayerList->GetItem().ID == id)
				{
					pPlayerList->GetItem().User->TeamNumber = tmp;
					break;
				}
			}
			Msg.Init();
			Msg.WriteMessageType(TEAM_CHANGED);
			Msg.WriteInt(tmp);
			Msg.WriteInt(id);
			Instance->pServer->Send(&Msg);
			break;

		case KICK_PLAYER:
			id = pMsg->ReadInt();
			CGameSettings::Instance->RemovePlayer(id);
			Instance->pServer->Send(pMsg);
			break;*/


		// Dobija se na CLOSE i na OPEN kao i na promenama tima
		case TABLE_INFO:
			if (Instance->Status == GS_HOSTING)
			{
				int index, status, prevstatus;
				index = pMsg->ReadInt();				
				status = pMsg->ReadInt();

				prevstatus = CGameSettings::ServerInstance->tmpPlayers[index].Status;
				CGameSettings::ServerInstance->tmpPlayers[index].Status = status;
				if (status == PCFG_OPEN || status == PCFG_CLOSE)
				{
					SAFE_DELETE(CGameSettings::ServerInstance->tmpPlayers[index].User);
					CGameSettings::ServerInstance->tmpPlayers[index].ID = -1;
					CGameSettings::ServerInstance->tmpPlayers[index].GameCreator = false;
					CGameSettings::ServerInstance->tmpPlayers[index].CanChange = false;									
				} else
				{
					CGameSettings::ServerInstance->tmpPlayers[index].User->TeamNumber = pMsg->ReadInt();
				}

				Instance->SendPlayersTableToAll(pClients);
			}
			break;

	}
}



void CGameServer::SendPlayersTableToAll(WeNetworkClients *AllClients)
{
	WeNetworkMessage Msg;
	for (AllClients->First(); !AllClients->End(); AllClients->Next())
	{
		// napravi tabelu sa parametrima za njega CanChange
		Msg.Init();
		Msg.WriteMessageType(PLAYER_INFO);
		
		for (int i = 0; i < MAX_PLAYERS; i++)
		{	
			Msg.WriteInt(CGameSettings::ServerInstance->tmpPlayers[i].ID);
			if (AllClients->GetItem()->GetUID() == CGameSettings::ServerInstance->tmpPlayers[i].ID)
			{
				// ovo je player koji je vlasnik ovog reda, ne moze da se menja
				Msg.WriteBool(false);
			} else
			{
				Msg.WriteBool(true);
			}

			Msg.WriteInt(CGameSettings::ServerInstance->tmpPlayers[i].Status);
			// ako je moj slot onda posalji MyProfile
			if (AllClients->GetItem()->GetUID() == CGameSettings::ServerInstance->tmpPlayers[i].ID) 
			{				
				Msg.WriteBool(true);
			}
			else Msg.WriteBool(false);

			if (CGameSettings::ServerInstance->tmpPlayers[i].User)
			{				
				Msg.WriteString(CGameSettings::ServerInstance->tmpPlayers[i].User->Name);			
				Msg.WriteInt(CGameSettings::ServerInstance->tmpPlayers[i].User->TeamNumber);			
			}
		}
		AllClients->GetItem()->SendMessage(&Msg);
	}

}


void CGameServer::UnknownMsgProc(WeNetworkMessageType type, WeNetworkMessage *pMsg, 
								 StringParam FromIP, unsigned short FromPort)
{
	WeNetworkMessage Msg;
	Msg.Init();
}

void CGameServer::OnRecquestGame(WeNetworkMessage *pMsg)
{
	if(Instance->Status != GS_HOSTING || Instance->SinglePlayer)
	{
		pMsg->WriteBool(false);
	}else	
	{
		pMsg->WriteBool(true);
		pMsg->WriteString(Instance->pServerInfo->Name);
		pMsg->WriteString(Instance->pServerInfo->Map);
	}
}

void CGameServer::SendPlayersInfo(WeNetworkClient *pClient, WeNetworkClients *pClients, CGamePlayerConfig *pNew)
{




	WeNetworkMessage Msg;
	CSmartList<CGamePlayerConfig> *pPlayers = &CGameSettings::Instance->Players;

	for(pClients->First(); !pClients->End(); pClients->Next())
	{
		Msg.Init();
		Msg.WriteMessageType(PLAYER_INFO);
		Msg.WriteBool( pNew->ID == pClients->GetItem()->GetUID() ); // is this owner of profile
		pNew->NetSerialize(Msg);
		pClients->GetItem()->SendMessage(&Msg);
	}


	for(CGameSettings::Instance->Players.First(); !CGameSettings::Instance->Players.End(); CGameSettings::Instance->Players.Next())
	{
		if(CGameSettings::Instance->Players.GetItem().ID != pNew->ID )
		{
			Msg.Init();
			Msg.WriteMessageType(PLAYER_INFO);
			Msg.WriteBool(false);
			CGameSettings::Instance->Players.GetItem().NetSerialize(Msg);
			pClient->SendMessage(&Msg);
		}
	}
}

bool CGameServer::CheckAllLoadedMap() const
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User)
		{
			if (!CGameSettings::ServerInstance->tmpPlayers[i].MapLoaded) return false;
		}
	}
	return true;
}

WeNetworkActor *CGameServer::OnRequestSpawn(int Type, WeNetworkClient *from)
{
 	CGamePlayerConfig *Player;
	// nema spawnging-a dok mapa nije ucitana
	if (!CMainGamePhase::Instance->Map.IsLoaded()) return NULL;


	Instance->SpawnAIPlayers(); // odmah cim se prvi spawnuje stvori ostale

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if(CGameSettings::ServerInstance->tmpPlayers[i].ID == from->GetUID()) 
		{
			Player = &CGameSettings::ServerInstance->tmpPlayers[i];
			CGameSettings::ServerInstance->tmpPlayers[i].MapLoaded = true;
		}
	}

	if(Instance->CheckAllLoadedMap())
	{
		WeNetworkMessage Msg;
		Msg.Init();
		Msg.WriteMessageType(START_RACE);		
		WeServer::Instance()->Send(&Msg);
		Instance->Status = GS_GAME_PLAYING;
	}

  
	CTuber *tuber;

	// napravi tuber u odnosu na podatke iz profila koji je trazio spawn. Postavi ga na nekoj startnoj lokaciji mape
	tuber = CTuberFabric::Instance->CreateTuber(Player->User->TuberDescription, Player->User);
	//char tmp[200];
	//sprintf(tmp, "Server: napravljen tuber za %s", Player->User->Name);
	//MessageBox(0, tmp, 0, 0);


	// Nadji startnu lokaciju iz mape i tamo ga spawnuj
	bool LocationFound = false;
	WeVector Location;
	WeMatrix TM;
	Location = WeVector(0,0,0);

	for (CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
	{
		// ako je za moj tim i prazna je.
		if (Player->User->TeamNumber == CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TeamNumber &&
			CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber == NULL)
		{
			LocationFound = true;
			Location = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Location;			

			TM = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TM;
			CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber = tuber;			
			break;
		}
	}


	if (!LocationFound)
	{
		// uzmi randomnu lokaciju
		for (CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
		{
			if (CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber == NULL)
			{
				LocationFound = true;
				Location = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Location;				
				TM = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TM;				
				CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber = tuber;			
				break;				
			}
		}
	}
	
	tuber->Location = Location; //WeVector(0, 0, 10);
	tuber->TM = TM;
	CMainGamePhase::Instance->Map.AddActor(tuber);
	tuber->Initialize();	


	return tuber;
}










void CGameServer::SpawnAIPlayers()
{
	if (SpawnedAIPlayers) return;


 	CGamePlayerConfig	*AIPlayer;
	CTuber				*tuber;
	
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User)
		{
			if (CGameSettings::ServerInstance->tmpPlayers[i].Status == PCFG_AIEASY ||
				CGameSettings::ServerInstance->tmpPlayers[i].Status == PCFG_AIMEDIUM ||
				CGameSettings::ServerInstance->tmpPlayers[i].Status == PCFG_AIHARD)
			{
				// spawnuj player-a
				AIPlayer = &CGameSettings::ServerInstance->tmpPlayers[i];
				CGameSettings::ServerInstance->tmpPlayers[i].MapLoaded = true;
		  
				// napravi tuber u odnosu na podatke iz profila koji je trazio spawn. Postavi ga na nekoj startnoj lokaciji mape
				tuber = CTuberFabric::Instance->CreateTuber(AIPlayer->User->TuberDescription, AIPlayer->User);

				// Nadji startnu lokaciju iz mape i tamo ga spawnuj
				bool LocationFound = false;
				WeVector Location;
				Location = WeVector(0,0,0);
				WeMatrix TM;

				for (CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
				{
					// ako je za moj tim i prazna je.
					if (AIPlayer->User->TeamNumber == CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TeamNumber &&
						CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber == NULL)
					{
						LocationFound = true;
						Location = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Location;
						TM = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TM;
						CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber = tuber;
						break;
					}
				}

				if (!LocationFound)
				{
					// uzmi randomnu lokaciju
					for (CMainGamePhase::Instance->Map.MapHeader.StartLocations.First(); !CMainGamePhase::Instance->Map.MapHeader.StartLocations.End(); CMainGamePhase::Instance->Map.MapHeader.StartLocations.Next())
					{
						if (CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber == NULL)
						{
							LocationFound = true;
							Location = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Location;
							TM = CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->TM;
							CMainGamePhase::Instance->Map.MapHeader.StartLocations.GetItem()->Tuber = tuber;
							break;
						}
					}
				}
			    
				tuber->Location = Location;
				tuber->TM = TM;
				CMainGamePhase::Instance->Map.AddActor(tuber);
				tuber->Initialize();	
			}
		}
	}
	SpawnedAIPlayers = true;
}