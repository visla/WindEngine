/*
     WeServer.cpp

	 Autor: Nebojsa Vislavski
	 Date: 18.9.2005.
	 Version: 1.0

	 This is server class

*/

#include <WeServer.h>
#include <WeNetworkClient.h>
#include <WeTimer.h>
#include <WeNetworkActor.h>
#include <WeProfiler.h>
#include <WeClient.h>


WeServer *WeServer::_instance = NULL;



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WeServer *WeServer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeServer();
	return _instance;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WeServer::WeServer()
{
	// clear callbacks
	OnClientConnected		= NULL;
	OnClientDisconnected	= NULL;
	OnProcessClientMessage	= NULL;
	OnProcessUnknownMessage	= NULL;
	OnClientAuthorize		= NULL;
	OnMakeServerInfo		= NULL;
	OnRequestSpawn			= NULL;
	LastUID					= 1;

	TickInterval = 100;
	PingInterval = 5;
	bStarted = false;
	bRequestAuthorization = false;
	LocalNetworkClient = NULL;
	
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
bool WeServer::Initialize()
{
	if (bStarted) return true;

	if (!WeNetworkDevice::Instance()->Initialize())
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Server(): Error initializing server");
		bStarted = false;
		return false;
	}


	OnPingChanged = NULL;
	TickInterval = 100;
	bRequestAuthorization = false;	


	LoadConfig();

	Status = WESERVER_NOTHOSTING;
	bStarted = true;
	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server initialized");
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
void  WeServer::ShutDown()
{
	if (!bStarted) return;

	if (Status == WESERVER_HOSTING)
	{
		StopHosting();
	}
	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server shutted down");

	bStarted = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void  WeServer::LoadConfig()
{
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void  WeServer::SaveConfig()
{
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
bool WeServer::StartHosting(StringParam hostname, int port, bool IsMultiplayer_, bool RequestAuthorization)
{
	if (!bStarted) return false;
	if (Status == WESERVER_HOSTING) return true;

	HostName = hostname;
	Port = port;
	bMultiplayer = IsMultiplayer_;
	bRequestAuthorization = RequestAuthorization;


	if (bMultiplayer)
	{
		if (HostName == (String)"")
		{
			Socket = WeNetworkDevice::Instance()->OpenUDPSocket(Port);	
			if (Socket != WESOCK_INVALIDSOCKET)
				HostName = WeNetworkDevice::Instance()->GetHostName(Socket);
		}
		else
		{
			Socket = WeNetworkDevice::Instance()->OpenUDPSocket(Port, HostName);
		}


		if (Socket == WESOCK_INVALIDSOCKET)
		{
			WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Server(): StartHosting(): failed creating socket");
			return false;
		}		
		WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server started hosting on port: %d", Port);
	} else
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server started hosting signleplayer game");
	}


	Status = WESERVER_HOSTING;
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
void WeServer::StopHosting()
{	
	if (!bStarted) return;
	if (Status != WESERVER_HOSTING) return;

	for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
	{		
		SAFE_DELETE(NetworkClients.GetItem());
	}
	NetworkClients.Clear();

	WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server stops hosting");
	WeNetworkDevice::Instance()->CloseSocket(Socket);

	Status = WESERVER_NOTHOSTING;

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::Reset()
{
	if (Status != WESERVER_HOSTING) return;

	StopHosting();
	StartHosting(HostName, Port, IsMultiplayer(), bRequestAuthorization);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  FindNetworkClient
//         Input Parametars :  Message
//         Output Parametars:  N/A
//         Comments         :  This will find network client based on received message
//
//////////////////////////////////////////////////////////////////////////
WeNetworkClient* WeServer::FindNetworkClient(WeNetworkMessage *Msg)
{
	// if this is local msg then find and return localnetworkclient
	CSmartListIterator<WeNetworkClient*> I;

	if (!Msg->Remote)
	{
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{
			//if (!NetworkClients.GetItem()->IsRemote)
			if (!I.GetItem()->IsRemote)
			{
				//return NetworkClients.GetItem();
				return I.GetItem();
			}
		}			
	} else
	{
		// for msgs received from network
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{
			if (memcmp(&I.GetItem()->RemoteAddress, &Msg->FromAddr, sizeof(I.GetItem()->RemoteAddress)) == 0)
			//if (memcmp(&NetworkClients.GetItem()->RemoteAddress, &Msg->FromAddr, sizeof(NetworkClients.GetItem()->RemoteAddress)) == 0)
			{
				return I.GetItem();
				//return NetworkClients.GetItem();
			}
		}	
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RequestSpawn
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This is called when requestmsg is received
//
//////////////////////////////////////////////////////////////////////////
void WeServer::RequestSpawn(WeNetworkMessage *Msg, WeNetworkClient *From)
{
	WeNetworkActor *tmpNA;
	int Type;

	// if we already have avatar for this player then do no spawn
	if (From->Avatar) return;

	Type = Msg->ReadInt();
	tmpNA = NULL;
	if (OnRequestSpawn) 
		tmpNA = OnRequestSpawn(Type, From);					
	
	if (tmpNA)
	{
		// send avatar message to From client. 
		From->SetAvatar(tmpNA);
		Message.Init();
		Message.WriteMessageType(WENETMESSAGE_SPAWNMESSAGE);
		Message.WriteInt(tmpNA->Type);		
		Message.WriteBool(true);		
		tmpNA->Server_MakeSpawnMessage(&Message);                
		From->SendMessage(&Message, false);

		// Send all others info about new player tmpNA
		CSmartListIterator<WeNetworkClient*> I;
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{
			if (From != I.GetItem())
			//if (From != NetworkClients.GetItem())
			{
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_SPAWNMESSAGE);
				Message.WriteInt(tmpNA->Type);		
				Message.WriteBool(false);
				tmpNA->Server_MakeSpawnMessage(&Message);                				
				I.GetItem()->SendMessage(&Message, false);
				//NetworkClients.GetItem()->SendMessage(&Message, false);
			}			
		}

		// Send to from info about all existing network actors except tmpNA
		for (WeNetworkActor::NetworkActors.First(); !WeNetworkActor::NetworkActors.End(); WeNetworkActor::NetworkActors.Next())
		{
			if (WeNetworkActor::NetworkActors.GetItem() != tmpNA)
			{
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_SPAWNMESSAGE);
				Message.WriteInt(WeNetworkActor::NetworkActors.GetItem()->Type);		
				Message.WriteBool(false);
				WeNetworkActor::NetworkActors.GetItem()->Server_MakeSpawnMessage(&Message);                				
				From->SendMessage(&Message, false);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ParseMessage
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will do some action from message arrived
//
//////////////////////////////////////////////////////////////////////////
void WeServer::ParseMessage(WeNetworkMessage *Msg)
{
	WeNetworkClient *From;
	// check to see from whom we got this packet
	From = FindNetworkClient(Msg);


	if (From)
	{		
		From->LastMessageTime = WeTimer::Instance()->GetSystemTime();
		Msg->BeginReading();
		WeNetworkMessageType Type = Msg->ReadMessageType();

 
		//IS_SEQUENCED(Type)
		{
			// We should write incoming, ack, dropped packets to client
			u_short Seq;
			u_short SeqAck;

			Msg->ReadSequences(Seq, SeqAck);

			if (Seq <= From->IncomingSequence)
			{
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "(Server): Sequence Missmatch: arrived (%d), current(%d)", Seq, From->IncomingSequence);
				//return; // ignore missmatched messages
			}

			From->DroppedPackets += Seq - From->IncomingSequence - 1;

			From->IncomingSequence = Seq;
			From->IncomingAcknowledged = SeqAck;
		} 

		IS_FOR_CLIENT(Type)
		{
			switch (Type)
			{

			case WENETMESSAGE_CONNECTING:
				// inform client that it is connected

				//unsigned long IP;
				sockaddr fromtmp;
				Msg->ReadData(&fromtmp, sizeof(sockaddr));
				HostName = WeNetworkDevice::Instance()->SockaddrToString(&fromtmp);
		
				// client is connected call callback
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_CONNECTED);
				From->SendMessage(&Message, false);

				if (OnClientConnected) OnClientConnected(From); 

				// if Authorization required
				if (bRequestAuthorization)
				{
					Message.Init();
					Message.WriteMessageType(WENETMESSAGE_AUTHORIZE);
					From->SendMessage(&Message, false);
				}			

				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server(): Client connected"); 
			break;			



			case WENETMESSAGE_DISCONNECT:
				
				// call callback
				if (OnClientDisconnected) OnClientDisconnected(From); 			
				// inform client that it is disconnected 
				//Message.Init();
				//Message.WriteMessageType(WENETMESSAGE_DISCONNECTED);
				//From->SendMessage(&Message, false);

				RemoveClient(From);			
				WeNetworkDevice::Instance()->Loger->LogString(LOG_INFO, "Info: Server(): Client diconnected"); 
				return;


			case WENETMESSAGE_PING:
				// just answer with pong
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_PONG);
				From->SendMessage(&Message, false);
				break;

			case WENETMESSAGE_PONG:
				From->Ping = WeTimer::Instance()->GetSystemTime() - From->PingSentTime;
				if (OnPingChanged) OnPingChanged(From);
				break;

			case WENETMESSAGE_REQUESTSPAWN:		
				RequestSpawn(Msg, From);
				break;

			case WENETMESSAGE_DELTACOMMAND:
				//if (From->Avatar) 
				//{			
				//	if (!From->Avatar->ReceivedDeltaCommand)
				//	{
				//		From->Avatar->Velocity = WeVector(0,0,0);
				//		From->Avatar->AngularVelocity = WeVector(0,0,0);
				//		From->Avatar->ReceivedDeltaCommand = true;
				//	}
					
					WeNetworkActor *Avatar;
					Avatar = WeNetworkActor::GetNetworkActor(Msg->ReadNetworkActorID());
					if (Avatar)
					//if (From->Avatar->GetID() == Msg->ReadNetworkActorID())
					{
						// Record velocity 
						//From->Avatar->TmpVelocity = From->Avatar->Velocity;
						//From->Avatar->TmpAngularVelocity = From->Avatar->AngularVelocity;

                        //From->Avatar->Server_HandleDeltaCommand(Msg); // this will make new velocity
						Avatar->Server_HandleDeltaCommand(Msg); // this will make new velocity


						//From->Avatar->RefVelocity = From->Avatar->Velocity;
						//From->Avatar->RefAngularVelocity = From->Avatar->AngularVelocity;
                        
						//From->Avatar->Velocity += From->Avatar->TmpVelocity;
						//From->Avatar->AngularVelocity += From->Avatar->TmpAngularVelocity;
						// add velocity
					}
				//}                
				break;

			case WENETMESSAGE_AUTHORIZATION:
				bool Authorized;
				Authorized = false;
				if (OnClientAuthorize) OnClientAuthorize(From, Msg, Authorized);


				if (Authorized)
				{
					From->bAuthorized = true;
					Message.Init();
					Message.WriteMessageType(WENETMESSAGE_AUTHORIZED);
					From->SendMessage(&Message, false);
				} else 
				{
					From->bAuthorized = false;
					Message.Init();
					Message.WriteMessageType(WENETMESSAGE_NOTAUTHORIZED);
					From->SendMessage(&Message, false);
				}
				break;



			}			

			// run callback for custom msg processing
			if (OnProcessClientMessage) OnProcessClientMessage(From, &NetworkClients, Type, Msg);

		} 
		IS_FOR_NETWORKACTOR(Type)
		{
			// This is message for network actor from client. Call his msg processing
			if (From->Avatar) 
			{
				Message.ReadNetworkActorID();

				From->Avatar->Server_HandleTickMessage(Type, Msg);

			}
		}

	} else
	{
		// this message is from unexisting client so only some messages can be processed
		// like CONNECT, CONNECTED

		Msg->BeginReading();
		WeNetworkMessageType Type = Msg->ReadMessageType();
			
		switch (Type)
		{
			case WENETMESSAGE_CONNECT:
				if (Msg->Remote) From = AddClient(&Msg->FromAddr);
				else From = AddClient(NULL);

				From->LastMessageTime = WeTimer::Instance()->GetSystemTime();

				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_CONNECTING);				
				Message.WriteInt(From->GetUID());

				From->SendMessage(&Message);

				//if (Msg->Remote)
				//	SendDirectMessage(&Msg->FromAddr, &Message, false);								
				//else 
				//	SendDirectMessage(NULL, &Message, false);
			break;

			case WENETMESSAGE_SERVERINFO:
				Message.Init();
				Message.WriteMessageType(WENETMESSAGE_SERVERINFO);				
				if (OnMakeServerInfo) OnMakeServerInfo(&Message);
				SendDirectMessage(&Msg->FromAddr, &Message, false);
			break;



			default: 
				// TODO: Check to see if there is reason for this
				String IPs;
				u_short tmp;
				tmp = ntohs(((sockaddr_in *)&Msg->FromAddr)->sin_port);
				IPs = WeNetworkDevice::Instance()->SockaddrToString(&Msg->FromAddr);

				if (OnProcessUnknownMessage) OnProcessUnknownMessage(Type, Msg, IPs, tmp);
		}
		



	}

}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Tick
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will read messages and send them
//
//////////////////////////////////////////////////////////////////////////
void WeServer::Tick()
{
    //sockaddr From;
	WeNetworkClient *WeNetworkClient = NULL;

	if (Status != WESERVER_HOSTING) return;


	// TEMP ----
	// clear velocities for na to RefVelocity so we can simulate movement
	for (WeNetworkActor::NetworkActors.First(); !WeNetworkActor::NetworkActors.End(); WeNetworkActor::NetworkActors.Next())
	{
	//	WeNetworkActor::NetworkActors.GetItem()->Velocity = WeNetworkActor::NetworkActors.GetItem()->RefVelocity;
	//	WeNetworkActor::NetworkActors.GetItem()->AngularVelocity = WeNetworkActor::NetworkActors.GetItem()->RefAngularVelocity;
		WeNetworkActor::NetworkActors.GetItem()->ReceivedDeltaCommand = false;
	}
	// ----


	if (bMultiplayer)
		CheckForTimeout(); // disconnect timed-out users
	// TODO: Add select function to test
	int ret;

	
	// Ping all users 
	static float PingElapsed = 0;
	PingElapsed += WeTimer::Instance()->GetDeltaTimeF();
	if (PingElapsed >= PingInterval)
	{
		SendPing();
		PingElapsed = 0;
	}


	// Read Messages
	WeNetworkMessage *tmpMsg;
	if (bMultiplayer)
	{
		tmpMsg = MessageQueue.GetMessageForWriting();
		while (ret = WeNetworkDevice::Instance()->GetPacket(Socket, tmpMsg->Data, &tmpMsg->FromAddr))
		{	
			// if we havent read anything
			if (ret <= 0) break;

			tmpMsg->SetSize(ret);		
			tmpMsg->Remote = true;
			MessageQueue.NextMessageForWriting();
			tmpMsg = MessageQueue.GetMessageForWriting();
		}
	}

	MessageQueue.BeginReading();
	while (tmpMsg = MessageQueue.GetMessageForReading())
	{
		WeProfiler::Instance()->ServerRecvBytesCounter += tmpMsg->Size;	
		ParseMessage(tmpMsg);
	}

	MessageQueue.Clear();
	
	// Try to send 
	Tick_CreateMessages();
    
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::Tick_CreateMessages()
{
	static int TickCount = 0;

	TickCount += WeTimer::Instance()->GetDeltaTimeDW();

	// is this time to tick
	if (TickCount > TickInterval)
	{
		CSmartListIterator<WeNetworkClient*> I;
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{
			I.GetItem()->MakeTickMessage();
			//NetworkClients.GetItem()->MakeTickMessage();
		}
		TickCount = 0;
	}

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::Send(WeNetworkMessage *Msg, bool Reliable, WeNetworkClient *Client)
{
	if (Status != WESERVER_HOSTING) return;	
	if (Client) 
	{
		Client->SendMessage(Msg, Reliable); 
	} else
	{
		CSmartListIterator<WeNetworkClient*> I;
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{
			I.GetItem()->SendMessage(Msg, Reliable);
			//NetworkClients.GetItem()->SendMessage(Msg, Reliable);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendDirectMessage
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  Sends message to ip even if not connected. These msg cant be sequenced. Port used is 
//								server port
//
//////////////////////////////////////////////////////////////////////////
void WeServer::SendDirectMessage(String IP, int Port, WeNetworkMessage *Msg, bool SendReliable)
{
    sockaddr_in CustomAddress;

	memset(&CustomAddress, 0, sizeof(CustomAddress));

	CustomAddress.sin_family = AF_INET;
	CustomAddress.sin_port = htons((u_short)Port);
	
	if (!WeNetworkDevice::Instance()->StringToSockaddr(IP, (sockaddr *)&CustomAddress))
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_ERROR, "Error: Client(): ConnectToHost(): Cannot reslove host name %s", IP);
		return;
	}

	WeProfiler::Instance()->ServerSendBytesCounter += Msg->Size;

	if (Msg->GetOverflow()) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendDirectMessage(): MessageOverflow");
		return;
	}

	if (!SendReliable)
	{
		WeNetworkDevice::Instance()->SendPacket(Socket, Msg->Data, Msg->Size, (sockaddr *)&CustomAddress);
	} else
	{
		// reliable sending ... add later 
	}
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendDirectMessage
//         Input Parametars :  SockAddr
//         Output Parametars:  N/A
//         Comments         :  Sends message to ip even if not connected. These msg cant be sequenced. Port used is 
//								server port
//
//////////////////////////////////////////////////////////////////////////
void WeServer::SendDirectMessage(sockaddr *to, WeNetworkMessage *Msg, bool SendReliable)
{	
	if (Msg->GetOverflow()) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: Client(): SendDirectMessage(): MessageOverflow");
		return;
	}

	WeProfiler::Instance()->ServerSendBytesCounter += Msg->Size;

	if (!to)
	{
		Msg->Remote = false;
		(*WeClient::Instance()->MessageQueue.GetMessageForWriting()) = *Msg;
		WeClient::Instance()->MessageQueue.NextMessageForWriting();		
	} else if (!SendReliable)
	{
		WeNetworkDevice::Instance()->SendPacket(Socket, Msg->Data, Msg->Size, to);
	} else
	{
		// reliable sending ... add later 
	}
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendPing
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will ping all clients
//
//////////////////////////////////////////////////////////////////////////
void WeServer::SendPing()
{
	if (Status != WESERVER_HOSTING) return;

	CSmartListIterator<WeNetworkClient*> I;
	for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
	//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
	{
		I.GetItem()->SendPing();        
		//NetworkClients.GetItem()->SendPing();        
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendChat
//         Input Parametars :  Text, ToClient
//         Output Parametars:  N/A
//         Comments         :  This sends chat message to all or only to someone. If Toclient is null then sends to all
//
//////////////////////////////////////////////////////////////////////////
void WeServer::SendChat(String Text, WeNetworkClient *ToClient)
{
	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_CHAT);
	Message.WriteString(Text);

	if (ToClient)
	{
		ToClient->SendMessage(&Message);
	} else
	{
		CSmartListIterator<WeNetworkClient*> I;
		for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
		//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
		{			
			I.GetItem()->SendMessage(&Message);
			//NetworkClients.GetItem()->SendMessage(&Message);
		}
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::CheckForTimeout()
{
	return; // SAMO ZA DEMO!!!

	// find what client is timed-out
	int CurrTime = WeTimer::Instance()->GetSystemTime();

	for (NetworkClients.First(); !NetworkClients.End(); )
	{
		// 10 seconds is time out
		if (CurrTime - NetworkClients.GetItem()->LastMessageTime > 30000)
		{
			Message.Init();
			Message.WriteMessageType(WENETMESSAGE_DISCONNECT); // disconect this client
			NetworkClients.GetItem()->SendMessage(&Message);
			// release client from list
			if (OnClientDisconnected) OnClientDisconnected(NetworkClients.GetItem());
			SAFE_DELETE(NetworkClients.GetItem());
			NetworkClients.Delete();
		} else 	NetworkClients.Next();
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WeNetworkClient *WeServer::AddClient(sockaddr *from)
{
	WeNetworkClient *NewClient = new WeNetworkClient(from, Socket, LastUID);	
	NetworkClients.PushBack(NewClient);
	LastUID++;
	return NewClient;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::RemoveClient(WeNetworkClient *ClientToRemove)
{
	for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
	{
		if (NetworkClients.GetItem() == ClientToRemove)
		{
			SAFE_DELETE(ClientToRemove);
			NetworkClients.Delete();
			return;
		}
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WeServer::SendGameCommand(WeNetworkMessage *Msg, WeNetworkActor *Sender)
{
	if (!Sender) return;

	Msg->WriteNetworkActorID(Sender->GetID());

	CSmartListIterator<WeNetworkClient*> I;
	for (I = NetworkClients.FirstI(); I != NetworkClients.EndI(); ++I)
	//for (NetworkClients.First(); !NetworkClients.End(); NetworkClients.Next())
	{
		if (I.GetItem()->IsRemote)
		//if (NetworkClients.GetItem()->IsRemote)
		{	
			I.GetItem()->SendMessage(Msg);
			//NetworkClients.GetItem()->SendMessage(Msg);
		}
	}
}




