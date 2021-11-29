/*
    WeNetworkMessage.h

	Autor: Nebojsa Vislavski
	Date: 14.9.2005.
	Version: 1.0

    This class is used to make network message. 
*/

#ifndef _WENETWORKMESSAGE_H
#define _WENETWORKMESSAGE_H

#include <WeStream.h>
#include <WindEngine.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WeNetworkDevice.h>

#define MAX_NETMESSAGE_LEN 1400


typedef char WeNetworkMessageType; // if you change this be sure to change read and write function also
typedef int  WeNetworkActorID; // if you change this be sure to change read and write function also

class WeClient;

class WINDENGINE_API WeNetworkMessage: public WeStream
{
protected:
	int		ReadCount;
	bool	Overflow;
	int		MaxSize;
	bool	bEof; // indicates end of message

	char *GetNewPoint(int length);


public:
	WeNetworkMessage(): Remote(true), Reliable(false), Sequenced(true) { }

	 bool		Reliable; // it this message reliable. Network Device responsible for sending msgs knows how to deal with reliable msgs
	 bool		Sequenced; // should this message be sequenced;	 
	 bool		Remote;    // this is remote msg meaning it should go to another application
	 sockaddr   FromAddr;	// can be null if it is local or message named for sending

	 char		Data[MAX_NETMESSAGE_LEN];
	 int		Size;
	 void		SetSize(int s) { Size = s; }

	 void		Init(int maxsize = MAX_NETMESSAGE_LEN);
	 void		Clear();
	 void		BeginReading(); // Users of this engine should call this very rarely in special situations
	 bool		Eof() { return ReadCount >= Size; }
	 bool		GetOverflow() { return Overflow; }


	// Reading methods
	 u_char		ReadByte();
	 u_short	ReadWord();
	 int		ReadInt();
	 u_int		ReadDWord();
	 float		ReadFloat();
	 double		ReadDouble();
	 String		ReadString();
	 char		ReadChar();
	 bool		ReadBool();
	 void*		ReadData(void *data, int size);
	 WeNetworkMessageType  ReadMessageType();
	 WeNetworkActorID	   ReadNetworkActorID();
	 void		ReadVector(WeVector &Vector);
	 void		ReadMatrix(WeMatrix &m);
	 void		ReadSequences(u_short &Outgoing, u_short &Incoming);
	 void		ReadQuaternion(WeQuaternion &Quat);

	 

    // Writing methods
	 void		WriteMessageType(WeNetworkMessageType Type);
	 void		WriteByte(u_char b);
	 void		WriteWord(u_short s);
	 void		WriteInt(int i);
	 void		WriteDWord(u_int dw);
	 void		WriteFloat(float f);
	 void		WriteDouble(double d);
	 void		WriteString(StringParam s);
	 void		WriteChar(char c);
	 void		WriteBool(bool b);
	 void		WriteData(void *data, int size);
	 void		WriteNetworkActorID(WeNetworkActorID Id);
	 void		WriteVector(WeVector &Vector);
	 void		WriteMatrix(WeMatrix &m);
	 void		WriteSequences(u_short Outgoing, u_short Incoming); // this should be called last
	 void		WriteQuaternion(WeQuaternion &Quat);
};


#endif