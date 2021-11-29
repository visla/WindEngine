/*
    WeNetworkClient.cpp

	Autor: Nebojsa Vislavski
	Date: 19.9.2005.
	Version: 1.0


	This is server-side client  

*/


#include <WeNetworkClient.h>
#include <WeNetworkDevice.h>
#include <WeNetworkMessage.h>
#include <WeNetworkActor.h>
#include <WeTimer.h>
#include <WeProfiler.h>
#include <WeClient.h>



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WeNetworkClient::WeNetworkClient(sockaddr *remoteaddr, SOCKET ServerSocket, int uid)
{
	Socket = ServerSocket;
	if (remoteaddr) 
	{
		RemoteAddress = * ((sockaddr_in *)remoteaddr);
		IsRemote = true;
	}
	else IsRemote = false;

	UID = uid;
	IncomingSequence = 0;
	OutgoingSequence = 1;
	IncomingAcknowledged = 0;
	Ping = 0;
	Latency = 0;
	LastMessageTime = 0;
	PingSentTime  = 0;
	Avatar = NULL;
	bAuthorized = false;
	TickCounter = 0;
	LastMakeMessageTime = 0;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Destructor
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This is called by server in order to make and send message from avatar
//
//////////////////////////////////////////////////////////////////////////
WeNetworkClient::~WeNetworkClient()
{
	Disconnect();
	
	//if (Avatar && IsRemote)
	//{
	//	SAFE_DELETE(Avatar);// = NULL;
	//}
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  MakeTickMessage
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This is called by server in order to make and send message from avatar
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkClient::MakeTickMessage()
{
	if (!Avatar) return;

	LastMessageTime = WeTimer::Instance()->GetSystemTime();

	// TODO: Add maybe One message for all these items
	
	for (WeNetworkActor::NetworkActors.First(); !WeNetworkActor::NetworkActors.End(); WeNetworkActor::NetworkActors.Next())
	{
        // if replication is to be done in this frame
		//if (WeNetworkActor::NetworkActors.GetItem()->TickCounter % WeNetworkActor::NetworkActors.GetItem()->Priority == 0) 
		//{
			if (WeNetworkActor::NetworkActors.GetItem()->Priority == -1) continue;
			if (WeNetworkActor::NetworkActors.GetItem()->Server_ReadyToSend(Avatar))
			{
				Message.Init();
				Message.WriteNetworkActorID(WeNetworkActor::NetworkActors.GetItem()->NetworkID);
				Message.WriteMessageType(WENETMESSAGE_REPLICATE);
				WeNetworkActor::NetworkActors.GetItem()->Server_Replicate(Avatar, &Message);
				printf("WENC: Replication Send\n");
				SendMessage(&Message, false);
			}
		//}

		// Do replication for NetworkActors` state
		for (WeNetworkActor::NetworkActors.GetItem()->States.First(); !WeNetworkActor::NetworkActors.GetItem()->States.End(); WeNetworkActor::NetworkActors.GetItem()->States.Next())
		{
			if (WeNetworkActor::NetworkActors.GetItem()->TickCounter % TickCounter % WeNetworkActor::NetworkActors.GetItem()->States.GetItem()->Priority == 0)
			{
				if (WeNetworkActor::NetworkActors.GetItem()->States.GetItem()->Priority == -1) continue;
				if (WeNetworkActor::NetworkActors.GetItem()->States.GetItem()->Server_ReadyToSend(Avatar))
				{
					Message.Init();
					Message.WriteNetworkActorID(WeNetworkActor::NetworkActors.GetItem()->NetworkID);
					WeNetworkActor::NetworkActors.GetItem()->States.GetItem()->Server_Replicate(Avatar, &Message);
					SendMessage(&Message, false);
				}
			}
		}
	}	
	
	//Message.WriteMessageType(WENETMESSAGE_REPLICATE);
	//Message.WriteNetworkActorID(Avatar->GetID());
	//Avatar->Server_Replicate(Avatar, &Message);
	//SendMessage(&Message);
	
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendMessage
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This is called to Send Message
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkClient::SendMessage(WeNetworkMessage *Msg, bool Reliable)
{
	if (Msg->GetOverflow()) 
	{
		WeNetworkDevice::Instance()->Loger->LogString(LOG_NOTICE, "Notice: NetworkClient(): SendMessage(): MessageOverflow");
		return;
	}

	Msg->WriteSequences(OutgoingSequence, IncomingSequence);	
	WeProfiler::Instance()->ServerSendBytesCounter += Msg->Size;

	if (!IsRemote)
	{
		Msg->Remote = false;
		(*WeClient::Instance()->MessageQueue.GetMessageForWriting()) = *Msg;
		WeClient::Instance()->MessageQueue.NextMessageForWriting();
	} else if (!Reliable)
	{		
		WeNetworkDevice::Instance()->SendPacket(Socket, Msg->Data, Msg->Size, (sockaddr *)&RemoteAddress);	
	} else
	{
		// reliable sending ... add later 
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
//         Function Name    :  SendPing
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This sends ping to client
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkClient::SendPing()
{
	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_PING);

	PingSentTime = WeTimer::Instance()->GetSystemTime();
	WeProfiler::Instance()->ServerSendBytesCounter += Message.Size;

	SendMessage(&Message, false);
}



void WeNetworkClient::Disconnect()
{
	Message.Init();
	Message.WriteMessageType(WENETMESSAGE_DISCONNECT);
	SendMessage(&Message, false);
}