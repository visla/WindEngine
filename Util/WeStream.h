/*
     WeStream.h

	 Autor: Nebojsa Vislavski
	 Date: 13.9.2005
	 Version: 1.0

     This is main abstract stream. If you want to create stream inherit this.
*/ 

#ifndef _WESTREAM_H
#define _WESTREAM_H

#include <CString.h>
#include <WindEngine.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WeQuaternion.h>

#define NULL	0

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;


class WINDENGINE_API WeStream
{
public:
	WeStream();
	virtual ~WeStream(){};
	bool	IsOpened;

	// Reading methods
	virtual	u_char	ReadByte() = 0;
	virtual u_short	ReadWord() = 0;
	virtual int		ReadInt() = 0;
	virtual u_int	ReadDWord() = 0;
	virtual float	ReadFloat() = 0;
	virtual double	ReadDouble() = 0;
	virtual String	ReadString() = 0;
	virtual char	ReadChar() = 0;
	virtual bool	ReadBool() = 0;
	virtual void*	ReadData(void *data, int size) = 0;
	virtual void	ReadVector(WeVector &v) = 0;
	virtual void	ReadMatrix(WeMatrix &m) = 0;
	virtual void	ReadQuaternion(WeQuaternion &Quat) = 0;

    // Writing methods
	virtual void	WriteByte(u_char b) = 0;
	virtual void	WriteWord(u_short s) = 0;
	virtual void	WriteInt(int i) = 0;
	virtual void	WriteDWord(u_int dw) = 0;
	virtual void	WriteFloat(float f) = 0;
	virtual void	WriteDouble(double d) = 0;
	virtual void	WriteString(StringParam s) = 0;
	virtual void	WriteChar(char c) = 0;
	virtual void	WriteBool(bool b) = 0;
	virtual void	WriteData(void *data, int size) = 0;
	virtual	void	WriteVector(WeVector &v) = 0;
	virtual	void	WriteMatrix(WeMatrix &m) = 0;
	virtual void	WriteQuaternion(WeQuaternion &Quat) = 0;
	

};

#endif