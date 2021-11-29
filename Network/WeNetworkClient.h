/*
    WeNetworkClient.h

	Autor: Nebojsa Vislavski
	Date: 19.9.2005.
	Version: 1.0


	This is server-side client  

*/

#ifndef _WENETWORKCLIENT_H
#define _WENETWORKCLIENT_H

#include <WeNetworkDevice.h>
#include <WeNetworkMessage.h>
#include <WindEngine.h>

class WeNetworkActor;

class WINDENGINE_API WeNetworkClient
{
   friend class WeServer;

protected:
	sockaddr_in			RemoteAddress; // server sets this
	sockaddr_in			RemoteReliableAddress; // server sets this
	SOCKET				Socket; // Server sets this
	SOCKET				ReliableSocket;  // Server sets this	

	unsigned  short		IncomingSequence;
	unsigned  short		OutgoingSequence;
	unsigned  short		IncomingAcknowledged;
	int					LastMakeMessageTime;
	int					Ping;         // this is ping for client. You should make you calculations according to this ping
	int					PingSentTime;
	int					DroppedPackets;
	int					Latency;
	int					LastMessageTime;
	bool				bAuthorized; 
	int					TickCounter; // used to count how many times MsgCreator has been called
	int					UID;	     // UID of this network client
	bool				IsRemote;    // this is set if RemoteAddress is valid. If this is false then it meand that all sending should be immedialely send to WeClient
	WeNetworkActor*		Avatar; // this object is avatar to network client. 

						WeNetworkClient(sockaddr *remoteaddr, SOCKET ServerSocket, int uid); // this is called by Server to create client with this remote address
						~WeNetworkClient();


public:

	int					GetUID() { return UID; }
	void				SetAvatar(WeNetworkActor *avatar) { Avatar = avatar; } 
	WeNetworkActor*		GetAvatar() { return Avatar; }
	int					GetLatency() { return Ping; }
	int					GetDroppedPackets() { return DroppedPackets; }
	// creates msg for remote computer. this is usually called when server needs to send msgs	
	virtual void		MakeTickMessage(); 
	virtual	void		SendMessage(WeNetworkMessage *Msg, bool Reliable = false);
	virtual void		SendPing();
	bool				Authorized() { return bAuthorized; }
	void				Disconnect(); // disconnects other side ie removes client

	WeNetworkMessage	Message;
};


#endif