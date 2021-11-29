/*
     CStaticString.h

	 Author: Nebojsa Vislavski
	 Date: 18.4.2005
	 Version: 1.22

	 This is template class used for static strings. Whenever you need static string than 
	 use this template class

	 Example of use:

	    CStaticString<20> s1; // creates string 20 chars long
		s1 = L"Ovo je nesto super i cool"; // this will automaticly truncate string to 20 chars. L is used for WCHAR and is needed
		s1 += L"ovo"; // ERROR! there is no operator += in static string. Use CString instead

   


*/


#ifndef _STATICSTRING_WCHAR
#define _STATICSTRING_WCHAR

//#include <cstring.h>
#include "CString.h" //ispravio Vlada (zbog putanja)
#include <WindEngine.h>

#define NAME_LENGTH 30
#define NAME_LENGTHx3 90


// This class is used when needed static string for file saving and similar tasks
template <int Size = 1>
class StaticString
{
public:
    StaticString();	
    StaticString(const char *const);
	StaticString(const String &str);
	StaticString(const StaticString &);

	operator String() { String tmp(wcString); return tmp; };
    const char *GetString() const { return wcString; };

	bool operator == (const StaticString &str) const;
	bool operator == (const String &str) const;

	bool operator != (const StaticString &str);

    char & operator[](int offset);
    char operator[](int offset) const;
	operator char *() {return wcString; };


protected:
   
	char wcString[Size+1];
};




template <int Size>
bool StaticString<Size>::operator == (const StaticString<Size> &str) const
{
  for (int i = 0; i < Size; i++) 
  {
    if (wcString[i] != str[i]) return false;
  }  
  return true;
}

template <int Size>
bool StaticString<Size>::operator == (const String &str) const
{
  int i = 0;
  while(1)
  {
    if (wcString[i] != str[i]) return false;
	if (i == Size) break;
	if (i == str.GetLen()) break;
	i++;
  }  
  return true;
}



template <int Size>
bool StaticString<Size>::operator != (const StaticString<Size> &str)
{
  for (int i = 0; i < Size; i++) 
  {
    if (wcString[i] != str[i]) return true;
  }  
  return false;
}


template <int Size>
StaticString<Size>::StaticString(const StaticString<Size> &str)
{
  for (int i = 0; i < Size; i++) wcString[i] = str[i];
  wcString[Size] = '\0';
}


template <int Size>
StaticString<Size>::StaticString(const String &str)
{
   int i;
   for (i = 0; i <= Size; i++) wcString[i] = '\0';
   if (str.GetLen() > Size) 
   {
	  for (i = 0; i < Size; i++) wcString[i] = str[i];
   } else 
   {
	   for (i = 0; i < str.GetLen(); i++) wcString[i] = str[i];
   }
   wcString[i] = '\0';	     
}


template <int Size>
StaticString<Size>::StaticString()
{
  for (int i = 0; i < Size; i++) wcString[i] = '\0';
  wcString[Size] = '\0';
}

template <int Size>
StaticString<Size>::StaticString(const char * const str)
{
   int i;
   for (i = 0; i <= Size; i++) wcString[i] = '\0';

   if (strlen(str) > Size) 
   {
	  for (int i = 0; i < Size; i++) wcString[i] = str[i];
   } else 
   {
     strcpy(wcString, str);
   }
   wcString[Size] = '\0';	   
}


template <int Size>
char& StaticString<Size>::operator [](int offset)
{ 
  if (offset >= Size) return wcString[Size-1];
  else return wcString[offset];
}

template <int Size>
char StaticString<Size>::operator [](int offset) const
{ 
  if (offset >= Size) return wcString[Size-1];
  else return wcString[offset];
}

typedef StaticString<NAME_LENGTH> StringName;
typedef StaticString<NAME_LENGTHx3> StringFullName;
typedef StaticString<NAME_LENGTHx3> StringHoleName;






























// This class is used when needed static string for file saving and similar tasks
template <int Size = 1>
class WStaticString
{
public:
    WStaticString();	
    WStaticString(const WCHAR *const);
	WStaticString(const WString &str);
	WStaticString(const WStaticString &);

	operator WString() { WString tmp(wcString); return tmp; };
    const WCHAR *GetString() const { return wcString; };

	bool operator == (const WStaticString &str) const;
	bool operator == (const WString &str) const;

	bool operator != (const WStaticString &str);

    WCHAR & operator[](int offset);
    WCHAR operator[](int offset) const;
	operator WCHAR *() {return wcString; };


protected:
   
	WCHAR wcString[Size+1];
};




template <int Size>
bool WStaticString<Size>::operator == (const WStaticString<Size> &str) const
{
  for (int i = 0; i < Size; i++) 
  {
    if (wcString[i] != str[i]) return false;
  }  
  return true;
}

template <int Size>
bool WStaticString<Size>::operator == (const WString &str) const
{
  int i = 0;
  while(1)
  {
    if (wcString[i] != str[i]) return false;
	if (i == Size) break;
	if (i == str.GetLen()) break;
	i++;
  }  
  return true;
}



template <int Size>
bool WStaticString<Size>::operator != (const WStaticString<Size> &str)
{
  for (int i = 0; i < Size; i++) 
  {
    if (wcString[i] != str[i]) return true;
  }  
  return false;
}


template <int Size>
WStaticString<Size>::WStaticString(const WStaticString<Size> &str)
{
  for (int i = 0; i < Size; i++) wcString[i] = str[i];
  wcString[Size] = '\0';
}


template <int Size>
WStaticString<Size>::WStaticString(const WString &str)
{
   int i;
   for (i = 0; i <= Size; i++) wcString[i] = '\0';
   if (str.GetLen() > Size) 
   {
	  for (i = 0; i < Size; i++) wcString[i] = str[i];
   } else 
   {
	   for (i = 0; i < str.GetLen(); i++) wcString[i] = str[i];
   }
   wcString[i] = '\0';	     
}


template <int Size>
WStaticString<Size>::WStaticString()
{
  for (int i = 0; i < Size; i++) wcString[i] = '\0';
  wcString[Size] = '\0';
}

template <int Size>
WStaticString<Size>::WStaticString(const WCHAR * const str)
{
   int i;
   for (i = 0; i <= Size; i++) wcString[i] = '\0';

   if (wcslen(str) > Size) 
   {
	  for (int i = 0; i < Size; i++) wcString[i] = str[i];
   } else 
   {
     wcscpy(wcString, str);
   }
   wcString[Size] = '\0';	   
}


template <int Size>
WCHAR& WStaticString<Size>::operator [](int offset)
{ 
  if (offset >= Size) return wcString[Size-1];
  else return wcString[offset];
}

template <int Size>
WCHAR WStaticString<Size>::operator [](int offset) const
{ 
  if (offset >= Size) return wcString[Size-1];
  else return wcString[offset];
}


typedef WStaticString<NAME_LENGTH> WStringName;
typedef WStaticString<NAME_LENGTHx3> WStringFullName;
typedef WStaticString<NAME_LENGTHx3> WStringHoleName;


#endif