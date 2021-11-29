/*
    WeFileStream.h

	Autor: Nebojsa Vislavski
	Date: 13.9.2005.
	Version: 1.0

	This is file stream. Use this to read and write to files.

*/


#ifndef _WEFILESTREAM_H
#define _WEFILESTREAM_H

#include <WeStream.h>
#include <stdio.h>
#include <CStaticString.h>
#include <WindEngine.h>
#include <WeColor.h>


class WINDENGINE_API WeFileStream: public WeStream
{
protected:

public:

	 bool	Exists(StringParam FileName);
	 bool	Append(StringParam FileName);
	 bool	Open(StringParam FileName);
	 bool	OpenNew(StringParam FileName);
	 void	Close();
	 void	Seek(long offset, int Seek = SEEK_SET); // SEEK_SET, SEEK_CUR, SEEK_END
	 long	Tell() { return ftell(f); }
	 long	GetSize();	 
	 int	Eof() { return feof(f); }

	 FILE		*f;
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
	 void		ReadStringName(StringName &s);
	 void		ReadVector(WeVector &v);
	 void		ReadMatrix(WeMatrix &m);
	 void		ReadColor(WeColor &col);
	 void		ReadQuaternion(WeQuaternion &Quat);

    // Writing methods
	 void	WriteByte(u_char b);
	 void	WriteWord(u_short s);
	 void	WriteInt(int i);
	 void	WriteDWord(u_int dw);
	 void	WriteFloat(float f);
	 void	WriteDouble(double d);
	 void	WriteString(StringParam s);
	 void	WriteChar(char c);
	 void	WriteBool(bool b);
	 void	WriteData(void *data, int size);
	 void	WriteStringName(StringName &s);
	 void	WriteVector(WeVector &v);
	 void	WriteMatrix(WeMatrix &m);
	 void	WriteColor(WeColor &col);
	 void	WriteQuaternion(WeQuaternion &Quat);

};


#endif