/*
    WeFileStream.cpp

	Autor: Nebojsa Vislavski
	Date: 13.9.2005.
	Version: 1.0

	This is file stream. Use this to read and write to files.

*/

#include <WeFileStream.h>
#include <malloc.h>



bool WeFileStream::Exists(StringParam FileName)
{
	FILE *tmp;
	tmp = fopen(FileName, "rb");
	if (!tmp) return false;
	fclose(tmp);
	return true;
}

bool WeFileStream::Append(StringParam FileName)
{
	if (IsOpened) Close();

	f = fopen(FileName, "ab+");
	if (!f) IsOpened = false;
	else IsOpened = true;
	    
	return IsOpened;
}



bool WeFileStream::Open(StringParam FileName)
{
	if (IsOpened) Close();

	f = fopen(FileName, "rb+");
	if (!f)
	{
		IsOpened = false;
	}
	else
	{
		IsOpened = true;

		// seek to begining
		fseek(f, 0, SEEK_SET);
	}
	return IsOpened;
}

long WeFileStream::GetSize()
{
    long s, tmp;
	tmp = Tell();
	Seek(0, SEEK_END);
	s = Tell();
	Seek(tmp);
	return s;	
}

bool WeFileStream::OpenNew(StringParam FileName)
{
	if (IsOpened) Close();
	f = fopen(FileName, "wb+");
	if (!f) IsOpened = false;
	else IsOpened = true;

	return IsOpened;
}


void WeFileStream::Close()
{
	if (f) fclose(f);
	IsOpened = false;
}

void  WeFileStream::Seek(long offset, int Seek)
{
	fseek(f, offset, Seek);
}


u_char WeFileStream::ReadByte()
{
	u_char c;	
	fread(&c, sizeof(u_char), 1, f);
	return c;
}

u_short WeFileStream::ReadWord()
{
	u_short c;
	fread(&c, sizeof(u_short), 1, f);
	return c;
}

u_int WeFileStream::ReadDWord()
{
	u_int c;
	fread(&c, sizeof(u_int), 1, f);
	return c;
}

int WeFileStream::ReadInt()
{
	int c;
	fread(&c, sizeof(int), 1, f);
	return c;
}

char WeFileStream::ReadChar()
{
	char c;
	fread(&c, sizeof(char), 1, f);
	return c;
}

bool WeFileStream::ReadBool()
{
	bool c;
	fread(&c, sizeof(bool), 1, f);
	return c;
}

float WeFileStream::ReadFloat()
{
	float c;
	fread(&c, sizeof(float), 1, f);
	return c;
}

double WeFileStream::ReadDouble()
{
	double c;
	fread(&c, sizeof(double), 1, f);
	return c;

}

String WeFileStream::ReadString()
{
	u_short size;
	char *s;
	String res;

	fread(&size, sizeof(u_short), 1, f);
	if (size == 0) return res;
	size++;

	s = (char *)malloc(sizeof(char)*size);
	fread(s, sizeof(char), size, f);
	res = s;
	free(s);	
	return res;
}

void *WeFileStream::ReadData(void *data, int size)
{
	if (size == 0) return NULL;

	fread(data, sizeof(u_char), size, f);
	return data;
}

void WeFileStream::ReadStringName(StringName &s)
{
	fread(s, sizeof(s), 1, f);
}


void  WeFileStream::WriteByte(u_char b)
{
	fwrite(&b, sizeof(u_char), 1, f);
}

void  WeFileStream::WriteWord(u_short s)
{
	fwrite(&s, sizeof(u_short), 1, f);
}


void  WeFileStream::WriteInt(int i)
{
	fwrite(&i, sizeof(int), 1, f);
}

void  WeFileStream::WriteDWord(u_int dw)
{
	fwrite(&dw, sizeof(u_int), 1, f);
}

void  WeFileStream::WriteFloat(float fl)
{
	fwrite(&fl, sizeof(float), 1, f);
}

void  WeFileStream::WriteDouble(double d)
{
	fwrite(&d, sizeof(double), 1, f);

}

void WeFileStream::WriteString(StringParam s)
{
	u_short len = s.GetLen();	
	fwrite(&len, sizeof(u_short), 1, f);
	if (len > 0)
		fwrite(s.GetString(), sizeof(u_char), len+1, f);
	char c = 0;	
}

void WeFileStream::WriteChar(char c)
{
	fwrite(&c, sizeof(char), 1, f);
}

void WeFileStream::WriteBool(bool b)
{
	fwrite(&b, sizeof(bool), 1, f);
}

void  WeFileStream::WriteData(void *data, int size)
{
	fwrite(data, sizeof(u_char), size, f);
}

void  WeFileStream::WriteStringName(StringName &s)
{
	fwrite(s, sizeof(s), 1, f);
}

void WeFileStream::WriteMatrix(WeMatrix &m)
{
	fwrite(&m, sizeof(WeMatrix), 1, f);
}

void WeFileStream::WriteQuaternion(WeQuaternion &Quat)
{
	fwrite(&Quat, sizeof(WeQuaternion), 1, f);
}


void WeFileStream::ReadMatrix(WeMatrix &m)
{
	fread(&m, sizeof(WeMatrix), 1, f);

}

void WeFileStream::WriteVector(WeVector &v)
{
	fwrite(&v, sizeof(WeVector), 1, f);
}

void WeFileStream::ReadVector(WeVector &v)
{
	fread(&v, sizeof(WeVector), 1, f);
}

void WeFileStream::WriteColor(WeColor &col)
{
	fwrite(&col, sizeof(WeColor), 1, f);
}

void WeFileStream::ReadColor(WeColor &col)
{
	fread(&col, sizeof(WeColor), 1, f);
}

void WeFileStream::ReadQuaternion(WeQuaternion &Quat)
{
	fread(&Quat, sizeof(WeQuaternion), 1, f);
}
