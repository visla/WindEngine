/*
     WeClient.h

	 Autor: Nebojsa Vislavski
	 Date: 16.9.2005.
	 Version: 1.0

	 Client for networking
       

*/



#ifndef _WECLIENT_H
#define _WECLIENT_H


#include <WeNetworkMessage.h>
#include <WeNetworkDevice.h>
#include <SmartList.h>
#include <CString.h>
#include <WindEngine.h>
#include <WeNetworkMessageQueue.h>

class WeNetworkActor;


#define WENETSTATE_NONE 0
#define WENETSTATE_CONNECTING 1
#define WENETSTATE_CONNECTED 2
#define WENETSTATE_DISCONNECTING 3
#define WENETSTATE_DISCONNECTED 4


#define WESTATUS_AUTHORIZED	5
#define WESTATUS_NOTAUTHORIZED 6
#define WESTATUS_AUTHORIZING 7




typedef void (*Client_Callback) ();
typedef void (*Client_MsgCallback)(WeNetworkMessageType Type, WeNetworkMessage *Msg);
typedef void (*Client_ReqAuthorizationCallback)(WeNetworkMessage *Msg);
typedef void (*Client_AuthorizedCallback)();
typedef void (*Client_NotAuthorizedCallback)();
typedef void (*Client_PingCallback)();
typedef void (*Client_ChatCallback)(WeNetworkMessage *Msg);
typedef void (*Client_ServerInfoCallback)(WeNetworkMessage *Msg);
typedef WeNetworkActor *(*Client_SpawnNewPlayer)(int Type);


class WINDENGINE_API WeClient
{
private:
	static WeClient*				_instance;
protected:
    int								Status;  // status of client
	bool							Started; // is client initazlied

	String							ServerName; // what is name of server. ie. "204.44.22.3" or "server.com"
	int								ServerPort; // port that server uses. Gathered from ClientSettings

	sockaddr_in						MyAddress;
	sockaddr_in						ServerAddress;
	SOCKET							Socket;
	SOCKET							ReliableSocket;
	// Statistics
	int								DroppedPackets;
	u_short							IncomingSequence;
	u_short							OutgoingSequence;
	u_short							IncomingAcknowledged;
	int								LastMessageTime;  // time of last message received
	int								Ping;
	int								PingSentTime; // time when PING was sent
	int								AvgReadBytesPerSec;
	int								AvgWriteBytesPerSec;
	bool							LocalCommunication; // this means all sending should go into Server`s queue immediately
	int								UID; // id of client


	void							DumpBuffer();
	void							ProcessMsgQueue();
	void							SpawnMessage(WeNetworkMessage *Msg);
 
public:
	WeNetworkMessage				Message;	
	WeNetworkMessageQueue			MessageQueue;
	float							PingInterval;  // in what intervals ping will be sent

	int								GetUID() { return UID; }
	static WeClient*				Instance();
									WeClient();
	virtual							~WeClient() { ShutDown(); }
	// init methods
	virtual bool					Initialize();
	virtual void					ShutDown();
	// call this when you want server to spawn player of some type
			void					RequestSpawn(int Type);
	// sending methods
	virtual void					SendMessage(WeNetworkMessage *Msg, bool SendReliable = false);	
	virtual void					SendMessage(bool SendReliable = false);
	// call this method when you want client to send delta command to server. 
	// This will set some params and call Client_makeDeltaCommand on server
			void					SendDeltaCommand(WeNetworkActor *Sender);
		
	virtual void					SendPing();
	virtual void					SendChat(String text); // sends text message to server

	void							LoadConfig(); // loads ClientSettings
	void							SaveConfig(); // Saves ClientSettings. USed rarely

	int								GetPing() { return Ping; }
	int								GetLatency() { return Ping; }
	int								GetDroppedPackets() { return DroppedPackets; }
	int								GetStatus() { return Status; }
	bool							IsConnected() { return Status == WENETSTATE_CONNECTED; }

	virtual	void					Tick();  // process all messages from network. 
										     // Sends messages from network clients and 
										    // calls message processing for network clients too. If started of course

	virtual void					ConnectToHost(String hostname, int port = WESERVER_PORT); // connects to host with this name
	virtual void					Disconnect();
	// this is called when you want to lookup for LAN servers without connecting. 
	// OnServerInfo callback is called for every lan server that responds
	// calling this method while local server is running has no sence
	virtual void					FindLANServers(int Port); 

	Client_Callback					OnConnected;  // called when client connects
	Client_Callback					OnDisconnected; // called when client disconnects
	// called when client needs to send authorization data
    // You should only use Msg.Write commands in this callback. You dont need to call Init on message or Send.
	// it is done automaticly
	Client_ReqAuthorizationCallback OnAuthorizationRequest; 
	Client_AuthorizedCallback		OnAuthorized;
	Client_NotAuthorizedCallback	OnNotAuthorized;
	Client_PingCallback				OnPing; // when ping is received
	// called when processing messages arrived for client. This cathes only custom msgs.
	// not catching msgs like Connect, Disconnect, Ping, Pong, ...
	// Dont use Begin Reading on Message if you dont really need to
	Client_MsgCallback				OnProcessMessage;  
	// When client receives chat msg
	Client_ChatCallback				OnChat;  
	// This is called when we receive server info from server
	Client_ServerInfoCallback		OnServerInfo;
	// this is called when you need to create new network actor or some other inherited from Network Actor
	// you create it by using type (if you create other than NA). 
	// return newly created object in this callback. If you return NULL nothing is happend
	Client_SpawnNewPlayer			OnSpawnNewPlayer;

};

#endif