/*

	CString.cpp

	Author: Nebojsa Vislavski
	Version: 1.2
	Date: 18.4.2005

	This is "simple" CString class

*/

#define _CSTRING_LIBBUILD

#include <CString.h>


char EmptyStr[1] = {'\0'};


String::String()
{
	//wcCString = new char[1];
	//wcCString[0] = '\0';
	wcCString = NULL;
	Len = 0;
}


String::String(int len)
{
	Len = len;
	if (Len > 0)
	{
		wcCString = new char[len+1];
		for (int i = 0; i <= len; i++) wcCString[i] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}

String::String(const String &s)
{ 
	Len = s.GetLen();
	if (Len > 0)
	{
		wcCString = new char[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = s[i];
		wcCString[Len] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}


String::String(const char * const str)
{ 
	Len = (int)strlen(str);
	if (Len > 0)
	{
		wcCString = new char[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = str[i];
		wcCString[Len] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}


String& String::operator= (const String &s)
{ 
	if (this == &s) return *this;
	if (wcCString)
	{
		delete[] wcCString;
		wcCString = NULL;
	}

	Len = s.GetLen();
	if (Len > 0)
	{
		wcCString = new char[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = s[i];
		wcCString[Len] = '\0';
	}
	return *this;
}

String::~String()
{
	if (wcCString)
	{
		delete[] wcCString;
		wcCString = NULL;
	}
	Len = 0;
}


bool String::operator ==(const String &s)
{
	// if they are same size
	if (Len != s.GetLen()) return false;
	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] != s[i]) return false;
	}
	return true;
}

bool String::operator !=(const String &s)
{
	if (Len != s.GetLen()) return true;
		for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] != s[i]) return true;
	} 
	return false;
}



char& String::operator [](int offset)
{
	if (Len == 0) return *((char*)NULL);
	else if (offset > Len) return wcCString[Len];
	else return wcCString[offset];
}

char String::operator [](int offset) const
{
	if (Len == 0) return '\0';
	else if (offset > Len) return wcCString[Len];
	else return wcCString[offset];
}


void String::operator +=(const String &s)
{
	int totallen = Len + s.GetLen();
	String tmp(totallen);
	int i,j;
	for (i = 0; i < Len; i++) tmp[i] = wcCString[i];
	for (j = 0; j < s.GetLen(); j++, i++) tmp[i] = s[j];
	if (totallen > 0)
		tmp[totallen] = '\0';
	*this = tmp;
} 


void String::operator +=(const char &c)
{	
	Grow(Len+1);
	wcCString[Len-1] = c;
	wcCString[Len] = '\0';
} 


String String::operator +(const String &s) const
{
	int totallen = Len + s.GetLen();
	String tmp(totallen);
	int i,j;
	for (i = 0; i < Len; i++) tmp[i] = wcCString[i];
	for (j = 0; j < s.GetLen(); j++, i++) tmp[i] = s[j];
	if (totallen > 0)
		tmp[totallen] = '\0';
	return tmp;
}


bool String::Grow(int NewSize)
{
	char *tmpBuffer;
	tmpBuffer = new char[NewSize+1];

	if (!tmpBuffer) return false;

	if (Len > 0)
		CopyMemory(tmpBuffer, wcCString, (Len+1) * sizeof(char));
	if (wcCString)
		delete[] wcCString;

	wcCString = tmpBuffer;
	Len = NewSize;
	return true;
}



String::operator char *()
{
	return (wcCString ? wcCString : EmptyStr);
}


String::operator const char *() const
{
	return (wcCString ? wcCString : EmptyStr);
}


const char *String::GetString() const
{
	return (wcCString ? wcCString : EmptyStr);
}


bool String::InsertChar(int Index, char c)
{
	if (Index < 0 || Index > Len) return false;

	if (!Grow(Len+1)) return false;

	MoveMemory(wcCString + Index + 1, wcCString + Index, sizeof(char)* (Len-Index));
	wcCString[Index] = c;
	return true;
}


bool String::RemoveChar(int Index)
{ 
	if (Index < 0 || Index >= Len) return false;

	MoveMemory(wcCString+Index, wcCString+Index+1,sizeof(char)* (Len-Index));
	Len--;

	if (Len == 0)
	{
		delete[] wcCString;
		wcCString = NULL;
	}

	return true;
} 


bool String::InsertString(int Index, char *str)
{
	if (Index < 0 || Index > Len) return false;
	if (!str) return false;

	int _strlen = (int)strlen(str);
	if (!Grow(Len + _strlen)) return false;

	MoveMemory(wcCString+Index+_strlen, wcCString+Index, sizeof(char)* (Len-_strlen-Index));
	CopyMemory(wcCString+Index, str, sizeof(char)*_strlen);
	return true;
}


bool String::AsBool()
{
	if (!wcCString) return false;
	char *pwc;
	pwc = strstr(wcCString, "true");
	if (pwc) return true;
	pwc = strstr(wcCString, "TRUE");
	if (pwc) return true; 
	return false;
}

int String::AsInt()
{
	if (!wcCString) return 0;
	int tmp; 
	sscanf(wcCString, "%d", &tmp);
	return tmp;
}

float String::AsFloat()
{
	if (!wcCString) return 0.0f;
	float tmp;
	sscanf(wcCString, "%f", &tmp);
	return tmp;
}


void String::AsChar(char *&outc)
{
	if (outc)
		delete outc;
	outc = new char[GetLen()+1]; 
	int i;
	for (i = 0; i < GetLen(); i++)
	{
		outc[i] = wcCString[i];
	}
	outc[i] = '\0';
}


String String::Extract(char Char, int PartNum)
{
	int start, end, partnum=-1;
	String tmpS;

	if (PartNum < 0) return tmpS;
 
	// nadjemo prvo part koji nas zanima i vidimo koliko je dugacak
	start = 0;
	end = -1;

	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] == Char)
		{
			partnum++;
			if (partnum == PartNum) 
			{				 
				tmpS.Grow(end-start+1);
				CopyMemory(tmpS.wcCString, &wcCString[start], sizeof(char)*(end-start+1));				 
				return tmpS;
			} else
			{ 
				start = i+1;
				end = i;
			}
		} else end++;
	}

	partnum++;
	if (partnum == PartNum) 
	{
		end = Len;
		tmpS.Grow(end-start);
		if (Len > 0)
			CopyMemory(tmpS.wcCString, &wcCString[start], sizeof(char)*(end-start));
		return tmpS;
	} else return tmpS; 
}


int String::SubPos(StringParam sub)
{
	bool good;

	if (sub.Len == 0)
	{
		return 0;
	}

	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] == sub.wcCString[0])
		{
			good = true;

			for (int j = 0; j < sub.Len; j++)
			{
				if (wcCString[i + j] != sub.wcCString[j])
				{
					good = false;
					break;
				}
			}

			if (good)
			{
				return i;
			}
		}
	}

	return -1;
}


int String::SubPosCaseInsensitive(StringParam sub)
{
	bool good;

	if (sub.Len == 0)
	{
		return 0;
	}

	for (int i = 0; i <= Len - sub.Len; i++)
	{
		good = true;

		for (int j = 0; j < sub.Len; j++)
		{
			char ch1 = wcCString[i + j];
			if ((ch1 >= 'a') && (ch1 <= 'z'))
			{
				ch1 += ('A' - 'a');
			}

			char ch2 = sub.wcCString[j];
			if ((ch2 >= 'a') && (ch2 <= 'z'))
			{
				ch2 += ('A' - 'a');
			}

			if (ch1 != ch2)
			{
				good = false;
				break;
			}
		}

		if (good)
		{
			return i;
		}
	}

	return -1;
}


bool String::EqualCaseInsensitive(StringParam sub)
{
	return ((Len == sub.GetLen()) && (SubPosCaseInsensitive(sub) == 0));
}


String String::Sub(int iFirst, int iLast)
{
	if (iFirst < 0)
		iFirst = 0;
	if (iLast > Len)
		iLast = Len;

	int len = iLast - iFirst;
	if (len < 0)
		len = 0;

	String Result(len);

	for (int i = 0; i < len; i++)
	{
		Result.wcCString[i] = wcCString[iFirst + i];
	}

	return Result;
}



















WString::WString()
{
	//wcCString = new WCHAR[1];
	//wcCString[0] = '\0';
	wcCString = NULL;
	Len = 0;
}


WString::WString(int len)
{
	Len = len;
	if (Len > 0)
	{
		wcCString = new WCHAR[len+1];
		for (int i = 0; i <= len; i++) wcCString[i] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}

WString::WString(const WString &s)
{ 
	Len = s.GetLen();
	if (Len > 0)
	{
		wcCString = new WCHAR[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = s[i];
		wcCString[Len] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}


WString::WString(const WCHAR * const str)
{ 
	Len = (int)wcslen(str);
	if (Len > 0)
	{
		wcCString = new WCHAR[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = str[i];
		wcCString[Len] = '\0';
	}
	else
	{
		wcCString = NULL;
	}
}


WString& WString::operator= (const WString &s)
{ 
	if (this == &s) return *this;
	if (wcCString)
	{
		delete[] wcCString;
		wcCString = NULL;
	}
	Len = s.GetLen();
	if (Len > 0)
	{
		wcCString = new WCHAR[Len+1];
		for (int i = 0; i < Len; i++) wcCString[i] = s[i];
		wcCString[Len] = '\0';
	}
	return *this;
}

WString::~WString()
{
	if (wcCString)
	{
		delete[] wcCString;
		wcCString = NULL;
	}
	Len = 0;
}


bool WString::operator ==(const WString &s)
{
	// if they are same size
	if (Len != s.GetLen()) return false;
	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] != s[i]) return false;
	}
	return true;
}

bool WString::operator !=(const WString &s)
{
	if (Len != s.GetLen()) return true;
	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] != s[i]) return true;
	}	 
	return false;
}



WCHAR& WString::operator [](int offset)
{
	if (Len == 0) return *((WCHAR*)NULL);
	else if (offset > Len) return wcCString[Len];
	else return wcCString[offset];
}

WCHAR WString::operator [](int offset) const
{
	if (Len == 0) return '\0';
	else if (offset > Len) return wcCString[Len];
	else return wcCString[offset];
}


void WString::operator +=(const WString &s)
{
	int totallen = Len + s.GetLen();
	WString tmp(totallen);
	int i,j;
	for (i = 0; i < Len; i++) tmp[i] = wcCString[i];
	for (j = 0; j < s.GetLen(); j++, i++) tmp[i] = s[j];
	if (totallen > 0)
		tmp[totallen] = '\0';
	*this = tmp;
} 

WString WString::operator +(const WString &s) const
{
	int totallen = Len + s.GetLen();
	WString tmp(totallen);
	int i,j;
	for (i = 0; i < Len; i++) tmp[i] = wcCString[i];
	for (j = 0; j < s.GetLen(); j++, i++) tmp[i] = s[j];
	if (totallen > 0)
		tmp[totallen] = '\0';
	return tmp;
}


bool WString::Grow(int NewSize)
{
	WCHAR *tmpBuffer;
	tmpBuffer = new WCHAR[NewSize+1];

	if (!tmpBuffer) return false;
	
	if (Len > 0)
		CopyMemory(tmpBuffer, wcCString, (Len+1) * sizeof(WCHAR));
	if (wcCString)
		delete[] wcCString;
	wcCString = tmpBuffer;
	Len = NewSize;
	return true;
}



bool WString::InsertChar(int Index, WCHAR c)
{
	if (Index < 0 || Index > Len) return false;

	if (!Grow(Len+1)) return false;

	MoveMemory(wcCString + Index + 1, wcCString + Index, sizeof(WCHAR)* (Len-Index));
	wcCString[Index] = c;
	return true;
}


bool WString::RemoveChar(int Index)
{ 
	if (Index < 0 || Index >= Len) return false;

	MoveMemory(wcCString+Index, wcCString+Index+1,sizeof(WCHAR)* (Len-Index));
	Len--;

	if (Len == 0)
	{
		delete[] wcCString;
		wcCString = NULL;
	}

	return true;
} 


bool WString::InsertString(int Index, WCHAR *str)
{
	if (Index < 0 || Index > Len) return false;
	if (!str) return false;

	int _strlen = (int)wcslen(str);
	if (!Grow(Len + _strlen)) return false;

	MoveMemory(wcCString+Index+_strlen, wcCString+Index, sizeof(WCHAR)* (Len-_strlen-Index));
	CopyMemory(wcCString+Index, str, sizeof(WCHAR)*_strlen);
	return true;
}

bool WString::AsBool()
{ 
	WCHAR *pwc;
	pwc = wcsstr(wcCString, L"true");
	if (pwc) return true;
	pwc = wcsstr(wcCString, L"TRUE");
	if (pwc) return true; 

	return false;
}

int WString::AsInt()
{
	int tmp; 
	swscanf(wcCString, L"%d", &tmp);
	return tmp;
}

float WString::AsFloat()
{
	float tmp;
	swscanf(wcCString, L"%f", &tmp);
	return tmp;
}


void WString::AsChar(char *&outc)
{
	if (!outc)
		delete outc;
	outc = new char[GetLen()+1]; 
	int i;
	for (i = 0; i < GetLen(); i++)
	{
		outc[i] = (char)wcCString[i];
	}
	outc[i] = '\0';
}


WString WString::Extract(WCHAR Char, int PartNum)
{
	int start, end, partnum=-1;
	WString tmpS;

	if (PartNum < 0) return tmpS;
 
	// nadjemo prvo part koji nas zanima i vidimo koliko je dugacak
	start = 0;
	end = -1;

	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] == Char)
		{
			partnum++;
			if (partnum == PartNum) 
			{				 
				tmpS.Grow(end-start+1);
				CopyMemory(tmpS.wcCString, &wcCString[start], sizeof(WCHAR)*(end-start+1));				 
				return tmpS;
			} else
			{ 
				start = i+1;
				end = i;
			}
		} else end++;
	}

	partnum++;
	if (partnum == PartNum) 
	{
		end = Len;
		tmpS.Grow(end-start);
		CopyMemory(tmpS.wcCString, &wcCString[start], sizeof(WCHAR)*(end-start));
		return tmpS;
	} else return tmpS; 
}


bool WString::Contains(WStringParam sub)
{
	bool good;

	if (sub.Len == 0)
	{
		return true;
	}

	for (int i = 0; i < Len; i++)
	{
		if (wcCString[i] == sub.wcCString[0])
		{
			good = true;

			for (int j = 0; j < sub.Len; j++)
			{
				if (wcCString[i + j] != sub.wcCString[j])
				{
					good = false;
					break;
				}
			}

			if (good)
			{
				return true;
			}
		}
	}

	return false;
}
