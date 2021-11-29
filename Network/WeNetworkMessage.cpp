/*
    WeNetworkMessage.cpp

	Autor: Nebojsa Vislavski
	Date: 14.9.2005.
	Version: 1.0

    This class is used to make network message. 
*/

#include <WeNetworkMessage.h>
#include <malloc.h>

const int StartOffset = 9;

void WeNetworkMessage::Init(int maxsize)
{
	Overflow = false;
	Remote = true;
	MaxSize = maxsize+StartOffset; // 9 more bytes for IS, OS and NetworkID, Type
	ReadCount = StartOffset;
	Size = StartOffset;
}


void WeNetworkMessage::Clear()
{
	Overflow = false;
	ReadCount = StartOffset;
	Size = StartOffset;
}


void WeNetworkMessage::BeginReading()
{
	bEof = false;
	ReadCount = StartOffset;
}


u_char WeNetworkMessage::ReadByte()
{
	u_char c;	
	if (ReadCount + 1 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 1);

	ReadCount += 1;
	return c;
}



u_short WeNetworkMessage::ReadWord()
{
	u_short c;	
	if (ReadCount + 2 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 2);

	ReadCount += 2;
	return c;
}

u_int WeNetworkMessage::ReadDWord()
{
	u_int c;	
	if (ReadCount + 4 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 4);

	ReadCount += 4;
	return c;
}

int WeNetworkMessage::ReadInt()
{
	int c;	
	if (ReadCount + 4 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 4);

	ReadCount += 4;
	return c;
}

char WeNetworkMessage::ReadChar()
{
	char c;	
	if (ReadCount + 1 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 1);

	ReadCount += 1;
	return c;
}

bool WeNetworkMessage::ReadBool()
{
	bool c;	
	if (ReadCount + 1 > Size) c = (bool)-1;
	else memcpy(&c, &Data[ReadCount], 1);

	ReadCount += 1;
	return c;
}

float WeNetworkMessage::ReadFloat()
{
	float c;	
	if (ReadCount + 4 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 4);

	ReadCount += 4;
	return c;
}

double WeNetworkMessage::ReadDouble()
{
	double c;	
	if (ReadCount + 8 > Size) c = -1;
	else memcpy(&c, &Data[ReadCount], 8);

	ReadCount += 8;
	return c;

}

String WeNetworkMessage::ReadString()
{
	u_short size;
	char *s;
	String res;

	size = ReadWord(); // read string size.
	if (size == -1) return res;
	size++;

    if (ReadCount + size <= Size)
	{
		s = (char *)malloc(sizeof(char)*size);
		ReadData(s, size);
		res = s;
		free(s);	
	}
	return res;
}

void WeNetworkMessage::ReadVector(WeVector &Vector)
{
	if (ReadCount + 12 > Size) return;
	else memcpy(&Vector, &Data[ReadCount], 12);

	ReadCount += 12;	
}

void WeNetworkMessage::ReadMatrix(WeMatrix &Matrix)
{
	int MatSize = (int)sizeof(WeMatrix);

	if((ReadCount + MatSize) > Size) return;
	else memcpy(&Matrix, &Data[ReadCount], MatSize);

	ReadCount += MatSize;
}

void WeNetworkMessage::ReadQuaternion(WeQuaternion &Quat)
{
	int MatSize = (int)sizeof(WeQuaternion);

	if((ReadCount + MatSize) > Size) return;
	else memcpy(&Quat, &Data[ReadCount], MatSize);

	ReadCount += MatSize;
}


void *WeNetworkMessage::ReadData(void *data, int size)
{
	if (ReadCount + size <= Size)
	{
	  memcpy(data, &Data[ReadCount], size);
	}
	ReadCount += size;

	return data;
}

WeNetworkMessageType WeNetworkMessage::ReadMessageType()
{
	WeNetworkMessageType c;	
	//if (ReadCount + 1 > Size) c = -1;
	//else memcpy(&c, &Data[8], sizeof(c));
	//ReadCount += 1;
	//return c;

	memcpy(&c, &Data[8], 1);
	return c;
}

WeNetworkActorID WeNetworkMessage::ReadNetworkActorID()
{
	WeNetworkActorID c;	
	//if (ReadCount + sizeof(c) > Size) c = -1;
	//else memcpy(&c, &Data[ReadCount], sizeof(c));

	//ReadCount += 4;
	//return c;

	memcpy(&c, &Data[4], 4);
	return c;
}

void WeNetworkMessage::ReadSequences(u_short &Outgoing, u_short &Incoming)
{
	memcpy(&Outgoing, &Data[0], 2);
	memcpy(&Incoming, &Data[2], 2);	
}


char *WeNetworkMessage::GetNewPoint(int length)
{
	char *tmpData;

	if (Size + length > MaxSize)
	{
		Overflow = true;
		Clear(); // to start over again
	}

	tmpData = Data + Size;
	Size += length;

	return tmpData;
}

void  WeNetworkMessage::WriteByte(u_char b)
{
	char *buf;
	buf = GetNewPoint(1);
	memcpy(buf, &b, 1);
}

void  WeNetworkMessage::WriteWord(u_short s)
{
	char *buf;
	buf = GetNewPoint(2);
	memcpy(buf, &s, 2);
}


void  WeNetworkMessage::WriteInt(int i)
{
	char *buf;
	buf = GetNewPoint(4);
	memcpy(buf, &i, 4);
}

void  WeNetworkMessage::WriteDWord(u_int dw)
{
	char *buf;
	buf = GetNewPoint(4);
	memcpy(buf, &dw, 4);
}

void  WeNetworkMessage::WriteFloat(float fl)
{
	char *buf;
	buf = GetNewPoint(4);
	memcpy(buf, &fl, 4);
}


void WeNetworkMessage::WriteVector(WeVector &Vector)
{
	char *buf;
	buf = GetNewPoint(12);
	memcpy(buf, &Vector, 12);
}




void WeNetworkMessage::WriteMatrix(WeMatrix &Matrix)
{
	int MatSize = (int)sizeof(WeMatrix);

	char *buf;
	buf = GetNewPoint(MatSize);
	memcpy(buf, &Matrix, MatSize);
}

void WeNetworkMessage::WriteQuaternion(WeQuaternion &Quat)
{
	int MatSize = (int)sizeof(WeQuaternion);

	char *buf;
	buf = GetNewPoint(MatSize);
	memcpy(buf, &Quat, MatSize);
}


void  WeNetworkMessage::WriteDouble(double d)
{
	char *buf;
	buf = GetNewPoint(sizeof(double));
	memcpy(buf, &d, sizeof(double));
}

void WeNetworkMessage::WriteString(StringParam s)
{
	char *buf;
	u_short len = s.GetLen();

	buf = GetNewPoint(2);
	memcpy(buf, &len, 2);

	WriteData((void *)s.GetString(), len+1);
}

void WeNetworkMessage::WriteChar(char c)
{
	char *buf;
	buf = GetNewPoint(1);
	memcpy(buf, &c, 1);
}

void WeNetworkMessage::WriteBool(bool b)
{
	char *buf;
	buf = GetNewPoint(sizeof(bool));
	memcpy(buf, &b, sizeof(bool));
}

void WeNetworkMessage::WriteMessageType(WeNetworkMessageType Type)
{
	//char *buf;
	//buf = GetNewPoint(sizeof(WeNetworkMessageType));
	memcpy(&Data[8], &Type, 4);
}

void WeNetworkMessage::WriteNetworkActorID(WeNetworkActorID Id)
{
	//char *buf;
	//buf = GetNewPoint(sizeof(WeNetworkActorID));
	memcpy(&Data[4], &Id, 4);
}

void  WeNetworkMessage::WriteData(void *data, int size)
{
	char *buf;
	buf = GetNewPoint(size);
	memcpy(buf, data, size);
}

void WeNetworkMessage::WriteSequences(u_short Outgoing, u_short Incoming)
{
	memcpy(&Data[0], &Outgoing, 2);
	memcpy(&Data[2], &Incoming, 2);
}