/*
    WeTextStream.cpp

	Autor: Nebojsa Vislavski
	Date: 24.3.2006.
	Version: 1.0

	This is file stream. Use this to read and write to files.

*/

#include <WeTextStream.h>
#include <malloc.h>



bool WeTextStream::Exists(StringParam FileName)
{
	FILE *tmp;
	tmp = fopen(FileName, "rt");
	if (!tmp) return false;
	fclose(tmp);
	return true;
}

bool WeTextStream::Append(StringParam FileName)
{
	if (IsOpened) Close();

	f = fopen(FileName, "at+");
	if (!f) IsOpened = false;
	else IsOpened = true;
	    
	return IsOpened;
}



bool WeTextStream::Open(StringParam FileName)
{
	if (IsOpened) Close();

	f = fopen(FileName, "rt+");
	if (!f) IsOpened = false;
	else IsOpened = true;

	return IsOpened;
}

long WeTextStream::GetSize()
{
    long s, tmp;
	tmp = Tell();
	Seek(0, SEEK_END);
	s = Tell();
	Seek(tmp);
	return s;	
}

bool WeTextStream::OpenNew(StringParam FileName)
{
	if (IsOpened) Close();
	f = fopen(FileName, "wt+");
	if (!f) IsOpened = false;
	else IsOpened = true;

	return IsOpened;
}


void WeTextStream::Close()
{
	if (f) fclose(f);
	IsOpened = false;
}

void  WeTextStream::Seek(long offset, int Seek)
{
//	fseek(f, offset, Seek);
}


u_char WeTextStream::ReadByte()
{
	u_char c;	
	fscanf(f, "%c", &c);
	return c;
}

u_short WeTextStream::ReadWord()
{
	u_short c;
	fscanf(f, "%d", &c);
	return c;
}

u_int WeTextStream::ReadDWord()
{
	u_int c;
	fscanf(f, "%d", &c);
	return c;
}

int WeTextStream::ReadInt()
{
	int c;
	fscanf(f, "%d", &c);
	return c;
}

char WeTextStream::ReadChar()
{
	char c;
	fscanf(f, "%c", &c);
	return c;
}

bool WeTextStream::ReadBool()
{
	
	char tmp[10];
	String s;

	fscanf(f, "%s", tmp);
	s = tmp;
	if (s == (String)"true") return true;
	else return false;
}

float WeTextStream::ReadFloat()
{
	float c;
	fscanf(f, "%f", &c);
	return c;
}

double WeTextStream::ReadDouble()
{
	double c;
	fscanf(f, "%f", &c);
	return c;
}

String WeTextStream::ReadString()
{
	String res;
	char tmp[500];
	fscanf(f, "%s", tmp);
	res = tmp;
	return res;
}

void *WeTextStream::ReadData(void *data, int size)
{
	if (size == 0) return NULL;

	for (int i = 0; i < size; i++)
	{
		fscanf(f, "%c", ((char *)(data))[i]);
	}
	return data;
}

void WeTextStream::ReadStringName(StringName &s)
{
	//fread(s, sizeof(s), 1, f);
}


void  WeTextStream::WriteByte(u_char b)
{
	fprintf(f, "%d", b);
}

void  WeTextStream::WriteWord(u_short s)
{
	fprintf(f, "%d", s);	
}


void  WeTextStream::WriteInt(int i)
{
	fprintf(f, "%d", i);
}

void  WeTextStream::WriteDWord(u_int dw)
{
	fprintf(f, "%d", dw);
}

void  WeTextStream::WriteFloat(float fl)
{
	fprintf(f, "%4.4f", fl);
}

void  WeTextStream::WriteDouble(double d)
{
	fprintf(f, "%4.4f", d);
}

void WeTextStream::WriteString(StringParam s)
{
	fprintf(f, "%s", s.GetString());
}

void WeTextStream::WriteChar(char c)
{
	fprintf(f, "%c" , c);
}

void WeTextStream::WriteBool(bool b)
{
	if (b)
		fprintf(f, "true");
	else 
		fprintf(f, "false");
}

void  WeTextStream::WriteData(void *data, int size)
{
	for (int i = 0; i < size; i++)
	{
		fprintf(f, "%d", ((char *)data)[i]);
	}
}

void  WeTextStream::WriteStringName(StringName &s)
{
//	fwrite(s, sizeof(s), 1, f);
}

void WeTextStream::WriteMatrix(WeMatrix &m)
{
	fwrite(&m, sizeof(WeMatrix), 1, f);
}

void WeTextStream::ReadMatrix(WeMatrix &m)
{
	fread(&m, sizeof(WeMatrix), 1, f);

}

void WeTextStream::WriteVector(WeVector &v)
{
	fwrite(&v, sizeof(WeVector), 1, f);
}

void WeTextStream::ReadVector(WeVector &v)
{
	fread(&v, sizeof(WeVector), 1, f);
}

void WeTextStream::WriteColor(WeColor &col)
{
	fwrite(&col, sizeof(WeColor), 1, f);
}

void WeTextStream::ReadColor(WeColor &col)
{
	fread(&col, sizeof(WeColor), 1, f);
}

void WeTextStream::ReadQuaternion(WeQuaternion &Quat)
{
}

void WeTextStream::WriteQuaternion(WeQuaternion &Quat)
{
}
