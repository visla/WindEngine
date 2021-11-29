/*
     WeServer.h

	 Autor: Nebojsa Vislavski
	 Date: 18.9.2005.
	 Version: 1.0

	 This is server class. It is not used to do messages. Network clients and clients
	 have algorithms for message processing and NewtrokActor also. 

*/

#ifndef _WESERVER_H
#define _WESERVER_H



#include <CString.h>
#include <SmartList.h>
#include <WeNetworkMessage.h>
#include <WeNetworkDevice.h>
#include <WeNetworkClient.h>
#include <WeNetworkMessageQueue.h>
#include <WindEngine.h>

#define WESERVER_NOTHOSTING 1
#define WESERVER_HOSTING	2

class WeNetworkClient;
class WeNetworkActor;

typedef CSmartList <WeNetworkClient *> WeNetworkClients;
typedef void (*Server_ClientCallback)(WeNetworkClient *);
typedef void (*Server_MsgCallback)(WeNetworkClient *, WeNetworkClients *, WeNetworkMessageType Type, WeNetworkMessage *);
typedef void (*Server_AuthorizeCallback)(WeNetworkClient *, WeNetworkMessage *, bool &Authorized);  // set Authorized to true to confirm
typedef void (*Server_UnknownMsgCallback)(WeNetworkMessageType, WeNetworkMessage *, StringParam FromIP, u_short FromPort);  // set Authorized to true to confirm
typedef void (*Server_MakeServerInfoCallback)(WeNetworkMessage *);
typedef WeNetworkActor *(*Server_RequestSpawn)(int Type, WeNetworkClient *From);


class WINDENGINE_API WeServer
{
friend class WeNetworkActor;

private:
    static WeServer				*_instance;


protected:
	WeNetworkClients				NetworkClients;
	WeNetworkClient*				LocalNetworkClient; // this is network client that talks to local client in the same process
	int								NextNetworkActorID;
	bool							bMultiplayer; // is game really multiplayer or just singleplayer
	bool							bRequestAuthorization; // does this server requests authorization
	bool							bStarted;
	int								Status;
	SOCKET							Socket; // used for communication
	SOCKET							ReliableSocket; 
	String							HostName;
	int								Port;	
	WeNetworkMessage				Message;
	int								LastUID;

	virtual void					ParseMessage(WeNetworkMessage *Msg);
			void					RequestSpawn(WeNetworkMessage *Msg, WeNetworkClient *From);
	// This will find network client based on received message
	WeNetworkClient*				FindNetworkClient(WeNetworkMessage *Msg); 
									WeServer();


public:
	int								TickInterval; // what is interval server is using to send messages
	int								PingInterval; // what is interval server will ping clients. Default is 5sec
	WeNetworkMessageQueue			MessageQueue;


	virtual	bool					Initialize();
	virtual void					ShutDown();
	virtual							~WeServer() { ShutDown(); }
	static WeServer*				Instance(); // singleton
	void							LoadConfig();
	void							SaveConfig();
	// this will make server hosting
	// Request authorization means that Authorization callbacks are going to be called
	// if hostname is empty string primary ip will be used
	virtual bool					StartHosting(StringParam hostname, int Port, bool IsMultiplayer, bool RequestAuthorization); 
	virtual void					StopHosting();
	String							GetHostName() { return HostName; }
	int								GetPort() { return Port; }
	int								GetStatus() { return Status; }
	bool							IsMultiplayer() { return bMultiplayer; }
	virtual void					Reset(); // this will reset server
	virtual void					Tick(); // this is called every frame. This processes arrived messages
	virtual void					Tick_CreateMessages(); // this is called when server needs to create new messages
			

	// this will send message to everyone on this IP. You dont have to be connected
	// this is used when selecting servers to connect or similar stuff
	virtual void					SendDirectMessage(String IP, int Port, WeNetworkMessage *Msg, bool SendReliable = false);
	virtual void					SendDirectMessage(sockaddr *to, WeNetworkMessage *Msg, bool SendReliable = false);

	// This is game command for clients. Call this when sending in game message. Like Fire bullet,
	// respawn, ... 
			void					SendGameCommand(WeNetworkMessage *Msg, WeNetworkActor *Sender);
	// This will send message to specific Client. If it is NULL then send to all
	// TODO: add secure sending
	// dont call send when sending in game commands for network actors
	virtual void					Send(WeNetworkMessage *Msg, bool Reliable = false, WeNetworkClient *Client = NULL); 
	virtual void					SendPing();  // sends ping to all clients	
	// user is responsible in cathing these messages in msg processing 	
	virtual void					SendChat(String Text, WeNetworkClient *ToClient = NULL); // sends chat message to specific client. If Null than send to all
	virtual void					CheckForTimeout(); // this will check if there is client that is time-out and needed to Disonnect
	virtual WeNetworkClient*		AddClient(sockaddr *from); // this will add new client and return pointer to new client
	virtual void					RemoveClient(WeNetworkClient *); // this is removing client from list

	// Callbacks
	Server_ClientCallback			OnClientConnected;  // called when client tries to connect.  
	Server_ClientCallback			OnClientDisconnected;	 // called when client disconnects.
	Server_ClientCallback			OnPingChanged; // called when ping is set for some client
	// Params. (FromCilient, Message, Authorized )
	// last param is set to true in callback if you want to signal successful authorization
	// dont use BeginReading on this msg just use reading
	Server_AuthorizeCallback		OnClientAuthorize; // called when client needs to authorize	
	// called when processing custom messages for clients
	// Params: (FromClient, NetworkClients, Message)
	// Dont use Begin reading just Read commands
	Server_MsgCallback				OnProcessClientMessage;	 
	// Called when server receives unknown message from unknown client
	Server_UnknownMsgCallback		OnProcessUnknownMessage; 
	Server_MakeServerInfoCallback	OnMakeServerInfo;
	Server_RequestSpawn				OnRequestSpawn;

 

};


#endif
