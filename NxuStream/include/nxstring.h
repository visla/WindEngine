//
//      nxString
//
// More convenient than using character arrays (char*). 
// Its your typical nxString class - nuff said.
// This is a general use class.  Not PhysX specific.
// This file has no dependancies on any other data types.
//
//

#ifndef NX_STRING_H
#define NX_STRING_H

#include <stdio.h>
#include <string.h>

#ifdef __CELLOS_LV2__
#	define stricmp strcasecmp
#	define _vsnprintf vsnprintf
#endif

class nxString {
  public:
				nxString();
				nxString(const nxString &t);
				nxString(const char *s);
				nxString(const char *s,int len);
	template<class T>
	explicit	nxString(const T &a){buf=NULL;prev=next=this;length=size=0;*this="";*this+=a;}
				~nxString();
	nxString &	operator =(const nxString &s);
	nxString &	operator =(const char *s);
	nxString &	operator+=(const char *b);
	nxString &	sprintf(char *format,...);
	const char  operator[](int i) const {return buf?buf[i]:'\0';}
	char &		operator[](int i){detach(); return buf[i];}
				operator const char *() const;
	//			operator char *() { detach(); return buf;}
	nxString &	DropFirstChars(char *delimeter=" \t");
	void		detach();  // unshare data with siblings copy if necessary
	void		resize(int _newsize);
private:
	char		*buf;
	int			length; // strlen
	int			size;   // memory allocated
protected:
	mutable const nxString	*prev;  // loop of siblings sharing the same data
	mutable const nxString	*next;  // loop of siblings sharing the same data
};

nxString operator+(const nxString &a,const nxString &b);

inline nxString operator+=(nxString &s,float a)
{
	char buf[128];
	sprintf(buf,"%g",a);
	return s+=buf;
}

inline nxString operator+=(nxString &s,int a)
{
	char buf[32];
	sprintf(buf,"%d",a);
	return s+=buf;
}
inline nxString operator+=(nxString &s,unsigned int a)
{
	char buf[32];
	sprintf(buf,"%d",a);
	return s+=buf;
}

template <class T>
inline nxString &append(nxString &s,T* a,int n,const char *delimeter="\n")
{
	for(int i=0;i<n;i++) 
	{
		s+=a[i];
		s+=delimeter;
	}
	return s;
}

int operator==(const nxString &a,const nxString &b);
int operator!=(const nxString &a,const nxString &b);
int operator> (const nxString &a,const nxString &b);
int operator< (const nxString &a,const nxString &b);
int operator>=(const nxString &a,const nxString &b);
int operator<=(const nxString &a,const nxString &b);
int operator==(const nxString &a,const char     *b);
int operator!=(const nxString &a,const char     *b);
int operator> (const nxString &a,const char     *b);
int operator< (const nxString &a,const char     *b);
int operator>=(const nxString &a,const char     *b);
int operator<=(const nxString &a,const char     *b);
int operator==(const char     *a,const nxString &b);
int operator!=(const char     *a,const nxString &b);
int operator> (const char     *a,const nxString &b);
int operator< (const char     *a,const nxString &b);
int operator>=(const char     *a,const nxString &b);
int operator<=(const char     *a,const nxString &b);


#endif
