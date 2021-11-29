/*
   CString.h

   Author: Nebojsa Vislavski
   Date: 18.4.2005.
   Version: 1.2

   This is "simple" CString class

   Examples of use:

      CString str1, str2;
      CStrings StringList;

	  StringList.PushBack(L"Element 1");
	  StringList.PushBack(L"Element 2");
	  StringList.Clear();

	  str1 = L"ako ";
	  str2 = L"je";
	  str1 = str1 + str2;  // str1 is "ako je"
	  str1.RemoveChar(0);  // str1 is "ko je"
	  str1.InsertChar(0, 't'); // str is "tko je"
	  str1.InsertCString(3, L"m lice"); // str is "tkom lice je"

	  wprintf(L"%s", str1);  // has casting operator to WCHAR*, so you dont need to use str1.GetCString()
*/


#ifndef _CString_WIDE
#define _CString_WIDE


//#ifndef _CSTRING_LIBBUILD
//  #pragma message("Automatic link to cstring.lib")
//  #pragma comment(lib, "cstring.lib")
//#endif


#include <windows.h>
#include <smartlist.h>
#include <stdio.h>
#include <WindEngine.h>


#define StringParam const String &
#define WStringParam const WString &



extern char EmptyStr[1];


class String;
typedef CSmartList<String> Strings;





class WINDENGINE_API String
{
       public:
          // constructors
           String();
           String(const char *const);
           String(const String &);
          ~String();

          // overloaded operators
          char & operator[](int offset);
          char operator[](int offset) const;
          String operator+(const String&)const;
          void operator+=(const String&);
		  void operator+=(const char&);
          String & operator= (const String &);
 	      bool operator== (const String &);
	      bool operator!= (const String &);		  

	      operator char *();
          operator const char *() const;
          // General accessors
          int GetLen()const { return Len; }
		  const char *GetString() const;
          
          // Insertiotion and removal functions
		  bool InsertChar(int Index, char c);
		  bool RemoveChar(int Index); 
		  bool InsertString(int Index, char *str); 

		  String Extract(char DelimiterChar, int PartNum);
		  int SubPos(StringParam sub);
		  int SubPosCaseInsensitive(StringParam sub);
		  int Contains(StringParam sub) { return (SubPos(sub) >= 0); };
		  int ContainsCaseInsensitive(StringParam sub) { return (SubPosCaseInsensitive(sub) >= 0); };

		  bool EqualCaseInsensitive(StringParam sub);

		  String Sub(int iFirst, int iLast); // first char is [iFirst], last char is [iLast - 1]

		  // casting methods
		  bool		AsBool();
		  int		AsInt();
          float		AsFloat();
		  void		AsChar(char *&outc);  // this needs to be created and allocated pointer
			         
          	  

       protected:
          String (int Size);         // private constructor creates string of this size

          bool Grow(int NewSize);

          char *wcCString;		  
          unsigned short Len;
};



class WString
{
       public:
          // constructors
           WString();
           WString(const WCHAR *const);
           WString(const WString &);
          ~WString();

          // overloaded operators
          WCHAR & operator[](int offset);
          WCHAR operator[](int offset) const;
          WString operator+(const WString&)const;
          void operator+=(const WString&);
          WString & operator= (const WString &);
 	      bool operator== (const WString &);
	      bool operator!= (const WString &);		  

	      operator WCHAR *() { return wcCString; } ;  
          operator const WCHAR *() const { return wcCString; };
          // General accessors
          int GetLen()const { return Len; }
          const WCHAR *GetString() const { return wcCString; };
          
          // Insertiotion and removal functions
		  bool InsertChar(int Index, WCHAR c);
		  bool RemoveChar(int Index); 
		  bool InsertString(int Index, WCHAR *str); 

		  WString Extract(WCHAR DelimiterChar, int PartNum);
		  bool Contains(WStringParam sub);

		  // casting methods
		  bool		AsBool();
		  int		AsInt();
          float		AsFloat();
		  void		AsChar(char *&outc);  // this needs to be created and allocated pointer
			         
          	  

       protected:
          WString (int Size);         // private constructor creates string of this size

          bool Grow(int NewSize);

          WCHAR *wcCString;		  
          unsigned short Len;
};



#endif