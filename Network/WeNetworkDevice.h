/*
     WeNetworkDevice.h

	 Autor: Nebojsa Vislavski
	 Date: 12.9.2005.
	 Version: 1.0 

	 Main functions for network working
   

*/

#ifndef _WENETWORKDEVICE_H
#define _WENETWORKDEVICE_H



#ifdef WIN32
	#ifndef _WINSOCKAPI_
	#define _WINSOCKAPI_
	#endif


	#include <windows.h>
	#include <winsock2.h>
    
    typedef int socklen_t;
    
#else
   // Unix-linux specific stuff
    typedef int SOCKET;

	#ifndef TRUE
	#define TRUE 1
	#endif

	#ifndef FALSE
	#define FALSE 0
	#endif


#endif




// standard headers
#include <stdio.h>
#include <CString.h>
#include <WeLoger.h>
#include <WindEngine.h>


#define WENETWORK_PROTOCOL_TCP 1
#define WENETWORK_PROTOCOL_UDP 2

#ifdef WIN32
	#define WESOCK_INVALIDSOCKET INVALID_SOCKET
#else
	#define WESOCK_INVALIDSOCKET -1
#endif


// Messages client and server uses
// from 0 to 100 are msgs for network actors that are sequenced
// from -1 to -60 are msgs for client not sequenced
// 101 - 120 msgs for clients that are sequenced
// -61 - -120 msgs for networkd actors not sequenced


#define IS_FOR_CLIENT(msgtype)  if ((msgtype <= -1 && msgtype >= -60) || (msgtype >= 51 && msgtype <= 125) )
#define IS_SEQUENCED(msgtype) if (msgtype > 0)
#define IS_NOTSEQUENCED(msgtype) if (msgtype < 0)
#define IS_FOR_NETWORKACTOR(msgtype) if ( (msgtype >= 0 && msgtype <= 50) || (msgtype <= -61 && msgtype >= -120) )

#define WENETMESSAGE_PING			-1
#define WENETMESSAGE_PONG			-2
#define WENETMESSAGE_CONNECT		-3
#define WENETMESSAGE_CONNECTING		-4
#define WENETMESSAGE_CONNECTED		-5
#define WENETMESSAGE_DISCONNECT		-6
#define WENETMESSAGE_DISCONNECTED	-7
#define WENETMESSAGE_CHAT			-8


// if you receive this message in client. Your callback should only write data and call sendMessage
#define WENETMESSAGE_AUTHORIZE		-9   // when server asks client to authorize
#define WENETMESSAGE_AUTHORIZATION	-10   // when client sends authorization message
#define WENETMESSAGE_AUTHORIZED		-11  // when server accepts authorization
#define WENETMESSAGE_NOTAUTHORIZED	-12  // when server denials authorization
#define WENETMESSAGE_SERVERINFO		-14  // this is server info msg

#define WENETMESSAGE_REQUESTSPAWN	-15
#define WENETMESSAGE_SPAWNMESSAGE	-16

#define WENETMESSAGE_DELTACOMMAND	-17
#define WENETMESSAGE_REPLICATE		-18


// -----------------------------

#define WESERVER_PORT 10200




class WINDENGINE_API WeNetworkDevice
{
friend class WeClient;
friend class WeServer;
friend class WeNetworkClient;

private:
	static WeNetworkDevice *_instance;

protected:
    bool Initialized;


	WeNetworkDevice(void);
	virtual ~WeNetworkDevice(void);

	virtual void SetLogLevel(int LogLevel); 	// 0 - no logging
	virtual bool InitializeWinSock(void); // add Unix sock initialization code later here
public:
	// singleton
	static WeNetworkDevice *Instance(void);
	virtual bool	Initialize(void); // Initiazlizes network device. Engine calls this
	virtual void	ShutDown(void);
	virtual SOCKET	Socket(int Protocol);

	// NetInterface - what ip should it use when binding. If not supplied it will bind to any local address
	// Port can also be not supplied
	virtual SOCKET	OpenUDPSocket(int Port = 0, char *NetInterface = NULL); 
	virtual SOCKET	OpenTCPSocket(int Port = 0, char *NetInterface = NULL);

	virtual bool	SetNonBlocking(SOCKET s, u_long SetMode);
	virtual bool	SetBroadcasting(SOCKET sock, int Mode);
	virtual bool 	StringToSockaddr(char *str, struct sockaddr *sadr);
			String	IPToString(unsigned long IP);
	virtual String	SockaddrToString(struct sockaddr *sadr);
	virtual String	GetHostName(SOCKET UDPsock);
	virtual void	CloseSocket(SOCKET sock);
	virtual void	SendPacket(SOCKET sock, char *data, int len, struct sockaddr *to);
	virtual int		GetPacket(SOCKET sock, char *data, struct sockaddr *from);
	virtual void	Broadcast(SOCKET sock, char *data, int len, int Port);


	WeLoger *Loger;

};





#endif