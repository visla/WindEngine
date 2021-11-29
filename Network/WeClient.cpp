/*
     WeClient.cpp

	 Autor: Nebojsa Vislavski
	 Date: 16.9.2005.
	 Version: 1.0

	 Client for networking
       

*/

#include <WeClient.h>
#include <WeTimer.h>
#include <WeNetworkActor.h>
#include <WeProfiler.h>
#include <WeServer.h>


WeClient *WeClient::_instance = NULL;

WeClient *WeClient::Instance()
{
	if (_instance) return _instance;
	_instance = new WeClient;
	return _instance;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeClient::WeClient()
{
	Started = false;
	Status = WENETSTATE_NONE;
	IncomingSequence = 0;
	OutgoingSequence = 1;
	IncomingAcknowledged = 0;
	DroppedPackets = 0;
	LastMessageTime = 0;
	Ping = 0;
	PingSentTime = 0;
	AvgReadBytesPerSec = 0;
	AvgWriteBytesPerSec = 0;
	ServerPort = 0;
	PingInterval = 2;
	LocalCommunication = false;
	UID = -1;

	// clear callbacks
	OnConnected = NULL;
	OnDisconnected = NULL;
	OnProcessMessage = NULL;
	OnAuthorizationRequest = NULL;
	OnAuthorized = NULL;
	OnNotAuthorized = NULL;
	OnPing = NULL;
	OnServerInfo = NULL;	
	OnSpawnNewPlayer = NULL;
	OnChat = NULL;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeClient::Initialize()
{
	if (Started) return true;

	if (!WeNetworkDevice::Instance()->Initialize())
	{		
		return false;
	}

	Socket = WeNetworkDevice::Instance()->OpenUDPSocket(); // this opens local UDP Socket	
	if (Socket == WESOCK_INVALIDSOCKET) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Client(): Initialized(): Error initalizing UDP socket");
	}

	Status = WENETSTATE_NONE;
	IncomingSequence = 0;
	OutgoingSequence = 1;
	IncomingAcknowledged = 0;
	DroppedPackets = 0;
	LastMessageTime = 0;
	Ping = 0;
	PingSentTime = 0;
	AvgReadBytesPerSec = 0;
	AvgWriteBytesPerSec = 0;
	ServerPort = 0;


	LoadConfig(); // to gather server port
	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Client initalized");

	Started = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeClient::ShutDown()
{
	if (!Started) return;


	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Client shutted down");
	WeNetworkDevice::Instance()->CloseSocket(Socket);	
	Started = false;
	LocalCommunication = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ConnectToHost
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  This will connect to host
//
//////////////////////////////////////////////////////////////////////////
void WeClient::ConnectToHost(String HostName, int port)
{
	if (!Started) return;

	UID = -1;
	MessageQueue.Clear();

	// if our local server is hoster then we should skip networking
	if (WeServer::Instance()->GetStatus() != WESERVER_HOSTING)
	{
		ServerPort = port;	
		DumpBuffer();
		ServerName = HostName;
		memset(&ServerAddress, 0, sizeof(ServerAddress));
		ServerAddress.sin_family = AF_INET;
		ServerAddress.sin_port = htons((u_short)ServerPort);		
		if (!WeNetworkDevice::Instance()->StringToSockaddr(HostName, (sockaddr *)&ServerAddress))
		{
			WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Client(): ConnectToHost(): Cannot reslove host name %s", HostName);
			return;
		}
		LocalCommunication = false;
	} else 
	{
		LocalCommunication = true;
	}
	

	Status = WENETSTATE_CONNECTING;
	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_CONNECT);	
	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Client connecting to host port:%d", ServerPort);
	SendMessage(&Message); // maybe to add reliable sending for connect msgs
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SendMessage(WeNetworkMessage *Msg, bool SendReliable)
{
	if (!Started) return;
	if (Status == WENETSTATE_DISCONNECTED) return; 
	if (Status != WENETSTATE_CONNECTING && Status != WENETSTATE_CONNECTED) return;

	if (Msg->GetOverflow()) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendMessage(): MessageOverflow");
		return;
	}
	
	
	Msg->WriteSequences(OutgoingSequence, IncomingSequence);
	WeProfiler::Instance()->ClientSendBytesCounter += Msg->Size;

	// if we should send it localy
	if (LocalCommunication)
	{
		Msg->Remote = false;
		(*WeServer::Instance()->MessageQueue.GetMessageForWriting()) = *Msg;
		WeServer::Instance()->MessageQueue.NextMessageForWriting();		
	} else 
	{
		WeNetworkDevice::Instance()->SendPacket(Socket, Msg->Data, Msg->Size, (sockaddr *)&ServerAddress);
	}

	Msg->BeginReading();
	WeNetworkMessageType Type = Msg->ReadMessageType();

	// if msg is sequenced
	//IS_SEQUENCED(Type)
	{
		OutgoingSequence++;
	}

}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
//void WeClient::SendMessageTo(WeNetworkMessage *Msg, sockaddr *addr, bool Reliable)
//{
//	if (!Started) return;
//	if (Status == WENETSTATE_DISCONNECTED) return; 
//
//	Msg->WriteSequences(OutgoingSequence, IncomingSequence);
//	WeProfiler::Instance()->ClientSendBytesCounter += Msg->Size;
//
//	if (Msg->GetOverflow()) 
//	{
//		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendMessageTo(): MessageOverflow");
//		return;
//	}
//
//	if (!Reliable)
//	{
//		WeNetworkDevice::Instance()->SendPacket(Socket, Msg->Data, Msg->Size, addr);
//	} else
//	{		
//	}
//
//	Msg->BeginReading();
//	WeNetworkMessageType Type = Msg->ReadMessageType();
//
//	// if msg is sequenced
//	IS_SEQUENCED(Type)
//	{
//		OutgoingSequence++;
//	}
//
//}
//
//





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendDirectMessage
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  Sends message to ip even if not connected. These msg cant be sequenced. Port used is 
//								server port
//
//////////////////////////////////////////////////////////////////////////
//void WeClient::SendDirectMessage(String IP, int Port, WeNetworkMessage *MSg, bool SendReliable)
//{
//    sockaddr_in CustomAddress;
//
//	memset(&CustomAddress, 0, sizeof(CustomAddress));
//
//	CustomAddress.sin_family = AF_INET;
//	CustomAddress.sin_port = htons((u_short)Port);
//	
//
//	if (!WeNetworkDevice::Instance()->StringToSockaddr(IP, (sockaddr *)&CustomAddress))
//	{
//		WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Client(): ConnectToHost(): Cannot reslove host name %s", IP);
//		return;
//	}
//
//
//	if (Message.GetOverflow()) 
//	{
//		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendDirectMessage(): MessageOverflow");
//		return;
//	}
//
//	WeProfiler::Instance()->ClientSendBytesCounter += Message.Size;
//	if (!SendReliable)
//	{
//		WeNetworkDevice::Instance()->SendPacket(Socket, Message.Data, Message.Size, (sockaddr *)&CustomAddress);
//	} else
//	{
//		// reliable sending ... add later 
//	}
//}
//



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SendMessage(bool SendReliable)
{
	if (!Started) return;
	if (Status == WENETSTATE_DISCONNECTED) return; 
	if (Status != WENETSTATE_CONNECTING && Status != WENETSTATE_CONNECTED) return;


	Message.WriteSequences(OutgoingSequence, IncomingSequence);
	WeProfiler::Instance()->ClientSendBytesCounter += Message.Size;

	if (Message.GetOverflow()) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendMessage(): MessageOverflow");
		return;
	}

	// if we should send it localy
	if (LocalCommunication)
	{
		Message.Remote = false;
		(*WeServer::Instance()->MessageQueue.GetMessageForWriting()) = Message;
		WeServer::Instance()->MessageQueue.NextMessageForWriting();
	}	
	else 
	{		
		WeNetworkDevice::Instance()->SendPacket(Socket, Message.Data, Message.Size, (sockaddr *)&ServerAddress);
	}

	Message.BeginReading();
	WeNetworkMessageType Type = Message.ReadMessageType();

	// if msg is sequenced
	IS_SEQUENCED(Type)
	{
		OutgoingSequence++;
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendPing
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Sends ping
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SendPing()
{
	if (!Started || Status == WENETSTATE_DISCONNECTED) return;

	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_PING);	
	PingSentTime = WeTimer::Instance()->GetSystemTime();
	WeProfiler::Instance()->ClientSendBytesCounter += Message.Size;
	SendMessage(&Message);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::LoadConfig()
{
	ServerPort = WESERVER_PORT;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SaveConfig()
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendChat
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SendChat(String text)
{
	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_CHAT);
	Message.WriteString(text);
	SendMessage();
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::DumpBuffer()
{
	char Data[MAX_NETMESSAGE_LEN];
	int ret;
	while ((ret = WeNetworkDevice::Instance()->GetPacket(Socket, Data, NULL)) > 0)
	{
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Disconnect
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Disconnects from server
//
//////////////////////////////////////////////////////////////////////////
void WeClient::Disconnect()
{
	if (!Started) return;

	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_DISCONNECT);
	SendMessage();

	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: WeClient(): Disconnect(): Client disconnected");
	if (OnDisconnected) OnDisconnected();
	Status = WENETSTATE_DISCONNECTED;	
	LocalCommunication = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Tick
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  this is called every frame and is used to process arrived msgs
//
//////////////////////////////////////////////////////////////////////////
void WeClient::Tick()
{
	// Parser messages and sends them from ActiveNetworkActor
	if (!Started) return;
	
	int ret;
	WeNetworkMessage *tmpMsg;


	/*for (WeNetworkActor::NetworkActors.First(); !WeNetworkActor::NetworkActors.End(); WeNetworkActor::NetworkActors.Next())
	{
		WeNetworkActor::NetworkActors.GetItem()->Velocity = WeVector(0,0,0);
		WeNetworkActor::NetworkActors.GetItem()->AngularVelocity = WeVector(0,0,0);
	}*/


	// -----------------------------------------------------
	// Ping server
	if (Status == WENETSTATE_CONNECTED)
	{
		static float Elapsed = 0;
		Elapsed += WeTimer::Instance()->GetDeltaTimeF();
		if (Elapsed >= PingInterval)
		{
			SendPing();
			Elapsed = 0;
		}
	}
	// -----------------------------------------------------

	// -----------------------------------------------------
	// Timeout



	// -----------------------------------------------------
	// Msg processing
	
	tmpMsg = MessageQueue.GetMessageForWriting();
	while (ret = WeNetworkDevice::Instance()->GetPacket(Socket, tmpMsg->Data, &tmpMsg->FromAddr))
	{	
		if (ret <= 0) break;
		MessageQueue.NextMessageForWriting();
		tmpMsg->SetSize(ret);
		tmpMsg = MessageQueue.GetMessageForWriting();		
	}

	ProcessMsgQueue();		
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Tick
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  this is called every frame and is used to process arrived msgs
//
//////////////////////////////////////////////////////////////////////////
void WeClient::FindLANServers(int Port)
{
	// this will init some socket if not initialized. and use it to send message to all 
	// lan.
	if (!Started) return;
	if (LocalCommunication) return;

    sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((u_short)Port);		
	//addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	WeNetworkDevice::Instance()->StringToSockaddr("255.255.255.255", (sockaddr *)&addr);

	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_SERVERINFO);	
	WeNetworkDevice::Instance()->SendPacket(Socket, Message.Data, Message.Size, (sockaddr *)&addr);	
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Tick
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  this is called every frame and is used to process arrived msgs
//
//////////////////////////////////////////////////////////////////////////
void WeClient::ProcessMsgQueue()
{
	// Queue Processing
	WeNetworkMessage *tmpMsg;
	MessageQueue.BeginReading();
	while (tmpMsg = MessageQueue.GetMessageForReading())
	{				
		WeNetworkMessageType Type;
		tmpMsg->BeginReading();		
		WeProfiler::Instance()->ClientRecvBytesCounter += tmpMsg->Size;
		Type = tmpMsg->ReadMessageType();

		//IS_SEQUENCED(Type)
		{
			// We should write incoming, ack, dropped packets to client
			u_short Seq; 
			u_short SeqAck;

			tmpMsg->ReadSequences(Seq, SeqAck);

			if (Seq <= IncomingSequence)
			{
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Sequence Missmatch: arrived (%d), current(%d)", Seq, IncomingSequence);
			}

			DroppedPackets += Seq - IncomingSequence - 1;

			IncomingSequence = Seq;
			IncomingAcknowledged = SeqAck;
		}


		IS_FOR_CLIENT(Type)
		{
			switch(Type)
			{
			case WENETMESSAGE_CONNECTING: 
				Status = WENETSTATE_CONNECTED;
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: WeClient(): Tick(): Client connecting to server");

				UID = tmpMsg->ReadInt();
				// Send servers IP and Connected msg 
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_CONNECTING);
				Message.WriteData(&tmpMsg->FromAddr, sizeof(sockaddr));
				SendMessage(&Message, false);

				break;

			case WENETMESSAGE_CONNECTED: 
				Status = WENETSTATE_CONNECTED;
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: WeClient(): Tick(): Client connected to server");				
				if (OnConnected) OnConnected();
				break;


			case WENETMESSAGE_DISCONNECT: 
				Status = WENETSTATE_DISCONNECTED;
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: WeClient(): Tick(): Client disconnected");
				if (OnDisconnected) OnDisconnected();
				break;

			case WENETMESSAGE_PING:
				// we should send pong back
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_PONG);
				SendMessage(&Message, false);				
				break;

			case WENETMESSAGE_PONG:
				Ping = WeTimer::Instance()->GetSystemTime() - PingSentTime;
				if (OnPing) OnPing();
				break;	

			case WENETMESSAGE_AUTHORIZE:
				Status = WESTATUS_AUTHORIZING;
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_AUTHORIZATION);
				if (OnAuthorizationRequest) OnAuthorizationRequest(&Message);
				SendMessage();
				break;

			case WENETMESSAGE_AUTHORIZED:
				Status = WESTATUS_AUTHORIZED;
				if (OnAuthorized) OnAuthorized();
				Status = WENETSTATE_CONNECTED; // return to connected state 
				break;

			case WENETMESSAGE_NOTAUTHORIZED:
				Status = WESTATUS_NOTAUTHORIZED;
				if (OnNotAuthorized) OnNotAuthorized();
				break;

			case WENETMESSAGE_SPAWNMESSAGE:
				SpawnMessage(tmpMsg);				
				break;

			case WENETMESSAGE_CHAT:
				if (OnChat) OnChat(tmpMsg);
				break;

			case WENETMESSAGE_SERVERINFO:
				if (OnServerInfo) OnServerInfo(tmpMsg);
				break;

			case WENETMESSAGE_REPLICATE:
				WeNetworkActor *NA;
				WeNetworkActorID ID;
				ID = tmpMsg->ReadNetworkActorID(); 
				NA = WeNetworkActor::GetNetworkActor(ID);
				if (NA) NA->Client_HandleReplication(tmpMsg);
				break;

			default:
				if (OnProcessMessage) { OnProcessMessage(Type, tmpMsg); }
			}

			
		} else
		{
			// This is message for network actor. Call his msg processing
			WeNetworkActor *NA;
			WeNetworkActorID ID = tmpMsg->ReadNetworkActorID(); 
			NA = WeNetworkActor::GetNetworkActor(ID);
			if (NA) NA->Client_HandleGameMessage(Type, tmpMsg);
		}
	}

	MessageQueue.Clear();
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::RequestSpawn(int Type)
{
	if (!Started) return;
	if (Status != WENETSTATE_CONNECTED) return;

	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_REQUESTSPAWN);
	Message.WriteInt(Type);
	SendMessage(&Message, false);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SpawnMessage(WeNetworkMessage *Msg)
{
	int Type;
	bool IsInputPlayer;
	WeNetworkActorID NAID;
	WeNetworkActor *tmpNA = NULL;


	Type = Msg->ReadInt();
	IsInputPlayer = Msg->ReadBool();
	NAID = Msg->ReadNetworkActorID();

	//printf("WECLIENT: SpawnMessage(): Received NAID %d\n", NAID);
	tmpNA = WeNetworkActor::GetNetworkActor(NAID);

	//if (tmpNA && IsInputPlayer)
	//{
	//	tmpNA->Client_HandleSpawnMessage(Msg, IsInputPlayer);

	if (!tmpNA)
	{
		if (OnSpawnNewPlayer) tmpNA = OnSpawnNewPlayer(Type);
	}

	if (tmpNA)
	{
		tmpNA->Client_HandleSpawnMessage(Msg, IsInputPlayer);
		tmpNA->IsInputPlayer = IsInputPlayer;
	}


}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeClient::SendDeltaCommand(WeNetworkActor *Sender)
{
	if (!Sender) return;

	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_DELTACOMMAND);
	Message.WriteNetworkActorID(Sender->GetID());
	Sender->Client_MakeDeltaCommand(&Message);
	SendMessage(&Message);
}
