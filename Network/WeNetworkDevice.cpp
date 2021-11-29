/*
     WeNetworkDevice.cpp

	 Autor: Nebojsa Vislavski
	 Date: 12.9.2005.
	 Version: 1.0 

	 Main functions for network working
   

*/

#include <wenetworkdevice.h>
#include <malloc.h>
#include <WeProfiler.h>



WeNetworkDevice *WeNetworkDevice::_instance = NULL;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
WeNetworkDevice::WeNetworkDevice(void)
{
	Initialized = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
WeNetworkDevice::~WeNetworkDevice(void)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
// Initiazlizes network device. Engine calls this
bool WeNetworkDevice::Initialize()
{
	if (Initialized) return true;

	Loger = new WeLoger("network.log");
	Loger->Start();


#ifdef WIN32
	Initialized = InitializeWinSock();
#else  
	 // Unix stuff
	 return false; // just for now
#endif

	 
	 
	 if (!Initialized) Loger->LogString(LOG_INFO, "Log: Network system NOT initialized");
	 else Loger->LogString(LOG_INFO, "Log: Network system initialized");

	
	return Initialized;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkDevice::ShutDown(void)
{
	if (!Initialized) return;

	Initialized = false;

	Loger->LogString(LOG_INFO, "Log: Network system shutted down");
	delete Loger;

#ifdef WIN32
	WSACleanup();
#endif
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
// singleton
WeNetworkDevice *WeNetworkDevice::Instance(void)
{
	if (_instance) return _instance;
	_instance = new WeNetworkDevice();
	return _instance;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
bool WeNetworkDevice::InitializeWinSock(void)
{
	WORD versionRequested;
	WSADATA wsaData;
	DWORD bufferSize = 0;
	LPWSAPROTOCOL_INFO SelectedProtocol;
	int NumProtocols;

	// Start WinSock2. If it fails, we do not need to call WSACleanup()
	versionRequested = MAKEWORD(2, 0);
	int error = WSAStartup(versionRequested, &wsaData);

	if(error) 
	{
		Loger->LogString(LOG_ERROR, "Log: Error WSAStartup");
		return false;
	}
	else
	{
		// Confirm that the WinSock2 DLL supports the exact version
		// we want. If not, call WSACleanup().

		if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
		{			
      	  Loger->LogString(LOG_ERROR, "Log: WinSock2 Dll dont support version we want");
		  WSACleanup();
		  return false;
		}
	}

	// Call WSAEnumProtocols to figure out how big of a buffer we need
	NumProtocols = WSAEnumProtocols(NULL, NULL, &bufferSize);	
	if((NumProtocols != SOCKET_ERROR) && (WSAGetLastError() != WSAENOBUFS))
	{
   	  Loger->LogString(LOG_ERROR, "Log: Error 31823091823");
	  WSACleanup();
	  return false;
	}

	// Allocate a buffer and call WSAEnumProtocols to get an array of
	// WSAPROTOCOL_INFO structs
	SelectedProtocol = (LPWSAPROTOCOL_INFO) malloc(bufferSize);
	if(SelectedProtocol == NULL)
	{
		Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Error Selected Protocol");
		WSACleanup();
		return false;
	}

	// Allocate memory for protocol list and define the protocols to
	// look for
	int protos[2];// = (int *) malloc(2*sizeof(int));
	protos[0] = IPPROTO_TCP;
	protos[1] = IPPROTO_UDP;
	
	NumProtocols = WSAEnumProtocols(protos, SelectedProtocol, &bufferSize);

	//free(protos);
	//protos = NULL;
	free(SelectedProtocol);
	SelectedProtocol = NULL;

	if(NumProtocols == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err == WSAENETDOWN) Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Network subsystem failed");
		if (err == WSANOTINITIALISED) Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Not Initialized");
		if (err == WSAEINVAL) Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Invalid call");
		if (err == WSAENOBUFS) Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Buffer too small");
		if (err == WSAEFAULT) Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Ivalid params address space");


		Loger->LogString(LOG_ERROR, "Log: InitializeWinSock(): Error NumProtocols");
		WSACleanup();
		return false;
	}
	return true;	
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkDevice::SetLogLevel(int LogLevel)
{
	Loger->Start(LogLevel);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Create
//         Input Parametars :  HWND, bool
//         Output Parametars:  bool
//         Comments         :  Create Dialog
//
//////////////////////////////////////////////////////////////////////////
SOCKET WeNetworkDevice::Socket(int Protocol)
{
   int Type;
   int Proto;
   SOCKET Socket;
   int ErrorNum;

   if (Protocol == WENETWORK_PROTOCOL_TCP)
   {
	   Type = SOCK_STREAM;
	   Proto = IPPROTO_TCP;
   } else 
   {
	   Type = SOCK_DGRAM;
	   Proto = IPPROTO_UDP;
   }

   if ((Socket = socket(AF_INET, Type, Proto)) == WESOCK_INVALIDSOCKET)
   {
	   Loger->LogString(LOG_ERROR, "Error: socket(): Error creating socket");
#ifdef WIN32
		ErrorNum = WSAGetLastError();
		Loger->LogString(LOG_ERROR, "Error: socket(): code: %d, error: %s", ErrorNum, strerror(ErrorNum));
#else 
		Loger->LogString(LOG_ERROR, "Error: socket(): error: %s", strerror(errno));
#endif
       return WESOCK_INVALIDSOCKET;
   }
   
   return Socket;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetNonBlocking
//         Input Parametars :  SOCKET s, SetMode
//         Output Parametars:  int
//         Comments         :  SetMode - 1 if non blocking is to be enables, and zero if disabled
//								returns 0 on success
//
//////////////////////////////////////////////////////////////////////////
bool WeNetworkDevice::SetNonBlocking(SOCKET s, u_long SetMode)
{
	int ErrorNum;
#ifdef WIN32
	if (ioctlsocket(s, FIONBIO, &SetMode) != 0)
	{
		ErrorNum = WSAGetLastError();
		Loger->LogString(LOG_ERROR, "Error: SetNonBlocking() : Code:%d, Error:%s", ErrorNum, strerror(ErrorNum));
		return false;
	}
#else
	if (ioctl(s, FIONBIO, &SetMode) != 0)
	{
		Loger->LogString(LOG_ERROR, "Error: SetNonBlocking() : Error:%s", strerror(errno));
		return false;
	}
#endif
	return true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetBroadcasting
//         Input Parametars :  SOCKET s, Mode
//         Output Parametars:  int
//         Comments         :  Mode- TRUE or FALSE
//
//////////////////////////////////////////////////////////////////////////
bool WeNetworkDevice::SetBroadcasting(SOCKET sock, int Mode)
{
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &Mode, sizeof(int)) == -1)
	{
		Loger->LogString(LOG_ERROR, "Error: SetBroadcasting() failed");
#ifdef WIN32
		int err = WSAGetLastError();
		Loger->LogString(LOG_ERROR, "Error: SetBroadcasting(): code:%d, error:%s", err, strerror(err));
		return false;
#else
		Loger->LogString(LOG_ERROR, "Error: SetBroadcasting(): code:%d, error:%s", errno, strerror(errno));
		return false;		

#endif
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetBroadcasting
//         Input Parametars :  SOCKET s, Mode
//         Output Parametars:  int
//         Comments         :  Mode- TRUE or FALSE
//
//////////////////////////////////////////////////////////////////////////
bool WeNetworkDevice::StringToSockaddr(char *str, struct sockaddr *sadr)
{
	if (sadr == NULL) return false;

	char copy[128];	

    struct sockaddr_in *addressPtr = (struct sockaddr_in *) sadr;
    strcpy(copy, str);
    // If the address string begins with a number, assume an IP address
    if(copy[0] >= '0' && copy[0] <= '9')
	{
		addressPtr->sin_addr.s_addr = inet_addr(copy);
		return true;
	}
	else return false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SockaddrToString
//         Input Parametars :  address
//         Output Parametars:  String
//         Comments         :  Converts sockaddr to string
//
//////////////////////////////////////////////////////////////////////////
String WeNetworkDevice::SockaddrToString(struct sockaddr *sadr)
{
	char *copy;	
	String Res;

	if (sadr == NULL) return Res;


    struct sockaddr_in *addressPtr = (struct sockaddr_in *) sadr;

	copy = inet_ntoa(addressPtr->sin_addr);
	if (copy)  Res = copy;
	else Res = "";

	return Res;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  IPToString
//         Input Parametars :  IP
//         Output Parametars:  String
//         Comments         :  Converts IP address to string
//
//////////////////////////////////////////////////////////////////////////
String WeNetworkDevice::IPToString(unsigned long IP)
{
	char *copy;	
	String Res;

	struct sockaddr_in addr;
	addr.sin_addr.s_addr = IP;

	copy = inet_ntoa(addr.sin_addr);
	if (copy)  Res = copy;
	else Res = "";

	return Res;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetHostName
//         Input Parametars :  Socket
//         Output Parametars:  String
//         Comments         :  This returns hostname of current socket. Use only for
//							   UDP sockets because they need this kind of check
//
//////////////////////////////////////////////////////////////////////////
String WeNetworkDevice::GetHostName(SOCKET Socket)
{	
	String HostName;

	// This just connects to 255.255.255.255, reads hostname from socket and 
	// disconnects. Cause this is UDP socket using connect has no impact on application
	// If you find better method then rewrite this

	sockaddr_in tmpaddr;
	memset(&tmpaddr, 0, sizeof(tmpaddr));
	WeNetworkDevice::Instance()->StringToSockaddr("255.255.255.255", (sockaddr *)&tmpaddr);
	tmpaddr.sin_port = 15000; // just dummy port
	tmpaddr.sin_family = AF_INET;
	connect(Socket, (sockaddr *)&tmpaddr, sizeof(tmpaddr));
	
	socklen_t len;
	len = sizeof(tmpaddr);
	getsockname(Socket, (struct sockaddr *) &tmpaddr, &len);
	HostName = inet_ntoa(tmpaddr.sin_addr);
	
	ZeroMemory(&tmpaddr, sizeof(tmpaddr));
	connect(Socket, (sockaddr *)&tmpaddr, sizeof(tmpaddr));

	return HostName;	
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  CloseSocket
//         Input Parametars :  SOCKET s
//         Output Parametars:  N/A
//         Comments         :  Closes opened socket
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkDevice::CloseSocket(SOCKET sock)
{
#ifdef WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SendPacket
//         Input Parametars :  SOCKET s, data, len, saddr
//         Output Parametars:  N/A
//         Comments         :  Sends data to addr using socket and data.
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkDevice::SendPacket(SOCKET sock, char *data, int len, struct sockaddr *to)
{
	int ret;
	ret = sendto(sock, data, len, 0, to, sizeof(sockaddr));


	if (ret == -1) 
	{
#ifdef WIN32
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK) return;
		Loger->LogString(LOG_ERROR, "Error: SendPacket(): code: %d, error: %s", err, strerror(err));
		return;
#else
		if (errno == EWOULDBLOCK) return;
		Loger->LogString(LOG_ERROR, "Error: SendPacket(): code: %d, error: %s", errno, strerror(errno));
		return;
#endif

	}
	return;
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetPacket
//         Input Parametars :  SOCKET s, data, saddr
//         Output Parametars:  N/A
//         Comments         :  Sends data to addr using socket and data.
//
//////////////////////////////////////////////////////////////////////////
int WeNetworkDevice::GetPacket(SOCKET sock, char *data, struct sockaddr *From)
{
    int ret;
	struct sockaddr tempFrom;
	socklen_t FromLen;

	FromLen = sizeof(tempFrom);

	ret = recvfrom(sock, data, 1400, 0, (struct sockaddr *) &tempFrom, &FromLen);

	if (From != NULL)
	{
		* (struct sockaddr *)From = tempFrom;
	}

	if (ret == -1)
	{
#ifdef WIN32
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK) return ret;
		if (err == WSAEMSGSIZE) return ret;

		Loger->LogString(LOG_ERROR, "Error: GetPacket(): code:%d, error:%s", err, strerror(err));
#else
		if (errno == EWOULDBLOCK) return ret;
		if (errno == ECONNREFUSED) return ret;

		Loger->LogString(LOG_ERROR, "Error: GetPacket(): code:%d, error:%s", errno, strerror(errno));
#endif

	}

	
	return ret;

}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Broadcast
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkDevice::Broadcast(SOCKET sock, char *data, int len, int Port)
{
	struct sockaddr_in ServerAddr;
	socklen_t SockLen;

	u_long inetAddr = inet_addr("255.255.255.255");
	memset(&ServerAddr, 0, sizeof(struct sockaddr_in));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.s_addr = inetAddr;

	SockLen = sizeof(ServerAddr);

	int ret = sendto(sock, data, len, 0, (struct sockaddr *) &ServerAddr, SockLen);

	if (ret == -1)
	{
#ifdef WIN32
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK) return;
		Loger->LogString(LOG_ERROR, "Error: Broadcast(): code:%d, error:%s", err, strerror(err));
#else
		if (errno == WSAEWOULDBLOCK) return;
		Loger->LogString(LOG_ERROR, "Error: Broadcast(): code:%d, error:%s", errno, strerror(errno));
#endif
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
SOCKET WeNetworkDevice::OpenUDPSocket(int Port, char *NetInterface)
{ 
	 SOCKET sock;
     struct sockaddr_in Address;
	 	 
	 memset(&Address, 0, sizeof(Address));

	 sock = Socket(WENETWORK_PROTOCOL_UDP);
	 if (sock == WESOCK_INVALIDSOCKET) return sock;

	 SetNonBlocking(sock, 1);
	 SetBroadcasting(sock, 1);
	 
	 // if we dont have local net address to bing use any
	 if (!NetInterface)
	 {	
		 Loger->LogString(LOG_NOTICE, "Notice: OpenUDPSocket(): NetInterface not supplied using any local address");
		 Address.sin_addr.s_addr = htonl(INADDR_ANY);
	 } else
	 {
		 Loger->LogString(LOG_NOTICE, "Notice: OpenUDPSocket(): Using netInterface '%s'", NetInterface);
		 StringToSockaddr(NetInterface, (struct sockaddr *) &Address);
	 }

	 if (Port == 0)
	 {
		 Loger->LogString(LOG_NOTICE, "Notice: OpenUDPSocket(): Port not supplied. Picking one automaticly");
		 Address.sin_port = 0;
	 } else
	 {
		 Address.sin_port = htons(Port);
	 }
	 Address.sin_family = AF_INET;

	 if (bind(sock, (struct sockaddr *) &Address, sizeof(Address)) == -1)
	 {
#ifdef WIN32
		 int err = WSAGetLastError();
		 Loger->LogString(LOG_ERROR, "Error: OpenUDPSocket(): Binderror, code:%d, error:%s", err, strerror(err));
#else
		 Loger->LogString(LOG_ERROR, "Error: OpenUDPSocket(): Binderror, code:%d, error:%s", errno, strerror(errno));
#endif
		 return WESOCK_INVALIDSOCKET;
	 }

	 socklen_t len;
	 len = sizeof(Address);
	 getsockname(sock, (struct sockaddr *) &Address, &len);

	 Loger->LogString(LOG_INFO, "Info: Socket opened, UDP = %d", ntohs(Address.sin_port)); 

	 return sock;
}






/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenTCPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  Opens TCP socket
//
//////////////////////////////////////////////////////////////////////////
SOCKET WeNetworkDevice::OpenTCPSocket(int Port, char *NetInterface)
{ 
	 SOCKET sock;
     struct sockaddr_in Address;
	 	 
	 memset(&Address, 0, sizeof(Address));

	 sock = Socket(WENETWORK_PROTOCOL_TCP);
	 if (sock == WESOCK_INVALIDSOCKET) return sock;

	 if (!SetNonBlocking(sock, 1)) return WESOCK_INVALIDSOCKET;
	 if (!SetBroadcasting(sock, 1)) return WESOCK_INVALIDSOCKET;
	 
	 
	 // if we dont have local net address to bing use any
	 if (!NetInterface)
	 {	
		 Loger->LogString(LOG_NOTICE, "Notice: OpenTCPSocket(): NetInterface not supplied using any local address");
		 Address.sin_addr.s_addr = htonl(INADDR_ANY);
	 } else
	 {
		 Loger->LogString(LOG_NOTICE, "Notice: OpenTCPSocket(): Using netInterface '%s'", NetInterface);
		 StringToSockaddr(NetInterface, (struct sockaddr *) &Address);
	 }

	 if (Port == 0)
	 {
		 Loger->LogString(LOG_NOTICE, "Notice: OpenTCPSocket(): Port not supplied. Picking one automaticly");
		 Address.sin_port = 0;
	 } else
	 {
		 Address.sin_port = htons(Port);
	 }
	 Address.sin_family = AF_INET;

	 if (bind(sock, (struct sockaddr *) &Address, sizeof(Address)) == -1)
	 {
#ifdef WIN32
		 int err = WSAGetLastError();
		 Loger->LogString(LOG_ERROR, "Error: OpenTCPSocket(): Binderror, code:%d, error:%s", err, strerror(err));
#else
		 Loger->LogString(LOG_ERROR, "Error: OpenTCPSocket(): Binderror, code:%d, error:%s", errno, strerror(errno));
#endif
		 return WESOCK_INVALIDSOCKET;
	 }

	 socklen_t len;
	 len = sizeof(Address);
	 getsockname(sock, (struct sockaddr *) &Address, &len);

	 Loger->LogString(LOG_INFO, "Info: Socket opened, TCP = %d", ntohs(Address.sin_port)); 

	 return sock;
}

